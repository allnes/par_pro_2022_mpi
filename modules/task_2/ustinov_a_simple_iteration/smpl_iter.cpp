// Copyright 2022 Ustinov A.
#include "../../../modules/task_2/ustinov_a_simple_iteration/smpl_iter.h"
#include <mpi.h>
#include <random>
#include <cmath>
#include <cstring>

using std::vector;
using std::mt19937_64;
using std::random_device;
using std::uniform_real_distribution;
using std::abs;
using std::max;
using std::memcpy;

vector<double> simple_iteration_method_parallel(
        const vector<double> &A_vector,
        const vector<double> &b_vector,
        int n,
        int max_iter_num,
        double epsilon) {
    vector<double> x_vector(n);  // vector of system's solution
    int size;           // total number of processes
    int rank;           // number of current process
    int b_begin;        // position of beginning of block in 'A', 'b' and 'x'
    int b_size;         // length of block in 'A', 'b' and 'x'
    double *A_buf;      // local buffer for rows of 'A'
    double *b_buf;      // local buffer for segment of 'b'
    double *x;          // pointer to vector 'x'
    double *x_buf;      // local buffer for part of current iteration's solution
    double local_diff;  // local norm of difference of curr and last solutions
    double total_diff;  // total norm of solutions' difference
    MPI_Status status;  // stores information about call of 'MPI_Recv'

    /* DATA PREPARATION */

    // 'A_buf' stores rows of 'A' in range [b_begin; b_begin + b_size)
    // 'b_buf' stores elements of 'b' in range [b_begin; b_begin + b_size)
    // 'x_buf' stores elements computed on current iteration
    //   in range [b_begin; b_begin + b_size)
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    b_begin = rank*n/size;
    b_size = ((rank+1)*n)/size - (rank*n)/size;
    A_buf = new double[b_size*n];
    b_buf = new double[b_size];
    x_buf = new double[b_size];
    x     = x_vector.data();

    if (rank == 0) {
        int sb_begin,  // position of beginning of the sent block
            sb_end;    // position of end of the sent block
        const double *A = A_vector.data();  // pointer to matrix 'A'
        const double *b = b_vector.data();  // pointer to vector 'b'

        memcpy(A_buf, A, b_size * n * sizeof(double));
        memcpy(b_buf, b, b_size * sizeof(double));
        for (int i = 1; i < size; ++i) {
            sb_begin = i*n/size;
            sb_end   = (i+1)*n/size;
            MPI_Send(A + sb_begin*n, (sb_end - sb_begin)*n, MPI_DOUBLE, i, 0,
                     MPI_COMM_WORLD);
            MPI_Send(b + sb_begin, sb_end - sb_begin, MPI_DOUBLE, i, 1,
                     MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(A_buf, b_size*n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(b_buf, b_size, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    }

    // divide rows of extended matrix by diagonal element
    for (int i = 0; i < b_size; ++i) {
        double aii = A_buf[(n+1)*i + b_begin];
        A_buf[(n+1)*i + b_begin] = 0.0;
        b_buf[i] /= aii;
        for (int j = 0; j < n; ++j)
            A_buf[i*n + j] /= aii;
    }


    /* MAIN PART */

    // the stopping criteria is reaching the given number of iterations
    for (int itern = 0; itern < max_iter_num; ++itern) {
        // perform current iteration
        for (int i = 0; i < b_size; ++i) {
            x_buf[i] = b_buf[i];
            for (int j = 0; j < n; ++j)
                x_buf[i] -= A_buf[i*n + j]*x[j];
        }
        // compute local difference between adjacent iteration results
        local_diff = 0.0;
        for (int i = 0; i < b_size; ++i)
            local_diff = max(local_diff, abs(x_buf[i] - x[b_begin + i]));
        MPI_Barrier(MPI_COMM_WORLD);
        // check total difference (reduced local differences on maximum)
        MPI_Reduce(&local_diff, &total_diff, 1, MPI_DOUBLE, MPI_MAX, 0,
                   MPI_COMM_WORLD);
        MPI_Bcast(&total_diff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if (total_diff < epsilon)
            break;

        // send last-iteration solution to all processes
        if (rank == 0) {
            memcpy(x, x_buf, b_size * sizeof(double));
            for (int i = 1; i < size; ++i)
                MPI_Recv(x + i*n/size, (i+1)*n/size - i*n/size, MPI_DOUBLE, i,
                         2, MPI_COMM_WORLD, &status);
        } else {
            MPI_Send(x_buf, b_size, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        }
        MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    delete[] A_buf;
    delete[] b_buf;
    delete[] x_buf;
    return x_vector;
}

vector<double> simple_iteration_method_sequential(
        const vector<double> &A_vector,
        const vector<double> &b_vector,
        int n,
        int max_iter_num,
        double epsilon) {
    vector<double> x_vector(n);  // vector of system's solution
    const double *A;  // pointer to matrix 'A'
    const double *b;  // pointer to vector 'b'
          double *x;  // pointer to vector 'x'
    double *A_buf;  // copy of 'A'
    double *b_buf;  // copy of 'b'
    double *x_new;  // result of current iteration
    double diff;    // norm of solutions' difference


    /* DATA PREPARATION */

    A = A_vector.data();
    b = b_vector.data();
    x = x_vector.data();
    A_buf  = new double[n*n];
    b_buf  = new double[n];
    x_new  = new double[n];

    memcpy(A_buf, A, n*n*sizeof(double));
    memcpy(b_buf, b, n*sizeof(double));

    // divide rows of extended matrix by diagonal element
    for (int i = 0; i < n; ++i) {
        double aii = A_buf[(n+1)*i];
        A_buf[(n+1)*i] = 0.0;
        b_buf[i] /= aii;
        for (int j = 0; j < n; ++j)
            A_buf[i*n + j] /= aii;
    }


    /* MAIN PART */

    // the stopping criteria is reaching the given number of iterations
    for (int itern = 0; itern < max_iter_num; ++itern) {
        // perform current iteration
        for (int i = 0; i < n; ++i) {
            x_new[i] = b_buf[i];
            for (int j = 0; j < n; ++j)
                x_new[i] -= A_buf[i*n + j]*x[j];
        }
        // compute local difference between sadjacent iteration results
        diff = 0.0;
        for (int i = 0; i < n; ++i)
            diff = max(diff, abs(x_new[i] - x[i]));
        if (diff < epsilon)
            break;
        memcpy(x, x_new, n*sizeof(double));
    }

    delete[] A_buf;
    delete[] b_buf;
    delete[] x_new;
    return x_vector;
}


std::vector<double> matrix_vector_product(
        const vector<double> &A_vector,
        const vector<double> &x_vector,
        int n) {
    vector<double> b_vector(n);
    const double *A = A_vector.data();
    const double *x = x_vector.data();
          double *b = b_vector.data();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            b[i] += A[i*n + j] * x[j];
    }

    return b_vector;
}

vector<double> get_random_vector(size_t n) {
    vector<double> vec(n);
    mt19937_64 generator {random_device {}()};
    uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < n; ++i)
        vec[i] = distribution(generator);

    return vec;
}

vector<double> get_random_stable_matrix(size_t n) {
    vector<double> mtx(n * n);
    double *data = mtx.data();
    double diagonal_element;
    mt19937_64 generator {random_device {}()};
    uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < n; ++i) {
        diagonal_element = 0.0;
        for (size_t j = 0; j < n; ++j) {
            data[i*n + j] = distribution(generator);
            diagonal_element += data[i*n + j];
        }
        data[i*(n+1)] = 2.0 * diagonal_element;
    }

    return mtx;
}
