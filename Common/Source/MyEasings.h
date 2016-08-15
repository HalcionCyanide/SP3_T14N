///////////////////////////////////////////////////////////////////////////////
/*!
\file   MyEasings.h
\author Lim Rui An, Ryan
\brief
Header file for easing helper functions
*/
///////////////////////////////////////////////////////////////////////////////
#ifndef MY_EASINGS_H
#define MY_EASINGS_H

#include <cmath>
#include <exception>
#include <ctime>
#include <cstdlib>

/******************************************************************************/
/*!
\brief
Easing namespace
*/
/******************************************************************************/
namespace Easing
{
	template<typename T>
	// No Easing, No Acceleration
	T LinearTween(T CurrentTime, T StartVal, T IntervalOfChange, T Duration)
	{
		return IntervalOfChange * CurrentTime/Duration + StartVal;
	}

	template<typename T>
	// Accelerating from zero velocity
	T QuadraticEaseIn(T CurrentTime, T StartVal, T IntervalOfChange, T Duration)
	{
		CurrentTime /= Duration;
		return IntervalOfChange * CurrentTime * CurrentTime + StartVal;
	}

	template<typename T>
	// Decelerating to zero velocity
	T QuadraticEaseOut(T CurrentTime, T StartVal, T IntervalOfChange, T Duration)
	{
		CurrentTime /= Duration;
		return -IntervalOfChange * CurrentTime * (CurrentTime - 2) + StartVal;
	}

	template<typename T>
	// Accelerate until halfway, then decelerate
	T QuadraticEaseInOut(T CurrentTime, T StartVal, T IntervalOfChange, T Duration)
	{
		CurrentTime /= Duration;
		if (CurrentTime < 1) return IntervalOfChange / 2 * CurrentTime * CurrentTime + StartVal;
		t--;
		return -IntervalOfChange/2 * (CurrentTime * (CurrentTime - 2) - 1) + StartVal;
	}
	
}//end namespace Easing

/******************************************************************************/
/*!
Class DivideByZero:
\brief	Exception for divide by zero
*/
/******************************************************************************/
class DivideByZero : public std::exception
{
public:
	const char* what() { return "Divide By Zero"; }
};

#endif /// MY_EASINGS_H
