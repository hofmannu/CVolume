/*
	File: basicMathOp.h
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 05.08.2020

	Description: Implementation of basic mathematical functions applied over arrays

	Changelog:
		2023-12-26: added static declaration to all functions of this class
*/

#ifndef BASICMATHOP_H
#define BASICMATHOP_H

#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

enum class PolarityHandling {
	ABS,
	POS,
	NEG,
	NONE
};

class basicMathOp
{
public:
	static void handlePolarity(float* _array,
	                           std::size_t _nElements,
	                           PolarityHandling _method);

	static float getNorm(const float* _array,
	                     std::size_t _nElements);

	// MULTIPLICATION
	// _array = _array * factor
	static void multiply(float* _array,
	                     const float _factor,
	                     std::size_t _nElements) ;

	// _arrayA = _arrayA * _arrayB
	static void multiply(float* _arrayA,
	                     const float*  _arrayB,
	                     std::size_t _nElements) ;

	// _arrayA = _arrayB * arrayC
	static void multiply(float* _arrayA,
	                     const float* _arrayB,
	                     const float* _arrayC,
	                     std::size_t _nElements);

	// _arrayA = _arrayB * _factor
	static void multiply(float* _arrayA,
	                     const float* _arrayB,
	                     float factor,
	                     std::size_t _nElements) ;

	// DIVISIONS
	// _array = _array / _factor
	static void divide(float* _array,
	                   const float _factor,
	                   std::size_t _nElements);

	// _arrayA = _arrayA / _arrayB
	static void divide(float* _arrayA,
	                   const float* _arrayB,
	                   std::size_t _nElements) ;
	// _arrayA = _arrayB / _arrayC
	static void divide(float* _arrayA,
	                   const float* _arrayB,
	                   const float* _arrayC,
	                   std::size_t _nElements) ;
	// _arrayA = _arrayB / _factor
	static void divide(float* _arrayA,
	                   const float* _arrayB,
	                   const float factor,
	                   std::size_t _nElements) ;

	static void normalize(float* _array,
	                      std::size_t _nElements) ;

	// SUBSTRACTIONS
	// arrayA = arrayA - arrayB
	static void substract(float* _arrayA,
	                      const float* _arrayB,
	                      std::size_t _nElements) ;
	// arrayA = arrayA - value
	static void substract(float* _arrayA,
	                      const float _value,
	                      std::size_t _nElements) ;
	// arrayA = arrayB - arrayC
	static void substract(float* _arrayA,
	                      const float* _arrayB,
	                      const float* _arrayC,
	                      std::size_t _nElements) ;

	// ADDITIONS
	// arrayA = arrayA + arrayB
	static void add(float* _arrayA,
	                const float* _arrayB,
	                std::size_t _nElements) ;
	// arrayA = arrayA + value
	static void add(float* _arrayA,
	                float _value,
	                std::size_t _nElements) ;

	// arrayA = arrayB + arrayC
	static void add(float* _arrayA,
	                const float* _arrayB,
	                const float* _arrayC,
	                std::size_t _nElements) ;

	// set an array to equal elements for its full length _arrayOut = _arrayIn
	static void assign(float* _arrayOut,
	                   const float* _arrayIn,
	                   std::size_t _nElements) ;

	// set all elements in array to random values
	static void assignRand(float* _array,
	                       std::size_t _nElements) ;

	[[nodiscard]] static float getMaxAbs(const float* _array,
	                                     std::size_t _nElements) ;
	// returns maximum absolute value in array
	[[nodiscard]] static float getMin(const float* _array,
	                                  std::size_t _nElements);

	[[nodiscard]] static float getMax(const float* _array,
	                                  std::size_t _nElements);


};

#endif