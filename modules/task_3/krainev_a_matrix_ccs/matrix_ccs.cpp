// Copyright 2022 Krainev Alexander
#include "../../../modules/task_3/krainev_a_matrix_ccs/matrix_ccs.h"

std::vector<double> gen_random_matrix_with_zeros(int r, int c) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> urd(-100, 100);

  int size = r * c;

  std::vector<double> matrix(size);
  for (int i = 0; i < size; i++) {
    double value = urd(gen);

    if (std::abs(value) > 15) {
      matrix[i] = 0;
    } else {
      matrix[i] = value;
    }
  }

  return matrix;
}

Matrix gen_random_matrix(int r, int c) {
  if (r < 1 || c < 1) {
    throw "Error";
  }

  std::vector<double> _matrix = gen_random_matrix_with_zeros(r, c);

  Matrix matrix;
  matrix.r = r;
  matrix.c = c;
  matrix.col.resize(c + 1);
  matrix.col[0] = 0;

  int size = r * c;
  int count;

  for (int x = 0; x < c; x++) {
    count = 0;

    for (int y = x; y < size; y += c) {
      if (_matrix[y] != 0) {
        matrix.non_zero.push_back(_matrix[y]);
        matrix.row.push_back(y / c);

        count++;
      }
    }

    matrix.col[x + 1] = matrix.col[x] + count;
  }

  matrix.count = matrix.col[c];
  return matrix;
}

std::vector<double> sequential(const Matrix& A, const Matrix& B) {
  if (A.c != B.r) {
    throw "Error";
  }
  std::vector<double> result(A.r * B.c, 0);

  for (int a = 0; a < A.c; a++) {
    for (int b = 0; b < B.c; b++) {
      for (int i = A.col[a]; i < A.col[a + 1]; i++) {
        for (int j = B.col[b]; j < B.col[b + 1]; j++) {
          if (a == B.row[j]) {
            result[A.row[i] * B.c + b] += A.non_zero[i] * B.non_zero[j];
          }
        }
      }
    }
  }
  return result;
}

Matrix Bcast(const Matrix& _M) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  Matrix M = _M;

  MPI_Bcast(&M.c, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&M.r, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&M.count, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (proc_rank > 0) {
    M.non_zero.resize(M.count);
    M.row.resize(M.count);
    M.col.resize(M.c + 1);
  }

  MPI_Bcast(M.non_zero.data(), M.count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(M.row.data(), M.count, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(M.col.data(), M.c + 1, MPI_INT, 0, MPI_COMM_WORLD);
  return M;
}

std::vector<double> parallel(const Matrix& _A, const Matrix& _B) {
  if (_A.c != _B.r) {
    throw "Error";
  }
  int proc_rank, proc_count;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  if (proc_count == 1) {
    return sequential(_A, _B);
  }

  Matrix A = Bcast(_A);
  Matrix B = Bcast(_B);

  std::vector<double> proc_result(A.r * B.c, 0);

  for (int a = proc_rank; a < A.c; a += proc_count) {
    for (int b = 0; b < B.c; b++) {
      for (int i = A.col[a]; i < A.col[a + 1]; i++) {
        for (int j = B.col[b]; j < B.col[b + 1]; j++) {
          if (a == B.row[j]) {
            proc_result[A.row[i] * B.c + b] += A.non_zero[i] * B.non_zero[j];
          }
        }
      }
    }
  }

  std::vector<double> result(A.r * B.c);

  MPI_Reduce(proc_result.data(), result.data(), A.r * B.c, MPI_DOUBLE, MPI_SUM,
             0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  return result;
}
