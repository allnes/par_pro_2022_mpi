// Copyright 2022 Ivlev A
#ifndef  MODULES_TASK_3_IVLEV_A_MULT_CCS_MULT_CCS_H_
#define  MODULES_TASK_3_IVLEV_A_MULT_CCS_MULT_CCS_H_

#include <mpi.h>
#include <vector>
#include <string>

class matrix_ccs {
 public:
    int m;
    int n;
    int val_n;
    double* values;
    int* rows;
    int* index;

    matrix_ccs(int m_, int n_, int val_n_);
    matrix_ccs(const matrix_ccs &c);

    void operator = (const matrix_ccs &c);
    friend bool operator== (const matrix_ccs &b, const matrix_ccs &c);

    double get(int m_, int n_);
    void create_rand();
    matrix_ccs mult(matrix_ccs b);
    matrix_ccs mpi_mult(matrix_ccs b);
    matrix_ccs get_column(int start, int col);
    matrix_ccs add_column_matrix(matrix_ccs c);
    void print();
    void all_print();

    ~matrix_ccs();
};

#endif  // MODULES_TASK_3_IVLEV_A_MULT_CCS_MULT_CCS_H_
