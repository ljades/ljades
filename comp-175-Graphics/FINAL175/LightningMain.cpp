#define NUM_OPENGL_LIGHTS 8

#include "lightning.h"
using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;
int recursive_depth = 1;
int fractal_depth = 0;
int render_fog = 0;
int light_len = 5;
int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 0;
float eyeY = 1;
float eyeZ = 3;
float lookX = 0;
float lookY = 0;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data\\tests\\lightning.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();

void setupCamera();
void updateCamera();




FlattenedNode* flattened_parse = NULL;


/** Hopefully this recursive function is cool **/
void recursive_flatten(Matrix *transform, SceneNode *child) {
	Matrix rec_transform = Matrix();
	Matrix tmp_trans = Matrix();
	FlattenedNode* tmp_flattened;
	if (transform != NULL) {
		rec_transform = *transform * rec_transform;
	}
	if (child == NULL) {
		return;
	}
	for (int i = 0; i < child->transformations.size(); i++) {
		switch (child->transformations[i]->type)
		{
			case TRANSFORMATION_TRANSLATE:
				{
					tmp_trans = tmp_trans * trans_mat(child->transformations[i]->translate);
					break;
				}
			case TRANSFORMATION_SCALE:
				{
					tmp_trans = tmp_trans * scale_mat(child->transformations[i]->scale);
					break;
				}
			case TRANSFORMATION_ROTATE:
				{
					tmp_trans = tmp_trans * rot_mat(child->transformations[i]->rotate, child->transformations[i]->angle);
					break;
				}
			case TRANSFORMATION_MATRIX:
				{
					tmp_trans = tmp_trans * child->transformations[i]->matrix;
					break;
				}
			default:
				break;
		}
	}
	rec_transform = rec_transform * tmp_trans;
	Point eyePoint = camera->GetEyePoint();
	for (int i = 0; i < child->primitives.size(); i++) {
		tmp_flattened = new FlattenedNode();
		tmp_flattened->primitive = child->primitives[i];
		tmp_flattened->transform = rec_transform;
		tmp_flattened->invTransform = invert(rec_transform);
		tmp_flattened->objEyeP = tmp_flattened->invTransform * eyePoint;
		SceneMaterial *currentMaterial = &(tmp_flattened->primitive->material);
		if (currentMaterial->textureMap != NULL && currentMaterial->textureMap->isUsed) {
			currentMaterial->openedTMap = new ppm(currentMaterial->textureMap->filename);
		}
		tmp_flattened->next = flattened_parse;
		flattened_parse = tmp_flattened;
	}
	for (int i = 0; i < child->children.size(); i++) {
		recursive_flatten(&rec_transform, child->children[i]);
	}
}

void reset_flattened()
{
	FlattenedNode *tmp_reset;
	while(flattened_parse != NULL) {
		tmp_reset = flattened_parse;
		flattened_parse = flattened_parse->next;
		delete tmp_reset;
	}
	flattened_parse = NULL;
}





void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

double isInLoS(Point eyePoint, Point otherPoint, Vector rayBetween)
{
	Vector normalRay = rayBetween;
	FlattenedNode *tmp = NULL;
	Matrix invTrans;
	Point objEyeP;
	Vector objRay;
	double shortest_t = -1;
	double tmp_t = -1;
	normalRay.normalize();

	shortest_t = ((rayBetween[0] / normalRay[0]) + (rayBetween[1] / normalRay[1]) + (rayBetween[2] / rayBetween[2])) / 3.0;
	tmp = flattened_parse;
	/*while (tmp != NULL) {
		invTrans = tmp->invTransform;
		objEyeP = invTrans * otherPoint;
		objRay = invTrans * normalRay;
		if (tmp->primitive->type == SHAPE_CUBE) {
			tmp_t = cube->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_CYLINDER) {
			tmp_t = cylinder->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_CONE) {
			tmp_t = cone->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_SPHERE) {
			tmp_t = sphere->Intersect(objEyeP, objRay);
		}
		if (tmp_t > 0.0000001) {
			if ((shortest_t < 0 || tmp_t < shortest_t) && tmp->primitive->material.cTransparent.r < 0.00000001) {
				return -1;
			}
		}
		tmp = tmp->next;
	}*/
	return shortest_t;
}

bool isInLoS_Directional(Point eyePoint, Vector rayBetween)
{
	Vector normalRay = -rayBetween;
	FlattenedNode *tmp = NULL;
	Matrix invTrans;
	Point objEyeP;
	Vector objRay;
	double shortest_t = 1;
	double tmp_t = -1;
	normalRay.normalize();

	tmp = flattened_parse;
	while (tmp != NULL) {
		invTrans = tmp->invTransform;
		objEyeP = invTrans * eyePoint;
		objRay = invTrans * normalRay;
		if (tmp->primitive->type == SHAPE_CUBE) {
			tmp_t = cube->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_CYLINDER) {
			tmp_t = cylinder->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_CONE) {
			tmp_t = cone->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_SPHERE) {
			tmp_t = sphere->Intersect(objEyeP, objRay);
		}
		if (tmp_t > 0.0000001) {
			return false;
		}
		tmp = tmp->next;
	}
	return true;
}

void getReflectI(Point vertex, Vector reflectedRay, double *Irr, double *Irg, double *Irb, SceneGlobalData globalData, int depth_remaining)
{
	if (depth_remaining <= 1) {
		return;
	} else {
		depth_remaining--;
	}
	double shortest_t = -1;
	double tmp_t = -1;
	double nextIrr, nextIrg, nextIrb, texR, texG, texB;
	FlattenedNode *tmp = NULL;
	FlattenedNode *shortest_object = NULL;
	SceneLightData lightData;
	Matrix invTrans;
	Matrix transInvTrans;
	Point objEyeP;
	Vector objRay;
	Vector worldNormal;
	tmp = flattened_parse;
	nextIrr = 0;
	nextIrg = 0;
	nextIrb = 0;

			
	while (tmp != NULL) {
		invTrans = tmp->invTransform;
		objEyeP = invTrans * vertex;
		objRay = invTrans * reflectedRay;
		if (tmp->primitive->type == SHAPE_CUBE) {
			tmp_t = cube->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_CYLINDER) {
			tmp_t = cylinder->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_CONE) {
			tmp_t = cone->Intersect(objEyeP, objRay);
		} else if (tmp->primitive->type == SHAPE_SPHERE) {
			tmp_t = sphere->Intersect(objEyeP, objRay);
		}
		if (tmp_t > 0.00001) {
			if (shortest_t < 0 || tmp_t < shortest_t) {
				shortest_t = tmp_t;
				shortest_object = tmp;
			}
		}
		tmp = tmp->next;
	}
	if (shortest_t > 0.0001) {
	//std::cerr << "Intersection found at (" << i << ", " << j << ") with the shape of: " << shortest_object->primitive->type << std::endl << "Distance is " << shortest_t << std::endl;
		texR = 0;
		texG = 0;
		texB = 0;
		invTrans = shortest_object->invTransform;
		objEyeP = invTrans * vertex;
		objRay = invTrans * reflectedRay;
		transInvTrans = transpose(invTrans);
		SceneMaterial *currentMaterial = &(shortest_object->primitive->material);
		if (shortest_object->primitive->type == SHAPE_CUBE) {
			worldNormal = transInvTrans * cube->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
		} else if (shortest_object->primitive->type == SHAPE_CYLINDER) {
			worldNormal = transInvTrans * cylinder->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
		} else if (shortest_object->primitive->type == SHAPE_CONE) {
			worldNormal = transInvTrans * cone->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
		} else if (shortest_object->primitive->type == SHAPE_SPHERE) {
			worldNormal = transInvTrans * sphere->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
		}
		worldNormal.normalize();
		double distanceFromLight = -1;
		bool isInLoS_d = false;
		Vector reflectionVector;
		Point interPoint = Point(vertex[0] + 0.999 * reflectedRay[0] * shortest_t, vertex[1] + 0.999*reflectedRay[1] * shortest_t, vertex[2] + 0.999*reflectedRay[2] * shortest_t);
		*Irr += (globalData.ka * currentMaterial->cAmbient.r);
		*Irg += (globalData.ka * currentMaterial->cAmbient.g);
		*Irb += (globalData.ka * currentMaterial->cAmbient.b);
		for (int m = 0; m < parser->getNumLights(); m++) {
			parser->getLightData(m, lightData);
			if (lightData.type == LIGHT_DIRECTIONAL) {
				isInLoS_d = isInLoS_Directional(interPoint, lightData.dir);
				if (isInLoS_d == true) {
					reflectionVector = 2 * worldNormal * dot(worldNormal, lightData.dir) - lightData.dir;
					reflectionVector.normalize();
					double lightVecDot = dot(worldNormal, -lightData.dir);
					double reflectVecDot = dot(reflectedRay, reflectionVector);
					if (lightVecDot < 0.0000000001) {
						lightVecDot = 0;
					}
					if (reflectVecDot < 0.0000000001) {
						reflectVecDot = 0;
					}
					*Irr += lightData.color.r * ((globalData.kd * currentMaterial->cDiffuse.r * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texR) * lightVecDot + globalData.ks * currentMaterial->cSpecular.r * pow(reflectVecDot, currentMaterial->shininess));
					*Irg += lightData.color.g * ((globalData.kd * currentMaterial->cDiffuse.g * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texG) * lightVecDot + globalData.ks * currentMaterial->cSpecular.g * pow(reflectVecDot, currentMaterial->shininess));
					*Irb += lightData.color.b * ((globalData.kd * currentMaterial->cDiffuse.b * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texB) * lightVecDot + globalData.ks * currentMaterial->cSpecular.b * pow(reflectVecDot, currentMaterial->shininess));
				}
			} else {
							
				Vector lightVec = interPoint - lightData.pos;
				distanceFromLight = isInLoS(interPoint, lightData.pos, lightVec);
				if (distanceFromLight > 0.00000001) {
					lightVec.normalize();
					double attenuation = 40.0 / (distanceFromLight * distanceFromLight);
					/*
					red += (lightData.color.r * globalData.kd * currentMaterial->cDiffuse.r * dot(worldNormal, -lightVec));
					green += (lightData.color.g * globalData.kd * currentMaterial->cDiffuse.g * dot(worldNormal, -lightVec));
					blue += (lightData.color.b * globalData.kd * currentMaterial->cDiffuse.b * dot(worldNormal, -lightVec));*/
					reflectionVector = 2 * worldNormal * dot(worldNormal, lightVec) - lightVec;
					reflectionVector.normalize();
					double lightVecDot = pow(dot(worldNormal, -lightVec), 2);
					double reflectVecDot = dot(reflectedRay, reflectionVector);
					if (lightVecDot < 0.0000000001) {
						lightVecDot = 0;
					}
					if (reflectVecDot < 0.0000000001) {
						reflectVecDot = 0;
					}
					*Irr += attenuation * lightData.color.r * ((globalData.kd * (currentMaterial->cDiffuse.r * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texR)) * lightVecDot + globalData.ks * currentMaterial->cSpecular.r * pow(reflectVecDot, currentMaterial->shininess));
					*Irg += attenuation * lightData.color.g * ((globalData.kd * (currentMaterial->cDiffuse.g * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texG)) * lightVecDot + globalData.ks * currentMaterial->cSpecular.g * pow(reflectVecDot, currentMaterial->shininess));
					*Irb += attenuation * lightData.color.b * ((globalData.kd * (currentMaterial->cDiffuse.b * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texB)) * lightVecDot + globalData.ks * currentMaterial->cSpecular.b * pow(reflectVecDot, currentMaterial->shininess));
				}
			}
		}
		//Recursion time!
		reflectionVector = reflectedRay - 2 * dot(reflectedRay, worldNormal) * worldNormal;
		reflectionVector.normalize();
		getReflectI(interPoint, reflectionVector, &nextIrr, &nextIrg, &nextIrb, globalData, depth_remaining);
		*Irr += globalData.ks * currentMaterial->cReflective.r * nextIrr;
		*Irg += globalData.ks * currentMaterial->cReflective.g * nextIrg;
		*Irb += globalData.ks * currentMaterial->cReflective.b * nextIrb;
		if (*Irr < 0.0005 && *Irg < 0.0005 && *Irb < 0.0005) {
			return;
		}
	} else {
		return;
	}
}




void callback_start(int id) {
	double convertedX = 0;
	double convertedY = 0;
	double shortest_t = -1;
	double red = 0;
	double green = 0;
	double blue = 0;
	double tmp_t = 0;
	double Ireflectr;
	int breaker;
	double Ireflectg;
	double Ireflectb;
	double texR, texG, texB;
	Point eyePoint = camera->GetEyePoint();
	Point filmToWorldP = Point();
	Vector worldRay = Vector();
	Vector worldNormal = Vector();
	Matrix invTrans;
	Matrix transInvTrans;
	Point objEyeP;
	Vector objRay;
	FlattenedNode *shortest_object = NULL;
	texR = 0;
	texG = 0;
	texB = 0;

	FlattenedNode *tmp;

	Matrix filmToWorldM = camera->GetFilmToWorld();

	cout << "start button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	SceneGlobalData globalData;
	parser->getGlobalData(globalData);
	SceneLightData lightData;

	pixelWidth = screenWidth;
	pixelHeight = screenHeight;

	updateCamera();

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	SceneNode* root = parser->getRootNode();

	if (flattened_parse != NULL) {
		reset_flattened();
	}
	if (filenamePath == "data\\tests\\lightning.xml") {
		Lightning superAwesome = Lightning(parser, fractal_depth, 0);
		superAwesome.generate(light_len);
		flattened_parse = superAwesome.prependLightning(flattened_parse, &eyePoint);
	} else if (filenamePath == "data\\tests\\fractalball.xml") {
		Lightning superAwesome = Lightning(parser, fractal_depth, 1);
		superAwesome.generate(light_len);
		flattened_parse = superAwesome.prependLightning(flattened_parse, &eyePoint);
	}
	if (render_fog) {
		recursive_flatten(NULL, root);
	}

	cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;


	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {
			shortest_t = -1;
			red = 0;
			green = 0;
			blue = 0;
			Ireflectr = 0;
			Ireflectg = 0;
			Ireflectb = 0;
			convertedX = -1.0 + 2.0 * ((float)i / (float)pixelWidth);
			convertedY = -1.0 + 2.0 * ((float)j / (float)pixelHeight);
			filmToWorldP = filmToWorldM * Point(convertedX, convertedY, -1.0);
			//filmToWorldM.print();
			worldRay = Vector(filmToWorldP[0] - eyePoint[0], filmToWorldP[1] - eyePoint[1], filmToWorldP[2] - eyePoint[2]);
			worldRay.normalize();
			//std::cerr << "World ray: " << worldRay[0] <<" " << worldRay[1] <<" " << worldRay[2] <<std::endl;
			tmp = flattened_parse;
			breaker = 0;
			while (tmp != NULL && breaker == 0) {
				/*invTrans = tmp->invTransform;
				objEyeP = invTrans * eyePoint;*/
				objEyeP = tmp->objEyeP;
				objRay = tmp->invTransform * worldRay;
				if (tmp->primitive->type == SHAPE_CUBE) {
					tmp_t = cube->Intersect(objEyeP, objRay);
				} else if (tmp->primitive->type == SHAPE_CYLINDER) {
					tmp_t = cylinder->Intersect(objEyeP, objRay);
				} else if (tmp->primitive->type == SHAPE_CONE) {
					tmp_t = cone->Intersect(objEyeP, objRay);
				} else if (tmp->primitive->type == SHAPE_SPHERE) {
					tmp_t = sphere->Intersect(objEyeP, objRay);
				}
				if (tmp_t > 0.00001) {
					if (shortest_t < 0 || tmp_t < shortest_t) {
						shortest_t = tmp_t;
						shortest_object = tmp;
						if (tmp->primitive->type == SHAPE_CUBE && filenamePath == "data\\tests\\lightning.xml") {
							breaker == 1;
						}
					}
				}
				tmp = tmp->next;
			}
			
			if (shortest_t > 0.0001) {
				//std::cerr << "Intersection found at (" << i << ", " << j << ") with the shape of: " << shortest_object->primitive->type << std::endl << "Distance is " << shortest_t << std::endl;
				if (isectOnly) {
					//std::cerr <<"calledthat";
					setPixel(pixels, i, j, 255, 255, 255);
				} else {
					texR = 0;
					texG = 0;
					texB = 0;
					invTrans = shortest_object->invTransform;
					objEyeP = invTrans * eyePoint;
					objRay = invTrans * worldRay;
					transInvTrans = transpose(invTrans);
					SceneMaterial *currentMaterial = &(shortest_object->primitive->material);
					if (shortest_object->primitive->type == SHAPE_CUBE) {
						worldNormal = transInvTrans * cube->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
					} else if (shortest_object->primitive->type == SHAPE_CYLINDER) {
						worldNormal = transInvTrans * cylinder->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
					} else if (shortest_object->primitive->type == SHAPE_CONE) {
						worldNormal = transInvTrans * cone->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
					} else if (shortest_object->primitive->type == SHAPE_SPHERE) {
						worldNormal = transInvTrans * sphere->findIsectNormal(objEyeP, objRay, shortest_t, &texR, &texG, &texB, currentMaterial);
					}
					worldNormal.normalize();
					double distanceFromLight = -1;
					bool isInLoS_d = false;
					Vector reflectionVector;
					Point interPoint = Point(eyePoint[0] + 0.99*worldRay[0] * shortest_t, eyePoint[1] + 0.99*worldRay[1] * shortest_t, eyePoint[2] + 0.99*worldRay[2] * shortest_t);
					red = (globalData.ka * currentMaterial->cAmbient.r);
					green = (globalData.ka * currentMaterial->cAmbient.g);
					blue = (globalData.ka * currentMaterial->cAmbient.b);
					for (int m = 0; m < parser->getNumLights(); m++) {
						parser->getLightData(m, lightData);
						if (lightData.type == LIGHT_DIRECTIONAL) {
							isInLoS_d = isInLoS_Directional(interPoint, lightData.dir);
							if (isInLoS_d == true) {
								reflectionVector = 2 * worldNormal * dot(worldNormal, lightData.dir) - lightData.dir;
								reflectionVector.normalize();
								double lightVecDot = dot(worldNormal, -lightData.dir);
								double reflectVecDot = dot(worldRay, reflectionVector);
								if (lightVecDot < 0.0000000001) {
									lightVecDot = 0;
								}
								if (reflectVecDot < 0.0000000001) {
									reflectVecDot = 0;
								}
								red += lightData.color.r * ((globalData.kd * (currentMaterial->cDiffuse.r * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texR)) * lightVecDot + globalData.ks * currentMaterial->cSpecular.r * pow(reflectVecDot, currentMaterial->shininess));
								green += lightData.color.g * ((globalData.kd * (currentMaterial->cDiffuse.g * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texG)) * lightVecDot + globalData.ks * currentMaterial->cSpecular.g * pow(reflectVecDot, currentMaterial->shininess));
								blue += lightData.color.b * ((globalData.kd * (currentMaterial->cDiffuse.b * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texB)) * lightVecDot + globalData.ks * currentMaterial->cSpecular.b * pow(reflectVecDot, currentMaterial->shininess));
							}
						} else {
							
							Vector lightVec = interPoint - lightData.pos;
							distanceFromLight = isInLoS(interPoint, lightData.pos, lightVec);
							if (distanceFromLight > 0.00000001) {
								lightVec.normalize();
								double attenuation = 30.0 / (distanceFromLight * distanceFromLight);
								/*
								red += (lightData.color.r * globalData.kd * currentMaterial->cDiffuse.r * dot(worldNormal, -lightVec));
								green += (lightData.color.g * globalData.kd * currentMaterial->cDiffuse.g * dot(worldNormal, -lightVec));
								blue += (lightData.color.b * globalData.kd * currentMaterial->cDiffuse.b * dot(worldNormal, -lightVec));*/
								reflectionVector = 2 * worldNormal * dot(worldNormal, lightVec) - lightVec;
								reflectionVector.normalize();
								double lightVecDot = dot(worldNormal, -lightVec);//pow(dot(worldNormal, -lightVec), 2);
								double reflectVecDot = dot(worldRay, reflectionVector);
								if (lightVecDot < 0.0000000001) {
									lightVecDot = 0;
								}
								if (reflectVecDot < 0.0000000001) {
									reflectVecDot = 0;
								}
								red += attenuation * lightData.color.r * ((globalData.kd * currentMaterial->cDiffuse.r * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texR) * lightVecDot + globalData.ks * currentMaterial->cSpecular.r * pow(reflectVecDot, currentMaterial->shininess));
								green += attenuation * lightData.color.g * ((globalData.kd * currentMaterial->cDiffuse.g * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texG) * lightVecDot + globalData.ks * currentMaterial->cSpecular.g * pow(reflectVecDot, currentMaterial->shininess));
								blue += attenuation * lightData.color.b * ((globalData.kd * currentMaterial->cDiffuse.b * (1.0 - currentMaterial->blend) + (currentMaterial->blend) * texB) * lightVecDot + globalData.ks * currentMaterial->cSpecular.b * pow(reflectVecDot, currentMaterial->shininess));
							}
						}
					}
					//Recursion time!
					reflectionVector = worldRay - 2 * dot(worldRay, worldNormal) * worldNormal;
					reflectionVector.normalize();
					getReflectI(interPoint, reflectionVector, &Ireflectr, &Ireflectg, &Ireflectb, globalData, recursive_depth);
					red += globalData.ks * currentMaterial->cReflective.r * Ireflectr;
					green += globalData.ks * currentMaterial->cReflective.g * Ireflectg;
					blue += globalData.ks * currentMaterial->cReflective.b * Ireflectb;
					Ireflectr = 0;
					Ireflectg = 0;
					Ireflectb = 0;
					interPoint = Point(eyePoint[0] + 1.01*worldRay[0] * shortest_t, eyePoint[1] + 1.01*worldRay[1] * shortest_t, eyePoint[2] + 1.01*worldRay[2] * shortest_t);
					getReflectI(interPoint, worldRay, &Ireflectr, &Ireflectg, &Ireflectb, globalData, recursive_depth);
					red += globalData.kt * currentMaterial->cTransparent.r * Ireflectr;
					green += globalData.kt * currentMaterial->cTransparent.g * Ireflectg;
					blue += globalData.kt * currentMaterial->cTransparent.b * Ireflectb;

					if (red > 1.0) {
						red = 1.0;
					}
					if (green > 1.0) {
						green = 1.0;
					}
					if (blue > 1.0) {
						blue = 1.0;
					}
					
					setPixel(pixels, i, j, red*255, green*255, blue*255);
				}
			} else {
				//std::cerr << "No intersection found at (" << i << ", " << j << ")" <<std::endl;
				setPixel(pixels, i, j, 0, 0, 0);
			}


			//replace the following code
			/*
			if ((i % 5 == 0) && (j % 5 == 0)) {
				setPixel(pixels, i, j, 255, 0, 0);
			}
			else {
				setPixel(pixels, i, j, 128, 128, 128);
			}
			*/
		}
	}
	glutPostRedisplay();
}




void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	if (flattened_parse != NULL) {
		reset_flattened();
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

	setupCamera();
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	camera->Reset();

	Point guiEye (eyeX, eyeY, eyeZ);
	Point guiLook(lookX, lookY, lookZ);
	camera->SetViewAngle(viewAngle);
	camera->Orient(guiEye, guiLook, camera->GetUpVector());
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);
	glui->add_checkbox("Fog", &render_fog);
	glui->add_spinner("Bolt Depth", 2, &light_len)->set_int_limits(5, 20);
	glui->add_spinner("Recursive Depth", 2, &recursive_depth)->set_int_limits(1, 8);
	glui->add_spinner("Fractal Depth", 2, &fractal_depth)->set_int_limits(0, 15);

	/*
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);*/

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();
	return EXIT_SUCCESS;
}



