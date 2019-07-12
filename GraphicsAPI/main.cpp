#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

#define pi (2*acos(0.0))
#define DEGREE_TO_RADIAN (pi/180.0)
#define HOMOGENEOUS_DIM 4

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

    void setValues(int new_n, int new_m, T** new_arr){
        if(n!=new_n || m!=new_m){
            printf("SIZE MISMATCH!!!");
            return;
        }
        else if(arr == NULL){
            printf("NOT INITIALIZED!!!");
            return;
        }

        for (int i = 0; i < n; i++) {
            if (arr[i] == NULL){
                printf("NOT INITIALIZED!!!");
                return;
            }
            for (int j = 0; j < m; ++j) {
                arr[i][j] = new_arr[i][j];
            }
        }
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

    Matrix operator + (Matrix const &obj) {
        if(n != obj.n || m != obj.m){
            printf("SIZE MISMATCH!!!");
            exit(1);
        }
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] + obj.arr[i][j];
            }
        }
        return result;
    }

    Matrix operator - (Matrix const &obj) {
        if(n != obj.n || m != obj.m){
            printf("SIZE MISMATCH!!!");
            exit(1);
        }
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] - obj.arr[i][j];
            }
        }

        return result;
    }

    Matrix operator + (T const &val) {
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] + val;
            }
        }
        return result;
    }

    Matrix operator - (T const &val) {
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] - val;
            }
        }
        return result;
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

    Matrix operator * (Matrix const &obj) {     //computationally inefficient matrix multiplication
        if(m != obj.n){
            printf("SIZE MISMATCH!!!");
            exit(1);
        }
        Matrix<T> result(n, obj.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < obj.m; j++) {
                T val = 0;
                for (int k = 0; k < m; k++) {
                    val += arr[i][k] * obj.arr[k][j];
                }
                result.arr[i][j] = val;
            }
        }

        return result;
    }

    Matrix operator * (T const &val) {     //scalar multiplication
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] * val;
            }
        }

        return result;
    }

    Matrix operator / (T const &val) {     //scalar division
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] / val;
            }
        }

        return result;
    }



    Matrix elementWiseProduct (Matrix const &obj) {
        if(n != obj.n || m != obj.m){
            printf("SIZE MISMATCH!!!");
            exit(1);
        }
        Matrix<T> result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result.arr[i][j] = arr[i][j] * obj.arr[i][j];
            }
        }

        return result;
    }

    T getTrace(){
        T val = 0;
        for (int i = 0; i < n && i < m; i++) {
            val += arr[i][i];
        }
        return val;
    }


    void printMatrix(){
        printf("\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                //printf("%f ", arr[i][j]);
                std::cout << arr[i][j] << " ";
            }
            printf("\n");
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

    void setN(int n) {
        Matrix::n = n;
    }

    int getM() const {
        return m;
    }

    void setM(int m) {
        Matrix::m = m;
    }

};

class Vector{
public:
    double x, y, z;

    Vector(double varX = 0, double varY  = 0, double varZ = 0){
        x = varX;
        y = varY;
        z = varZ;
    }

    void normalize(){
        double val = sqrt(x*x + y*y + z*z);
        x = x/val;
        y = y/val;
        z = z/val;
    }

    Vector operator + (Vector const &obj) {
        Vector result;
        result.x = x + obj.x;
        result.y = y + obj.y;
        result.z = z + obj.z;

        return result;
    }

    Vector operator * (double const &val) {
        Vector result;
        result.x = x * val;
        result.y = y * val;
        result.z = z * val;

        return result;
    }

    void operator = (Vector const &obj) {
        x = obj.x;
        y = obj.y;
        z = obj.z;

    }

};

class Point{
public:
    double x, y, z;

    Vector operator - (Point const &obj) {
        Vector result;
        result.x = x - obj.x;
        result.y = y - obj.y;
        result.z = z - obj.z;

        return result;
    }


};



Vector crossProduct(Vector a, Vector b){
    Vector result;

    result.x = a.y*b.z - b.y*a.z;
    result.y = a.z*b.x - b.z*a.x;
    result.z = a.x*b.y - b.x*a.y;

    return result;
}

double dotProduct(Vector a, Vector b){
    return a.x*b.x + a.y*b.y +a.z*b.z;
}

//Vector rotateVector(Vector v, Vector refer, double rotationAngle){
//    Vector result, perp;
//
//    double radianRotationAngle = rotationAngle*pi/180.0;
//
//    //perp = refer X v
//    //result = v*cos(radianRotationAngle) + perp*sin(radianRotationAngle)
//
//    perp = crossProduct(refer, v);
//
//    result.x = v.x*cos(radianRotationAngle) + perp.x*sin(radianRotationAngle);
//    result.y = v.y*cos(radianRotationAngle) + perp.y*sin(radianRotationAngle);
//    result.z = v.z*cos(radianRotationAngle) + perp.z*sin(radianRotationAngle);
//
//
//    return result;
//}

Vector rotateVectorRodrigues(Vector x, Vector a, double theta){
    double thetaRad = DEGREE_TO_RADIAN*theta;
    Vector result = x*(cos(thetaRad)) + a * (dotProduct(a, x) * (1- cos(thetaRad))) + crossProduct(a, x) * (sin(thetaRad));

    return result;
}

int main() {

    stack <Matrix<double >> saveStack;
    Matrix <double> transformationMatrix(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, true);

    printf("Initial Transformation Matrix:");
    transformationMatrix.printMatrix();

    //input and output files
    FILE * sceneFile = fopen("scene.txt", "r");
    FILE * stage1File = fopen("stage1.txt", "w");
    FILE * stage2File = fopen("stage2.txt", "w");
    FILE * stage3File = fopen("stage3.txt", "w");
    if(sceneFile == NULL || stage1File == NULL || stage2File==NULL || stage3File==NULL){
        printf("ERROR OPENING FILE %d %d %d %d\n", sceneFile, stage1File, stage2File, stage3File);
        exit(0);
    }

//=======================================================================================================
//View Transformation
//=======================================================================================================

    //gluLookAt parameters
    Point eye, look;
    Vector up;

    fscanf(sceneFile, "%lf %lf %lf", &eye.x, &eye.y, &eye.z);
    fscanf(sceneFile, "%lf %lf %lf", &look.x, &look.y, &look.z);
    fscanf(sceneFile, "%lf %lf %lf", &up.x, &up.y, &up.z);

//    printf("%lf %lf %lf\n", eyePos.x, eyePos.y, eyePos.z);
//    printf("%lf %lf %lf\n", lookPos.x, lookPos.y, lookPos.z);
//    printf("%lf %lf %lf\n", upVector.x, upVector.y, upVector.z);

    Vector lVector = look - eye;
    lVector.normalize();

    Vector rVector = crossProduct(lVector , up);
    rVector.normalize();

    Vector uVector = crossProduct(rVector, lVector);

    Matrix <double > T(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, true);
    T.setVal(0, HOMOGENEOUS_DIM-1, -eye.x);
    T.setVal(1, HOMOGENEOUS_DIM-1, -eye.y);
    T.setVal(2, HOMOGENEOUS_DIM-1, -eye.z);
    printf("\nT Matrix:");
    T.printMatrix();

    Matrix <double > R(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, true);
    R.setVal(0, 0,  rVector.x);
    R.setVal(0, 1,  rVector.y);
    R.setVal(0, 2,  rVector.z);

    R.setVal(1, 0,  uVector.x);
    R.setVal(1, 1,  uVector.y);
    R.setVal(1, 2,  uVector.z);

    R.setVal(2, 0,  -lVector.x);
    R.setVal(2, 1,  -lVector.y);
    R.setVal(2, 2,  -lVector.z);
    printf("\nR Matrix:");
    R.printMatrix();

    Matrix <double > V = R*T;
    printf("\nV Matrix:");
    V.printMatrix();

//=======================================================================================================
//Projection Transformation
//=======================================================================================================
    //gluPerspective parameters
    double fovY, aspectRatio, near, far;

    fscanf(sceneFile, "%lf %lf %lf %lf", &fovY, &aspectRatio, &near, &far);
//    printf( "%lf %lf %lf %lf\n", fovY, aspectRatio, near, far);


    double fovX = fovY * aspectRatio;
    double t = near * tan(DEGREE_TO_RADIAN*fovY/2.0);
    double r = near * tan(DEGREE_TO_RADIAN*fovX/2.0);

    Matrix <double > P(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, false);
    P.setVal(0, 0, near/r);

    P.setVal(1, 1, near/t);

    P.setVal(2, 2, -(far+near)/(far-near));
    P.setVal(2, 3, -(2*far*near)/(far-near));

    P.setVal(3, 2, -1);

    printf("\nP Matrix:");
    P.printMatrix();


    char *inputString = new char[128];

    while (true){
        fscanf(sceneFile, "%s", inputString);
        printf("%s\n", inputString);
        if(strcmp(inputString, "end")==0){
            break;
        }
        else if(strcmp(inputString, "triangle")==0){
            for (int i = 0; i < 3; i++) {
                Point p;
                fscanf(sceneFile, "%lf %lf %lf", &p.x, &p.y, &p.z);

                Matrix <double > pointMatrix(HOMOGENEOUS_DIM, 1, false);
                pointMatrix.setVal(0, 0, p.x);
                pointMatrix.setVal(1, 0, p.y);
                pointMatrix.setVal(2, 0, p.z);
                pointMatrix.setVal(3, 0, 1);

                double w = 0;

                Matrix <double > stage1Matrix = transformationMatrix*pointMatrix;
                w = stage1Matrix.getVal(HOMOGENEOUS_DIM-1, 0);
                stage1Matrix.setVal(0, 0, stage1Matrix.getVal(0, 0)/w);
                stage1Matrix.setVal(1, 0, stage1Matrix.getVal(1, 0)/w);
                stage1Matrix.setVal(2, 0, stage1Matrix.getVal(2, 0)/w);

                Matrix <double > stage2Matrix = V*stage1Matrix;
                w = stage2Matrix.getVal(HOMOGENEOUS_DIM-1, 0);
                stage2Matrix.setVal(0, 0, stage2Matrix.getVal(0, 0)/w);
                stage2Matrix.setVal(1, 0, stage2Matrix.getVal(1, 0)/w);
                stage2Matrix.setVal(2, 0, stage2Matrix.getVal(2, 0)/w);

                Matrix <double > stage3Matrix = P*stage2Matrix;
                w = stage3Matrix.getVal(HOMOGENEOUS_DIM-1, 0);
                stage3Matrix.setVal(0, 0, stage3Matrix.getVal(0, 0)/w);
                stage3Matrix.setVal(1, 0, stage3Matrix.getVal(1, 0)/w);
                stage3Matrix.setVal(2, 0, stage3Matrix.getVal(2, 0)/w);

//                printf("\npointMatrix Matrix:");
//                pointMatrix.printMatrix();

                fprintf(stage1File, "%lf %lf %lf\n", stage1Matrix.getVal(0,0), stage1Matrix.getVal(1,0), stage1Matrix.getVal(2,0));
//                printf("\nstage1Matrix Matrix:");
//                stage1Matrix.printMatrix();

                fprintf(stage2File, "%lf %lf %lf\n", stage2Matrix.getVal(0,0), stage2Matrix.getVal(1,0), stage2Matrix.getVal(2,0));
//                printf("\nstage2Matrix Matrix:");
//                stage2Matrix.printMatrix();

                fprintf(stage3File, "%lf %lf %lf\n", stage3Matrix.getVal(0,0), stage3Matrix.getVal(1,0), stage3Matrix.getVal(2,0));
//                printf("\nstage3Matrix Matrix:");
//                stage3Matrix.printMatrix();

            }
            fprintf(stage1File, "\n");
            fprintf(stage2File, "\n");
            fprintf(stage3File, "\n");


        }
        else if(strcmp(inputString, "translate")==0){
            Vector translate;
            fscanf(sceneFile, "%lf %lf %lf",&translate.x, &translate.y, &translate.z);

            Matrix <double > translateMatrix(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, true);
            translateMatrix.setVal(0, HOMOGENEOUS_DIM-1, translate.x);
            translateMatrix.setVal(1, HOMOGENEOUS_DIM-1, translate.y);
            translateMatrix.setVal(2, HOMOGENEOUS_DIM-1, translate.z);

            printf("translateMatrix:");
            translateMatrix.printMatrix();

            Matrix <double > temp = transformationMatrix*translateMatrix;
            transformationMatrix = temp;
            printf("transformationMatrix after translate:");
            transformationMatrix.printMatrix();


        }
        else if(strcmp(inputString, "scale")==0){
            Vector scale;
            fscanf(sceneFile, "%lf %lf %lf",&scale.x, &scale.y, &scale.z);

            Matrix <double > scaleMatrix(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, true);
            scaleMatrix.setVal(0, 0, scale.x);
            scaleMatrix.setVal(1, 1, scale.y);
            scaleMatrix.setVal(2, 2, scale.z);

            printf("scaleMatrix:");
            scaleMatrix.printMatrix();

            Matrix <double > temp = transformationMatrix*scaleMatrix;
            transformationMatrix = temp;
            printf("transformationMatrix after scale:");
            transformationMatrix.printMatrix();
        }
        else if(strcmp(inputString, "rotate")==0){
            double angle;
            Vector a;

            fscanf(sceneFile, "%lf %lf %lf %lf",&angle, &a.x, &a.y, &a.z);
            a.normalize();

            Vector i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);

            Vector c1 = rotateVectorRodrigues(i, a, angle);
            Vector c2 = rotateVectorRodrigues(j, a, angle);
            Vector c3 = rotateVectorRodrigues(k, a, angle);

            Matrix <double > rotationMatrix(HOMOGENEOUS_DIM, HOMOGENEOUS_DIM, true);

            rotationMatrix.setVal(0, 0, c1.x);
            rotationMatrix.setVal(1, 0, c1.y);
            rotationMatrix.setVal(2, 0, c1.z);

            rotationMatrix.setVal(0, 1, c2.x);
            rotationMatrix.setVal(1, 1, c2.y);
            rotationMatrix.setVal(2, 1, c2.z);

            rotationMatrix.setVal(0, 2, c3.x);
            rotationMatrix.setVal(1, 2, c3.y);
            rotationMatrix.setVal(2, 2, c3.z);

            printf("rotationMatrix:");
            rotationMatrix.printMatrix();

            Matrix <double > temp = transformationMatrix*rotationMatrix;
            transformationMatrix = temp;
            printf("transformationMatrix after rotation:");
            transformationMatrix.printMatrix();



        }
        else if(strcmp(inputString, "push")==0){
            saveStack.push(transformationMatrix.getCopy());
        }
        else if(strcmp(inputString, "pop")==0){
            transformationMatrix = saveStack.top();
            saveStack.pop();

            printf("Transformation Matrix after pop:");
            transformationMatrix.printMatrix();
        }
        else{
//            printf("ERROR IN INPUT");
//            break;
        }




    }


    return 0;
}