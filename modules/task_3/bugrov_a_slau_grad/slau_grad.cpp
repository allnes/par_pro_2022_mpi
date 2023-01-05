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
    matrix_or_vector[i] = engine() % 7;
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
  Vector p_part(sendcounts[rank]);
  Vector A_part(matr_sendcounts[rank]);
  Vector matrxp_part(sendcounts[rank]);

  matrxp = MatrVecMult(A, x);
  for (j = 0; j < n; j++) {
    p[j] = b[j] - matrxp[j];
    r[j] = p[j];
  }

  // nt MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root,
  // MPI_Comm comm)
  MPI_Bcast(p.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(r.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrxp.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  /*int MPI_Scatterv(const void* sendbuf, const int sendcounts[],
                 const int displs[], MPI_Datatype sendtype, void* recvbuf,
                 int recvcount, MPI_Datatype recvtype, int root,
                 MPI_Comm comm)*/
  MPI_Scatterv(p.data(), sendcounts, displs, MPI_DOUBLE, p_part.data(),
               sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatterv(matrxp.data(), sendcounts, displs, MPI_DOUBLE,
               matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  MPI_Scatterv(A.data(), matr_sendcounts, matr_displs, MPI_DOUBLE,
               A_part.data(), matr_sendcounts[rank], MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  //MPI_Bcast(&alpha, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  //MPI_Bcast(&beta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


  for (i = 0; i < n; i++) {
    matrxp_part = MatrVecMult(A_part, p_part);
    MPI_Gatherv(matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, matrxp.data(),
                sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(matrxp.data(), sendcounts, displs, MPI_DOUBLE,
                 matrxp_part.data(), sendcounts[rank], MPI_DOUBLE, 0,
                 MPI_COMM_WORLD);
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
    MPI_Bcast(p.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(p.data(), sendcounts, displs, MPI_DOUBLE, p_part.data(),
                 sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  delete[] sendcounts;
  delete[] matr_sendcounts;
  delete[] displs;
  delete[] matr_displs;
  /*delete[] p_part;
  delete[] A_part;
  delete[] matrxp_part;*/
  return x;
}

// Vector getParSlauGrad(Vector A, Vector b, int n) {
//  int proc_num, rank;
//  const kEps = 0.01;
//  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//  int i, j;
//  Vector x(n);
//  Vector r(n), p(n), matrxp(n);
//  double alpha, beta;
//  double rr;
//  int part = n / proc_num;
//  int remainder = n % proc_num;
//
//  int* sendcounts = new int[proc_num];
//  int* displs = new int[proc_num];
//  for (i = 0; i < proc_num - 1; i++) {
//    sendcounts[i] = part;
//    displs[i] = part * i;
//  }
//  sendcounts[proc_num - 1] = part + remainder;
//  displs[proc_num - 1] = part * (proc_num - 1);
//
//  double x_part = new double[sendcounts[rank]];
//  double p_part = new double[sendcounts[rank]];
//  double r_part = new double[sendcounts[rank]];
//  double matrxp_part = new double[sendcounts[rank]];
//  double A_part = new double[sendcounts[rank] * n];
//
//  /*int MPI_Scatterv(const void* sendbuf, const int sendcounts[],
//                   const int displs[], MPI_Datatype sendtype, void* recvbuf,
//                   int recvcount, MPI_Datatype recvtype, int root,
//                   MPI_Comm comm)*/
//  if (rank == 0) {
//    rr = ScalMult(r, r);
//    matrxp = MatrVecMult(A, x);
//    for (j = 0; j < n; j++) {
//      p[j] = b[j] - matrxp[j];
//      r[j] = p[j];
//    }
//    alpha = rr / ScalMult(matr_p, p);
//    beta = ScalMult(r, r) / rr;
//  }
//  MPI_Scatter(&rr, 1, MPI_DOUBLE, &&rr, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//  MPI_Scatter(&alpha, 1, MPI_DOUBLE, &&rr, 1, MPI_DOUBLE, 0,
//  MPI_COMM_WORLD); MPI_Scatter(&beta, 1, MPI_DOUBLE, &&rr, 1, MPI_DOUBLE, 0,
//  MPI_COMM_WORLD);
//
//  MPI_Scatterv(x + rank * part, sendcounts, displs, MPI_DOUBLE, x_part,
//               sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
//  MPI_Scatterv(p + rank * part, sendcounts, displs, MPI_DOUBLE, p_part,
//               sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
//  MPI_Scatterv(r + rank * part, sendcounts, displs, MPI_DOUBLE, r_part,
//               sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
//  MPI_Scatterv(matrxp + rank * part, sendcounts, displs, MPI_DOUBLE,
//               matrxp_part, sendcounts[rank], MPI_DOUBLE, 0,
//               MPI_COMM_WORLD);
//  MPI_Scatterv(A + rank * part*n, sendcounts, displs, MPI_DOUBLE, A_part,
//               sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
//
//  for (i = 0; i < sendcounts[rank]; i++) {
//    if (sqrt(ScalMult(r, r)) > kEps) {
//      break;
//    }
//  }
//  return x;
//}
