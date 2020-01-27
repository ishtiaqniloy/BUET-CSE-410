#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define SLICES 50
#define STACKS 50

#define WHEEL_RADIUS 20
#define WHEEL_WIDTH 10

#define WHEEL_FORWARD_MOVE_ANGLE 3.0
#define WHEEL_FORWARD_MOVE_DISTANCE (WHEEL_RADIUS*WHEEL_FORWARD_MOVE_ANGLE*pi/180.0)
#define WHEEL_ANGLEX_CHANGE 1.0

#define ANGLE_CHANGE 2.0
#define SQUARE_SIDE 20



class Point{
public:
	double x, y, z;
};

class Vector{
public:
	double x, y, z;
};


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;

Point wheelCenter;
Vector wheelFront;
double wheelAngleX;
double wheelRotationAngle;


double angle1, angle2, angle3, angle4, angle5;




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


void drawAxes(){
	if(drawaxes==1){
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

void drawGrid(){
	int i;
	if(drawgrid==1){
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				//if(i==0)
					//continue;	//SKIP the MAIN axes

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


void drawAxle(double radius, double width){
    glColor3f(0.7,0.7,0.7);
	glBegin(GL_QUADS);{
		glVertex3f(0, width, radius);
		glVertex3f(0, -width, radius);
		glVertex3f(0, -width, -radius);
		glVertex3f(0, width, -radius);
	}glEnd();

	glBegin(GL_QUADS);{
		glVertex3f(-radius, -width, 0);
		glVertex3f(-radius,  width, 0);
		glVertex3f( radius,  width, 0);
		glVertex3f( radius, -width, 0);
	}glEnd();

}


void drawWheelWithQuads(double radius, double width, int slices, int stacks){
    int i;
    Point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=slices;i++){
        points[i].x=radius*cos(((double)i/(double)slices)*2*pi);
        points[i].y=radius*sin(((double)i/(double)slices)*2*pi);
    }
    //draw segments using generated points
    for(int k = -stacks/2; k < stacks/2; k++){
        for(i=0;i<slices;i++){
            glColor3f((double)i/(double)slices,(double)i/(double)slices,(double)i/(double)slices);
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i].x,points[i].y, k*width/stacks);
                glVertex3f(points[i+1].x,points[i+1].y, k*width/stacks);
                glVertex3f(points[i+1].x,points[i+1].y, (k+1)*width/stacks);
                glVertex3f(points[i].x,points[i].y, (k+1)*width/stacks);

            }
            glEnd();
        }
    }

}

void drawWheelWithLines(double radius, double width, int slices,int stacks){
	int i;
    Point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=slices;i++){
        points[i].x=radius*cos(((double)i/(double)slices)*2*pi);
        points[i].y=radius*sin(((double)i/(double)slices)*2*pi);
    }
    //draw segments using generated points
    for(int k = -stacks/2; k < stacks/2; k++){
        for(i=0;i<slices;i++){
            glColor3f((double)i/(double)slices,(double)i/(double)slices,(double)i/(double)slices);
            glBegin(GL_LINES);
            {
                glVertex3f(points[i].x,points[i].y, k*width/stacks);
                glVertex3f(points[i+1].x,points[i+1].y, k*width/stacks);
            }
            glEnd();
        }
    }
}





void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':

		    angle1+=ANGLE_CHANGE;
		    if(angle1 > 90){
                angle1 = 90;
		    }

			break;

		case '2':
		    angle1-=ANGLE_CHANGE;
		    if(angle1 < 0){
                angle1 = 0;
		    }

			break;


		case '3':

		    angle2+=ANGLE_CHANGE;
		    if(angle2 > 90){
                angle2 = 90;
		    }

			break;

		case '4':
		    angle2-=ANGLE_CHANGE;
		    if(angle2 < 0){
                angle2 = 0;
		    }

			break;


		case '5':

		    angle3+=ANGLE_CHANGE;
		    if(angle3 > 90){
                angle3 = 90;
		    }

			break;

		case '6':
		    angle3-=ANGLE_CHANGE;
		    if(angle3 < 0){
                angle3 = 0;
		    }

			break;


		case '7':

		    angle4+=ANGLE_CHANGE;
		    if(angle4 > 90){
                angle4 = 90;
		    }

			break;

		case '8':
		    angle4-=ANGLE_CHANGE;
		    if(angle4 < 0){
                angle4 = 0;
		    }

			break;


		case '9':

		    angle5+=ANGLE_CHANGE;
		    if(angle5 > 90){
                angle5 = 90;
		    }

			break;

		case '0':
		    angle5-=ANGLE_CHANGE;
		    if(angle5 < 0){
                angle5 = 0;
		    }

			break;


		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;
		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;
		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;
		case GLUT_KEY_INSERT:
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


void drawSquare(float a){
    glBegin(GL_QUADS);
            {
                glVertex3f(-a, a, 0);
                glVertex3f(a, a, 0);
                glVertex3f(a, -a, 0);
                glVertex3f(-a, -a, 0);
            }
            glEnd();


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

	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

	/*

    glTranslatef(wheelCenter.x, wheelCenter.y, wheelCenter.z);
    glRotatef(wheelAngleX, 0, 0, 1);
    glRotatef(wheelRotationAngle, 0, 1, 0);

    drawAxle(WHEEL_RADIUS, 0.3*WHEEL_WIDTH);

    glRotatef(90, 1, 0, 0); //to rotate the wheel only
    //drawWheelWithLines(WHEEL_RADIUS, WHEEL_WIDTH, SLICES, STACKS);
    drawWheelWithQuads(WHEEL_RADIUS, WHEEL_WIDTH, SLICES, STACKS);*/



    //square0:
    glColor3f(1,0,0);
    drawSquare(SQUARE_SIDE);


    //square1:
    glColor3f(0,1,0);

    glPushMatrix();
    glTranslatef(0, SQUARE_SIDE, 0);
    glRotatef(angle1, 1, 0, 0);
    glTranslatef(0, SQUARE_SIDE, 0);
    drawSquare(SQUARE_SIDE);

    glPopMatrix();


    //square2:
    glColor3f(0,0,1);

    glPushMatrix();
    glTranslatef(-SQUARE_SIDE, 0, 0);
    glRotatef(angle2, 0, 1, 0);
    glTranslatef(-SQUARE_SIDE, 0, 0);
    drawSquare(SQUARE_SIDE);

    glPopMatrix();



    //square3:
    glColor3f(1,1,0);

    glPushMatrix();
    glTranslatef(0, -SQUARE_SIDE, 0);
    glRotatef(-angle3, 1, 0, 0);
    glTranslatef(0, -SQUARE_SIDE, 0);
    drawSquare(SQUARE_SIDE);

    glPopMatrix();


    //square4:
    glColor3f(1,0,1);

    glPushMatrix();
    glTranslatef(SQUARE_SIDE, 0, 0);
    glRotatef(-angle4, 0, 1, 0);
    glTranslatef(SQUARE_SIDE, 0, 0);
    drawSquare(SQUARE_SIDE);

    glPopMatrix();


    //square5:
    glColor3f(0,1,1);

    glPushMatrix();
    glTranslatef(SQUARE_SIDE, 0, 0);
    glRotatef(-angle4, 0, 1, 0);
    glTranslatef(2*SQUARE_SIDE, 0, 0);
    glRotatef(-angle5, 0, 1, 0);
    glTranslatef(SQUARE_SIDE, 0, 0);
    drawSquare(SQUARE_SIDE);

    glPopMatrix();




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=120.0;
	cameraAngle=1.0;

	wheelAngleX = 0;
	wheelRotationAngle = 0;

	wheelCenter.x = 0;
	wheelCenter.y = 0;
	wheelCenter.z = WHEEL_RADIUS;

    wheelFront.x = 1;
    wheelFront.y = 0;
    wheelFront.z = 0;

    angle1 = 0;
    angle2 = 0;
    angle3 = 0;
    angle4 = 0;
    angle5 = 0;


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
    printf("%f %f\n", WHEEL_FORWARD_MOVE_ANGLE, WHEEL_FORWARD_MOVE_DISTANCE);

	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

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
