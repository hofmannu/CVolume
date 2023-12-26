/*
	c++ based implementation of a volume handling class
	Author: Urs Hofmann
	Mail: hofmannU@ethz.ch
	Date: 05.08.2020

	Changelog:
		hofmannu - 14.08.2020 - added const to get functions
		hofmannu - 14.08.2020 - added #ifndef loop
		hofmannu - 26.02.2022 - added operators
		hofmannu - 26.02.2022 - moved to more consistent naming scheme


	Todo:
		hofmannu - add paraview export functionality
		hofmannu 
*/

#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <time.h>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include "baseClass.h"
#include <H5Cpp.h>
#include "basicMathOp.h"
#include "vtkwriter.h"
#include "griddedData.h"
#include "../lib/nifti/niftilib/nifti1.h"


#define MIN_HEADER_SIZE 348
#define NII_HEADER_SIZE 352

using namespace std;

class volume : public baseClass, public basicMathOp
{

public:
	// class constructor and destructor
	volume(); // class constructor
	volume(const std::size_t _dim0, const std::size_t _dim1, const std::size_t _dim2);
	volume(const volume& obj);

	~volume(); // class destructor

	
	void construct(); // common tasks for all constructor types

	// check if volumes are the same or not the same
	bool operator == (const volume& volumeB) const;
	bool operator != (const volume& volumeB) const;

	// assignment operator
	void operator = (const volume& volumeB);
	void operator = (const float setVal); // set entire volume to a value
	// volume& operator = (volume& volumeB);
	// volume operator = (volume& volumeB);
	// volume operator = (const volume&& volumeB);

	// multiplication operator
	volume& operator *= (const float multVal);
	volume& operator *=(const volume& volumeB);
	volume operator *(const float multVal) const;
	volume operator *(const volume& volumeB) const;

	// division operator
	volume& operator /= (const float divVal);
	volume& operator /= (const volume& volumeB);
	volume operator / (const float divVal) const;
	volume operator / (const volume& volumeB) const;

	// addition operator
	volume& operator += (const volume& volumeB);
	volume& operator += (const float addVal);
	volume operator + (const float addVal) const;
	volume operator + (const volume& volumeB) const;

	// substraction operator
	volume& operator -= (const volume& volumeB);
	volume& operator -= (const float subsVal);
	volume operator -(const float subsVal) const;
	volume operator -(const volume& volumeB) const;

	float& operator[] (const std::size_t idx);
	float operator[] (const std::size_t idx) const;

	// return a 3d indexed element of the matrix
	const float operator()(const std::size_t ix, const std::size_t iy, const std::size_t iz);

	// important properties
	float get_minVal() const {return minVal;};
	float get_maxVal() const {return maxVal;};
	float get_maxAbsVal() const {return maxAbsVal;};

	// dimensionality of the dataset	
	void set_dim(const std::size_t dim0, const std::size_t dim1, const std::size_t dim2);
	void set_dim(const std::size_t* _dim);
	void set_dim(const std::size_t _dim, const std::size_t newDim);
	std::size_t get_dim(const std::size_t _dim) const; // returns dimension along axis

	// define resolution of the dataset
	void set_res(const float* dx);
	void set_res(const float dx0, const float dx1, const float dx2);
	void set_res(const std::size_t _dim, const float _res);

	// get resolution functions
	float get_res(const std::size_t _dim) const {return res[_dim];};
	float get_res0() const {return res[0];}; // get resolution along first dimension
	float get_res1() const {return res[1];}; // get resolution along second dimension
	float get_res2() const {return res[2];}; // get resolution along third dimension

	// define origin of the dataset
	void set_origin(const float* _origin);
	void set_origin(const std::size_t _dim, const float _origin);
	void set_origin(const float origin0, const float origin1, const float origin2);
	float get_origin(const std::size_t _dim) const {return origin[_dim];};

	// different ways to define matrix elements
	void set_value(const float value); // set whole array to certain value
	void set_value(const std::size_t x0, const std::size_t x1, const std::size_t x2, const float value);
	void set_value(const std::size_t* pos, const float value);
	void set_value(const std::size_t iElem, const float value);

	// return the value of a certain position
	float get_value(const std::size_t x0, const std::size_t x1, const std::size_t x2) const;
	float get_value(const std::size_t iElem) const;
	float get_value(const std::size_t* pos) const;

	// get position along axis
	float get_pos0(const std::size_t idx0) const;
	float get_pos1(const std::size_t idx1) const;
	float get_pos2(const std::size_t idx2) const;
	float get_pos(const std::size_t idx, const std::size_t iDim) const;

	float get_centerPos(const std::size_t _dim); // returns center position along dimension

	// get index of a certain position
	std::size_t get_idx0(const float pos0) const;
	std::size_t get_idx1(const float pos1) const;
	std::size_t get_idx2(const float pos2) const;
	std::size_t get_idx(const float pos, const std::size_t iDim) const;

	float get_length(const std::size_t _dim) const; 
	// returns length of dataset along a certain dimension

	std::size_t get_nElements() const;
	void alloc_memory(); // allocates the memory of our array and slices
	void free_memory(); // frees the memory of our arrays and slices

	void readFromFile(const string _filePath); // read from file, distinguish type by ending
	void saveToFile(const string _filePath) const; // save to file, distinguish type by ending

	// reading and writing from and to h5
	void read_h5(const string _filePath);
	void save_h5(const string _filePath) const;

	// read and save from and to nii
	void read_nii(const string _filePath);
	void save_nii(const string _filePath) const;

	void print_information() const; 

	float get_minPos(const uint8_t _dim) const; // returns minimum position along dimension
	float get_maxPos(const uint8_t _dim) const;
	float getRangeLimitedPos(const float pos, const uint8_t _dim) const;

	void getCroppedVolume(
		float* vol, // array containing cropped volume
		const std::size_t start0, const std::size_t stop0,
		const std::size_t start1, const std::size_t stop1,
		const std::size_t start2, const std::size_t stop2) const;

	void get_croppedVolume(
		float* vol, // array containing cropped volume 
		const std::size_t* startIdx, 
		const std::size_t* stopIdx) const;

	void crop(const std::size_t* startIdx, const std::size_t* stopIdx);

	void exportVtk(const string filePath);

	void calcMinMax();
	void calcMips();

	// everything related to cropped mips
	void calcCroppedMips();
	void calcCroppedMips(const float* cropRange);
	float* get_croppedMipZ();
	float* get_croppedMipZ(const float* _cropZ);
	float* get_croppedMipX();
	float* get_croppedMipX(const float* _cropX);
	float* get_croppedMipY();
	float* get_croppedMipY(const float* _cropY);
	void set_cropRangeX(const float* _cropX);
	void set_cropRangeY(const float* _cropY);
	void set_cropRangeZ(const float* _cropZ);
	bool get_updatedCropRange() const {return updatedCropRange;};

	float get_minValCrop() const {return minValCrop;};
	float get_maxValCrop() const {return maxValCrop;};

	float* get_pdata() {return data;};
	const float* get_pdata() const {return data;};
	void set_pdata(float* _data);

	// get slices of volume
	float* get_psliceZ(const std::size_t zLevel);
	float* get_psliceX(const std::size_t xLevel);
	float* get_psliceY(const std::size_t yLevel);
	float* get_psliceZ(const float zPos);
	float* get_psliceX(const float xPos);
	float* get_psliceY(const float yPos);

	float* get_mipX() {return mipX;};
	float* get_mipY() {return mipY;};
	float* get_mipZ() {return mipZ;};

	void normalize();
	float get_norm() const;
	void fill_rand(); // fill array with random values
	void fill_rand(const float maxVal); // fill array with random values
	void fill_rand(const float minVal, const float maxVal); // fill array with random values

private:
	std::string inPath; // path pointing to our input file

	std::size_t dim[3] = {0, 0, 0}; // dimensionailty of volume
	std::size_t nElements = 0; // overall number of elements in 
	float origin[3] = {0, 0, 0}; // origin of volume
	float res[3] = {1, 1, 1}; // resolution of volume

	float minVal = 0; // minimum value in full dataset
	float maxVal = 0; // maximum value in full dataset
	float maxAbsVal = 0; // absolute maximum value in full dataset

	float* data; // matrix containing data
	bool isMemAlloc = 0; // did we allocate memory

	// z slice array which will be only updated if new slice is requested
	float* sliceZ; // indexing [iX, iY]
	std::size_t lastSliceZ = 0;
	float* sliceX; // indexing [iZ, iY]
	std::size_t lastSliceX = 0;
	float* sliceY; // indexing [iX, iZ]
	std::size_t lastSliceY = 0;

	// maximum intensity projections
	float* mipZ; // indexing: [iX, iY], iX + nX * iY
	float* mipX; // indexing: [iZ, iY], iZ + nZ * iY
	float* mipY; // indexing: [iX, iZ], iX + nX * iZ

	// maximum intensity projections (cropped)
	float* croppedMipZ; // indexing: [iX, iY], iX + nX * iY
	float* croppedMipX; // indexing: [iZ, iY], iZ + nZ * iY
	float* croppedMipY; // indexing: [iX, iZ], iX + nX * iZ

	float cropRange[6] = {0, 0, 0, 0, 0, 0}; // crop range for submips
	bool updatedCropRange = 0; // indicator if the cropped boundaries were updated
	// order: zMin, zMax, xMin, xMax, yMin, yMax
	float minValCrop = 0;
	float maxValCrop = 0;

	const int processor_count; //!< number of CPU processing units
	
	vector<std::thread> workers;

	nifti_1_header hdr;
};

#endif
