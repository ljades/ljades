#ifndef SPHERE_H 
#define SPHERE_H

#include "Shape.h"
#include <errno.h>

class Sphere : public Shape {

public:
	Sphere() { num_points = x_points * (y_segments - 1); };

	double Intersect(Point eyePointP, Vector rayV)
	{
		double A, B, C; 
		double temp_t, temp_t2;

		//std::cout<<"EyepointP is ("<<eyePointP[0] <<", " <<eyePointP[1] <<", " <<eyePointP[2] <<")" <<std::endl;
		//std::cout<<"rayV is ("<<rayV[0] <<", " <<rayV[1] <<", " <<rayV[2] <<")" <<std::endl;
		Vector eyeVector = eyePointP - Point(0, 0, 0);
		A = dot(rayV, rayV);
		B = 2.0 * dot(eyeVector, rayV);
		C = dot(eyeVector, eyeVector) - (RADIUS * RADIUS); 

		float determinant = B * B - 4.0 * A * C; 
		//std::cout << "A is: " << A << ", B is: " << B << ", C is: "<< C << std::endl << "Determinant is: " << determinant << std::endl;

		//exit(0);
		if (determinant < 0) {
			//std::cerr << "No intersect" << std::endl;
			return -1;
		} else if (determinant == 0) {
			//std::cerr << "One intersection at " << -1.0 * B/ (2.0 * A) << std::endl;
			return (-1.0 * B) / (2.0 * A);
		} else {
			//std::cerr << "Two intersections at " << (-1.0 * B + sqrt(determinant)) / (2.0 * A)
			//		  << " and "                 << (-1.0 * B - sqrt(determinant)) / (2.0 * A) <<std::endl;
			temp_t = (-1 * B - sqrt(determinant)) / (2 * A);
			temp_t2 =(-1 * B + sqrt(determinant)) / (2 * A);
			if (temp_t < temp_t2
				&& temp_t > 0.00001) {
					return temp_t;
			} else if (temp_t2 < temp_t
				&& temp_t2 > 0.00001) {
					return temp_t2;
			} else {
				return -1;
			}
		}
			
	};
	Vector findIsectNormal(Point eyePoint, Vector ray, double dist, double *texR, double *texG, double *texB, SceneMaterial *currentMaterial)
	{
		double temp_x;
		double temp_y;
		double temp_z;
		double theta, phi, unit_x, unit_y;
		int texX, texY;
		int ppmWidth, ppmHeight;
		char *colors;


		temp_x  = eyePoint[0] + ray[0] * dist;
		temp_y  = eyePoint[1] + ray[1] * dist;
		temp_z  = eyePoint[2] + ray[2] * dist;
		if (currentMaterial->textureMap != NULL && currentMaterial->textureMap->isUsed) {
			ppmWidth = currentMaterial->openedTMap->getWidth();
			ppmHeight = currentMaterial->openedTMap->getHeight();
			colors = currentMaterial->openedTMap->getPixels();
			theta = atan2(temp_z, temp_x);
			if (theta < -0.00000000001) {
				unit_x = -theta / (2.0 * PI);
			} else if (theta >= 0.0000000001) {
				unit_x = 1 - (theta / (2.0 * PI));
			} else {
				unit_x = 0;
			}
			phi = asin(temp_y / RADIUS);
			unit_y = (phi / PI) + SCALE;
			texX = (int)((unit_x * currentMaterial->textureMap->repeatU) * (double)ppmWidth) % ppmWidth;
			texY = (int)((unit_y * currentMaterial->textureMap->repeatV) * (double)ppmHeight) % ppmHeight;
			*texR = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3]) / 255.0;
			*texG = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 1]) / 255.0;
			*texB = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 2]) / 255.0;
		}
		return Vector(temp_x, temp_y, temp_z);
	};

	~Sphere() {};

private:
    /*
     * calculate_points - (re)calculate point locations for all faces at
     * program initiation and upon change in either m_segmentsX or m_segmentsY
     */
    void calculate_points()
    {
        if (points != NULL) {
            delete [] points;
        }
        double theta = 2*PI/x_segments;
        double phi = PI/y_segments; /* increment in phi */
        num_points = x_points * y_points;
        points = new Point[num_points];

        for (int i = 0; i < y_segments; i++) {
            for (int j = 0; j < x_points; j++) {
                points[j + (x_points * i)] = 
                    Point(SCALE * cos(j * theta) * sin((i + 1) * phi),
                          SCALE * cos((i + 1) * phi),
                          SCALE * sin(j * theta) * sin((i + 1) * phi));
            }
        }
    };

    /* draw_side_normals - draw the sphere normals */
	void draw_side_normals() 
    {
        for (int i = 0; i < num_points; i++) {
            glBegin(GL_LINES);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]);
                glVertex3f(points[i][X] * 1.2,
                           points[i][Y] * 1.2,
                           points[i][Z] * 1.2);
            glEnd();
        }
	};

    /** override functions implmented in Shape superclass **/
    void draw_top_normals() {};
    void draw_bottom_normals() {};
};

#endif
