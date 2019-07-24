#include<cstdio>
#include<cstdlib>
#include<cmath>
#include <iostream>

#include <vector>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define DEGREE_TO_RADIAN (pi/180.0)

#define CAMERA_ANGLE_CHANGE 0.8
#define CAMERA_POS_CHANGE 3

#define BOARD_LIMIT 1000
#define BOARD_SIZE 20

//object types:
#define BOARD 0
#define SQUARE 1
#define SPHERE 2
#define TRIANGLE 3

using namespace std;


template <typename T>
class Matrix{
    T **arr;
    int n;
    int m;

public:
    Matrix(int n_var, int m_var, bool initializeIdentity = true){
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
        else if(val == -0){
            val = 0;
        }
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

Vector3D rotateVector(Vector3D v, Vector3D refer, double rotationAngle){
    Vector3D result, perp;

    double radianRotationAngle = rotationAngle*DEGREE_TO_RADIAN;

    perp = crossProduct(refer, v);

    result = v*cos(radianRotationAngle) + perp*sin(radianRotationAngle);

    return result;
}

Vector3D rotateVectorRodrigues(Vector3D x, Vector3D a, double rotationAngle){
    double thetaRad = DEGREE_TO_RADIAN*rotationAngle;
    Vector3D result = x*(cos(thetaRad)) + a * (dotProduct(a, x) * (1- cos(thetaRad))) + crossProduct(a, x) * (sin(thetaRad));

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

    void operator = (Point3D const &obj) {
        x = obj.x;
        y = obj.y;
        z = obj.z;

    }

    Point3D getCopy(){
        Point3D result;
        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    }


};


class colorRGB{
public:
    double r, g, b;

    colorRGB operator * (double const &val) {
        colorRGB result;
        result.r = r * val;
        result.g = g * val;
        result.b = b * val;

        return result;
    }

    colorRGB operator + (double const &val) {
        colorRGB result;
        result.r = r + val;
        result.g = g + val;
        result.b = b + val;

        return result;
    }


    colorRGB operator + (colorRGB const &obj) {
        colorRGB result;
        result.r = obj.r;
        result.g = obj.g;
        result.b = obj.b;

        return result;
    }

     void operator = (colorRGB const &obj) {
        r = obj.r;
        g = obj.g;
        b = obj.b;

    }

    colorRGB getCopy(){
        colorRGB result;
        result.r = r;
        result.g = g;
        result.b = b;

        return result;
    }


};


class SceneObject{
public:
    int type;
    colorRGB objectColor;

    Point3D a, b, c, d;
    double r;
    Vector3D normal;




    colorRGB getObjectColor(){
        return objectColor.getCopy();
    }




};


Point3D camera_pos;
Vector3D camera_u, camera_r, camera_l;
vector <SceneObject *> objects;


void drawSquare(double a){
	glBegin(GL_QUADS);{
		glVertex3f( a,  a, 0);
		glVertex3f( a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a,  a, 0);
	}glEnd();
}

void drawBoard(){
	for(int i = -BOARD_LIMIT; i < BOARD_LIMIT; i+=BOARD_SIZE ){
        for(int j = -BOARD_LIMIT; j < BOARD_LIMIT; j += BOARD_SIZE ){
            if(abs((i+j)/BOARD_SIZE)%2==0){
                glColor3f(0, 0, 0);
            }
            else{
                glColor3f(1, 1, 1);
            }

            glBegin(GL_QUADS);{
                glVertex3f( i,  j, 0);
                glVertex3f( i, j+BOARD_SIZE, 0);
                glVertex3f(i+BOARD_SIZE, j+BOARD_SIZE, 0);
                glVertex3f(i+BOARD_SIZE,  j, 0);
            }glEnd();


        }

	}
}



void captureScene(){
    //capture here
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

    drawBoard();

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

	objects.clear();


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
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(650, 650);
	glutInitWindowPosition(50, 50);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Fully Controllable Camera & Sphere to/from Cube");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}




/*
void drawSpherePart(double radius,int slices,int stacks){
	Point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++){
		h=radius*sin(((double)i/(double)stacks)*(pi*0.5));
		r=radius*cos(((double)i/(double)stacks)*(pi*0.5));
		for(j=0;j<=slices;j++){
			points[i][j].x=r*cos(((double)j/(double)slices)*pi*0.5);
			points[i][j].y=r*sin(((double)j/(double)slices)*pi*0.5);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++){
		for(j=0;j<slices;j++){
			glBegin(GL_QUADS);{
			    //upper part
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}
*/

/*
void drawCylinderPart(double radius, double height, int slices,int stacks){
	Point points[100][100];
	int i,j;
	double h;
	//generate points
	for(i=0;i<=stacks;i++){
		h = height*(double)i/(double)stacks;
		for(j=0;j<=slices;j++){
			points[i][j].x=radius*cos(((double)j/(double)slices)*pi*0.5);
			points[i][j].y=radius*sin(((double)j/(double)slices)*pi*0.5);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++){
		for(j=0;j<slices;j++){
			glBegin(GL_QUADS);{
			    //upper half
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower half
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
*/

/*
void drawCube(){
    glColor3f(1, 1, 1);   //white color

    //up
    glPushMatrix();
    glTranslatef(0, 0, MAX_LEN);
    drawSquare(currentLen);
    glPopMatrix();

    //down
    glPushMatrix();
    glTranslatef(0, 0, -MAX_LEN);
    drawSquare(currentLen);
    glPopMatrix();

    //left
    glPushMatrix();
    glTranslatef(MAX_LEN, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(currentLen);
    glPopMatrix();

    //right
    glPushMatrix();
    glTranslatef(-MAX_LEN, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(currentLen);
    glPopMatrix();

    //back
    glPushMatrix();
    glTranslatef(0, -MAX_LEN, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(currentLen);
    glPopMatrix();

    //front
    glPushMatrix();
    glTranslatef(0, MAX_LEN, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(currentLen);
    glPopMatrix();

}
*/

/*
void drawSphere(){
    glColor3f(1, 0, 0);   //red color

    //upper half sphere
    glPushMatrix();
    glTranslatef(currentLen, currentLen, currentLen);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glTranslatef(currentLen, currentLen, currentLen);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    glTranslatef(currentLen, currentLen, currentLen);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270, 0, 0, 1);
    glTranslatef(currentLen, currentLen, currentLen);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();


    //lower half sphere
    glPushMatrix();
    glTranslatef(currentLen, currentLen, -currentLen);
    glRotatef(180, 1, 1, 0);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glTranslatef(currentLen, currentLen, -currentLen);
    glRotatef(180, 1, 1, 0);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    glTranslatef(currentLen, currentLen, -currentLen);
    glRotatef(180, 1, 1, 0);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270, 0, 0, 1);
    glTranslatef(currentLen, currentLen, -currentLen);
    glRotatef(180, 1, 1, 0);
    drawSpherePart(MAX_LEN - currentLen, SLICES, STACKS);
    glPopMatrix();


}*/

/*
void drawCylinder(){
    glColor3f(0, 1, 0);   //green color

    //sides
    glPushMatrix();
    glTranslatef(currentLen, currentLen, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();



    //upper parts
    glPushMatrix();
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270, 0, 0, 1);
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();



    //lower parts
    glPushMatrix();
    glRotatef(-45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glRotatef(-45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    glRotatef(-45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270, 0, 0, 1);
    glRotatef(-45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(currentLen, currentLen, 0);
    glRotatef(90, 1, 1, 0);
    drawCylinderPart(MAX_LEN-currentLen, currentLen, SLICES, STACKS);
    glPopMatrix();

}
*/










