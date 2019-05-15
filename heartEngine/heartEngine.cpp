#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "transform.h"
#include <cmath>
#include <iostream>
#include <algorithm>

#include <string>
#include <vector>

const TGAColor white = TGAColor (255, 255, 255, 255);
const TGAColor red   = TGAColor (255, 0,   0,   255);
const TGAColor blue  = TGAColor (0,   0,   255, 255);
const TGAColor green = TGAColor (0,   255, 0,   255);

const int A = 1280;
const int B = 720;
const int C = 555;
const int Ah = A/2;
const int Bh = B/2;
const int INF = 2020202020;

std::vector <int> zbuffer;

inline void triangle ( Vec3i t0, Vec3i t1, Vec3i t2, TGAImage &image, TGAColor color )
{
	if ( t0.y == t1.y && t1.y == t2.y ) return;
	if ( t0.y > t1.y ) std::swap ( t0, t1 );
	if ( t1.y > t2.y ) std::swap ( t1, t2 );
	if ( t0.y > t1.y ) std::swap ( t0, t1 );

	int total_height = t2.y - t0.y;

	for ( int i = 0; i < total_height; ++ i )
	{
		int second_half = i > t1.y-t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
		float alpha = (float)i/total_height;
		float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height;
		
		Vec3i AA =               t0 + Vec3f(t2-t0)*alpha;
	    Vec3i BB = second_half ? t1 + Vec3f(t2-t1)*beta : t0 + Vec3f(t1-t0)*beta;

		if ( AA.x > BB.x ) std::swap ( AA, BB );
		for ( int j = AA.x; j <= BB.x; ++ j )
		{
			float phi = BB.x==AA.x ? 1. : (float)(j-AA.x)/(float)(BB.x-AA.x);
			Vec3i P = Vec3f(AA) + Vec3f(BB-AA)*phi;
			int idx = P.x+P.y*B;
			
			if (P.x >= 1 && P.y >= 1 && P.x <= 1280 && P.y <= 720)
			if ( P.z > 0 && zbuffer[idx] < P.z)
			{
				// std::cout << idx << " " << AA.x << " " << BB.x << " " << j << std::endl;
				zbuffer[idx] = P.z;
				image.set ( P.x, P.y, color );
			}
		}
	}
}

std::vector < Vec3f > light_dir;

void draw_object ( Model &model, TGAImage &image, TGAColor color )
{
	for ( int i = 0; i < model.nfaces (); ++ i )
	{
		std::vector < int > face = model.face ( i );
		Vec3i screen_coords[3];
		Vec3f world_coords[3];

		for ( int j = 0; j < 3; ++ j )
		{
			Vec3f v = model.verts_[face[j]];
			screen_coords[j] = Vec3i ( v.x*A/300., v.y*B/150., v.z*25555 );
			world_coords[j] = v;
		}
		// std::cout << "screen: " << screen_coords[0] << " " << screen_coords[1] << " " << screen_coords[2] << std::endl;
		// std::cout << "world: " << world_coords[0] << " " << world_coords[1] << " " << world_coords[2] << std::endl;
		float intensity = 0.1;
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		n.normalize ();
		for ( int j = 0; j < (int)light_dir.size (); ++ j )
		{
			light_dir[j].normalize ();
			float temp = n*light_dir[j];
			if ( temp > 0 ) intensity += temp; 
		}
		triangle (screen_coords[0], screen_coords[1], screen_coords[2],
			      image,
				  TGAColor ( std::min ( (int)(color.r*intensity), 255 ),
				  std::min ( (int)(color.g*intensity), 255 ),
				  std::min ( (int)(color.b*intensity), 255 ), 255 )
				 );
	}
}


std::string frame ( int f )
{
	std::string name = "output/output";
	std::string num  = std::to_string ( f );
	std::string zero = "0";
	while ( zero.size () < 4-num.size () ) zero += "0";
	return name+zero+num+".tga";
}

int main (int argc, char **argv)
{
	TGAImage image ( A, B, TGAImage::RGB );
	zbuffer.resize (A*B, -INF);

	light_dir.push_back ( Vec3f ( 1, -2,  -1 ) );
	// light_dir.push_back ( Vec3f ( -1, 0.5, 0 ) );

	Model object ( "monkey.obj" );
	
	move ( object, Vec3f ( 135, 70, 1000 ) ); // INITIAL POSITION


	// TEST
	// rotateY ( object, object.pivot_, -35. );
	// draw_object ( object, image, TGAColor ( 230, 20, 20, 255 )  );
	// image.flip_vertically ();
	// image.write_tga_file ("./output/monkey.tga");
	// return 0;


	for ( int f = 0; f < 24*10; ++ f )
	{
		std::cout << "frame: " << f << std::endl;
		zbuffer.assign (A*B, -INF);
		image.clear ();
		rotateY ( object, object.pivot_, 3.75 );
		draw_object ( object, image, TGAColor ( 230, 20, 20, 255 )  );
		std::cout << frame ( f ) << std::endl;
		image.flip_vertically ();
		image.write_tga_file ( frame ( f ).c_str () );
	}
}
