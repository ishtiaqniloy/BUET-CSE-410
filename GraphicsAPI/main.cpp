#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

#define pi (2*acos(0.0))

using namespace std;

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

class Point{
public:
    double x, y, z;
};

class Vector{
public:
    double x, y, z;
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
    FILE * sceneFile = fopen("scene.txt", "r");
    FILE * stage1File = fopen("stage1.txt", "w");
    FILE * stage2File = fopen("stage2.txt", "w");
    FILE * stage3File = fopen("stage3.txt", "w");
    if(sceneFile == NULL || stage1File == NULL || stage2File==NULL || stage3File==NULL){
        printf("ERROR OPENING FILE %d %d %d %d\n", sceneFile, stage1File, stage2File, stage3File);
        exit(0);
    }

    



    return 0;
}