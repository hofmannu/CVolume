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
		hofmannu - move all those raw allocations to vector
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
	volume(const std::size_t _dim0,
	       const std::size_t _dim1,
	       const std::size_t _dim2);
	volume(const volume& obj);

	// check if volumes are the same or not the same
	[[nodiscard]] bool operator == (const volume& volumeB) const;
	[[nodiscard]] bool operator != (const volume& volumeB) const;

	// assignment operator
	void operator = (const volume& volumeB);

	/// \brief asign constant value to all entries of data
	/// \param setVal value to assign
	void operator = ( float setVal);

	// volume& operator = (volume& volumeB);
	// volume operator = (volume& volumeB);
	// volume operator = (const volume&& volumeB);

	// multiplication operator
	volume& operator *= ( float multVal);
	volume& operator *=(const volume& volumeB);
	volume operator *( float multVal) const;
	volume operator *(const volume& volumeB) const;

	// division operator
	volume& operator /= ( float divVal);
	volume& operator /= (const volume& volumeB);
	volume operator / ( float divVal) const;
	volume operator / (const volume& volumeB) const;

	// addition operator
	volume& operator += (const volume& volumeB);
	volume& operator += ( float addVal);
	volume operator + ( float addVal) const;
	volume operator + (const volume& volumeB) const;

	// substraction operator
	volume& operator -= (const volume& volumeB);
	volume& operator -= ( float subsVal);
	volume operator -( float subsVal) const;
	volume operator -(const volume& volumeB) const;

	[[nodiscard]] float& operator[] ( std::size_t idx);
	[[nodiscard]] float operator[] ( std::size_t idx) const;

	// return a 3d indexed element of the matrix
	[[nodiscard]] const float operator()( std::size_t ix,
	                                      std::size_t iy,
	                                      std::size_t iz);

	// important properties
	[[nodiscard]] float get_minVal() const {return minVal;};
	[[nodiscard]] float get_maxVal() const {return maxVal;};
	[[nodiscard]] float get_maxAbsVal() const {return maxAbsVal;};

	// dimensionality of the dataset
	void set_dim( std::size_t dim0,  std::size_t dim1,  std::size_t dim2);
	void set_dim( const std::size_t* _dim);
	void set_dim( std::size_t _dim,  std::size_t newDim);
	[[nodiscard]] std::size_t get_dim( std::size_t _dim) const; // returns dimension along axis

	// define resolution of the dataset
	void set_res(const float* dx);
	void set_res( float dx0,  float dx1,  float dx2);
	void set_res( std::size_t _dim,  float _res);

	// get resolution functions
	[[nodiscard]] float get_res(const std::size_t _dim) const {return res[_dim];};
	[[nodiscard]] float get_res0() const {return res[0];}; // get resolution along first dimension
	[[nodiscard]] float get_res1() const {return res[1];}; // get resolution along second dimension
	[[nodiscard]] float get_res2() const {return res[2];}; // get resolution along third dimension

	// define origin of the dataset
	void set_origin(const float* _origin);
	void set_origin( std::size_t _dim,  float _origin);
	void set_origin( float origin0,  float origin1,  float origin2);
	[[nodiscard]] float get_origin( std::size_t _dim) const {return origin[_dim];};

	// different ways to define matrix elements
	void set_value( float value); // set whole array to certain value
	void set_value( std::size_t x0,  std::size_t x1,  std::size_t x2, const float value);
	void set_value(const std::size_t* pos,  float value);
	void set_value( std::size_t iElem, const float value);

	// return the value of a certain position
	[[nodiscard]] float get_value( std::size_t x0,  std::size_t x1,  std::size_t x2) const;
	[[nodiscard]] float get_value( std::size_t iElem) const;
	[[nodiscard]] float get_value( const std::size_t* pos) const;

	// get position along axis
	[[nodiscard]] float get_pos0( std::size_t idx0) const;
	[[nodiscard]] float get_pos1( std::size_t idx1) const;
	[[nodiscard]] float get_pos2( std::size_t idx2) const;
	[[nodiscard]] float get_pos( std::size_t idx, const std::size_t iDim) const;

	/// \brief returns the center position along dimension
	/// \param _dim the dimension along which we want to receive the center
	[[nodiscard]] float get_centerPos(std::size_t _dim) const;

	// get index of a certain position
	[[nodiscard]] std::size_t get_idx0( float pos0) const;
	[[nodiscard]] std::size_t get_idx1( float pos1) const;
	[[nodiscard]] std::size_t get_idx2( float pos2) const;
	[[nodiscard]] std::size_t get_idx( float pos,  std::size_t iDim) const;

	[[nodiscard]] float get_length(const std::size_t _dim) const;
	// returns length of dataset along a certain dimension

	[[nodiscard]] std::size_t get_nElements() const;
	void alloc_memory(); // allocates the memory of our array and slices

	void readFromFile(const std::string& _filePath); // read from file, distinguish type by ending
	void saveToFile(const std::string& _filePath) const; // save to file, distinguish type by ending

	// reading and writing from and to h5
	void read_h5(const std::string& _filePath);
	void save_h5(const std::string& _filePath) const;

	// read and save from and to nii
	void read_nii(const std::string& _filePath);
	void save_nii(const std::string& _filePath) const;

	void print_information() const;

	/// \brief returns minimum position along dimension
	/// \param _dim the dimension along which we want to receive the min position
	[[nodiscard]] float get_minPos(const uint8_t _dim) const;

	/// \brief returns maximum position along dimension
	/// \param _dim the dimension along which we want to receive the min position
	[[nodiscard]] float get_maxPos(const uint8_t _dim) const;

	[[nodiscard]] float getRangeLimitedPos(const float pos, const uint8_t _dim) const;

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

	void exportVtk(const std::string& filePath);

	void calcMinMax();
	void calcMips();

	// everything related to cropped mips
	void calcCroppedMips();
	void calcCroppedMips(const float* cropRange);
	[[nodiscard]] float* get_croppedMipZ();
	[[nodiscard]] float* get_croppedMipZ(const float* _cropZ);
	[[nodiscard]] float* get_croppedMipX();
	[[nodiscard]] float* get_croppedMipX(const float* _cropX);
	[[nodiscard]] float* get_croppedMipY();
	[[nodiscard]] float* get_croppedMipY(const float* _cropY);
	void set_cropRangeX(const float* _cropX);
	void set_cropRangeY(const float* _cropY);
	void set_cropRangeZ(const float* _cropZ);
	[[nodiscard]] bool get_updatedCropRange() const {return updatedCropRange;}

	float get_minValCrop() const {return minValCrop;}
	float get_maxValCrop() const {return maxValCrop;}

	/// \brief get a pointer to the data array
	/// \returns a pointer to the data array
	[[nodiscard]] float* get_pdata() {return data.data();}

	/// \brief return a pointer to the const data array
	/// \returns a pointer to the const data array
	[[nodiscard]] const float* get_pdata() const {return data.data();}

	// get slices of volume
	[[nodiscard]] float* get_psliceZ(const std::size_t zLevel);
	[[nodiscard]] float* get_psliceX(const std::size_t xLevel);
	[[nodiscard]] float* get_psliceY(const std::size_t yLevel);
	[[nodiscard]] float* get_psliceZ(const float zPos);
	[[nodiscard]] float* get_psliceX(const float xPos);
	[[nodiscard]] float* get_psliceY(const float yPos);

	[[nodiscard]] float* get_mipX() {return mipX.data();};
	[[nodiscard]] float* get_mipY() {return mipY.data();};
	[[nodiscard]] float* get_mipZ() {return mipZ.data();};

	void normalize();
	[[nodiscard]] float get_norm() const;
	void fill_rand(); // fill array with random values
	void fill_rand(const float maxVal); // fill array with random values
	void fill_rand(const float minVal, const float maxVal); // fill array with random values

private:
	std::string inPath; // path pointing to our input file

	std::size_t dim[3] = {0, 0, 0}; // dimensionailty of volume
	std::size_t nElements = 0; // overall number of elements in
	float origin[3] = {0.0f, 0.0f, 0.0f}; // origin of volume
	float res[3] = {1.0f, 1.0f, 1.0f}; // resolution of volume

	float minVal = 0.0f; // minimum value in full dataset
	float maxVal = 0.0f; // maximum value in full dataset
	float maxAbsVal = 0.0f; // absolute maximum value in full dataset

	std::vector<float> data; // matrix containing data

	// z slice array which will be only updated if new slice is requested
	std::vector<float> sliceZ; // indexing [iX, iY]
	std::size_t lastSliceZ = 0;
	std::vector<float> sliceX; // indexing [iZ, iY]
	std::size_t lastSliceX = 0;
	std::vector<float> sliceY; // indexing [iX, iZ]
	std::size_t lastSliceY = 0;

	// maximum intensity projections
	std::vector<float> mipZ; // indexing: [iX, iY], iX + nX * iY
	std::vector<float> mipX; // indexing: [iZ, iY], iZ + nZ * iY
	std::vector<float> mipY; // indexing: [iX, iZ], iX + nX * iZ

	// maximum intensity projections (cropped)
	std::vector<float> croppedMipZ; // indexing: [iX, iY], iX + nX * iY
	std::vector<float> croppedMipX; // indexing: [iZ, iY], iZ + nZ * iY
	std::vector<float> croppedMipY; // indexing: [iX, iZ], iX + nX * iZ

	// crop range for submips
	float cropRange[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	bool updatedCropRange = false; // indicator if the cropped boundaries were updated
	// order: zMin, zMax, xMin, xMax, yMin, yMax
	float minValCrop = 0.0f;
	float maxValCrop = 0.0f;

	const int processor_count; //!< number of CPU processing units
	std::vector<std::thread> workers;

	nifti_1_header hdr;
};

#endif
