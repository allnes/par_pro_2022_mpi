// Copyright 2022 Bugrov Andrey
#include "../../../modules/task_3/bugrov_a_slau_grad/slau_grad.h"

#include <mpi.h>

#include <cmath>
#include <ctime>
#include <random>

Vector getRandom(int sz) {
  std::mt19937 engine(time(0));
  Vector matrix_or_vector(sz);
  for (int i = 0; i < sz; i++) {
    matrix_or_vector[i] = 3 + engine() % 7;
  }
  return matrix_or_vector;
}

Vector MatrVecMult(const Vector& matrix, const Vector& vec) {
  int i;
  int n = vec.size();
  Vector result(n);
  int j;
  for (i = 0; i < n; i++) {
    result[i] = 0;
  }
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      result[i] += matrix[i * n + j] * vec[j];
    }
  }
  return result;
}

// m is row number
Vector ParMatrVecMult(const Vector& matrix, const Vector& vec, int m) {
  int i;
  int n = vec.size();
  Vector result(m);
  int j;
  for (i = 0; i < m; i++) {
    result[i] = 0;
  }
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      result[i] += matrix[i * n + j] * vec[j];
    }
  }
  return result;
}

double ScalMult(const Vector& first, const Vector& second) {
  int i;
  int n = first.size();
  double result = 0;
  for (i = 0; i < n; i++) {
    result += first[i] * second[i];
  }
  return result;
}

Vector getSeqSlauGrad(const Vector& A, const Vector& b, int n) {
  int i;
  int j;
  Vector r(n), p(n), matrxp(n);
  double alpha, beta;
  double rr;
  Vector x = getRandom(n);
  matrxp = MatrVecMult(A, x);
  for (j = 0; j < n; j++) {
    p[j] = b[j] - matrxp[j];
    r[j] = p[j];
  }
  for (i = 0; i < n; i++) {
    matrxp = MatrVecMult(A, p);
    rr = ScalMult(r, r);
    alpha = rr / ScalMult(matrxp, p);
    for (j = 0; j < n; j++) {
      x[j] += alpha * p[j];
      r[j] -= alpha * matrxp[j];
    }
    beta = ScalMult(r, r) / rr;
    for (j = 0; j < n; j++) {
      p[j] = r[j] + beta * p[j];
    }
  }
  return x;
}

Vector getParSlauGrad(const Vector& A, const Vector& b, int n) {
  int proc_num, rank;
  int i, j;
  Vector x = getRandom(n);
  Vector r(n), p(n), matrxp(n);
  double alpha, beta;
  double rr;  // (r, r)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int part = n / proc_num;
  int remainder = n % proc_num;

  int* sendcounts = new int[proc_num];
  int* matr_sendcounts = new int[proc_num];
  int* displs = new int[proc_num];
  int* matr_displs = new int[proc_num];
  for (i = 0; i < proc_num - 1; i++) {
    sendcounts[i] = part;
    displs[i] = part * i;
    matr_sendcounts[i] = part * n;
    matr_displs[i] = part * i * n;
  }
  sendcounts[proc_num - 1] = part + remainder;
  displs[proc_num - 1] = part * (proc_num - 1);
  matr_sendcounts[proc_num - 1] = (part + remainder) * n;
  matr_displs[proc_num - 1] = part * n * (proc_num - 1);
  Vector A_part(matr_sendcounts[rank]);
  Vector matrxp_part(sendcounts[rank]);

  matrxp = MatrVecMult(A, x);
  for (j = 0; j < n; j++) {
    p[j] = b[j] - matrxp[j];
    r[j] = p[j];
  }

  //// because x is random => matrxp is random => r,p are random
  MPI_Bcast(p.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(r.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  MPI_Scatterv(matrxp.data(), sendcounts, displs, MPI_DOUBLE,
               matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  MPI_Scatterv(A.data(), matr_sendcounts, matr_displs, MPI_DOUBLE,
               A_part.data(), matr_sendcounts[rank], MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  MPI_Gatherv(matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, matrxp.data(),
              sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  for (i = 0; i < n; i++) {
    matrxp_part = ParMatrVecMult(
        A_part, p, sendcounts[rank]);  // because it needs part, not part * n
    MPI_Gatherv(matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, matrxp.data(),
                sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Scatterv(matrxp.data(), sendcounts, displs, MPI_DOUBLE,
    //             matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, 0,
    //             MPI_COMM_WORLD);
    MPI_Bcast(matrxp.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    rr = ScalMult(r, r);
    alpha = rr / ScalMult(matrxp, p);
    for (j = 0; j < n; j++) {
      x[j] += alpha * p[j];
      r[j] -= alpha * matrxp[j];
    }
    beta = ScalMult(r, r) / rr;
    for (j = 0; j < n; j++) {
      p[j] = r[j] + beta * p[j];
    }
  }
  /*delete[] sendcounts;
  delete[] matr_sendcounts;
  delete[] displs;
  delete[] matr_displs;*/
  return x;
}
