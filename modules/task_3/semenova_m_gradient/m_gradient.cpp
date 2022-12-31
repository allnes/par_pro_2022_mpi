// Copyright 2022 Semenova Veronika
#include <mpi.h>

#include <random>

#include <iostream>

#include "../../modules/task_3/semenova_m_gradient/m_gradient.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution < int > dist(0, 10);

Vector RandVec(int n) {
  Vector V(n);
  for (int i = 0; i < n; ++i) {
    V[i] = dist(gen) % 3;
  }
  return V;
}
Vector RandMat(int n) {
  Vector M(n * n);
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      M[i * n + j] = dist(gen) % 3;
      M[j * n + i] = dist(gen) % 3;
    }
  }
  return M;
}

double scalar_mult(const Vector & x,
  const Vector & y) {
  double res = 0;
  for (int i = 0; i < x.size(); i++) {
    res = res + x[i] * y[i];
  }
  return res;
}
Vector mult_MxV(const Vector & M,
  const Vector & V) {
  Vector res(M.size() / V.size());
  for (int i = 0; i < M.size() / V.size(); i++) {
    res[i] = 0;
    for (int j = 0; j < V.size(); j++) {
      res[i] += M[i * V.size() + j] * V[j];
    }
  }
  return res;
}

Vector Serial_method_gradient(const Vector & A,
  const Vector & b, int n) {
  Vector r0(n), p0(n), tmp, x(n);
  double E = 0.01, c1 = 0.0, c2 = 0.0;
  for (int i = 0; i < n; i++) {
    x[i] = 1;
  }
  tmp = mult_MxV(A, x);
  for (int i = 0; i < n; i++) {
    r0[i] = b[i] - tmp[i];
    p0[i] = r0[i];
  }

  int j = 0;
  do {
    tmp = mult_MxV(A, p0);
    double t = scalar_mult(r0, r0);
    c1 = t / scalar_mult(p0, tmp);
    for (int i = 0; i < n; i++) {
      x[i] = x[i] + c1 * p0[i];
      r0[i] = r0[i] - c1 * tmp[i];
    }
    c2 = scalar_mult(r0, r0) / t;
    for (int i = 0; i < n; i++) {
      p0[i] = r0[i] + c2 * p0[i];
    }
    j++;
  } while (sqrt(scalar_mult(r0, r0)) > E && j <= n);
  return x;
}
Vector Paralle_method_gradient(const Vector & A,
  const Vector & b, int n) {
  int ProcNum = 0, rank = 0;
  double E = 0.01, c1 = 0.0, c2 = 0.0, y, part_t, part_z, t, z;
  Vector A1 = A, b1 = b, x(n);
  for (int i = 0; i < n; i++) {
    x[i] = 1;
  }

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int nP = n / ProcNum;
  int flag = n % ProcNum;
  MPI_Bcast(A1.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(b1.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  Vector partA(n * nP + flag * n);
  if (rank == 0) {
    for (int i = 0; i < n * nP + n * flag; i++) {
      partA[i] = A1[i];
    }
    if (nP != 0) {
      for (int i = 1; i < ProcNum; i++) {
        MPI_Send(& A1[0] + i * nP * n + flag * n, nP * n, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
      }
    }
  } else {
    if (nP != 0) {
      MPI_Recv(& partA[0], nP * n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, & status);
    }
  }

  Vector tmp = mult_MxV(partA, x);
  Vector r0(nP + flag), r1(nP + flag), p0(n), part_p(nP + flag), p_res(nP + flag);
  if (rank == 0) {
    for (int i = 0; i < nP + flag; i++) {
      r0[i] = b1[i] - tmp[i];
      p0[i] = r0[i];
    }
    if (nP != 0) {
      for (int i = 1; i < ProcNum; i++) {
        MPI_Recv(& p0[0] + i * nP + flag, nP, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, & status);
      }
    }
  } else {
    for (int i = 0; i < nP; i++)
      r0[i] = b1[rank * nP + flag + i] - tmp[i];
    if (nP != 0) {
      MPI_Send(& r0[0], nP, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }
  }
  MPI_Bcast(p0.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  int j = 0;
  do {
    tmp = mult_MxV(partA, p0);
    if (rank == 0) {
      for (int i = 0; i < nP + flag; i++) {
        part_p[i] = p0[i];
      }
    } else {
      for (int i = 0; i < nP; i++) {
        part_p[i] = p0[rank * nP + flag + i];
      }
    }
    part_t = scalar_mult(r0, r0);
    part_z = scalar_mult(part_p, tmp);
    MPI_Allreduce(& part_t, & t, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(& part_z, & z, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    c1 = t / z;
    for (int i = 0; i < n; i++) {
      x[i] += c1 * p0[i];
    }
    for (int i = 0; i < nP + flag; i++) {
      r1[i] = r0[i] - c1 * tmp[i];
    }
    part_t = scalar_mult(r1, r1);
    MPI_Allreduce(& part_t, & y, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    c2 = y / t;
    if (rank == 0) {
      for (int i = 0; i < nP + flag; i++) {
        p0[i] = r1[i] + c2 * p0[i];
      }
      if (nP != 0) {
        for (int i = 1; i < ProcNum; i++) {
          MPI_Recv(& p0[0] + i * nP + flag, nP, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, & status);
        }
      }
    } else {
      for (int i = 0; i < nP; i++) {
        p_res[i] = r1[i] + c2 * p0[rank * nP + flag + i];
      }
      if (nP != 0) {
        MPI_Send(& p_res[0], nP, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
      }
    }
    MPI_Bcast(p0.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    r0 = r1;
    j++;
  } while ((sqrt(y) > E) && (j <= n));
  return x;
}
