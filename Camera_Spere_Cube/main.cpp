#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define CAMERA_ANGLE_CHANGE 0.5
#define CAMERA_POS_CHANGE 3

#define MAX_LEN 20.0
#define LEN_CHANGE 0.5


class Point
{
public:
	double x, y, z;
};

class Vector
{
public:
	double x, y, z;
};

int drawgrid;
int drawaxes;
double angle;
double currentLen;

Point camera_pos;
Vector camera_u, camera_r, camera_l;

Vector crossProduct(Vector a, Vector b){
    Vector result;

    result.x = a.y*b.z - b.y*a.z;
    result.y = a.z*b.x - b.z*a.x;
    result.z = a.x*b.y - b.x*a.y;

    return result;
}

Vector rotateVector(Vector v, Vector refer, double rotationAngle){
    Vector result, perp;

    double radianRotationAngle = rotationAngle*pi/180.0;

    //perp = refer X v
    //result = v*cos(radianRotationAngle) + perp*sin(radianRotationAngle)

    perp = crossProduct(refer, v);

    result.x = v.x*cos(radianRotationAngle) + perp.x*sin(radianRotationAngle);
    result.y = v.y*cos(radianRotationAngle) + perp.y*sin(radianRotationAngle);
    result.z = v.z*cos(radianRotationAngle) + perp.z*sin(radianRotationAngle);


    return result;
}


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a,  a, 0);
		glVertex3f( a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a,  a, 0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    Point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    Point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	Point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
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
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void drawSphereCube()
{
    glColor3f(1,1,1);   //white color
    glPushMatrix();     //clean stack

    //up
    glTranslatef(0, 0, MAX_LEN);
    drawSquare(currentLen);
    glPopMatrix();
    glPushMatrix();

    //down
    glTranslatef(0, 0, -MAX_LEN);
    drawSquare(currentLen);
    glPopMatrix();
    glPushMatrix();

    //left
    glTranslatef(MAX_LEN, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(currentLen);
    glPopMatrix();
    glPushMatrix();

    //right
    glTranslatef(-MAX_LEN, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(currentLen);
    glPopMatrix();
    glPushMatrix();

    //back
    glTranslatef(0, -MAX_LEN, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(currentLen);
    glPopMatrix();
    glPushMatrix();

    //front
    glTranslatef(0, MAX_LEN, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(currentLen);
    glPopMatrix();

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

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    currentLen -= LEN_CHANGE;
		    if(currentLen<0){
                currentLen = 0;
		    }
			break;
		case GLUT_KEY_END:
		    currentLen += LEN_CHANGE;
		    if(currentLen>MAX_LEN){
                currentLen = MAX_LEN;
		    }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid=1-drawgrid;
			}

			break;

		case GLUT_MIDDLE_BUTTON:
			//........
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

    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z,  camera_pos.x+camera_l.x, camera_pos.y+camera_l.y, camera_pos.z+camera_l.z,   camera_u.x, camera_u.y, camera_u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    drawSphereCube();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera

    //printf("From animate\n");
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	angle=0;

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

    currentLen = MAX_LEN*0.75;

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















