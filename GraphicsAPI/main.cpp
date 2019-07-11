#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

#define pi (2*acos(0.0))
#define dim 4

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
    void normalize(){
        double val = sqrt(x*x + y*y + z*z);
        x = x/val;
        y = y/val;
        z = z/val;
    }
};

class Point{
public:
    double x, y, z;

    Vector operator - (Point const &obj) {
        Vector result;
        result.x = obj.x - x;
        result.y = obj.y - y;
        result.z = obj.z - z;

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

int main() {

    stack <Matrix<double >> saveStack;
    Matrix <double> transformationMatrix(dim, dim, true);

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

    //gluLookAt parameters
    Point eyePos, lookPos;
    Vector upVector;

    fscanf(sceneFile, "%lf %lf %lf", &eyePos.x, &eyePos.y, &eyePos.z);
    fscanf(sceneFile, "%lf %lf %lf", &lookPos.x, &lookPos.y, &lookPos.z);
    fscanf(sceneFile, "%lf %lf %lf", &upVector.x, &upVector.y, &upVector.z);

//    printf("%lf %lf %lf\n", eyePos.x, eyePos.y, eyePos.z);
//    printf("%lf %lf %lf\n", lookPos.x, lookPos.y, lookPos.z);
//    printf("%lf %lf %lf\n", upVector.x, upVector.y, upVector.z);

    //View Transformation Matrix

    Vector l = lookPos - eyePos;
    l.normalize();

    Vector r = crossProduct(l , upVector);
    r.normalize();

    Vector u = crossProduct(r, l);

    Matrix <double > T(dim, dim, true);
    T.setVal(0, dim-1, -eyePos.x);
    T.setVal(1, dim-1, -eyePos.y);
    T.setVal(2, dim-1, -eyePos.z);
    printf("\nT Matrix:");
    T.printMatrix();

    Matrix <double > R(dim, dim, true);
    R.setVal(0, 0,  r.x);
    R.setVal(0, 1,  r.y);
    R.setVal(0, 2,  r.z);

    R.setVal(1, 0,  u.x);
    R.setVal(1, 1,  u.y);
    R.setVal(1, 2,  u.z);

    R.setVal(2, 0,  -l.x);
    R.setVal(2, 1,  -l.y);
    R.setVal(2, 2,  -l.z);
    printf("\nR Matrix:");
    R.printMatrix();

    Matrix <double > V = R*T;
    printf("\nV Matrix:");
    V.printMatrix();





    //gluPerspective parameters
    double fovY, aspectRatio, near, far;

    fscanf(sceneFile, "%lf %lf %lf %lf", &fovY, &aspectRatio, &near, &far);
//    printf( "%lf %lf %lf %lf\n", fovY, aspectRatio, near, far);


    char *inputString = new char[128];

    while (true){
        fscanf(sceneFile, "%s", inputString);
        printf("%s\n", inputString);
        if(strcmp(inputString, "end")==0){
            break;
        }
        else if(strcmp(inputString, "triangle")==0){

        }
        else if(strcmp(inputString, "translate")==0){

        }
        else if(strcmp(inputString, "scale")==0){

        }
        else if(strcmp(inputString, "rotate")==0){

        }
        else if(strcmp(inputString, "push")==0){
            saveStack.push(transformationMatrix);
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