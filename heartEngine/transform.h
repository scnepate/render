#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "geometry.h"
#include "model.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265359

inline void move (Model &object, Vec3f t)
{
	for (int i = 0; i < (int)object.verts_.size (); ++ i)
		object.verts_[i] = object.verts_[i] + t;
	object.Center ();
}

inline void rotateX (Model &object, Vec3f pivot, float angle)
{
	for (int i = 0; i < (int)object.verts_.size (); ++ i)
	{
		Vec3f temp = object.verts_[i];
		Vec3f p    = temp - pivot;
		float l    = sqrt (p.z*p.z + p.y*p.y);

		float cur_angle = atan2 (p.y, p.z);
		if (cur_angle < 0) cur_angle += 2.*PI;

		//std::cout << "cur_angle = " <<  cur_angle << "\n";
		temp.z = cos (cur_angle+angle/180.*PI) * l + pivot.z;
		temp.y = sin (cur_angle+angle/180.*PI) * l + pivot.y;
		object.verts_[i] = temp;
	}
	object.Center ();
}


inline void rotateY (Model &object, Vec3f pivot, float angle)
{
	for (int i = 0; i < (int)object.verts_.size (); ++ i)
	{
		Vec3f temp = object.verts_[i];
		Vec3f p    = temp - pivot;
		float l    = sqrt (p.x*p.x + p.z*p.z);

		float cur_angle = atan2 (p.z, p.x);
		if (cur_angle < 0) cur_angle += 2.*PI;

		//std::cout << "cur_angle = " <<  cur_angle*180/PI << "\n";
		temp.x = cos (cur_angle+angle/180.*PI) * l + pivot.x;
		temp.z = sin (cur_angle+angle/180.*PI) * l + pivot.z;
		object.verts_[i] = temp;
	}
	object.Center ();
}


inline void rotateZ (Model &object, Vec3f pivot, float angle)
{
	for (int i = 0; i < (int)object.verts_.size (); ++ i)
	{
		Vec3f temp = object.verts_[i];
		Vec3f p    = temp - pivot;
		float l    = sqrt (p.x*p.x + p.y*p.y);

		float cur_angle = atan2 (p.x, p.y);
		if (cur_angle < 0) cur_angle += 2.*PI;

		//std::cout << "cur_angle = " <<  cur_angle << "\n";
		temp.y = cos (cur_angle+angle/180.*PI) * l + pivot.y;
		temp.x = sin (cur_angle+angle/180.*PI) * l + pivot.x;
		object.verts_[i] = temp;
	}
	object.Center ();
}

#endif // _TRANSFORM_
