#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {

public:
	Cylinder() { num_points = x_points * y_points; };

	double Intersect(Point eyePointP, Vector rayV)
	{
		double A, B, C;
		double r0 = rayV[0];
		double r2 = rayV[2];
		double p0 = eyePointP[0];
		double p2 = eyePointP[2];
		double shortest_t = -1;
		double temp_t;
		double temp_t2;
		double temp_x;
		double temp_z;
		double plane_y = SCALE;

		A = r0*r0 + r2*r2;
		B = 2.0 * (r0*p0 + r2*p2);
		C = p0*p0 + p2*p2 - (RSQUARED); 

		//sides of the cylinder
		double determinant = B * B - 4 * A * C;
		if (determinant < 0) {
			//std::cerr << "No intersect" << std::endl;
			shortest_t = -1;
		} else if (determinant == 0) {
			//std::cerr << "One intersection at " << -1 * B/ (2 * A) << std::endl;
			shortest_t = -1 * B / (2* A);
		} else {
			//std::cerr << "Two intersections at " << (-1 * B + sqrt(determinant)) / (2 * A)
			//		  << " and "                 << (-1 * B - sqrt(determinant)) / (2 * A);
			double sqrtDet = sqrt(determinant);
			temp_t = (-1 * B - sqrtDet) / (2 * A);
			temp_t2 =(-1 * B + sqrtDet) / (2 * A);
			if (temp_t < temp_t2) {
					shortest_t = (-1 * B - sqrtDet) / (2 * A);
			} else if (temp_t2 < temp_t) {
					shortest_t = (-1 * B + sqrtDet) / (2 * A);
			} else {
				shortest_t = -1;
			}
		}
		double temp_y = eyePointP[1] + rayV[1] * shortest_t;
		if (temp_y < -0.5 || temp_y > 0.5) {
			shortest_t = -1;
		}
		/*
		//top and bottom faces
		if (rayV[1] != 0) {
			temp_t = (plane_y - eyePointP[1]) / rayV[1];
			temp_x = eyePointP[0] + rayV[0] * (temp_t);
			temp_z = eyePointP[2] + rayV[2] * (temp_t);
			if ((temp_x * temp_x) + (temp_z * temp_z) <= RSQUARED) {
				if (temp_t > 0) {
					if (shortest_t < 0 || temp_t < shortest_t) {
						shortest_t = temp_t;
					}
				}
			}
			plane_y = -SCALE;
			temp_t = (plane_y - eyePointP[1]) / rayV[1];
			temp_x = eyePointP[0] + rayV[0] * (temp_t);
			temp_z = eyePointP[2] + rayV[2] * (temp_t);
			if ((temp_x * temp_x) + (temp_z * temp_z) <= RSQUARED) {
				if (temp_t > 0) {
					if (shortest_t < 0 || temp_t < shortest_t) {
						shortest_t = temp_t;
					}
				}
			}
		}*/
		/*if (shortest_t == 0) {
			std::cerr << "No intersection found." <<std::endl;
		} else {
			std::cerr << "Cylinder Intersecting at (" << eyePointP[0] + rayV[0] * (temp_t) << ", " << eyePointP[1] + rayV[1] * (temp_t) << ", " << eyePointP[2] + rayV[2] * (temp_t) << ")" << std::endl;
		}*/
		return shortest_t;
	};
	Vector findIsectNormal(Point eyePoint, Vector ray, double dist, double *texR, double *texG, double *texB, SceneMaterial *currentMaterial)
	{
		double temp_x;
		double temp_y;
		double temp_z;
		double theta, unit_x;
		int texX, texY;
		int ppmWidth, ppmHeight;
		char *colors = NULL;
		bool isTextured = false;

		temp_x  = eyePoint[0] + ray[0] * dist;
		temp_y  = eyePoint[1] + ray[1] * dist;
		temp_z  = eyePoint[2] + ray[2] * dist;
		if (currentMaterial->textureMap != NULL && currentMaterial->textureMap->isUsed) {
			ppmWidth = currentMaterial->openedTMap->getWidth();
			ppmHeight = currentMaterial->openedTMap->getHeight();
			colors = currentMaterial->openedTMap->getPixels();
			isTextured = true;
		}


		if (fabs(temp_y - SCALE) <= 0.00001) {
			if (isTextured) {
				texX = (int)(((temp_x + SCALE) * currentMaterial->textureMap->repeatU) * (double)ppmWidth) % ppmWidth;
				texY = (int)(((temp_z + SCALE) * currentMaterial->textureMap->repeatV) * (double)ppmHeight) % ppmHeight;
				*texR = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3]) / 255.0;
				*texG = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 1]) / 255.0;
				*texB = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 2]) / 255.0;
			}
			return Vector(0, 1, 0);
		} else if (fabs(temp_y + SCALE) <= 0.00001) {
			if (isTextured) {
				texX = (int)(((temp_x + SCALE) * currentMaterial->textureMap->repeatU) * (double)ppmWidth) % ppmWidth;
				texY = (int)(((temp_z + SCALE) * currentMaterial->textureMap->repeatV) * (double)ppmHeight) % ppmHeight;
				*texR = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3]) / 255.0;
				*texG = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 1]) / 255.0;
				*texB = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 2]) / 255.0;
			}
			return Vector(0, -1, 0);
		} else {
			if (isTextured) {
				theta = atan2(temp_z, temp_x);
				if (theta < -0.00000000001) {
					unit_x = -theta / (2.0 * PI);
				} else if (theta >= 0.0000000001) {
					unit_x = 1 - (theta / (2.0 * PI));
				} else {
					unit_x = 0;
				}
				texX = (int)((unit_x * currentMaterial->textureMap->repeatU) * (double)ppmWidth) % ppmWidth;
				texY = (int)(((temp_y + SCALE) * currentMaterial->textureMap->repeatV) * (double)ppmHeight) % ppmHeight;
				*texR = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3]) / 255.0;
				*texG = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 1]) / 255.0;
				*texB = (double)((unsigned char)colors[((texY * ppmWidth) + texX) * 3 + 2]) / 255.0;
			}
			return Vector(temp_x, 0, temp_z);
		}
	};

	~Cylinder() {};
   
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
        double y_increment = 1.0/y_segments;
        num_points = x_points * y_points;
        points = new Point[num_points];
        
        for (int i = 0; i < y_points; i++) {
            for (int j = 0; j < x_points; j++) {
                points[j + (x_points * i)] = 
                    Point(SCALE * sin(j * theta), 
                          SCALE - (i * y_increment),
                          (-1.0 * SCALE) * cos(j * theta));
            }
        }
    };

    /*
     * draw_side_normals - draw the cylinder normals, minus the top and
     * bottom face.
     */
	void draw_side_normals() 
    {
        double theta = 2*PI/x_segments;
        
        for (int i = 0; i < num_points - 1; i++) {
            glBegin(GL_LINES);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]); 
                glVertex3f(points[i][X] + N_MAG * sin((i % x_points) * theta),
                           points[i][Y], 
                           points[i][Z] 
                            + (-1.0 * N_MAG) * cos((i % x_points) * theta));
            glEnd();
        }
	};
};

#endif
