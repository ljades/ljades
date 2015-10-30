#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"

#define THRESHOLD1 100
#define THRESHOLD2 175
#define THRESHOLD3 195
#define STOCKSCALE 0.15
#define FRACSCALE 0.5
#define BOLT_ENDPOINT 0.9

struct FlattenedNode {
	Matrix transform;
	Matrix invTransform;
	Point objEyeP;
	ScenePrimitive* primitive;
	FlattenedNode * next;
};

struct LightningNode {
	Matrix transform;
	unsigned short numBranches;
	unsigned short depth;
	LightningNode **branches;
	LightningNode *next;
};

class Lightning {
public:
	Lightning(SceneParser *parser, unsigned short max_depth, unsigned typing);

	void generate(unsigned length);

	FlattenedNode *prependLightning(FlattenedNode *graphList, Point *eyePoint);
	
	void refresh();

	~Lightning();
private:
	void fractalize(LightningNode *lightningRoot);
	void generate_ball();
	void fractalize_ball(LightningNode *lightningRoot);
	LightningNode *getRootCopy();
	void centerAtOrigin(Point maxPoint, Point minPoint);
	void applyToPath(LightningNode *root, Matrix transform, unsigned short depth);
	void removeBranch(LightningNode *root);
	FlattenedNode *prepend_recursive(FlattenedNode *graphList, LightningNode *branch, Point *eyePoint);
	FlattenedNode *lightning_recursive_flatten(FlattenedNode *graphList, Matrix transform, Matrix *rTrans, SceneNode *child, Point *eyePoint);

	unsigned short maxFracDepth;
	LightningNode *head;
	SceneNode *singleBolt;
	Point headPoint;
	Point tailPoint;
	Point topPoint;
	Point botPoint;
	Point lPoint;
	Point rPoint;
	Point fPoint;
	Point bPoint;
	unsigned fracType;
	unsigned branchesPerLine;
};


#endif