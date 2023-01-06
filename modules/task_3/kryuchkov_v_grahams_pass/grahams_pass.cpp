// Copyright 2022 Kryuchkov Vladimir
#include "../../../modules/task_3/kryuchkov_v_grahams_pass/grahams_pass.h"

#include <mpi.h>

#include <cmath>
#include <random>
#include <stack>
#include <vector>

std::vector<int> p_0(2);

int rotate(const std::vector<int>& a, const std::vector<int>& b,
           const std::vector<int>& c) {
  int res = (b[0] - a[0]) * (c[1] - b[1]) - (b[1] - a[1]) * (c[0] - b[0]);
  if (res == 0) {
    return 0;
  }
  if (res > 0) {
    return 1;
  } else {
    return -1;
  }
}

int distance(const std::vector<int>& a, const std::vector<int>& b) {
  return (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]);
}

int compare(const void* a, const void* b) {
  int* p = reinterpret_cast<int*>(const_cast<void*>(a));
  int* q = reinterpret_cast<int*>(const_cast<void*>(b));
  std::vector<int> p_1 = {*p, *(p + 1)};
  std::vector<int> p_2 = {*q, *(q + 1)};
  int res = rotate(p_0, p_1, p_2);
  if (res == 0) {
    if (distance(p_0, p_2) >= distance(p_0, p_1)) {
      return 1;
    } else {
      return -1;
    }
  }
  return res;
}

std::vector<int> get_points(int count_p, int centre_x, int centre_y, int r) {
  std::mt19937 gen(std::random_device().operator()());
  std::uniform_int_distribution<> range(5, r);
  std::vector<int> res(count_p * 2);
  int count_r = count_p / 63 + 1;
  int remains = 63 - count_p % 63;
  int j = 0;
  for (double i = 0; i < 6.27; i += 0.1) {
    for (int k = 0; k < count_r; k++) {
      if ((remains) && (!k)) {
        remains--;
        continue;
      }
      res[j * 2] = centre_x + static_cast<int>((range(gen)) * cos(i));
      res[j * 2 + 1] = centre_y + static_cast<int>((range(gen)) * sin(i));
      j++;
      if (j == count_p) {
        break;
      }
    }
    if (j == count_p) {
      break;
    }
  }
  return res;
}

std::vector<int> next_to_top(std::stack<std::vector<int>> st) {
  std::vector<int> p = st.top();
  st.pop();
  std::vector<int> res = st.top();
  st.push(p);
  return res;
}

std::vector<int> grahams_sequential(const std::vector<int>& p, int count_points,
                                    int* size_shell) {
  std::vector<int> points = p;
  int x_min = points[0], min_p = 0;
  for (int i = 1; i < count_points; i++) {
    if ((points[i * 2] < x_min) ||
        (points[i * 2] == x_min && points[i * 2 + 1] < points[min_p + 1])) {
      x_min = points[i * 2];
      min_p = i * 2;
    }
  }
  int tmp_x = points[0], tmp_y = points[1];
  points[0] = points[min_p];
  points[1] = points[min_p + 1];
  points[min_p] = tmp_x;
  points[min_p + 1] = tmp_y;
  p_0[0] = points[0];
  p_0[1] = points[1];
  qsort(points.data() + 2, (count_points - 1), 8, compare);
  std::stack<std::vector<int>> st;
  st.push({points[0], points[1]});
  st.push({points[2], points[3]});
  st.push({points[4], points[5]});
  for (int i = 3; i < count_points; i++) {
    while (rotate(next_to_top(st), st.top(),
                  {points[i * 2], points[i * 2 + 1]}) != -1)
      st.pop();
    st.push({points[i * 2], points[i * 2 + 1]});
  }
  int st_size = st.size();
  *size_shell = st_size;
  std::vector<int> shell(st_size * 2);
  std::vector<int> tmp(2);
  for (int i = 0; i < st_size; ++i) {
    tmp = st.top();
    shell[i * 2] = tmp[0];
    shell[i * 2 + 1] = tmp[1];
    st.pop();
  }
  return shell;
}

std::vector<int> grahams_parallel(const std::vector<int>& points,
                                  int count_points, int* size_shell) {
  int proc_count, proc_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  // int proc = count_points / 3 > proc_count ? proc_count : count_points / 3;
  int proc;
  if (count_points / 3 > proc_count) {
    proc = proc_count;
  } else {
    proc = count_points / 3;
  }
  int whole = count_points / proc;
  int remains = count_points % proc;
  int num_elem_block = 0;
  // if (proc_rank < proc) num_elem_block = whole + (proc_rank < remains ? 1 :
  // 0);
  if (proc_rank < proc) {
    num_elem_block = whole;
    if (proc_rank < remains) {
      num_elem_block += 1;
    }
  }
  std::vector<int> rcount(num_elem_block * 2);
  std::vector<int> sendcounts(0);
  std::vector<int> displs(0);
  if (proc_rank == 0) {
    sendcounts.resize(proc_count);
    displs.resize(proc_count);
    for (int i = 0; i < proc; i++) {
      sendcounts[i] = whole * 2;
    }
    for (int i = proc; i < proc_count; i++) {
      sendcounts[i] = 0;
    }
    for (int i = 0; i < remains; i++) {
      sendcounts[i] += 2;
    }
    displs[0] = 0;
    for (int i = 1; i < proc_count; i++) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
  }
  MPI_Scatterv(points.data(), sendcounts.data(), displs.data(), MPI_INT,
               rcount.data(), 2 * num_elem_block, MPI_INT, 0, MPI_COMM_WORLD);
  int shell_local_size = 0;
  int* p_sls = &shell_local_size;
  std::vector<int> shell_local(0);
  std::vector<int> shell_global(0);
  int size_global_shell = 0;
  if (proc_rank < proc) {
    shell_local = grahams_sequential(rcount, num_elem_block, p_sls);
    if (proc_rank == 0) {
      size_global_shell = shell_local_size;
      int buf;
      sendcounts[0] = shell_local_size * 2;
      for (int i = 1; i < proc; i++) {
        MPI_Recv(&buf, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        sendcounts[i] = buf * 2;
        size_global_shell += buf, displs[i] = displs[i - 1] + sendcounts[i - 1];
      }
      for (int i = proc; i < proc_count; i++) {
        sendcounts[i] = 0;
        displs[i] = displs[i - 1] + sendcounts[i - 1];
      }
      shell_global.resize(size_global_shell * 2);
    } else {
      MPI_Send(&shell_local_size, 1, MPI_INT, 0, proc_rank, MPI_COMM_WORLD);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Gatherv(shell_local.data(), shell_local_size * 2, MPI_INT,
              shell_global.data(), sendcounts.data(), displs.data(), MPI_INT, 0,
              MPI_COMM_WORLD);
  std::vector<int> res(0);
  if (proc_rank == 0) {
    if (proc_count) {
      res = grahams_sequential(shell_global, size_global_shell, size_shell);
    } else {
      *size_shell = size_global_shell;
      return shell_global;
    }
  }
  return res;
}
