#ifndef COHE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape {

public:
	Cone() { num_points = x_points * y_segments; };


	double Intersect(Point eyePointP, Vector rayV)
	{
		double A, B, C; 


		double shortest_t = -1;
		double temp_t;
		double temp_t2;
		double temp_x;
		double temp_z;
		double plane_y = -SCALE;

		A = rayV[0] * rayV[0] + rayV[2] * rayV[2] - RADIUS*RADIUS*rayV[1]*rayV[1];
		B = 2*eyePointP[0]*rayV[0] + 2*eyePointP[2]*rayV[2] - RADIUS*eyePointP[1]*rayV[1] + RADIUS*RADIUS*rayV[1];
		C = (eyePointP[0] * eyePointP[0]) + (eyePointP[2]*eyePointP[2]) + RADIUS*RADIUS*eyePointP[1] - RADIUS*RADIUS*eyePointP[1]*eyePointP[1] - RADIUS*RADIUS*RADIUS*RADIUS; 

		//sides of the cylinder
		float determinant = B * B - 4 * A * C; 
		if (determinant < 0) {
			//std::cerr << "No intersect" << std::endl;
			shortest_t = -1;
		} else if (determinant == 0) {
			//std::cerr << "One intersection at " << -1 * B/ (2 * A) << std::endl;
			shortest_t = (-1 * B) / (2* A);
			//std::cerr << shortest_t << " at line 41"<<std::endl;
		} else {
			temp_t = (-1 * B - sqrt(determinant)) / (2 * A);
			temp_t2 =(-1 * B + sqrt(determinant)) / (2 * A);
			if (temp_t < temp_t2 && temp_t > 0) {
				shortest_t = temp_t;
			} else if (temp_t2 > 0) {
				shortest_t = temp_t2;
			} else {
				shortest_t = -1;
			}
			
		}
		double temp_y = eyePointP[1] + rayV[1] * shortest_t;
		if (temp_y < -0.5 || temp_y > 0.5) {
			shortest_t = -1;
		}

		//bottom faces
		temp_t = (plane_y - eyePointP[1]) / rayV[1];
		temp_x = eyePointP[0] + rayV[0] * (temp_t);
		temp_z = eyePointP[2] + rayV[2] * (temp_t);
		if ((temp_x * temp_x) + (temp_z * temp_z) <= RADIUS * RADIUS) {
			if (temp_t > 0) {
				if (shortest_t < 0 || temp_t < shortest_t) {
					shortest_t = temp_t;
					//std::cerr << shortest_t << " at line 64"<<std::endl;
				}
			}
		}
		/*if (shortest_t == 0) {
			std::cerr << "No intersection found." <<std::endl;
		} else {
			std::cerr << "Cone Intersecting at (" << eyePointP[0] + rayV[0] * (temp_t) << ", " << eyePointP[1] + rayV[1] * (temp_t) << ", " << eyePointP[2] + rayV[2] * (temp_t) << ")" << std::endl;
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
		double slope = SCALE / HEIGHT;

		temp_x  = eyePoint[0] + ray[0] * dist;
		temp_y  = eyePoint[1] + ray[1] * dist;
		temp_z  = eyePoint[2] + ray[2] * dist;
		if (currentMaterial->textureMap != NULL && currentMaterial->textureMap->isUsed) {
			ppmWidth = currentMaterial->openedTMap->getWidth();
			ppmHeight = currentMaterial->openedTMap->getHeight();
			colors = currentMaterial->openedTMap->getPixels();
			isTextured = true;
		}
		
		if (fabs(temp_y + SCALE) <= 0.00001) {
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
			return Vector(temp_x, sqrt(temp_x*temp_x + temp_z*temp_z) * slope, temp_z);
		}
	};


	~Cone() {};

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
        double radius_increment = SCALE/y_segments;
        num_points = x_points * y_segments;
        points = new Point[num_points];

        for (int i = 0; i < y_segments; i++) {
            for (int j = 0; j < x_points; j++) {
                points[j + (x_points * i)] = 
                    Point((radius_increment * (i + 1)) * sin(j * theta), 
                          SCALE - ((i + 1) * y_increment),
                          (radius_increment * -1 * (i + 1)) * cos(j * theta));
            }
        }
    };

    /* 
     * draw_top_normals - draws normals derived from the sides at the 
     * top tip of the cone.
     * */
    void draw_top_normals()
    {
        double theta = 2*PI/x_segments;
        double slope = SCALE/HEIGHT;

        for (int i = num_points - x_points; i < num_points - 1; i++) {
            glBegin(GL_LINES);
                glVertex3f(top[X], top[Y], top[Z]);
                glVertex3f(top[X] + (N_MAG * cos((i % x_points) * theta)), 
                           top[Y] + (N_MAG * slope), 
                           top[Z] + (N_MAG * -sin((i % x_points) * theta)));
            glEnd(); 
        }
    };
    
    /*
     * draw_side_normals - draw the cone normals, minus the tip and the
     * bottom face.
     */
	void draw_side_normals() 
    {
        double theta = 2*PI/x_segments;
        double slope = SCALE/HEIGHT;
        
        for (int i = 0; i < num_points; i++) {
            glBegin(GL_LINES);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]); 
                glVertex3f(points[i][X] + 
                                (N_MAG * sin((i % x_points) * theta)), 
                           points[i][Y] + (N_MAG * slope), 
                           points[i][Z] + 
                                (N_MAG * -cos((i % x_points) * theta)));
            glEnd();
        }
	};

    /** override function implmented in Shape superclass **/
    void draw_top_center_normals() {};
};

#endif
