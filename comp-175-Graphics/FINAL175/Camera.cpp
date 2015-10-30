#include "Camera.h"

#define PI_DEG 180.0

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Orient(Point& eye, Point& focus, Vector& up)
{
    look = focus - eye;
    Orient(eye, look, up);
}

void Camera::Orient(Point& eye, Vector& look, Vector& up) 
{
    this->eyePoint = eye;
    this->look = look;
    this->up = up;

    w = normalize(-look);
    u = normalize(cross(up, w));
    v = normalize(cross(w, u));
}

Matrix Camera::GetProjectionMatrix() 
{
	Matrix p = Matrix();
    Matrix s = Matrix();
    viewAngle = (viewAngle/PI_DEG) * PI;
    double c = -nearPlane/farPlane;

    s(0, 0) = 1.0/(tan(viewAngle/2) * farPlane * GetScreenWidthRatio());
    s(1, 1) = 1.0/(tan(viewAngle/2) * farPlane);
    s(2, 2) = 1.0/farPlane;

    p(2, 2) = -1.0/(c + 1.0);
    p(2, 3) = c/(c + 1.0);
    p(3, 2) = -1.0;
    p(3, 3) = 0;

	return p * s;
}

Matrix Camera::GetFilmToWorld() {
	Matrix t_inv = Matrix();
    Matrix r_inv = Matrix();
	double radViewAngle = (viewAngle/180.0) * PI;
    t_inv(0, 3) = -eyePoint[0];
    t_inv(1, 3) = -eyePoint[1];
    t_inv(2, 3) = -eyePoint[2];

    for (int i = 0; i < 4; i++) {
        r_inv(0, i) = u[i];
        r_inv(1, i) = v[i]; 
        r_inv(2, i) = w[i];
    }

	//Matrix t = invert(t_inv);
	//Matrix r = invert(r_inv);
	Matrix s = Matrix();
	s(0, 0) = 1.0/(tan(radViewAngle/2) * farPlane * GetScreenWidthRatio());
    s(1, 1) = 1.0/(tan(radViewAngle/2) * farPlane);
    s(2, 2) = 1.0/farPlane;
	//s.print();
	//r_inv.print();
	//t_inv.print();
	return invert(s * r_inv * t_inv);
}

void Camera::SetViewAngle (double viewAngle) { this->viewAngle = viewAngle; }

void Camera::SetNearPlane (double nearPlane) { this->nearPlane = nearPlane; }

void Camera::SetFarPlane (double farPlane) { this->farPlane = farPlane; }

void Camera::SetScreenSize (int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

Matrix Camera::GetModelViewMatrix()
{
    Matrix t_inv = Matrix();
    Matrix r_inv = Matrix();

    t_inv(0, 3) = -eyePoint[0];
    t_inv(1, 3) = -eyePoint[1];
    t_inv(2, 3) = -eyePoint[2];

    for (int i = 0; i < 4; i++) {
        r_inv(0, i) = u[i];
        r_inv(1, i) = v[i]; 
        r_inv(2, i) = w[i];
    }
	return r_inv * t_inv;
}

Matrix Camera::GetInverseTRMatrix()
{
    Matrix t_inv = Matrix();
    Matrix r_inv = Matrix();

    t_inv(0, 3) = -eyePoint[0];
    t_inv(1, 3) = -eyePoint[1];
    t_inv(2, 3) = -eyePoint[2];

    for (int i = 0; i < 4; i++) {
        r_inv(0, i) = u[i];
        r_inv(1, i) = v[i]; 
        r_inv(2, i) = w[i];
    }
	return t_inv * r_inv;
}

void Camera::RotateV(double angle) 
{
    angle = (angle/PI_DEG) * PI;
    Vector temp_u = (u * cos(angle)) + (w * -sin(angle));
    w = (u * sin(angle)) + (w * cos(angle));
    u = temp_u;
}

void Camera::RotateU(double angle) {
    angle = (angle/PI_DEG) * PI;
    Vector temp_v = v * cos(angle) + (w * sin(angle));
    w = (v * -sin(angle)) + (w * cos(angle));
    v = temp_v;
}

void Camera::RotateW(double angle)
{
    angle = (angle/PI_DEG) * PI;
    Vector temp_u = (u * cos(angle)) + (v * -sin(angle));
    v = (u * sin(angle)) + (v * cos(angle));
    u = temp_u;
}

/* not called in assignment 2 */
void Camera::Translate(const Vector &v) 
{
    for (int i = 0; i < 3; i++) {
        eyePoint[i] = eyePoint[i] - v[i];
    }
}

/* may not work */
void Camera::Rotate(Point p, Vector axis, double degrees) {
	/*
	double Vangle = atan(axis[2] / axis[0]) * PI_DEG / PI;
	double Wangle = atan( axis[1] / 
							sqrt( ( axis[0] * axis[0] ) + ( axis[2] * axis[2] ) ) )
							* PI_DEG / PI;
	double Uangle = degrees;
	//Translate to origin
	for (int i = 0; i < 3; i++) {
		eyePoint[i] -= p[i];
	}
	//Perform rotations about individual axes
	RotateV(Vangle);
	RotateW(Wangle);
	RotateU(Uangle);
	//Translate back to the point
	for (int i = 0; i < 3; i++) {
		eyePoint[i] += p[i];
	} */
}

Point Camera::GetEyePoint() { return eyePoint; }

Vector Camera::GetLookVector() { return look; }

Vector Camera::GetUpVector() { return up; }

double Camera::GetViewAngle() { return viewAngle; }

double Camera::GetNearPlane() { return nearPlane; }

double Camera::GetFarPlane() { return farPlane; }

int Camera::GetScreenWidth() { return screenWidth; }

int Camera::GetScreenHeight() { return screenHeight; }

/* this might not work */
double Camera::GetFilmPlaneDepth() { 
	//((width/2) / depth) = tan(viewangle/2)
	//depth = (width/(2*tan(viewAngle/2)))
	return (screenWidth / (2 * tan(viewAngle / 2))); 
}

double Camera::GetScreenWidthRatio() { return screenWidth/screenHeight; }


void Camera::Reset() {
	Orient(Point(2, 2, 2), Vector(-2, -2, -2), Vector(0, 1, 0));
	this->viewAngle = 45;
	this->nearPlane = 0.001;
	this->farPlane = 30;
	this->screenWidth = 500;
	this->screenHeight = 500;
}
