#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model
{
public:
	std::vector<Vec3f> verts_;
	std::vector <std::vector<int> > faces_;
	Vec3f pivot_;
	Model (const char *filename);
	~Model ();
	int nverts ();
	int nfaces ();
	Vec3f Center ();
	Vec3f pivot ();
	Vec3f vert (int i);
	std::vector <int> face (int idx);
};

#endif //__MODEL_H__
