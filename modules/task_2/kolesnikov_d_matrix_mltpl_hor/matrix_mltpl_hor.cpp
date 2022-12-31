// Copyright 2022 Kolesnikov Denis
#include "../../../modules/task_2/kolesnikov_d_matrix_mltpl_hor/matrix_mltpl_hor.h"

vector<int> GenRndMtrx(int size_x, int size_y) {
  std::random_device dev;
  std::mt19937_64 gen(dev());
  std::uniform_int_distribution<int> dist(0, 1000);
  vector<int> result(size_x * size_y);
  for (int i = 0; i < size_x * size_y; ++i) {
    result[i] = dist(gen);
  }
  return result;
}

int CoorldLin(int x, int y, int size_x) {
  return y * size_x + x;
}

vector<int> MatrixMtlplSeq(
    const vector<int>& a,
    int a_height,
    int a_width,
    const vector<int>& b,
    int b_height,
    int b_width) {
  if (a_height == 0 || a_width == 0 || b_height == 0 || b_width == 0) {
    return vector<int>();
  }
  int tmp;
  vector<int> c(a_height*b_width);
  for (int i = 0; i < a_height; i++) {
    for (int j = 0; j < b_width; j++) {
      c[i * b_width + j] = 0;
      tmp = 0;
      for (int k = 0; k < a_width; k ++) {
        tmp += a[i * a_width + k] * b[k * b_width + j];
      }
      c[i * b_width + j] = tmp;
    }
  }
  return c;
}

vector<int> MatrixMtlplPrl(
    const vector<int>& a,
    int a_height,
    int a_width,
    const vector<int>& b,
    int b_height,
    int b_width) {
  int proc_num;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int i = 0, j = 0;
  int row_a = a_height / proc_num;
  int row_b = b_height / proc_num;
  int row_c = row_a;
  int part_a = row_a * a_width;
  int part_b = row_b * b_width;
  int part_c = row_c * b_width;

  vector<int> c(a_height * b_width);
  vector<int> buf_a(part_a);
  vector<int> buf_b(part_b);
  vector<int> buf_c(part_c);

  MPI_Scatter(
    a.data(),
    part_a,
    MPI_INT,
    buf_a.data(),
    part_a,
    MPI_INT,
    0,
    MPI_COMM_WORLD);
  MPI_Scatter(
    b.data(),
    part_b,
    MPI_INT,
    buf_b.data(),
    part_b,
    MPI_INT,
    0,
    MPI_COMM_WORLD);
  int k = 0 , tmp = 0;
  int next_proc = rank + 1;
  if ( rank == proc_num - 1 ) next_proc = 0;
  int prev_proc = rank - 1;
  if ( rank == 0 ) prev_proc = proc_num - 1;
  MPI_Status Status;
  int prev_data_num = rank;
  for (int p = 0; p < proc_num; p++) {
    for (i = 0; i < row_a; i++) {
        for (j = 0; j < b_width; j++) {
          tmp = 0;
          for (k = 0; k < row_b; k++) {
              tmp += buf_a[prev_data_num * row_b + i * a_width + k] * buf_b[k * b_width + j];
          }
          buf_c[i * b_width + j] += tmp;
        }
    }
    prev_data_num -= 1;
    if (prev_data_num < 0) {
      prev_data_num = proc_num - 1;
    }
    MPI_Sendrecv_replace(
        buf_b.data(),
        part_b,
        MPI_INT,
        next_proc,
        0,
        prev_proc,
        0,
        MPI_COMM_WORLD,
        &Status);
  }
  MPI_Gather(
    buf_c.data(),
    part_c,
    MPI_INT,
    c.data(),
    part_c,
    MPI_INT,
    0,
    MPI_COMM_WORLD);
  return c;
}
