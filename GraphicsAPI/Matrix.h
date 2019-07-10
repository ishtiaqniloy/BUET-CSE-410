//
// Created by Ishtiaq Niloy on 10-Jul-19.
//

#include <cstdio>
#include <iostream>

template <typename T>
class Matrix{
    T **arr;
    int n;
    int m;

public:
    Matrix(int n_var, int m_var){
        n = n_var;
        m = m_var;

        arr = new T *[n];
        for (int i = 0; i < n; i++) {
            arr[i] = new T[m];
            for (int j = 0; j < m; j++) {
                arr[i][j] = 0;
            }
        }
    }

    ~Matrix(){
        for (int i = 0; i < n; ++i) {
            delete []arr[i];
        }
        delete []arr;
    }

    void setValues(int new_n, int new_m, T** new_arr){
        if(n!=new_n || m!=new_m){
            printf("SIZE MISMATCH!!!");
            return;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; ++j) {
                arr[i][j] = new_arr[i][j];
            }
        }
    }


    void printMatrix(){
        printf("\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; ++j) {
                //printf("%f ", arr[i][j]);
                std::cout << arr[i][j] << " ";
            }
            printf("\n");
        }

    }

};