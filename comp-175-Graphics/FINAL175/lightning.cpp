#include "lightning.h"

Lightning::Lightning(SceneParser *parser, unsigned short max_depth, unsigned typing)
{
	singleBolt = parser->getBoltNode();
	fracType = typing;
	maxFracDepth = max_depth;
	head = NULL;
	if (fracType == 1) {
		topPoint = Point(0,  0.6, 0);
		botPoint = Point(0, -0.6, 0);
		lPoint = Point(-0.6,  0, 0);
		rPoint = Point(0.6,   0, 0);
		fPoint = Point(0,  0, 0.6);
		bPoint = Point(0, 0, -0.6);
	} else {
		headPoint = Point(0,  BOLT_ENDPOINT, 0);
		tailPoint = Point(0, -BOLT_ENDPOINT, 0);
	}
	branchesPerLine = 0;
}

void Lightning::generate(unsigned length)
{
	if (fracType == 1) {
		generate_ball();
		return;
	}
	double rotX;
	double rotY;
	double rotZ;
	float scale;
	int branchingTest;
	unsigned totalBolts = 0;
	Point minPoint, maxPoint;
	Point tmpPoint;
	Vector translate;
	LightningNode *tmp;
	LightningNode *prev;
	if (head != NULL || length == 0) {
		refresh();
	}
	if (length == 0) {
		return;
	}
	head = new LightningNode;
	tmp = head;
	prev = head;
	for (unsigned i = 0; i < length; i++) {
		tmp->depth = maxFracDepth;
		//Random rotation of the bolt
		srand(time(NULL)*i);
		rotX = ((double)(rand() % 60) - 30.0) * PI / 180.0;
		srand(time(NULL)*(i+i));
		rotY = ((double)(rand() % 360)) * PI / 180.0;
		srand(time(NULL)*(i * i));
		rotZ = ((double)(rand() % 60) - 30.0) * PI / 180.0;
		tmp->transform = rotY_mat(rotY) * (rotZ_mat(rotZ) * rotX_mat(rotX));
		
		//Random scaling of the bolt
		scale = 1.0;//((double)(rand() % 3) / 10.0) + 0.8;
		tmp->transform = scale_mat(Vector(scale, scale, scale)) * tmp->transform;

		//Inheriting previous matrices
		if (tmp == head) {
			minPoint = tmp->transform * headPoint;
			maxPoint = minPoint;
			tmpPoint = tmp->transform * tailPoint;
			if (tmpPoint[0] > maxPoint[0]) {
				maxPoint[0] = tmpPoint[0];
			} else if (tmpPoint[0] < minPoint[0]) {
				minPoint[0] = tmpPoint[0];
			} 
			if (tmpPoint[1] > maxPoint[1]) {
				maxPoint[1] = tmpPoint[1];
			} else if (tmpPoint[1] < minPoint[1]) {
				minPoint[1] = tmpPoint[1];
			} 
			if (tmpPoint[2] > maxPoint[2]) {
				maxPoint[2] = tmpPoint[2];
			} else if (tmpPoint[2] < minPoint[2]) {
				minPoint[2] = tmpPoint[2];
			}
		} else {
			tmp->transform = prev->transform * tmp->transform;
			tmpPoint = tmp->transform * headPoint;
			translate = (prev->transform * tailPoint) - (tmpPoint);
			tmp->transform = trans_mat(translate) * tmp->transform;
			if (tmpPoint[0] > maxPoint[0]) {
				maxPoint[0] = tmpPoint[0];
			} else if (tmpPoint[0] < minPoint[0]) {
				minPoint[0] = tmpPoint[0];
			} 
			if (tmpPoint[1] > maxPoint[1]) {
				maxPoint[1] = tmpPoint[1];
			} else if (tmpPoint[1] < minPoint[1]) {
				minPoint[1] = tmpPoint[1];
			} 
			if (tmpPoint[2] > maxPoint[2]) {
				maxPoint[2] = tmpPoint[2];
			} else if (tmpPoint[2] < minPoint[2]) {
				minPoint[2] = tmpPoint[2];
			}
			tmpPoint = tmp->transform * tailPoint;
			if (tmpPoint[0] > maxPoint[0]) {
				maxPoint[0] = tmpPoint[0];
			} else if (tmpPoint[0] < minPoint[0]) {
				minPoint[0] = tmpPoint[0];
			} 
			if (tmpPoint[1] > maxPoint[1]) {
				maxPoint[1] = tmpPoint[1];
			} else if (tmpPoint[1] < minPoint[1]) {
				minPoint[1] = tmpPoint[1];
			}
			if (tmpPoint[2] > maxPoint[2]) {
				maxPoint[2] = tmpPoint[2];
			} else if (tmpPoint[2] < minPoint[2]) {
				minPoint[2] = tmpPoint[2];
			}
		}

		//Give it branches
		srand((time(NULL) * 24678 * i) + (i *2));
		branchingTest = (rand()) % 200;
		if (branchingTest >= THRESHOLD3) {
			tmp->numBranches = 3;
			tmp->branches = new LightningNode*[3];
			tmp->branches[0] = NULL;
			tmp->branches[1] = NULL;
			tmp->branches[2] = NULL;
			branchesPerLine += 3;
		} else if (branchingTest >= THRESHOLD2) {
			tmp->numBranches = 2;
			tmp->branches = new LightningNode*[2];
			tmp->branches[0] = NULL;
			tmp->branches[1] = NULL;
			branchesPerLine += 2;
		} else if (branchingTest >= THRESHOLD1) {
			tmp->numBranches = 1;
			tmp->branches = new LightningNode*[1];
			tmp->branches[0] = NULL;
			branchesPerLine++;
		} else {
			tmp->numBranches = 0;
			tmp->branches = NULL;
		}

		if (i < (length - 1)) {
			tmp->next = new LightningNode;
			prev = tmp;
			tmp = tmp->next;
			tmp->next = NULL;
		}
	}
	std::cout<<"Total bolts rendered: ";
	for (int i = 0; i <= maxFracDepth; i++) {
		totalBolts += length * pow(branchesPerLine, i);
	}
	std::cout<<totalBolts<<std::endl;
	centerAtOrigin(maxPoint, minPoint);
	//TODO: Fractal compatibility
	tmp = head;
	while (tmp != NULL) {
		fractalize(tmp);
		tmp = tmp->next;
	}
}

void Lightning::generate_ball()
{
	double rotX;
	double rotY;
	double rotZ;
	float scale;
	int branchingTest;
	unsigned totalBolts = 0;
	Point minPoint, maxPoint;
	Point tmpPoint;
	Vector translate;
	LightningNode *tmp;
	LightningNode *prev;
	if (head != NULL || length == 0) {
		refresh();
	}
	if (length == 0) {
		return;
	}
	head = new LightningNode;
	tmp = head;
	prev = head;

	tmp->depth = maxFracDepth;
		
	tmp->transform = Matrix();


	//Give it branches

	tmp->numBranches = 6;
	tmp->branches = new LightningNode*[6];
	tmp->branches[0] = NULL;
	tmp->branches[1] = NULL;
	tmp->branches[2] = NULL;
	tmp->branches[3] = NULL;
	tmp->branches[4] = NULL;
	tmp->branches[5] = NULL;
	branchesPerLine += 6;

	tmp->next = NULL;

	std::cout<<"Total bolts rendered: ";
	for (int i = 0; i <= maxFracDepth; i++) {
		totalBolts += 1 * pow(branchesPerLine, i);
	}
	std::cout<<totalBolts<<std::endl;
	//TODO: Fractal compatibility
	fractalize(tmp);

}

void Lightning::centerAtOrigin(Point maxPoint, Point minPoint)
{
	Vector translate = (Point(0.0,0.0,0.0) - (maxPoint + minPoint)) / 2.0;
	Matrix tMat = trans_mat(translate);
	applyToPath(head, tMat, head->depth);
}

void Lightning::applyToPath(LightningNode *root, Matrix transform, unsigned short depth)
{
	LightningNode *tmp = root;
	while (tmp != NULL) {
		tmp->transform = transform * tmp->transform;
		tmp->depth = depth;
		tmp = tmp->next;
	}
}

FlattenedNode *Lightning::prependLightning(FlattenedNode *graphList, Point *eyePoint)
{
	if (head == NULL) {
		return NULL;
	}
	return prepend_recursive(graphList, head, eyePoint);
}

FlattenedNode *Lightning::prepend_recursive(FlattenedNode *graphList, LightningNode *branch, Point *eyePoint)
{
	LightningNode *tmp = branch;
	while (tmp != NULL) {
		graphList = lightning_recursive_flatten(graphList, tmp->transform, NULL, singleBolt, eyePoint);
		for (int i = 0; i < tmp->numBranches; i++) {
			if (tmp->branches[i] != NULL) {
				graphList = prepend_recursive(graphList, tmp->branches[i], eyePoint);
			}
		}
		tmp = tmp->next;
	}
	return graphList;
}

FlattenedNode *Lightning::lightning_recursive_flatten(FlattenedNode *graphList, Matrix transform, Matrix *rTrans, SceneNode *child, Point *eyePoint) 
{
	Matrix rec_transform = Matrix();
	Matrix tmp_trans = Matrix();
	FlattenedNode* tmp_flattened;
	if (rTrans == NULL) {
		rec_transform = transform * rec_transform;
	} else {
		rec_transform = *rTrans;
	}
	if (child == NULL) {
		return graphList;
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
	for (int i = 0; i < child->primitives.size(); i++) {
		tmp_flattened = new FlattenedNode();
		tmp_flattened->primitive = child->primitives[i];
		if (fracType == 1) {
			tmp_flattened->transform = scale_mat(Vector(0.35, 0.35, 0.35)) * rec_transform;
		} else {
			tmp_flattened->transform = scale_mat(Vector(STOCKSCALE, STOCKSCALE, STOCKSCALE)) * rec_transform;
		}
		tmp_flattened->invTransform = invert(tmp_flattened->transform);
		tmp_flattened->objEyeP = tmp_flattened->invTransform * *eyePoint;
		SceneMaterial *currentMaterial = &(tmp_flattened->primitive->material);
		if (currentMaterial->textureMap != NULL && currentMaterial->textureMap->isUsed) {
			currentMaterial->openedTMap = new ppm(currentMaterial->textureMap->filename);
		}
		tmp_flattened->next = graphList;
		graphList = tmp_flattened;
	}
	for (int i = 0; i < child->children.size(); i++) {
		graphList = lightning_recursive_flatten(graphList, transform, &rec_transform, child->children[i], eyePoint);
	}
	return graphList;
}

void Lightning::fractalize(LightningNode *lightningRoot)
{
	if (head == NULL || lightningRoot->depth == 0) {
		return;
	}
	if (fracType == 1) {
		fractalize_ball(lightningRoot);
		return;
	}
	Matrix transform = Matrix();
	Vector translateToTail;
	double rotX;
	double rotY;
	double rotZ;
	float scale;
	LightningNode *tmp;
	scale = pow(FRACSCALE, (maxFracDepth - lightningRoot->depth + 1));
	for (int i = 0; i < lightningRoot->numBranches; i++) {
		lightningRoot->branches[i] = getRootCopy();
		tmp = lightningRoot->branches[i];
		srand(time(NULL) * (i + 5));
		rotX = ((double)(rand() % 60) - 30.0) * PI / 180.0;
		srand(time(NULL) * i);
		rotY = ((double)(rand() % 360)) * PI / 180.0;
		srand(time(NULL) + i);
		rotZ = ((double)(rand() % 60) - 30.0) * PI / 180.0;
		transform = rotY_mat(rotY) * (rotZ_mat(rotZ) * rotX_mat(rotX)) * scale_mat(Vector(scale, scale, scale));
		transform = lightningRoot->transform * transform;
		translateToTail = (lightningRoot->transform * tailPoint) - (transform * (tmp->transform * headPoint));
		transform = trans_mat(translateToTail) * transform;
		applyToPath(tmp, transform, lightningRoot->depth - 1);
		//TODO: Insert while loop for recursion
		while (tmp != NULL) {
			fractalize(tmp);
			tmp = tmp->next;
		}
	}
}

void Lightning::fractalize_ball(LightningNode *lightningRoot)
{
	if (head == NULL || lightningRoot->depth == 0) {
		return;
	}
	Matrix transform = Matrix();
	Vector translateToTail;
	double rotX;
	double rotY;
	double rotZ;
	float scale;
	LightningNode *tmp;
	scale = pow(0.7, (maxFracDepth - lightningRoot->depth + 1));
	for (int i = 0; i < lightningRoot->numBranches; i++) {
		lightningRoot->branches[i] = getRootCopy();
		tmp = lightningRoot->branches[i];
		if (i == 0) {
			tailPoint = topPoint;
			headPoint = botPoint;
		} else if (i == 1) {
			tailPoint = botPoint;
			headPoint = topPoint;
		} else if (i == 2) {
			tailPoint = lPoint;
			headPoint = rPoint;
		} else if (i == 3) {
			tailPoint = rPoint;
			headPoint = lPoint;
		} else if (i == 4) {
			tailPoint = fPoint;
			headPoint = bPoint;
		} else if (i == 5) {
			tailPoint = bPoint;
			headPoint = fPoint;
		} else {
			exit(1);
		}
		transform = lightningRoot->transform * scale_mat(Vector(scale, scale, scale));
		translateToTail = (lightningRoot->transform * tailPoint) - (transform * (tmp->transform * headPoint));
		transform = trans_mat(translateToTail) * transform;
		applyToPath(tmp, transform, lightningRoot->depth - 1);
		//TODO: Insert while loop for recursion
		while (tmp != NULL) {
			fractalize(tmp);
			tmp = tmp->next;
		}
	}
}

LightningNode *Lightning::getRootCopy()
{
	if (head == NULL) {
		return NULL;
	}
	LightningNode *copy = new LightningNode;
	LightningNode *tmp = head;
	LightningNode *copy_tmp = copy;
	

	while (tmp != NULL) {
		copy_tmp->next = NULL;
		copy_tmp->depth = maxFracDepth;
		copy_tmp->transform = tmp->transform;
		copy_tmp->numBranches = tmp->numBranches;
		copy_tmp->branches = new LightningNode*[copy_tmp->numBranches];
		for (int i = 0; i < copy_tmp->numBranches; i++) {
			copy_tmp->branches[i] = NULL;
		}
		if (tmp->next != NULL) {
			copy_tmp->next = new LightningNode;
		}
		tmp = tmp->next;
		copy_tmp = copy_tmp->next;
	}
	return copy;
}

void Lightning::refresh()
{
	if (head == NULL) {
		return;
	}
	//TODO: Finish this
	removeBranch(head);
}

void Lightning::removeBranch(LightningNode *root)
{
	LightningNode *tmp = NULL;
	while(root != NULL) {
		for (int i = 0; i < root->numBranches; i++) {
			if (root->branches[i] != NULL) {
				removeBranch(root->branches[i]);
			}
		}
		/*if (root->branches != NULL) {
			delete root->branches;
		}*/
		tmp = root;
		root = root->next;
		delete tmp;
	}
	return;
}

Lightning::~Lightning()
{
	refresh();
}