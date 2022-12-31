// Copyright 2022 Zorin Oleg
#include "../../modules/task_3/zorin_o_moore_alg/moore_alg.h"

#include <mpi.h>

#include <deque>
#include <iostream>
#include <string>

Vector expandVector(const Vector& V, int shift) {
  Vector res = V;
  res.insert(res.end(), shift, INF);
  return res;
}

Matrix expandMatrix(const Matrix& M, int shift) {
  Matrix res = M;

  for (Vector& row : res) {
    row = expandVector(row, shift);
  }
  res.insert(res.end(), shift, Vector(M.size() + shift, INF));

  return res;
}

Vector MooreAlgSequential(const Matrix& weight_matrix, int start) {
  int size = weight_matrix.size();
  std::deque<int> Q;
  Q.push_back(start);
  Vector d(size, INF);
  d[start] = 0;
  Vector was(size);

  while (!Q.empty()) {
    int curr = Q.front();
    Q.pop_front();
    int weight_curr = d[curr];
    const Vector& curr_weight_row = weight_matrix[curr];
    for (int i = 0; i < size; i++) {
      if (curr_weight_row[i] < INF) {
        int tmp = d[i];
        d[i] = std::min(tmp, weight_curr + curr_weight_row[i]);
        if (d[i] < tmp) {
          for (auto it = Q.begin(); it != Q.end(); it++) {
            if (*it == i) {
              Q.erase(it);
              was[i] = 1;
              break;
            }
          }
          if (was[i]) {
            Q.push_front(i);
          } else {
            Q.push_back(i);
          }
        }
      }
    }
  }

  return d;
}

Vector MooreAlgParallel(const Matrix& weight_matrix, int start) {
  int prank, psize;
  MPI_Comm_size(MPI_COMM_WORLD, &psize);
  MPI_Comm_rank(MPI_COMM_WORLD, &prank);

  Matrix M = weight_matrix;
  int size = weight_matrix.size();
  int delta = size / psize;
  int shift = 0;
  if (size % psize != 0) {
    shift = psize * (++delta) - size;
    M = expandMatrix(M, shift);
    size = M.size();
  }

  Vector d(size, INF);
  d[start] = 0;
  Vector was(size);

  std::deque<int> Q;
  Q.push_back(start);
  while (!Q.empty()) {
    int curr = Q.front();
    Q.pop_front();
    int weight = d[curr];
    Vector curr_weight_row = M[curr];
    Vector to_front(size, -1);
    Vector to_back(size, -1);
    Vector to_erase(size, -1);
    for (int i = prank * delta; i < prank * delta + delta; i++) {
      if (curr_weight_row[i] < INF) {
        int tmp = d[i];
        d[i] = std::min(tmp, weight + curr_weight_row[i]);
        if (d[i] < tmp) {
          for (auto it = Q.begin(); it != Q.end(); it++) {
            if (*it == i) {
              to_erase[i] = i;
              was[i] = 1;
              break;
            }
          }
          if (was[i]) {
            to_front[i] = i;
          } else {
            to_back[i] = i;
          }
        }
      }
    }

    MPI_Allgather(MPI_IN_PLACE, delta, MPI_INT,
                  to_front.data(), delta, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, delta, MPI_INT,
                  to_back.data(), delta, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, delta, MPI_INT,
                  to_erase.data(), delta, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, delta, MPI_INT, d.data(), delta,
                  MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, delta, MPI_INT, was.data(), delta,
                  MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < size; i++) {
      if (to_erase[i] > -1) {
        for (auto it = Q.begin(); it != Q.end(); it++) {
          if (*it == to_erase[i]) {
            Q.erase(it);
            break;
          }
        }
      }
    }
    for (int i = 0; i < size; i++) {
      if (to_front[i] > -1) {
        Q.push_front(to_front[i]);
      }
    }
    for (int i = 0; i < size; i++) {
      if (to_back[i] > -1) {
        Q.push_front(to_back[i]);
      }
    }
  }

  d.erase(d.end() - shift, d.end());
  return d;
}
