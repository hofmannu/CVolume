#include "volume.h"

volume::volume()
{
	className = "volume";
	processor_count = std::thread::hardware_concurrency();
}

volume::volume(const std::size_t _dim0, const std::size_t _dim1, const std::size_t _dim2)
{
	set_dim(_dim0, _dim1, _dim2);
	alloc_memory();
	volume();
}

volume::~volume()
{

	// if memory was allocated in data, release now
	if (isMemAlloc)
	{
		delete[] data;
		delete[] sliceZ;
		delete[] sliceX;
		delete[] sliceY;
		delete[] mipZ;
		delete[] mipX;
		delete[] mipY;
	}
}

// copy constructor (this is new instance, obj is the one we copy from
volume::volume(volume& obj)
{
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		this->set_dim(iDim, obj.get_dim(iDim));
		this->origin[iDim] = obj.get_origin(iDim);
		this->res[iDim] = obj.get_res(iDim);
	}

	this->alloc_memory();
	memcpy(this->data, obj.get_pdata(), this->nElements * sizeof(float));
	return;
}

// equal and not equal operators
bool volume::operator == (const volume& volumeB) const
{
	bool isSame = 1;

	// check if number of elements is the same
	if (this->nElements != volumeB.get_nElements())
	{
		printf("Arrays need to have the same size for this operation\n");
		throw "InvalidSize";
	}

	for (std::size_t iElement = 0; iElement < this->nElements; iElement++)
	{
		if (this->data[iElement] != volumeB.get_value(iElement))
		{
			isSame = 0;
		}
	}
	return isSame;
}

bool volume::operator != (const volume& volumeB) const
{
	bool isSame = 1;

	// check if number of elements is the same
	if (this->nElements != volumeB.get_nElements())
	{
		printf("Arrays need to have the same size for this operation\n");
		throw "InvalidSize";
	}

	for (std::size_t iElement = 0; iElement < this->nElements; iElement++)
	{
		if (this->data[iElement] != volumeB.get_value(iElement))
		{
			isSame = 0;
		}
	}
	return (!isSame);
}

// sign constant value to all entries of data, origin, and resolution
volume& volume::operator = (const float setVal)
{
	for (std::size_t iElem = 0; iElem < this->nElements; iElem++)
	{
		this->data[iElem] = setVal;
	}
	return *this;
}

// copy operator
volume& volume::operator = (volume& volumeB)
{
	if (nElements == volumeB.get_nElements())
	{
		memcpy(this->data, volumeB.get_pdata(), this->nElements * sizeof(float));
	}
	else // size is different, lets first resize output volume 
	{
		#pragma unroll
		for (std::size_t iDim = 0; iDim < 3; iDim++)
		{
			this->set_dim(iDim, volumeB.get_dim(iDim));
				}
		this->alloc_memory();
		memcpy(this->data, volumeB.get_pdata(), this->nElements * sizeof(float));
	}

	// push resolution and origin over
	#pragma unroll
	for (std::size_t iDim = 0; iDim < 3; iDim++)
	{
		this->set_res(iDim, volumeB.get_res(iDim));
		this->set_origin(iDim, volumeB.get_origin(iDim));
	}

	return *this;
}

// multiplication operator
volume& volume::operator *= (const float multVal)
{
	for (std::size_t iElem = 0; iElem < this->nElements; iElem++)
	{
		this->data[iElem] =	this->data[iElem] * multVal;
	}
	return *this;
}

// division operator
// volume volume::operator /(const float divVal)
// {
// 	volume retVol(this->dim[0], this->dim[1], this->dim[2]);
// 	for (unsigned int idx = 0; idx < this->nElements; idx++)
// 		retVol.set_value(idx, this->data[idx] / divVal);
// 	return retVol;
// }

volume& volume::operator /=(const float divVal)
{
	const float multVal = 1.0f / divVal; 
	for (std::size_t iElem = 0; iElem < this->nElements; iElem++)
	{
		this->data[iElem] =	this->data[iElem] * multVal;
	}
	return *this;

}

// addition operator
volume& volume::operator +=(const volume& volumeB)
{
	if (this->nElements != volumeB.get_nElements())
	{
		printf("Volumes must have the same number of elements for this\n");
		throw "InvalidSize";
	}

	for (std::size_t iElem = 0; iElem < this->nElements; iElem++)
	{
		this->data[iElem] = this->data[iElem] + volumeB[iElem];
	}

	return *this;
}

// substraction operator
volume& volume::operator -=(const volume& volumeB)
{
	if (this->nElements != volumeB.get_nElements())
	{
		printf("Volumes must have the same number of elements for this\n");
		throw "InvalidSize";
	}

	for (std::size_t iElem = 0; iElem < this->nElements; iElem++)
	{
		this->data[iElem] = this->data[iElem] - volumeB[iElem];
	}

	return *this;
}

float volume::operator[] (const std::size_t idx) const
{
	return *(data + idx);
}

const float volume::operator()(const std::size_t ix, const std::size_t iy, const std::size_t iz)
{
	return get_value(ix, iy, iz);
}

// get pointer to slice with z as normal
float* volume::get_psliceZ(const std::size_t zLevel)
{
	if (zLevel != lastSliceZ)
	{
		lastSliceZ = zLevel;
		// update slice
		for (std::size_t ix = 0; ix < dim[0]; ix++)
		{
			for (std::size_t iy = 0; iy < dim[1]; iy++)
			{
				const std::size_t sliceIdx = ix + iy * dim[0];
				sliceZ[sliceIdx] = get_value(ix, iy, zLevel);
			}
		}
	}

	return sliceZ;
}

// get pointer to slice with x as normal
float* volume::get_psliceX(const std::size_t xLevel)
{
	if (xLevel != lastSliceX)
	{
		lastSliceX = xLevel;
		// update slice
		for (std::size_t iz = 0; iz < dim[2]; iz++)
		{
			for (std::size_t iy = 0; iy < dim[1]; iy++)
			{
				const std::size_t sliceIdx = iz + iy * dim[2];
				sliceX[sliceIdx] = get_value(xLevel, iy, iz);
			}
		}
	}

	return sliceX;
}

// get pointer to slice with y as normal
float* volume::get_psliceY(const std::size_t yLevel)
{
	if (yLevel != lastSliceY)
	{
		lastSliceY = yLevel;
		// update slice
		for (std::size_t iz = 0; iz < dim[2]; iz++)
		{
			for (std::size_t ix = 0; ix < dim[0]; ix++)
			{
				const std::size_t sliceIdx = ix + iz * dim[0];
				sliceY[sliceIdx] = get_value(ix, yLevel, iz);
			}
		}
	}

	return sliceY;
}

// get slice of volume at position
float* volume::get_psliceZ(const float zPos)
{
	uint64_t zIdx = get_idx(zPos, 0);
	return get_psliceZ(zIdx);
}

float* volume::get_psliceX(const float xPos)
{
	uint64_t xIdx = get_idx(xPos, 1);
	return get_psliceX(xIdx);
}

float* volume::get_psliceY(const float yPos)
{
	uint64_t yIdx = get_idx(yPos, 2);
	return get_psliceY(yIdx);
}

float volume::get_length(const std::size_t _dim) const
{
	return (float) dim[_dim] * res[_dim];
}

void volume::alloc_memory()
{
	// if any memory was allocated before, make sure to free it first
	if (isMemAlloc)
	{
		delete[] data;
		delete[] sliceX;
		delete[] sliceY;
		delete[] sliceZ;
		delete[] mipX;
		delete[] mipY;
		delete[] mipZ;
		delete[] croppedMipX;
		delete[] croppedMipY;
		delete[] croppedMipZ;
	}


	// allocate memory for data array	
	data = new float[get_nElements()];
	
	// allocate memory for crosssections
	sliceZ = new float [dim[0] * dim[1]]; 
	sliceX = new float [dim[1] * dim[2]];
	sliceY = new float [dim[0] * dim[2]];

	// allocate memory for mips
	mipZ = new float [dim[0] * dim[1]];
	mipX = new float [dim[1] * dim[2]];
	mipY = new float [dim[0] * dim[2]];

	// allocate memory for cropped mips
	croppedMipZ = new float [dim[0] * dim[1]];
	croppedMipX = new float [dim[1] * dim[2]];
	croppedMipY = new float [dim[0] * dim[2]];
	isMemAlloc = 1; // set flag for memory allocation
	return;
}

// scales whole data array by a number (deprecated, will be removed soon)
void volume::multiply(const float factor)
{
	unsigned int nElements = dim[0] * dim[1] * dim[2];
	for (unsigned int iElement = 0; iElement < nElements; iElement++)
		data[iElement] *= factor;

	return;
}

// define dimensions of dataset
void volume::set_dim(const std::size_t dim0, const std::size_t dim1, const std::size_t dim2)
{
	dim[0] = dim0;
	dim[1] = dim1;
	dim[2] = dim2;
	nElements = dim[0] * dim[1] * dim[2];
	return;
}

void volume::set_dim(const std::size_t* _dim)
{
	set_dim(_dim[0], _dim[1], _dim[2]);
	return;
}

void volume::set_dim(const std::size_t _dim, const std::size_t newDim)
{
	dim[_dim] = newDim;
	nElements = dim[0] * dim[1] * dim[2];
	return;
}

uint64_t volume::get_dim(const std::size_t _dim) const
{
	return dim[_dim];
}

// set origin of volumetrix dataset
void volume::set_origin(const float* _origin)
{
	origin[0] = _origin[0];
	origin[1] = _origin[1];
	origin[2] = _origin[2];
	return;
}

void volume::set_origin(const float origin0, const float origin1, const float origin2)
{
	origin[0] = origin0;
	origin[1] = origin1;
	origin[2] = origin2;
	return;
}

void volume::set_origin(const std::size_t _dim, const float _origin)
{
	origin[_dim] = _origin;
	return;
}

// set resolution of volumetric dataset
void volume::set_res(const float* dx)
{
	#pragma unroll
	for (uint8_t iDim = 0; iDim < 3; iDim++)
		set_res(iDim, dx[iDim]);
	return; 
}

// define resolution in one go
void volume::set_res(const float dx0, const float dx1, const float dx2)
{
	set_res(0, dx0);
	set_res(1, dx1);
	set_res(2, dx2);
	return;
}

void volume::set_res(const std::size_t _dim, const float _res)
{
	if (_res <= 0)
	{
		printf("Resolution along %d axis needs to be bigger then 0\n", _dim);
		throw "invalidValue";
	}
	res[_dim] = _res;
	return;
}


// sets whole array to a certain value
void volume::set_value(const float value)
{
	for (unsigned int iElement = 0; iElement < get_nElements(); iElement++)
		data[iElement] = value;

	return;
}

// set only one specific value in volume defined by index
void volume::set_value(
	const std::size_t x0, const std::size_t x1, const std::size_t x2, const float value)
{
	unsigned int index = x0 + dim[0] * (x1 + x2 * dim[1]);
	data[index] = value;
	return;
}

void volume::set_value(const std::size_t iElem, const float value)
{
	data[iElem] = value;
	return;
}

// set only one specific value in volume
void volume::set_value(const std::size_t* pos, const float value)
{
	std::size_t index = pos[0] + dim[0] * (pos[1] + pos[2] * dim[1]);
	data[index] = value;
	return;
}

float volume::get_value(const std::size_t x0, const std::size_t x1, const std::size_t x2) const
{
	const std::size_t idx = x0 + dim[0] * (x1 + x2 * dim[1]);
	return data[idx];
}

float volume::get_value(const std::size_t iElem) const {return data[iElem];}

// pass position as a 3 element vector to indices
float volume::get_value(const std::size_t* pos) const
{
	const unsigned int idx = pos[0] + dim[0] * (pos[1] + pos[2] * dim[1]);
	return data[idx];
}

// get position along axis
float volume::get_pos0(const std::size_t idx0) const
{
	return get_pos(idx0, 0);
}

float volume::get_pos1(const std::size_t idx1) const
{
	return get_pos(idx1, 1);
}

float volume::get_pos2(const std::size_t idx2) const
{
	return get_pos(idx2, 2);
}

float volume::get_pos(const std::size_t idx, const std::size_t iDim) const
{
	return origin[iDim] + (float) idx * res[iDim];
}

float volume::get_centerPos(const std::size_t _dim)
{
	const float centerPos = origin[_dim] + ((float) dim[_dim] * res[_dim]) / 2;
	return centerPos;
}

// get index along a certain dimension
uint64_t volume::get_idx0(const float pos0) const {return get_idx(pos0, 0);}
uint64_t volume::get_idx1(const float pos1) const {return get_idx(pos1, 1);}
uint64_t volume::get_idx2(const float pos2) const {return get_idx(pos2, 2);}

uint64_t volume::get_idx(const float pos, const std::size_t iDim) const
{
	if (pos < origin[iDim])
	{
		printf("[volume] hitting lower boundary of volume along dim %d\n", iDim);
		return 0;
	}
	else
	{
		const std::size_t outputIdx = (pos - origin[iDim]) / res[iDim] + 0.5;
		if (outputIdx > (dim[iDim] - 1)){
			printf("[volume] hitting upper boundary of volume along dim %d\n", iDim);
			return dim[iDim] - 1;
		}
		else
			return outputIdx;
	}
}

std::size_t volume::get_nElements() const
{
	return dim[0] * dim[1] * dim[2];
}


// returns the file extension of a given path
string getFileExt(const string _filePath)
{

	size_t i = _filePath.rfind('.', _filePath.length());
   if (i != string::npos) {
      return(_filePath.substr(i+1, _filePath.length() - i));
   }

  return(""); 
}

// saving and reading data from and to h5 file
void volume::saveToFile(const string _filePath) const
{
	// requires implementation
	const string ext = getFileExt(_filePath);

	if (!strcmp(ext.c_str(), "h5"))
	{
		printf("Saving to h5 file...\n");
		save_h5(_filePath);
	}
	else if (!strcmp(ext.c_str(), "nii"))
	{
		printf("Saving to nii file...\n");
		save_nii(_filePath);
	}
	else
	{
		throw "InvalidType";
	}
	return;
}

void volume::readFromFile(const string _filePath)
{
	// requires implementation
	// requires implementation
	const string ext = getFileExt(_filePath);

	if (!strcmp(ext.c_str(), "h5"))
	{
		printf("Reading form h5 file...\n");
		read_h5(_filePath);
	}
	else if (!strcmp(ext.c_str(), "nii"))
	{
		printf("Reading form nii file...\n");
		read_nii(_filePath);
	}
	else
	{
		printf("I do not support loading from this file type.\n");
		throw "InvalidType";
	}

	return;
}

// saves our dataset to a nii file
void volume::save_nii(const string _filePath) const
{
	// outPath = _filePath;

	nifti1_extender pad={0,0,0,0};
  int ret;
  nifti_1_header hdr_cpy = hdr;

  hdr_cpy.datatype = DT_FLOAT;
  
  FILE *fp = fopen(_filePath.c_str(), "w");
  if (fp == NULL) 
  {
    printf("Error opening header file %s for write\n", _filePath.c_str());
    throw "FileError";
  }

  ret = fwrite(&hdr_cpy, MIN_HEADER_SIZE, 1, fp);
  if (ret != 1) 
  {
    printf("Error writing header file %s\n", _filePath.c_str());
    throw "FileError";
  }

  ret = fwrite(&pad, 4, 1, fp);
  if (ret != 1) 
  {
    printf("Error writing header file extension pad %s\n", _filePath.c_str());
   throw "FileError";
  }

  ret = fwrite(data, sizeof(float), nElements, fp);
  if (ret != nElements) {
    printf("Error writing data to %s\n", _filePath.c_str());
    throw "FileError";
  }

  fclose(fp);

	return;
}

// reads the dataset from our nii file
void volume::read_nii(const string _filePath)
{
	inPath = _filePath;

	// open and read header
  FILE *fp = fopen(inPath.c_str(), "r");
  if (fp == NULL) 
  {
    printf("Error opening header file %s\n", inPath.c_str());
    throw "FileError";
  }

  int ret = fread(&hdr, MIN_HEADER_SIZE, 1, fp);
  if (ret != 1) 
  {
    printf("Error reading header file %s\n", inPath.c_str());
    throw "OperationFailed";
  }

  ret = fseek(fp, (long)(hdr.vox_offset), SEEK_SET);
  if (ret != 0) 
  {
    printf("Error doing fseek() to %ld in data file %s\n",
      (long)(hdr.vox_offset), inPath.c_str());
    throw "InvalidOperation";
  }

  // move dimensions from header struct to volume
  set_dim(hdr.dim[1], hdr.dim[2], hdr.dim[3]);
  set_res(hdr.pixdim[1], hdr.pixdim[2], hdr.pixdim[3]);
  // printf("Dataset dimensions: %lu x %lu x %lu", 
  // 	hdr.dim[1], hdr.dim[2], hdr.dim[3]);
  alloc_memory();

  if (hdr.datatype == DT_FLOAT)
  {
    ret = fread(data, sizeof(float), nElements, fp);
    if (ret != nElements)
    {
      printf("Error reading volume 1 from %s (%d)\n", inPath.c_str(), ret);
      exit(1);
    }
  }
  else if (hdr.datatype == DT_INT16)
  {
    int16_t* tempArray = new int16_t[hdr.dim[1] * hdr.dim[2] * hdr.dim[3]];
    ret = fread(tempArray, sizeof(int16_t), hdr.dim[1] * hdr.dim[2] * hdr.dim[3], fp);
    if (ret != nElements)
    {
      printf("Error reading volume 1 from %s (%d)\n",
        inPath.c_str(), ret);
      exit(1);
    }

    // convert to float
    for (int iElem = 0; iElem < (hdr.dim[1] * hdr.dim[2] * hdr.dim[3]); iElem++)
      data[iElem] = (float) tempArray[iElem];

    delete[] tempArray;
  }
  else
  {
    printf("Data type %d requires implementation!\n", hdr.datatype);
    throw "InvalidValue";
  }

  fclose(fp);

  // scale the data buffer
  if (hdr.scl_slope != 0) 
  {
    for (int i=0; i < hdr.dim[1] * hdr.dim[2] * hdr.dim[3]; i++)
      data[i] = (data[i] * hdr.scl_slope) + hdr.scl_inter;
  }

	return;
}

// save fata to a h5 file
void volume::save_h5(const string _filePath) const
{
	H5::H5File file(_filePath, H5F_ACC_TRUNC);

	// write resolutiion to file
	const hsize_t col_dims = 3;
	H5::DataSpace mspaceRes(1, &col_dims);
	H5::DataSet resDataset = file.createDataSet(
		"dr", H5::PredType::NATIVE_FLOAT, mspaceRes);
	resDataset.write(res, H5::PredType::NATIVE_FLOAT);
	resDataset.close();

	// write origin to file
	H5::DataSpace mspaceOrigin(1, &col_dims);
	H5::DataSet originDataset = file.createDataSet(
		"origin", H5::PredType::NATIVE_FLOAT, mspaceOrigin);
	originDataset.write(origin, H5::PredType::NATIVE_FLOAT);
	originDataset.close();

	// write dimension to file
	H5::DataSpace mspaceDim(1, &col_dims);
	H5::DataSet dimDataset = file.createDataSet(
		"dim", H5::PredType::NATIVE_UINT, mspaceDim);
	dimDataset.write(dim, H5::PredType::NATIVE_UINT64);
	dimDataset.close();

	// w4rite actual datamatrix to file
	const hsize_t col_data = nElements;
	H5::DataSpace mspaceData(1, &col_data);
	H5::DataSet dataDataset = file.createDataSet(
		"vol", H5::PredType::NATIVE_FLOAT, mspaceData);
	dataDataset.write(data, H5::PredType::NATIVE_FLOAT);
	dataDataset.close();

	file.close();
	return;
}

// read data from a h5 file
void volume::read_h5(const string _filePath)
{
	inPath = _filePath;
	H5::H5File file(_filePath, H5F_ACC_RDONLY); // open dataset as read only

	// load resolution from file
	H5::DataSet resDataset = file.openDataSet("dr"); // init dataset for res 
	const hsize_t col_dims = 3;
	H5::DataSpace mspaceRes (1, &col_dims); 
	H5::DataSpace filespace = resDataset.getSpace();
	resDataset.read(res, H5::PredType::NATIVE_FLOAT, mspaceRes, filespace); 	

	// load origin from file
	H5::DataSet originDataset = file.openDataSet("origin"); // dataset for origin
	H5::DataSpace mspaceOrigin (1, &col_dims); 
	filespace = originDataset.getSpace();
	originDataset.read(origin, H5::PredType::NATIVE_FLOAT, mspaceOrigin, filespace);

	// load number of dimensions from file
	H5::DataSet dimDataset = file.openDataSet("dim"); // init dimension reader
	H5::DataSpace mspaceDim (1, &col_dims);
	filespace = dimDataset.getSpace();
	dimDataset.read(dim, H5::PredType::NATIVE_UINT64, mspaceDim, filespace);
	nElements = dim[0] * dim[1] * dim[2];

	// read actual datamatrix
	H5::DataSet dataDataset = file.openDataSet("vol"); // read actual dataset
	const hsize_t col_dims_data = nElements; 
	H5::DataSpace mspaceData (1, &col_dims_data);	
	filespace = dataDataset.getSpace();
	
	alloc_memory();

	dataDataset.read(data, H5::PredType::NATIVE_FLOAT, mspaceData, filespace);

	isMemAlloc = 1;

	file.close();
	return;
}

// prints all the required information about the loaded volume
void volume::print_information() const
{
	printf("Volumetric dataset properties: \n");
	printf(" - origin:        %.2e, %.2e, %.2e \n", origin[0], origin[1], origin[2]);
	printf(" - resolution:    %.2e, %.2e, %.2e \n", res[0], res[1], res[2]);
	printf(" - dimensions:    %lu, %lu, %lu \n", dim[0], dim[1], dim[2]);
	printf(" - first element: %f \n - last element:  %f\n", data[0], data[nElements - 1]);
	return;
}

float volume::get_minPos(const uint8_t _dim) const {return origin[_dim];}
float volume::get_maxPos(const uint8_t _dim) const
{
	return origin[_dim] + res[_dim] * ((float) dim[_dim] - 1.0);
}

float volume::getRangeLimitedPos(const float pos, const uint8_t _dim) const
{
	float posOut = pos;
	if (posOut < get_minPos(_dim))
		posOut = get_minPos(_dim);

	if (posOut > get_maxPos(_dim))
		posOut = get_maxPos(_dim);

	return posOut;
}

// get cropped volume, start and stopped passed as array
void volume::get_croppedVolume(
	float* vol, const uint64_t *startIdx, const uint64_t *stopIdx) const
{
 
	const uint64_t start0 = startIdx[0];
	const uint64_t stop0 = stopIdx[0];
	const uint64_t start1 = startIdx[1];
	const uint64_t stop1 = stopIdx[1];
	const uint64_t start2 = startIdx[2];
	const uint64_t stop2 = stopIdx[2];

 getCroppedVolume(vol, start0, stop0, start1, stop1, start2, stop2);
 return;
}

void volume::getCroppedVolume(
	float* vol, // array pointing to output volume
	const uint64_t start0, const uint64_t stop0,
	const uint64_t start1, const uint64_t stop1,
	const uint64_t start2, const uint64_t stop2) const
{

	// check that stop index is always bigger then start index
	if (start0 >= stop0)
	{
		printf("Stop index must be bigger then start index along dim0\n");
		throw "invalidVal";
	}

	if (start1 >= stop1)
	{
		printf("Stop index must be bigger then start index along dim1\n");
		throw "invalidVal";
	}

	if (start2 >= stop2)
	{
		printf("Stop index must be bigger then start index along dim2\n");
		throw "invalidVal";
	}

	// calc number of elements in each dimension
	const uint64_t n0 = stop0 - start0 + 1; // n along t/z
	const uint64_t n1 = stop1 - start1 + 1; // n alonmg x
	const uint64_t n2 = stop2 - start1 + 1; // n along y
	const uint64_t nElements = n0 * n1 * n2;

	// set all values in volume for now to 0
	for (uint64_t idx = 0; idx < nElements; idx++)
		vol[idx] = 0;

	// check if all stop indices are within range
	const uint64_t stopTrue0 = (stop0 >= dim[0]) ? (dim[0] - 1) : stop0;
	if (stopTrue0 != stop0)
		printf("Cropping not doable in dim0\n");

	const uint64_t stopTrue1 = (stop1 >= dim[1]) ? (dim[1] - 1) : stop1;
	if (stopTrue1 != stop1)
		printf("Cropping not doable in dim1\n");
	
	const uint64_t stopTrue2 = (stop2 >= dim[2]) ? (dim[2] - 1) : stop2;
	if (stopTrue2 != stop2)
		printf("Cropping not doable in dim2\n");

	// index in original data matrix
	// idx = i0 + n0 * i1 + n0 * n1 * i2
	uint64_t idx; // index in original data matrix
	uint64_t offset2, offset1; // offsets in original data matrix
	
	// index in nnewly created data matrix
	// idxOut = j0 + m0 * j1 + m0 * m1 * j2

	// original --> new
	// i0 --> i0 - start0
	// i1 --> i1 - start1
	// i2 --> i2 - start2
	
	uint64_t idxOut;
	uint64_t offset2Out, offset1Out; // offset in output data matrix
	#pragma unroll
	for (uint64_t i2 = start2; i2 <= stopTrue2; i2++){
		offset2 = i2 * dim[0] * dim[1];
		offset2Out = (i2 - start2) * n0 * n1;
		#pragma unroll
		for (uint64_t i1 = start1; i1 <= stopTrue1; i1++){
			offset1 = i1 * dim[0];
			offset1Out = (i1 - start1) * n0;
			#pragma unroll
			for (unsigned int i0 = start0; i0 < stopTrue0; i0++){
				idx = offset2 + offset1 + i0;
				idxOut = offset2Out + offset1Out + (i0 - start0);
				vol[idxOut] = data[idx];
			}
		}
	}

	return;
}

void volume::crop(const uint64_t* startIdx, const uint64_t* stopIdx)
{
	uint64_t dimNew[3] = {0,0,0};
	#pragma unroll
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		if (stopIdx[iDim] < startIdx[iDim])
		{
			printf("Stop index must be larger then start index\n");
			throw "InvalidValue";
		}

		if (stopIdx[iDim] > dim[iDim])
		{
			printf("Cropping is exceeding array dimensions along dim %d (%lu of %lu)\n", 
				iDim, stopIdx[iDim], dim[iDim]);
			throw "InvalidValue";
		}
		dimNew[iDim] = stopIdx[iDim] - startIdx[iDim] + 1;
	}

	float* newData = new float [dimNew[0] * dimNew[1] * dimNew[2]];

	for (uint64_t iz = 1; iz < dimNew[2]; iz++)
	{
		const uint64_t izOld = iz + startIdx[2];
		for (uint64_t iy = 0; iy < dimNew[1]; iy++)
		{
			const uint64_t iyOld = iy + startIdx[1];
			const uint64_t idxNew = dimNew[0] * (iy + dimNew[1] * iz);
			const uint64_t idxOld = startIdx[0] + dim[0] * (iyOld + dim[1] * izOld);
			memcpy(&newData[idxNew], &data[idxOld], dimNew[0] * sizeof(float));
		}
	}

	// update dimensions and origin
	#pragma unroll
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		origin[iDim] = origin[iDim] + res[iDim] * (float) startIdx[iDim];
		set_dim(iDim, dim[iDim]);
		dim[iDim] = dimNew[iDim];
	}

	alloc_memory();
	delete[] data;
	data = newData;

	return;
}

// calculates maximum and minimum value in matrix
void volume::calcMinMax()
{
	minVal = getMin(data, nElements);
	maxVal = getMax(data, nElements);

	if (abs(minVal) > abs(maxVal))
	{
		maxAbsVal = abs(minVal);
	}
	else
	{
		maxAbsVal = abs(maxVal);
	}
	return;
}

void volume::exportVtk(const string filePath)
{
	// create copy of datset before messing with polarity
	float* dataCopy = new float [nElements];
	assign(dataCopy, data, nElements);
	
	// const string polarityHandling (sett.get_polarityHandling());
	
	// handlePolarity(dataCopy, estimAbs.nElements, polarityHandling);

	vtkwriter outputter; // prepare output pipeline
	const string title ("reconVol"); // generate title
	outputter.set_title(title); // define title of outut volume
	const string type ("STRUCTURED_POINTS"); 
	outputter.set_type(type); // define output type
	const string outputPath (filePath);
	outputter.set_outputPath(outputPath);

	float * dataOld = data;
	data = dataCopy;

	griddedData myData;
	myData.data = data;
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		myData.origin[iDim] = origin[iDim];
		myData.res[iDim] = res[iDim];
		myData.dim[iDim] = dim[iDim];
	}

	outputter.set_structuredPoints(&myData);
	outputter.set_binary();
	outputter.write();
	delete[] dataCopy;
	data = dataOld;
	return;
}

void volume::set_pdata(float* _data)
{
	data = _data;
	return;
}

// calculates the maximum intensity projections over the full volume
void volume::calcMips()
{
	// set all elements of z mip to 0
	for (uint64_t iX = 0; iX < dim[1]; iX++)
		for (uint64_t iY = 0; iY < dim[2]; iY++)
			mipZ[iX + iY * dim[1]] = 0;

	// set all elements of x mip to 0
	for (uint64_t iZ = 0; iZ < dim[0]; iZ++)
		for (uint64_t iY = 0; iY < dim[2]; iY++)
			mipX[iZ + iY * dim[0]] = 0;

	// set all elements of y mip to 0
	for (uint64_t iZ = 0; iZ < dim[0]; iZ++)
		for (uint64_t iX = 0; iX < dim[1]; iX++)
			mipY[iZ + dim[0] * iX] = 0;

	// go through all elements and check the maximum value
	for (uint64_t iZ = 0; iZ < dim[0]; iZ++)
	{
		for (uint64_t iX = 0; iX < dim[1]; iX++)
		{
			for (uint64_t iY = 0; iY < dim[2]; iY++)
			{
				const float currVal = abs(data[iZ + dim[0] * (iX + dim[1] * iY)]);
				// update z mip
				if (currVal > mipZ[iX + iY * dim[1]])
				{
					mipZ[iX + iY * dim[1]] = currVal;
				}

				// update x mip
				if (currVal > mipX[iZ + iY * dim[0]])
				{
					mipX[iZ + iY * dim[0]] =  currVal;
				}

				// update y mip
				if (currVal > mipY[iX + dim[1] * iZ])
				{
					mipY[iX + dim[1] * iZ] = currVal;
				}
			}
		}
	}

	return;
}

// update the cropping range
void volume::calcCroppedMips(const float* _cropRange)
{
	for (uint8_t idx = 0; idx < 6; idx++)
	{
			cropRange[idx] = _cropRange[idx];
	}
	calcCroppedMips();

	return;
}

struct thread_data
{
	uint64_t thread_id;
	uint64_t startY;
	uint64_t stopY;
	uint64_t cropX[2];
	uint64_t cropZ[2];
	float* ptrMipX;
	float* ptrMipY;
	float* ptrMipZ;
	uint64_t dim[3];
	float* data;
};

void *CalcSubMips(void *threadarg) 
{
   struct thread_data *my_data;
   my_data = (struct thread_data *) threadarg;

  // go through all elements and check the maximum value
	# pragma unroll
	for (uint64_t iY = my_data->startY; iY <= my_data->stopY; iY++)
	{
		#pragma unroll
		for (uint64_t iX = my_data->cropX[0]; iX < my_data->cropX[1]; iX++)
		{
			# pragma unroll
			for (uint64_t iZ = my_data->cropZ[0]; iZ < my_data->cropZ[1]; iZ++)
			{
				
				const uint64_t currIdx = iZ + my_data->dim[0] * (iX + my_data->dim[1] * iY);
				const float currVal = abs(my_data->data[currIdx]);

				// update z mip
				const uint64_t idxZ = iX + iY * my_data->dim[1];
				if (currVal > my_data->ptrMipZ[idxZ])
					my_data->ptrMipZ[idxZ] = currVal;

				// update x mip
				const uint64_t idxX = iZ + iY * my_data->dim[0];
				if (currVal > my_data->ptrMipX[idxX])
					my_data->ptrMipX[idxX] = currVal;

				// update y mip
				const uint64_t idxY = iX + my_data->dim[1] * iZ;
				if (currVal > my_data->ptrMipY[idxY])
					my_data->ptrMipY[idxY] = currVal;	
			}
		}
	}
   pthread_exit(NULL);
}

// calculates the maximum intensity projections over a cropped range
void volume::calcCroppedMips()
{
	clock_t tStart = clock();
	// set all elements of mips to 0
	for (uint64_t iElem = 0; iElem < (dim[1] * dim[2]); iElem++)
		croppedMipZ[iElem] = 0;

	for (uint64_t iElem = 0; iElem < (dim[0] * dim[2]); iElem++)
		croppedMipX[iElem] = 0;

	for (uint64_t iElem = 0; iElem < (dim[0] * dim[1]); iElem++)
		croppedMipY[iElem] = 0;

	const uint64_t idxCropRange[6] = {
		get_idx0(cropRange[0]),
		get_idx0(cropRange[1]),
		get_idx1(cropRange[2]),
		get_idx1(cropRange[3]),
		get_idx2(cropRange[4]),
		get_idx2(cropRange[5])
	};

	const uint64_t deltaZ = idxCropRange[1] - idxCropRange[0] + 1; // total number of zs
	const uint64_t deltaX = idxCropRange[3] - idxCropRange[2] + 1; // total number of zs
	const uint64_t deltaY = idxCropRange[5] - idxCropRange[4] + 1; // total number of zs
	
	printf("Cropping range: %lu ... %lu, %lu ... %lu, %lu ... %lu\n",
		idxCropRange[0], idxCropRange[1],
		idxCropRange[2], idxCropRange[3],
		idxCropRange[4], idxCropRange[5]);

	pthread_t threads[processor_count];
	pthread_attr_t attr;
	void *status;

	// Initialize and set thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	struct thread_data td[processor_count];
	float* mipYTemp[processor_count];

	const uint64_t nProcessorY = ceil((float) deltaY / ((float) processor_count));
	for (uint64_t iProcessor = 0; iProcessor < processor_count; iProcessor++)
	{
		td[iProcessor].thread_id = iProcessor;
		td[iProcessor].startY = iProcessor * nProcessorY;
		const uint64_t stopY = (iProcessor + 1) * nProcessorY - 1;
		td[iProcessor].stopY = (stopY >= deltaY) ? (deltaY - 1) : stopY;
		
		// include offset for ys
		td[iProcessor].startY += idxCropRange[4];
		td[iProcessor].stopY += idxCropRange[4];

		mipYTemp[iProcessor] = new float [dim[0] * dim[1]];
		for (uint64_t iElem = 0; iElem < (dim[0] * dim[1]); iElem++)
			mipYTemp[iProcessor][iElem] = 0;
		td[iProcessor].ptrMipX = croppedMipX;
		td[iProcessor].ptrMipY = mipYTemp[iProcessor];
		td[iProcessor].ptrMipZ = croppedMipZ;

		for (uint8_t iDim = 0; iDim < 3; iDim++)
			td[iProcessor].dim[iDim] = dim[iDim];

		td[iProcessor].cropZ[0] = idxCropRange[0];
		td[iProcessor].cropZ[1] = idxCropRange[1];
		td[iProcessor].cropX[0] = idxCropRange[2];
		td[iProcessor].cropX[1] = idxCropRange[3];

		td[iProcessor].data = data;

		// printf("worker %d runs from %d to %d\n", iProcessor, td[iProcessor].startY, td[iProcessor].stopY);

		int rc = pthread_create(&threads[iProcessor], &attr, CalcSubMips, (void *)&td[iProcessor]);
		if (rc) {
	  	cout << "Error:unable to create thread," << rc << endl;
	    exit(-1);
	  }
	} 

	// wait for ecxecution
	pthread_attr_destroy(&attr);
  for (uint64_t iProcessor = 0; iProcessor < processor_count; iProcessor++ ) {
      int 	rc = pthread_join(threads[iProcessor], &status);
      if (rc) {
         cout << "Error: unable to join," << rc << endl;
         exit(-1);
      }
      // cout << "Main: completed thread id :" << iProcessor ;
      // cout << "  exiting with status :" << status << endl;
   }

	// fuse y mips
  for (uint64_t iProcessor = 0; iProcessor < processor_count; iProcessor++)
  {
  	for (uint64_t idxLin = 0; idxLin < dim[0] * dim[1]; idxLin++)
  	{
  		if (mipYTemp[iProcessor][idxLin] > croppedMipY[idxLin])
  			croppedMipY[idxLin] = mipYTemp[iProcessor][idxLin];
  	}
  }

	// delete y mips
	for (uint64_t iProcessor = 0; iProcessor < processor_count; iProcessor++)
		delete[] mipYTemp[iProcessor];
		
	// set min and max val of cropping to the very first element
	const uint64_t startIdx = idxCropRange[0] + dim[0] * (idxCropRange[2] + 
		dim[1] * idxCropRange[4]);
	maxValCrop = abs(data[startIdx]);
	minValCrop = abs(data[startIdx]);

	// for maximum value it is sufficient to compare against one mip
	for (uint64_t iElem = 0; iElem < (dim[1] * dim[2]); iElem++)
	{
		if (croppedMipZ[iElem] > maxValCrop)
			maxValCrop = croppedMipZ[iElem];

		if (croppedMipZ[iElem] < minValCrop)
			minValCrop = croppedMipZ[iElem];
	}

	// for minimum value we also need to compare against the two other
	for (uint64_t iElem = 0; iElem < (dim[0] * dim[2]); iElem++)
	{
		if (croppedMipX[iElem] < minValCrop)
			minValCrop = croppedMipX[iElem];
	}

	for (uint64_t iElem = 0; iElem < (dim[0] * dim[1]); iElem++)
	{
		if (croppedMipY[iElem] < minValCrop)
			minValCrop = croppedMipY[iElem];
	}

	updatedCropRange = 0;
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	return;
}

float* volume::get_croppedMipX()
{
	return croppedMipX;
}

float* volume::get_croppedMipY()
{
	return croppedMipY;
}

float* volume::get_croppedMipZ()
{

	return croppedMipZ;
}

float* volume::get_croppedMipX(const float* _cropX)
{
	bool flagChanged = 0;
	for (uint8_t iElem = 0; iElem < 2; iElem++)
	{
		if (_cropX[iElem] != cropRange[iElem + 2])
		{
			flagChanged = 1;
			cropRange[iElem + 2] = _cropX[iElem];
		}
	}

	// if any cropping range is actually new here, lets update the mips
	if (flagChanged)
		calcCroppedMips();

	return croppedMipX;
}

float* volume::get_croppedMipZ(const float* _cropZ)
{
	bool flagChanged = 0;
	for (uint8_t iElem = 0; iElem < 2; iElem++)
	{
		if (_cropZ[iElem] != cropRange[iElem])
		{
			flagChanged = 1;
			cropRange[iElem] = _cropZ[iElem];
		}
	}

	if (flagChanged)
		calcCroppedMips();

	return croppedMipZ;
}

float* volume::get_croppedMipY(const float* _cropY)
{
	bool flagChanged = 0;
	for (uint8_t iElem = 0; iElem < 2; iElem++)
	{
		if (_cropY[iElem] != cropRange[iElem + 4])
		{
			flagChanged = 1;
			cropRange[iElem + 4] = (float) _cropY[iElem];
		}
	}

	if (flagChanged)
		calcCroppedMips();

	return croppedMipY;
}

// define the cropping range along x
void volume::set_cropRangeX(const float* _cropX)
{
	for (uint8_t iElem = 0; iElem < 2; iElem++)
	{
		if (cropRange[iElem + 2] != _cropX[iElem])
		{
			cropRange[iElem + 2] = _cropX[iElem];
			updatedCropRange = 1;
		}
	}
	return;
}

// define the cropping range along z
void volume::set_cropRangeZ(const float* _cropZ)
{
	for (uint8_t iElem = 0; iElem < 2; iElem++)
	{
		if (cropRange[iElem] != _cropZ[iElem])
		{
			cropRange[iElem] = _cropZ[iElem];
			updatedCropRange = 1;
		}
	}
	return;
}

// define the cropping range along y
void volume::set_cropRangeY(const float* _cropY)
{
	for (uint8_t iElem = 0; iElem < 2; iElem++)
	{
		if (cropRange[iElem + 4] != _cropY[iElem])
		{
			cropRange[iElem + 4] = _cropY[iElem];
			updatedCropRange = 1;
		}
	}
	return;
}

// normalize the entire array
void volume::normalize()
{
	const float normVal = getNorm(data, nElements);
	if (normVal > 0)
	{
		const float rnormVal = 1.0f / normVal;
		for (uint64_t iElem = 0; iElem < nElements; iElem++)
		{
			data[iElem] = data[iElem] * rnormVal;
		}
	}
	return;
}

float volume::get_norm() const
{
	return getNorm(data, nElements);
}


// subroutines to fill volume with random numbers
void volume::fill_rand()
{
	fill_rand(0.0f, 1.0f);
	return;
}

// fills volume with random values between 0 and maxVal
void volume::fill_rand(const float maxVal)
{
	fill_rand(0.0f, maxVal);
	return;
}

// fills volume with random numbers between minVal and maxVal
void volume::fill_rand(const float minVal, const float maxVal)
{
	srand(time(0));
	const float irmax = 1.0f / ((float) RAND_MAX);
	#pragma unroll
	for (uint64_t iElem = 0; iElem < nElements; iElem++)
	{
		const float randVal = ((float) rand()) * irmax;
		data[iElem] = (randVal * (maxVal - minVal)) - minVal; 
	}
	return;
}