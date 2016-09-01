/****************************************************************************/
/*!
\file Boundary2D.h
\author Tan Teck Ling
\par email: 153782U@mymail.nyp.edu.sg
\brief
Inherits from the Boundary class to perform non-axis aligned collision.
*/
/****************************************************************************/
#ifndef BOUNDARY2D_H
#define BOUNDARY2D_H

#include "Boundary.h"
#include "Vector3.h"

class Boundary2D : public Boundary
{
public:
	Boundary2D();
	Boundary2D(const Vector3 &pos, const Vector3 &scale, const float &rotationAngle = 0);
	Boundary2D(const Boundary2D &bounds);
	virtual ~Boundary2D();

	virtual bool CheckCollision(const Boundary2D &object)const;
	virtual bool CheckCollision(const Vector3 &point)const;
};
#endif