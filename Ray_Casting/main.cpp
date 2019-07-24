#include<cstdio>
#include<cstdlib>
#include<cmath>
#include <ctime>

#include <iostream>
#include <vector>

#include <windows.h>
#include <glut.h>

#include "bitmap_image.hpp"

#define INF 999999
#define pi (2*acos(0.0))
#define DEGREE_TO_RADIAN (pi/180.0)

#define CAMERA_ANGLE_CHANGE 0.8
#define CAMERA_POS_CHANGE 3

//gluPerspective parameters
#define fov_Y 80
#define ASPECT_RATIO 1
#define NEAR_DIST 1
#define FAR_DIST 1000

#define BOARD_LIMIT 1000
#define BOARD_SIZE 20

//object types:
#define BOARD 0
#define SQUARE 1
#define SPHERE 2
#define TRIANGLE 3

using namespace std;

int LOR;    //level of recursion
int num_pixels; //number of pixels along one axis
int num_objects; //number of objects in the scene
int num_lights; //number of light sources



template <typename T>
class Matrix{
    T **arr;
    int n;
    int m;

public:
    Matrix(int n_var, int m_var, bool initializeIdentity = false){
        n = n_var;
        m = m_var;

        arr = new T *[n];
        for (int i = 0; i < n; i++) {
            arr[i] = new T[m];
            for (int j = 0; j < m; j++) {
                arr[i][j] = 0;
            }
        }

        if(initializeIdentity){
            for (int i = 0; i < n && i < m; i++) {
                arr[i][i] = 1;
            }
        }

    }

    ~Matrix(){
        for (int i = 0; i < n; ++i) {
            delete []arr[i];
        }
        delete []arr;
    }

    Matrix(const Matrix& obj){
        n = obj.n;
        m = obj.m;

        arr = new T *[n];
        for (int i = 0; i < n; i++) {
            arr[i] = new T[m];
            for (int j = 0; j < m; j++) {
                arr[i][j] = obj.arr[i][j];
            }
        }
    }

    Matrix getCopy(){
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j];
            }
        }
        return result;
    }

    Matrix transpose(){
        Matrix<T> result(m, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[j][i] = arr[i][j];
            }
        }
        return result;
    }

    void setVal(int i, int j, T val){
        if(i >= n || j >= m){
            return;
        }
//        else if(val == -0){
//            val = 0;
//        }
        arr[i][j] = val;
    }

    T getVal(int i, int j){
        if(i >= n || j >= m){
            return 1;
        }
        return arr[i][j];
    }

    void operator = (Matrix const &obj) {
        n = obj.n;
        m = obj.m;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                arr[i][j] = obj.arr[i][j];
            }
        }
    }

    T **getArr() const {
        return arr;
    }

    void setArr(T **arr) {
        Matrix::arr = arr;
    }

    int getN() const {
        return n;
    }

    int getM() const {
        return m;
    }

};


class Vector3D{

public:
    double x, y, z;
    Vector3D(double varX = 0, double varY  = 0, double varZ = 0){
        x = varX;
        y = varY;
        z = varZ;
    }

    Vector3D getCopy(){
        Vector3D result;
        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    }

    Vector3D getOppositeVector(){
        Vector3D result;
        result.x = -x;
        result.y = -y;
        result.z = -z;

        return result;
    }


    double getVal(){
        return sqrt(x*x + y*y + z*z);
    }

    void normalize(){
        double val = getVal();
        x = x/val;
        y = y/val;
        z = z/val;
    }

    Vector3D getUnitVector(){
        Vector3D result = getCopy();
        result.normalize();

        return result;
    }

    Matrix <double > toColumnMatrix(){
        Matrix <double > result(3, 0, false);

        result.setVal(0, 0, x);
        result.setVal(1, 0, y);
        result.setVal(2, 0, z);

        return result;
    }

    Vector3D operator + (Vector3D const &obj) {
        Vector3D result;
        result.x = x + obj.x;
        result.y = y + obj.y;
        result.z = z + obj.z;

        return result;
    }

    Vector3D operator * (double const &val) {
        Vector3D result;
        result.x = x * val;
        result.y = y * val;
        result.z = z * val;

        return result;
    }

    void operator = (Vector3D const &obj) {
        x = obj.x;
        y = obj.y;
        z = obj.z;

    }

    double getX() const {
        return x;
    }

    void setX(double x) {
        Vector3D::x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        Vector3D::y = y;
    }

    double getZ() const {
        return z;
    }

    void setZ(double z) {
        Vector3D::z = z;
    }

};

Vector3D crossProduct(Vector3D a, Vector3D b){
    Vector3D result;

    result.setX(a.getY()*b.getZ() - b.getY()*a.getZ());
    result.setY(a.getZ()*b.getX() - b.getZ()*a.getX());
    result.setZ(a.getX()*b.getY() - b.getX()*a.getY());

    return result;
}

double dotProduct(Vector3D a, Vector3D b){
    return a.getX()*b.getX() + a.getY()*b.getY() +a.getZ()*b.getZ();
}

double getRadAngleVectors(Vector3D a, Vector3D b){
    return (1.0*dotProduct(a,b)/(a.getVal(), b.getVal()));

}

Vector3D rotateVector(Vector3D v, Vector3D refer, double rotationAngle){
    Vector3D result, perp;

    double radianRotationAngle = rotationAngle*DEGREE_TO_RADIAN;

    perp = crossProduct(refer, v);

    result = v*cos(radianRotationAngle) + perp*sin(radianRotationAngle);

    result.normalize();

    return result;
}

Vector3D rotateVectorRodrigues(Vector3D x, Vector3D a, double rotationAngle){
    double thetaRad = DEGREE_TO_RADIAN*rotationAngle;
    Vector3D result = x*(cos(thetaRad)) + a * (dotProduct(a, x) * (1- cos(thetaRad))) + crossProduct(a, x) * (sin(thetaRad));

    result.normalize();

    return result;
}


class Point3D{
public:
    double x, y, z;
    Point3D(double varX = 0, double varY  = 0, double varZ = 0){
        x = varX;
        y = varY;
        z = varZ;
    }

    double getX() const {
        return x;
    }

    void setX(double x) {
        Point3D::x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        Point3D::y = y;
    }

    double getZ() const {
        return z;
    }

    void setZ(double z) {
        Point3D::z = z;
    }

    Vector3D operator - (Point3D const &obj) {
        Vector3D result;
        result.setX(x - obj.getX());
        result.setY(y - obj.getY());
        result.setZ(z - obj.getZ());

        return result;
    }

    Point3D operator + (Vector3D const &obj) {
        Point3D result;
        result.setX(x - obj.getX());
        result.setY(y - obj.getY());
        result.setZ(z - obj.getZ());

        return result;
    }

    void operator = (Point3D const &obj) {
        x = obj.x;
        y = obj.y;
        z = obj.z;

    }

    void operator = (double const &val) {
        x = val;
        y = val;
        z = val;
    }


    Point3D getCopy(){
        Point3D result;
        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    }


};


double getDistance(Point3D a, Point3D b){
    return (a-b).getVal();
}

class ColorRGB{
public:
    double r, g, b;

    ColorRGB(double varR = 1, double varG  = 1, double varB = 1){
        r = varR;
        g = varG;
        b = varB;
    }

    ColorRGB operator * (double const &val) {
        ColorRGB result;
        result.r = r * val;
        result.g = g * val;
        result.b = b * val;

        return result;
    }

    ColorRGB operator + (double const &val) {
        ColorRGB result;
        result.r = r + val;
        result.g = g + val;
        result.b = b + val;

        return result;
    }


    ColorRGB operator + (ColorRGB const &obj) {
        ColorRGB result;
        result.r = obj.r;
        result.g = obj.g;
        result.b = obj.b;

        return result;
    }

     void operator = (ColorRGB const &obj) {
        r = obj.r;
        g = obj.g;
        b = obj.b;

    }

    void operator = (double const &val) {
        r = val;
        g = val;
        b = val;
    }

    ColorRGB getCopy(){
        ColorRGB result;
        result.r = r;
        result.g = g;
        result.b = b;

        return result;
    }


};



Point3D camera_pos;
Vector3D camera_u, camera_r, camera_l;
vector <Point3D *> lights;


class SceneObject{
public:
    ColorRGB objectColor;
    double ambCoeff, diffCoeff, specCoeff, refCoeff, specExp;


    void setColor(ColorRGB color){
        objectColor = color.getCopy();
    }

    ColorRGB getObjectColor(){
        return objectColor.getCopy();
    }

    void setCoefficients(double a, double d, double s, double r, double se){
        ambCoeff = a;
        diffCoeff = d;
        specCoeff = s;
        refCoeff = r;
        specExp = se;
    }

    virtual ColorRGB getColorAt(){
        printf("BASE CLASS!!!!!");
        return objectColor;
    }

    virtual void draw(){
        //do nothing
        printf("BASE CLASS!!!!!");
    }

    virtual void printProperties(){
        //do nothing
        printf("BASE CLASS!!!!!");
    }

};

class CheckerBoard : public SceneObject{
public:
    Vector3D normal;

    CheckerBoard(){
        Vector3D n(0, 0, 1);
        normal = n;
    }


    ColorRGB getColorAt(){
        return objectColor;
    }

    void draw(){
        glColor3f(objectColor.r, objectColor.g, objectColor.b);
        for(int i = -BOARD_LIMIT; i < BOARD_LIMIT; i+=BOARD_SIZE ){
            for(int j = -BOARD_LIMIT; j < BOARD_LIMIT; j += BOARD_SIZE ){
                if(abs((i+j)/BOARD_SIZE)%2){
                    glBegin(GL_QUADS);{
                        glVertex3f( i,  j, 0);
                        glVertex3f( i, j+BOARD_SIZE, 0);
                        glVertex3f(i+BOARD_SIZE, j+BOARD_SIZE, 0);
                        glVertex3f(i+BOARD_SIZE,  j, 0);
                    }glEnd();
                }
            }
        }
    }

};

class Sphere : public SceneObject{
public:
    Point3D center;
    double radius;


    ColorRGB getColorAt(){
        return objectColor;
    }

    void draw(){
        //printf("In sphere draw function...\n");
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);

        glColor3f(objectColor.r, objectColor.g, objectColor.b);
        int slices = 50, stacks = 50;
        Point3D points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0;i<=stacks;i++){
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0;i<stacks;i++){
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }glEnd();
            }
        }

        glPopMatrix();
    }

    void printProperties(){
        //do nothing
        printf("Sphere: %f %f %f %f %f %f %f %f %f %f %f %f\n", center.x, center.y, center.z, radius, objectColor.r, objectColor.g, objectColor.b, ambCoeff, diffCoeff, specCoeff, refCoeff, specExp);
    }

};

class Square : public SceneObject{
public:
    Point3D a;
    double length;

    Vector3D normal;

    Square(){
        Vector3D n(0, 0, 1);
        normal = n;
    }

    void setNormal(Vector3D n){
        normal = n;
    }

    ColorRGB getColorAt(){
        return objectColor;
    }

    void draw(){
        glColor3f(objectColor.r, objectColor.g, objectColor.b);
        glBegin(GL_QUADS);{
            glVertex3f( a.x,  a.y, a.z);
            glVertex3f( a.x+length,  a.y, a.z);
            glVertex3f( a.x+length,  a.y+length, a.z);
            glVertex3f( a.x,  a.y+length, a.z);
        }glEnd();
    }

};


class Triangle : public SceneObject{
public:
    Point3D a, b, c;

    Vector3D normal;

    setNormal(){
        Vector3D v1 = (b-a);
        Vector3D v2 = (c-a);

        normal = crossProduct(v1, v2);
        normal.normalize();

    }

    ColorRGB getColorAt(){
        return objectColor;
    }


    void draw(){
        glColor3f(objectColor.r, objectColor.g, objectColor.b);
        glBegin(GL_TRIANGLES);{
            glVertex3f( a.x,  a.y, a.z);
            glVertex3f( b.x,  b.y, b.z);
            glVertex3f( c.x,  c.y, c.z);
        }glEnd();
    }

};


vector <SceneObject *> objects;


void createImage(Matrix <ColorRGB> colorArr){
    int n = colorArr.getN();
    int m = colorArr.getM();
    bitmap_image image(n, m);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            ColorRGB temp = colorArr.getVal(x, y);
            image.set_pixel(x, y, 255*temp.r, 255*temp.g, 255*temp.b);
        }
    }
    image.save_image("out.bmp");
    printf("Created bmp image\n");

}


void captureScene(){
    //capture here
    clock_t startTime, endTime;
    double cpu_time_used;
    startTime = clock();
    printf("\n\nCapture Scene initiated\n");

    Matrix <Point3D> pixelPositions(num_pixels, num_pixels, false);
    Matrix <ColorRGB> pixelColors(num_pixels, num_pixels, false);


    //working with pixelPositions array
    Vector3D look = camera_l.getCopy();
    look.normalize();

    Vector3D up = camera_u.getCopy();
    up.normalize();
    Vector3D down = up.getOppositeVector();
    down.normalize();

    Vector3D right = camera_r.getCopy();
    right.normalize();
    Vector3D left = right.getOppositeVector();
    left.normalize();


    Point3D midPoint = camera_pos+(camera_l*NEAR_DIST);
    double half = NEAR_DIST*tan(DEGREE_TO_RADIAN*fov_Y);
    Point3D highPoint = midPoint + (left*half) + (up*half);

    double pixelLen = 2.0*half/num_pixels;

    for (int i = 0; i < num_pixels; i++) {
        for (int j = 0; j < num_pixels; j++) {
            Point3D temp = highPoint + (down*(i+0.5) + right*(j+0.5))*pixelLen;
            pixelPositions.setVal(i, j, temp);
        }
    }

    /*for (int i = 0; i < num_pixels; i++) {
        for (int j = 0; j < num_pixels; j++) {
            printf("<%f,%f,%f> ", pixelPositions.getVal(i,j).x, pixelPositions.getVal(i,j).y, pixelPositions.getVal(i,j).z);
        }
        printf("\n");
    }*/





    createImage(pixelColors);
    endTime = clock();
    cpu_time_used = (1000*(double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("Time taken in capturing the scene: %f ms\n", cpu_time_used);

}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			camera_l = rotateVector(camera_l, camera_u, CAMERA_ANGLE_CHANGE);
			camera_r = rotateVector(camera_r, camera_u, CAMERA_ANGLE_CHANGE);
			break;

        case '2':
			camera_l = rotateVector(camera_l, camera_u, -CAMERA_ANGLE_CHANGE);
			camera_r = rotateVector(camera_r, camera_u, -CAMERA_ANGLE_CHANGE);
			break;

        case '3':
			camera_u = rotateVector(camera_u, camera_r, CAMERA_ANGLE_CHANGE);
			camera_l = rotateVector(camera_l, camera_r, CAMERA_ANGLE_CHANGE);
			break;

        case '4':
			camera_u = rotateVector(camera_u, camera_r, -CAMERA_ANGLE_CHANGE);
			camera_l = rotateVector(camera_l, camera_r, -CAMERA_ANGLE_CHANGE);
			break;

        case '5':
			camera_u = rotateVector(camera_u, camera_l, -CAMERA_ANGLE_CHANGE);
			camera_r = rotateVector(camera_r, camera_l, -CAMERA_ANGLE_CHANGE);
			break;

        case '6':
			camera_u = rotateVector(camera_u, camera_l, CAMERA_ANGLE_CHANGE);
			camera_r = rotateVector(camera_r, camera_l, CAMERA_ANGLE_CHANGE);
			break;

        case '0':
            captureScene();
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			cameraHeight -= 3.0;
            camera_pos.x += -camera_l.x*CAMERA_POS_CHANGE;
            camera_pos.y += -camera_l.y*CAMERA_POS_CHANGE;
            camera_pos.z += -camera_l.z*CAMERA_POS_CHANGE;
			break;

		case GLUT_KEY_UP:		// up arrow key
//			cameraHeight += 3.0;
            camera_pos.x += camera_l.x*CAMERA_POS_CHANGE;
            camera_pos.y += camera_l.y*CAMERA_POS_CHANGE;
            camera_pos.z += camera_l.z*CAMERA_POS_CHANGE;
			break;

		case GLUT_KEY_RIGHT:
//			cameraAngle += 0.03;
            camera_pos.x += camera_r.x*CAMERA_POS_CHANGE;
            camera_pos.y += camera_r.y*CAMERA_POS_CHANGE;
            camera_pos.z += camera_r.z*CAMERA_POS_CHANGE;
			break;

		case GLUT_KEY_LEFT:
//			cameraAngle -= 0.03;
            camera_pos.x += -camera_r.x*CAMERA_POS_CHANGE;
            camera_pos.y += -camera_r.y*CAMERA_POS_CHANGE;
            camera_pos.z += -camera_r.z*CAMERA_POS_CHANGE;
			break;

		case GLUT_KEY_PAGE_UP:
            camera_pos.x += camera_u.x*CAMERA_POS_CHANGE;
            camera_pos.y += camera_u.y*CAMERA_POS_CHANGE;
            camera_pos.z += camera_u.z*CAMERA_POS_CHANGE;
			break;

		case GLUT_KEY_PAGE_DOWN:
		    camera_pos.x += -camera_u.x*CAMERA_POS_CHANGE;
            camera_pos.y += -camera_u.y*CAMERA_POS_CHANGE;
            camera_pos.z += -camera_u.z*CAMERA_POS_CHANGE;
			break;

		case GLUT_KEY_HOME:
			break;

		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			break;

		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

    gluLookAt(camera_pos.getX(), camera_pos.getY(), camera_pos.getZ(),  camera_pos.getX()+camera_l.getX(), camera_pos.getY()+camera_l.getY(), camera_pos.getZ()+camera_l.getZ(),   camera_u.getX(), camera_u.getY(), camera_u.getZ());


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

    //draw objects
	for(int i=0; i<objects.size(); i++){
        objects[i]->draw();
	}


    //draw lights
	ColorRGB white;
	Sphere temp;
    SceneObject *obj = &temp;
	for(int i=0; i<lights.size(); i++){
        temp.setColor(white);
        temp.radius = 2;
        temp.center = *(lights[i]);
        obj->draw();
	}

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera

    //printf("From animate\n");
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	camera_pos.x = 100;
	camera_pos.y = 100;
	camera_pos.z = 50;

	camera_u.x = 0;
	camera_u.y = 0;
	camera_u.z = 1;

	camera_r.x = -1.0/sqrt(2);
	camera_r.y = 1.0/sqrt(2);
	camera_r.z = 0;

	camera_l.x = -1.0/sqrt(2);
	camera_l.y = -1.0/sqrt(2);
	camera_l.z = 0;

	LOR = 0;
	num_pixels = 0;
	num_objects = 0;
	num_lights = 0;

	objects.clear();
	lights.clear();


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(fov_Y,	ASPECT_RATIO,	NEAR_DIST,	FAR_DIST);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void takeSceneInput(){
//input and output files
    FILE * sceneFile = fopen("description.txt", "r");
    if(sceneFile == NULL){
        printf("ERROR OPENING FILE %d\n", sceneFile);
        exit(0);
    }

    char *dummy = new char[80];

    fscanf(sceneFile, "%d", &LOR);
    fscanf(sceneFile, "%d", &num_pixels);
    fgets(dummy, 10, sceneFile);

    fscanf(sceneFile, "%d", &num_objects);

    char *type = new char[80];
    double x, y, z, r, h, cr, cg, cb, a, d, s, re, se;

    for(int i=0; i<num_objects; i++){
        fgets(dummy, 10, sceneFile);
        fscanf(sceneFile, "%s", type);
        printf("%s\n", type);
        fscanf(sceneFile, "%lf %lf %lf", &x, &y, &z);
        if(strcmp(type, "sphere")==0){
            fscanf(sceneFile, "%lf", &r);
        }
        else if(strcmp(type, "pyramid")==0){
            fscanf(sceneFile, "%lf %lf", &r, &h);
        }

        fscanf(sceneFile, "%lf %lf %lf", &cr, &cg, &cb);
        fscanf(sceneFile, "%lf %lf %lf %lf", &a, &d, &s, &re);
        fscanf(sceneFile, "%lf", &se);

        if(strcmp(type, "sphere")==0){
            printf("%s %f %f %f %f %f %f %f %f %f %f %f %f\n", type, x, y, z, r, cr, cg, cb, a, d, s, re, se);

            Point3D center(x, y, z);
            ColorRGB color(cr, cg, cb);
            Sphere *tempSpehre = new Sphere();
            SceneObject *tempObj = tempSpehre;

            tempSpehre->center = center;
            tempSpehre->radius = r;
            tempObj->setColor(color);
            tempObj->setCoefficients(a, d, s, re, se);

            objects.push_back(tempObj);


        }
        else if(strcmp(type, "pyramid")==0){
            printf("%s %f %f %f %f %f %f %f %f %f %f %f %f %f\n", type, x, y, z, r, h, cr, cg, cb, a, d, s, re, se);

            Point3D pointA(x, y, z);
            Point3D pointB(x+r, y, z);
            Point3D pointC(x+r, y+r, z);
            Point3D pointD(x, y+r, z);
            Point3D pointT(x+(0.5*r), y+(0.5*r), z+h);
            ColorRGB color(cr, cg, cb);

            //base square
            Square *tempSquare = new Square();
            SceneObject *tempObj = tempSquare;
            tempSquare->a = pointA;
            tempSquare->length = r;

            tempObj->setColor(color);
            tempObj->setCoefficients(a, d, s, re, se);
            objects.push_back(tempObj);


            //triangles
            Triangle *tempTriangle;

            //triangle1
            tempTriangle = new Triangle();
            tempObj = tempTriangle;
            tempTriangle->a = pointA;
            tempTriangle->b = pointB;
            tempTriangle->c = pointT;
            tempTriangle->setNormal();

            tempObj->setColor(color);
            tempObj->setCoefficients(a, d, s, re, se);
            objects.push_back(tempObj);

            //triangle2
            tempTriangle = new Triangle();
            tempObj = tempTriangle;
            tempTriangle->a = pointB;
            tempTriangle->b = pointC;
            tempTriangle->c = pointT;
            tempTriangle->setNormal();

            tempObj->setColor(color);
            tempObj->setCoefficients(a, d, s, re, se);
            objects.push_back(tempObj);

            //triangle3
            tempTriangle = new Triangle();
            tempObj = tempTriangle;
            tempTriangle->a = pointC;
            tempTriangle->b = pointD;
            tempTriangle->c = pointT;
            tempTriangle->setNormal();

            tempObj->setColor(color);
            tempObj->setCoefficients(a, d, s, re, se);
            objects.push_back(tempObj);

            //triangle4
            tempTriangle = new Triangle();
            tempObj = tempTriangle;
            tempTriangle->a = pointD;
            tempTriangle->b = pointA;
            tempTriangle->c = pointT;
            tempTriangle->setNormal();

            tempObj->setColor(color);
            tempObj->setCoefficients(a, d, s, re, se);
            objects.push_back(tempObj);

        }


    }

    CheckerBoard *board = new CheckerBoard();
    SceneObject *tempObj = board;
    ColorRGB white;
    tempObj->setColor(white);
    objects.push_back(tempObj);


    //lights
    fgets(dummy, 10, sceneFile);
    fgets(dummy, 10, sceneFile);
    fscanf(sceneFile, "%d", &num_lights);

    for(int i; i<num_lights; i++){
        fscanf(sceneFile, "%lf %lf %lf", &x, &y, &z);
        Point3D *lightPos = new Point3D(x, y, z);
        lights.push_back(lightPos);

        printf("light at %f %f %f\n", lights[i]->x, lights[i]->y, lights[i]->z);
    }

    printf("LOR=%d, num_pixels=%d, num_objects=%d, num_lights=%d\n", LOR, num_pixels, num_objects, num_lights);


}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(650, 650);
	glutInitWindowPosition(50, 50);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Fully Controllable Camera & Sphere to/from Cube");

	init();


	takeSceneInput();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

