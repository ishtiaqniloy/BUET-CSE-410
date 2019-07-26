#include<cstdio>
#include<cstdlib>
#include<cmath>
#include <ctime>

#include <iostream>
#include <vector>

#include <windows.h>
#include <glut.h>

#include "bitmap_image.hpp"

#define INF 99999
#define EPSILON 0.00001
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

#define VERBOSE 0


using namespace std;

int LOR;    //level of recursion
int num_pixels; //number of pixels along one axis
int num_objects; //number of objects in the scene
int num_lights; //number of light sources


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


    Vector3D operator + (Vector3D const &obj) {
        Vector3D result;
        result.x = x + obj.x;
        result.y = y + obj.y;
        result.z = z + obj.z;

        return result;
    }

    Vector3D operator - (Vector3D const &obj) {
        Vector3D result;
        result.x = x - obj.x;
        result.y = y - obj.y;
        result.z = z - obj.z;

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

    bool operator == (Vector3D const &obj) {
        return (x == obj.x && y == obj.y && z == obj.z);
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

Vector3D vecI(1, 0, 0);
Vector3D vecJ(0, 1, 0);
Vector3D vecK(0, 0, 1);

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

    if(a.getVal()==0 || b.getVal() == 0){
        return -INF;
    }

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

    Point3D(Vector3D v){
        x = v.x;
        y = v.y;
        z = v.z;
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
        result.setX(x + obj.getX());
        result.setY(y + obj.getY());
        result.setZ(z + obj.getZ());

        return result;
    }

    void operator = (Point3D const &obj) {
        x = obj.x;
        y = obj.y;
        z = obj.z;

    }

    void setPoint3D(Point3D newPoint){
        x = newPoint.x;
        y = newPoint.y;
        z = newPoint.z;
    }

    bool operator == (Point3D const &obj) {
        return (x == obj.x && y == obj.y && z == obj.z);
    }

    void operator = (double const &val) {
        x = val;
        y = val;
        z = val;
    }

    bool operator == (double const &val) {
        if(x == val && y == val && z == val)
            return true;
        else
            return false;
    }


    Point3D getCopy(){
        Point3D result;
        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    }

    Vector3D getPositionVector(){
        Vector3D result;
        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    }


};

Point3D origin(0, 0, 0);
Point3D INF_Point(INF, INF, INF);

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

    void setColor(ColorRGB newColor){
        r = newColor.r;
        g = newColor.g;
        b = newColor.b;
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
        result.r = r + obj.r;
        result.g = g + obj.g;
        result.b = b + obj.b;

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

ColorRGB white(1, 1, 1);
ColorRGB black(0, 0, 0);
ColorRGB red(1, 0, 0);
ColorRGB green(0, 1, 0);
ColorRGB blue(0, 0, 1);
ColorRGB yellow(1, 1, 0);
ColorRGB cyan(0, 1, 1);
ColorRGB magenta(1, 0, 1);


class LightSource{
public:
    ColorRGB lightColor;
    Point3D lightPosition;
    double radius;

    LightSource(){
        lightColor = white.getCopy();
        Point3D point(0, 0, 0);
        lightPosition = point;
        radius = 2;
    }

    LightSource(ColorRGB color, Point3D point, double r = 2){
        lightColor = color;
        lightPosition = point;
        radius = r;
    }
};

Point3D camera_pos;
Vector3D camera_u, camera_r, camera_l;
vector <LightSource *> lights;


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

    virtual ColorRGB getColorAt(Point3D pos){
        printf("BASE CLASS!!!!!");
        return objectColor.getCopy();
    }

    virtual Vector3D getNormalAt(Point3D pos){
        printf("BASE CLASS!!!!!");
        Vector3D temp;
        return temp;
    }

    virtual void draw(){
        //do nothing
        printf("BASE CLASS!!!!!");
    }

    virtual Point3D getIntersection(Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST){
        //do nothing
        printf("BASE CLASS!!!!!");

        return INF_Point.getCopy();
    }

    virtual void printProperties(){
        //do nothing
        printf("BASE CLASS!!!!!");
    }

};

class CheckerBoard : public SceneObject{
public:
    CheckerBoard(){
        setCoefficients(1, 0, 0, 0, 1);
    }

    Vector3D getNormalAt(Point3D pos){
        return vecK;
    }

    ColorRGB getColorAt(Point3D pos){
        int i =  floor(1.0*pos.getX()/BOARD_SIZE);
        int j =  floor(1.0*pos.getY()/BOARD_SIZE);
        if( abs(i+j)%2){
            return objectColor.getCopy();
        }
        else{
            return black.getCopy();
        }
    }

    void draw(){
        for(int i = -BOARD_LIMIT; i < BOARD_LIMIT; i+=BOARD_SIZE ){
            for(int j = -BOARD_LIMIT; j < BOARD_LIMIT; j += BOARD_SIZE ){
                if(abs((i+j)/BOARD_SIZE)%2){
                    glColor3f(objectColor.r, objectColor.g, objectColor.b);
                }
                else{
                    glColor3f(0, 0, 0);
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

    Point3D getIntersection(Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST){

        Point3D result = INF_Point.getCopy();
        if( abs(Vd.z) < EPSILON){  //PARALLEL
            if(eyePos.z == 0){  //eye on the same plane as checkerboard
                result = (eyePos + (Vd*NEAR_DIST));
            }
            else{
                //printf("CASE 1\n");
                //printf("Vd    : <%f, %f, %f>\n", Vd.x, Vd.y, Vd.z);
                return INF_Point.getCopy();
            }

        }
        else{
            double t = -1.0*(eyePos.z)/Vd.z;
            if(t < 0){    //BACKSIDE
                //printf("CASE 2\n");
                return INF_Point.getCopy();
            }
            result = eyePos + (Vd*t);   //intersecting point on the plane
        }

        if(getDistance(eyePos, result) < nearPointDistance){    //BEFORE NEAR PLANE
            //printf("CASE 3\n");
            return INF_Point.getCopy();
        }

        return result;

    }


};

class Sphere : public SceneObject{
public:
    Point3D center;
    double radius;

    Sphere(){
        setCoefficients(1, 0, 0, 0, 1);
    }

    ColorRGB getColorAt(Point3D pos){
        return objectColor.getCopy();
    }


    Vector3D getNormalAt(Point3D pos){
        return (pos-center);
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


    Point3D getIntersection(Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST){

        Vector3D Ve = eyePos.getPositionVector()-center.getPositionVector();

        if(VERBOSE >= 2){
            printf("In sphere intersection: \n");
            printf("Eye   : <%f, %f, %f>\n", eyePos.x, eyePos.y, eyePos.z);
            printf("Eye   : <%f, %f, %f>\n", center.x, center.y, center.z);
            printf("Vd    : <%f, %f, %f>\n", Vd.x, Vd.y, Vd.z);
            printf("Ve    : <%f, %f, %f>\n", Ve.x, Ve.y, Ve.z);

        }

        double a = dotProduct(Vd, Vd);
        double b = 2*dotProduct(Ve, Vd);
        double c = dotProduct(Ve, Ve) - radius*radius;

        double det = b*b-4*a*c;

        if(VERBOSE >= 1){
            printf("a=%f, b=%f, c=%f, det=%f\n", a, b, c, det);
        }




        if(det < 0){
            //solution not real
            return INF_Point.getCopy();
        }

        double t1 = 1.0*(-b + sqrt(det))/(2*a);
        double t2 = 1.0*(-b - sqrt(det))/(2*a);

        Point3D p1 = eyePos + (Vd*t1);
        Point3D p2 = eyePos + (Vd*t1);

        double dist1 = getDistance(p1, eyePos);
        double dist2 = getDistance(p2, eyePos);

        if((t1 < 0 || dist1 < nearPointDistance) && (t2 < 0 || dist2 < nearPointDistance)){ //both invalid
            return INF_Point.getCopy();
        }
        else if(t2 < 0 || dist2 < nearPointDistance){   //p2 invalid
            return p1;
        }
        else{   //only p2 valid
            return p2;
        }
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
        normal = vecK;
        setCoefficients(1, 0, 0, 0, 1);
    }

    void setNormal(Vector3D n){
        normal = n;
        normal.normalize();
    }

    ColorRGB getColorAt(Point3D pos){
        return objectColor.getCopy();
    }


    Vector3D getNormalAt(Point3D pos){
        return normal.getCopy();
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

    Point3D getIntersection(Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST){

        Vector3D Ve = (a-eyePos);
        Vector3D p0 = a.getPositionVector();
        Vector3D r0 = eyePos.getPositionVector();

        Point3D result = INF_Point.getCopy();

        double denominator = dotProduct(Vd,normal);

        if(denominator < 0){
            normal = normal.getOppositeVector();
            denominator = dotProduct(Vd,normal);
        }

        if( abs(denominator)<EPSILON && abs(dotProduct(Ve,normal))<EPSILON){ //parallel and on the same plane
            result = (eyePos + (Vd*NEAR_DIST));
        }
        else if(abs(denominator)<EPSILON){   //only parallel
            //printf("Case 1\n");
            return INF_Point.getCopy();
        }
        else{
            double t = 1.0*(dotProduct(Ve,normal))/denominator;

            if(t < 0){  //BACKSIDE
                //printf("Case 2\n");
                return INF_Point.getCopy();
            }

            result = eyePos + (Vd*t);   //intersecting point on the plane
        }

        if(getDistance(eyePos, result) < nearPointDistance){    //BEFORE NEAR PLANE
            //printf("Case 3\n");
            return INF_Point.getCopy();
        }

        //upto this, result carries the intersection point of the plane and the ray

        if( abs(result.z - a.z) > EPSILON){    //Invalid somehow
            //printf("Case 4\n");
            return INF_Point.getCopy();
        }

        if(  result.x < a.x || result.y < a.y || result.x > a.x+length || result.y > a.y+length){  //not inside triangle
            //printf("Case 5\n");
            return INF_Point.getCopy();
        }

        //printf("SQUARE HIT\n");
        return result;

    }


};


bool checkSide(Point3D c1, Point3D c2, Point3D a, Point3D b){
    Vector3D AB = (b-a);
    Vector3D AC1 = (c1-a);
    Vector3D AC2 = (c2-a);

    Vector3D cross1 = crossProduct(AB, AC1);
    Vector3D cross2 = crossProduct(AB, AC2);

    return (dotProduct(cross1, cross2) >= 0);

}


class Triangle : public SceneObject{
public:
    Point3D a, b, c;

    Vector3D normal;

    Triangle(){
        setCoefficients(1, 0, 0, 0, 1);
    }

    setNormal(){
        Vector3D v1 = (b-a);
        Vector3D v2 = (c-a);

        normal = crossProduct(v1, v2);
        normal.normalize();

    }

    ColorRGB getColorAt(Point3D pos){
        return objectColor.getCopy();
    }

    Vector3D getNormalAt(Point3D pos){
        return normal.getCopy();
    }


    void draw(){
        glColor3f(objectColor.r, objectColor.g, objectColor.b);
        glBegin(GL_TRIANGLES);{
            glVertex3f( a.x,  a.y, a.z);
            glVertex3f( b.x,  b.y, b.z);
            glVertex3f( c.x,  c.y, c.z);
        }glEnd();
    }

    Point3D getIntersection(Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST){

        Vector3D Ve = (a-eyePos);
        Vector3D p0 = a.getPositionVector();
        Vector3D r0 = eyePos.getPositionVector();

        Point3D result = INF_Point.getCopy();

        double denominator = dotProduct(Vd,normal);

        if(denominator < 0){
            normal = normal.getOppositeVector();
            denominator = dotProduct(Vd,normal);
        }

        if( abs(denominator)<EPSILON && abs(dotProduct(Ve,normal))<EPSILON){ //parallel and on the same plane
            result = (eyePos + (Vd*NEAR_DIST));
        }
        else if(abs(denominator)<EPSILON){   //only parallel
            //printf("Case 1\n");
            return INF_Point.getCopy();
        }
        else{
            double t = 1.0*(dotProduct(Ve,normal))/denominator;

            if(t < 0){  //BACKSIDE
                //printf("Case 2\n");
                return INF_Point.getCopy();
            }

            result = eyePos + (Vd*t);   //intersecting point on the plane
        }

        if(getDistance(eyePos, result) < nearPointDistance){    //BEFORE NEAR PLANE
            //printf("Case 3\n");
            return INF_Point.getCopy();
        }

        //upto this, result carries the intersection point of the plane and the ray

        if(!checkSide(result, a, b,c) || !checkSide(result, b, c,a) || !checkSide(result, c, a,b)){ //outside triangle
            //printf("Case 4\n");
            return INF_Point.getCopy();
        }

        return result;

    }

};


vector <SceneObject *> objects;


void createImage(ColorRGB** colorArr, int row = num_pixels, int col = num_pixels, char* imageName = NULL){
    if(imageName == NULL){
        imageName = new char[32];
        strcpy(imageName, "out.bmp");
    }

    bitmap_image image(row, col);
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            image.set_pixel(j, i, 255*colorArr[i][j].r, 255*colorArr[i][j].g, 255*colorArr[i][j].b);
        }
    }
    image.save_image(imageName);
    printf("Created image %s\n", imageName);

}


ColorRGB findPixelColor(Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST, int recurLevel = 1);

ColorRGB getLightColors(Point3D minPoint, SceneObject *minObject, Point3D eyePos, Vector3D Vd, double nearPointDistance = NEAR_DIST, int recurLevel = 1){
    //printf("In getLightColors()\n");
    if(recurLevel == 0){
        return black.getCopy();
    }
    ColorRGB objectColor = minObject->getColorAt(minPoint);
    Vector3D normal = minObject->getNormalAt(minPoint);
    ColorRGB lColor = black.getCopy();

    int len = lights.size();
    for(int i=0; i<len; i++){
        LightSource *currLight = lights[i];

        Vector3D L = (minPoint-currLight->lightPosition);
        if(dotProduct(L,normal) > 0){
            normal = normal.getOppositeVector().getCopy();
            normal.normalize();
        }

        Vector3D R = normal*2*dotProduct(L,normal)-L;
        double theta = getRadAngleVectors(R,normal);

        Vector3D V = Vd.getOppositeVector();
        double phi = getRadAngleVectors(R, V);

        if(cos(theta) > EPSILON){   //diffusion light
            //printf("Adding diff\n");
            lColor = lColor + objectColor*cos(theta)*minObject->diffCoeff;
            //lColor = lColor + objectColor*dotProduct(L*(-1),normal)*minObject->diffCoeff;
        }

        if(cos(phi) > EPSILON){   //specular light
            //printf("Adding spec\n");
            lColor = lColor + currLight->lightColor*(pow(cos(phi), minObject->specExp))*minObject->specCoeff;
            //lColor = lColor + currLight->lightColor*(pow(dotProduct(R,V), minObject->specExp))*minObject->specCoeff;
        }
        if(recurLevel > 1){
            //return lColor;  //without reflection
            //printf("Adding ref\n");
            return (lColor+findPixelColor(minPoint, R, NEAR_DIST, recurLevel-1));
        }

    }

    return lColor;

}


ColorRGB findPixelColor(Point3D eyePos, Vector3D Vd, double nearPointDistance, int recurLevel){

    if(recurLevel == 0){
        return black.getCopy();
    }

    int len = objects.size();

    int minIdx = -1;
    double minDist = INF;
    Point3D minPoint = INF_Point.getCopy();
    ColorRGB minColor  = black;

    for(int i=0; i<len; i++){
        Point3D intersectPoint = objects[i]->getIntersection(eyePos, Vd, nearPointDistance);
        if(VERBOSE >= 1  ){
            printf("Object %d-> Intersect Point: <%f,%f,%f>\n", i, intersectPoint.x, intersectPoint.y, intersectPoint.z);
        }
        if(intersectPoint == INF_Point){
            continue;
        }
        double dist = getDistance(intersectPoint, eyePos);
        if(dist < nearPointDistance){
            continue;
        }
        if(dist < minDist){
            minDist = dist;
            minIdx = i;
            minPoint = intersectPoint;
        }

    }

    if(minDist < INF){
        if(VERBOSE >=1){
            printf("Pixel hit with object %d\n", minIdx);
        }

        SceneObject* minObject = objects[minIdx];

        //minColor = minObject->getColorAt(minPoint);   //basic
        //minColor = minObject->getColorAt(minPoint)*minObject->ambCoeff;   //only  ambient light
        //minColor = minObject->getColorAt(minPoint)*minObject->ambCoeff + getLightColors(minPoint, minObject, eyePos, Vd, nearPointDistance, recurLevel);

    }



    return minColor;


}

Point3D ** getPixelPositions(){
    //working with pixelPositions array
    Point3D **result = new Point3D *[num_pixels];
    for(int i=0; i<num_pixels; i++){
        result[i] = new Point3D [num_pixels];
    }

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

    if(VERBOSE >= 0){
        printf("Camera : <%f, %f, %f>\n", camera_pos.x, camera_pos.y, camera_pos.z);
        printf("Look   : <%f, %f, %f>\n", look.x, look.y, look.z);
        printf("Up     : <%f, %f, %f>\n", up.x, up.y, up.z);
        printf("Down   : <%f, %f, %f>\n", down.x, down.y, down.z);
        printf("Right  : <%f, %f, %f>\n", right.x, right.y, right.z);
        printf("Left   : <%f, %f, %f>\n", left.x, left.y, left.z);
    }




    Point3D midPoint = camera_pos+(camera_l*NEAR_DIST);
    double half = NEAR_DIST*tan(DEGREE_TO_RADIAN*fov_Y*0.5);
    Point3D highPoint = midPoint + (left*half) + (up*half);

    double pixelLen = 2.0*half/num_pixels;

    if(VERBOSE >= 0){
         printf("Mid   : <%f, %f, %f>\n", midPoint.x, midPoint.y, midPoint.z);
        printf("High  : <%f, %f, %f>\n", highPoint.x, highPoint.y, highPoint.z);
        printf("Screen Size = %f\n", 2.0*half);
        printf("Pixel Size  = %f\n", pixelLen);
        printf("\n");
    }





    for (int i = 0; i < num_pixels; i++) {
        for (int j = 0; j < num_pixels; j++) {
            Point3D temp = highPoint + (down*(i+0.5) + right*(j+0.5))*pixelLen;
            result[i][j].setPoint3D(temp.getCopy());
        }
    }

    return result;


}

void captureScene(){
    //capture here
    clock_t startTime, endTime;
    double cpu_time_used;
    startTime = clock();
    printf("\n\nCapture Scene initiated\n");

    Point3D** pixelPositions = getPixelPositions();

    ColorRGB** pixelColors = new ColorRGB*[num_pixels];
    //initialize
    for (int i = 0; i < num_pixels; i++) {
        pixelColors[i] = new ColorRGB[num_pixels];
        for (int j = 0; j < num_pixels; j++) {
            pixelColors[i][j].setColor(black.getCopy());
        }
    }

    //for each pixel
    for (int i = 0; i < num_pixels; i++) {
        for (int j = 0; j < num_pixels; j++) {
            Point3D pixelPos = pixelPositions[i][j];

            Vector3D Vd = (pixelPos-camera_pos);
            Vd.normalize();

            if(VERBOSE >= 1){
                printf("Pixel : <%d,%d>\n", i, j);
                printf("Pos   : <%f, %f, %f>\n", pixelPos.x, pixelPos.y, pixelPos.z);
                printf("Cam   : <%f, %f, %f>\n", camera_pos.x, camera_pos.y, camera_pos.z);
                printf("Vd    : <%f, %f, %f>\n", Vd.x, Vd.y, Vd.z);
            }


            double nearPointDistance = getDistance(pixelPos, camera_pos);

            ColorRGB pixelColor = findPixelColor(camera_pos, Vd, nearPointDistance, LOR); //camera_pos or pixelPos???

            pixelColors[i][j].setColor(pixelColor.getCopy());
        }
    }


    createImage(pixelColors, num_pixels, num_pixels, "output.bmp");
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
	Sphere temp;
    SceneObject *obj = &temp;
	for(int i=0; i<lights.size(); i++){
        LightSource *light = lights[i];

        temp.setColor(light->lightColor);
        temp.radius = light->radius;
        temp.center = light->lightPosition;

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

    //checkerboard
    CheckerBoard *board = new CheckerBoard();
    SceneObject *tempObj = board;
    tempObj->setColor(white.getCopy());
    tempObj->setCoefficients(0.4, 0.2, 0.2, 0.2, 1);
    objects.push_back(tempObj);

    //lights
    fgets(dummy, 10, sceneFile);
    fgets(dummy, 10, sceneFile);
    fscanf(sceneFile, "%d", &num_lights);

    for(int i; i<num_lights; i++){
        fscanf(sceneFile, "%lf %lf %lf", &x, &y, &z);
        Point3D lightPos(x, y, z);

        LightSource *light = new LightSource(white.getCopy(), lightPos);
        lights.push_back(light);

        printf("light at %f %f %f\n", lights[i]->lightPosition.x, lights[i]->lightPosition.y, lights[i]->lightPosition.z);
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














