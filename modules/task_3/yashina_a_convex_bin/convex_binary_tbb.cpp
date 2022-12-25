// Copyright 2020 Silenko Dmitrii

#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include "../../../modules/task_3/silenko_d_convex_binary_tbb/convex_binary_tbb.h"

int** getRandomMas(const int n, const int m) {
  if (n <= 0) {
    throw "Wrong rows";
  } else if (m <= 0) {
    throw "wrong columns";
  }
  int** mas = new int*[n];
  std::mt19937 seed(n);
  std::uniform_real_distribution<> rnd(0, 200);
  for (int i = 0; i < n; i++)
    mas[i] = new int[m];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      mas[i][j] = static_cast<int>(rnd(seed)) % 2;
    }
  }
  return mas;
}

double length(int* p1, int* p2) {
  return(sqrt((p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1])));
}

double cosvec(int* p1, int* p2, int* p3) {
  double ax = p2[0] - p1[0];
  double ay = p2[1] - p1[1];
  double bx = p3[0] - p1[0];
  double by = p3[1] - p1[1];
  return ((ax*bx + ay * by) / (sqrt(ax*ax + ay * ay) * sqrt(bx*bx + by * by)));
}

std::vector<int*> Jarvis(int** convex_mas, int n) {
  std::vector<int*> res;
  if (n == 1) {
    res.push_back(convex_mas[0]);
  } else if (n == 2) {
    res.push_back(convex_mas[0]);
    res.push_back(convex_mas[1]);
  } else {
    int m = 0;

    res.push_back(convex_mas[m]);

    int* last = new int[2];
    int* beforelast = new int[2];
    last = convex_mas[m];
    beforelast[0] = convex_mas[m][0] - 2;
    beforelast[1] = convex_mas[m][1];

    double mincos, cos;
    int minind = 0;
    double maxlen = 0;
    while (1) {
      mincos = 2;
      for (int i = 0; i < n; i++) {
        cos = round(cosvec(last, beforelast, convex_mas[i]) * 10000000) / 10000000;
        if (cos < mincos) {
          minind = i;
          mincos = cos;
          maxlen = length(last, convex_mas[i]);
        } else {
          if (cos == mincos) {
            double len = length(last, convex_mas[i]);
            if (len > maxlen) {
              minind = i;
              maxlen = len;
            }
          }
        }
      }

      beforelast = last;
      last = convex_mas[minind];
      if (last == convex_mas[m])
        break;
      res.push_back(convex_mas[minind]);
    }
  }
  return res;
}

std::vector<int*> getComponent(int** mas, int n, int m, int** *convex_mas, int *num) {
  *num = 0;
  int kn = 0, km = 0, cur = 1;
  int A, B, C;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      kn = j - 1;
      if (kn+1 <= 0) {
        kn = 1;
        B = 0;
      } else {
        B = mas[i][kn];
      }
      km = i - 1;
      if (km+1 <= 0) {
        km = 1;
        C = 0;
      } else {
        C = mas[km][j];
      }
      A = mas[i][j];

      if (A == 0) {
      } else {
        *num = *num + 1;
        if (B == 0 && C == 0) {
          cur++;
          mas[i][j] = cur;
        }
        if (B != 0 && C == 0) {
          mas[i][j] = B;
        }
        if (B == 0 && C != 0) {
          mas[i][j] = C;
        }
        if (B != 0 && C != 0) {
          if (B == C) {
            mas[i][j] = B;
          } else {
            mas[i][j] = B;
            for (int k = 0; k < i; k++)
              for (int kk = 0; kk < m; kk++)
                if (mas[k][kk] == C)
                  mas[k][kk] = B;
          }
        }
      }
    }

  (*convex_mas) = new int*[*num];
  for (int i = 0; i < *num; i++)
    (*convex_mas)[i] = new int[3];

  int tmp = 0;
  for (int i =0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (mas[i][j] != 0) {
        (*convex_mas)[tmp][0] = j;
        (*convex_mas)[tmp][1] = i;
        (*convex_mas)[tmp][2] = mas[i][j];
        tmp++;
      }
    }

  std::vector<int*> res;

  res = Jarvis(*convex_mas, *num);
  return res;
}

std::vector<int*> Inside(std::vector<int*> hull, int** convex_mas, int n) {
  std::vector<int*> res;
  std::vector<int> err;
  std::vector<int> cur;

  int s = hull.size();
  for (int i = 0; i < s; i++) {
    int* tmp = hull[i];
    cur.push_back(tmp[2]);
  }

  for (int i = 0; i < n; i++) {
    auto result1 = std::find(cur.begin(), cur.end(), convex_mas[i][2]);
    auto result2 = std::find(err.begin(), err.end(), convex_mas[i][2]);
    if (result2 != err.end()) {
      continue;
    } else {
      if (result1 != cur.end()) {
        res.push_back(convex_mas[i]);
        int* tmp = convex_mas[i];
        err.push_back(tmp[2]);
      }
    }
  }
  return res;
}

std::vector<int*> getComponent_TBB(int** mas, int n, int m, int** *convex_mas, int *num, int num_thr) {
  tbb::task_scheduler_init init(num_thr);
  *num = 0;
  int kn = 0, km = 0, cur = 1;
  int A, B, C;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      kn = j - 1;
      if (kn + 1 <= 0) {
        kn = 1;
        B = 0;
      } else {
        B = mas[i][kn];
      }
      km = i - 1;
      if (km + 1 <= 0) {
        km = 1;
        C = 0;
      } else {
        C = mas[km][j];
      }
      A = mas[i][j];

      if (A == 0) {
      } else {
        *num = *num + 1;
        if (B == 0 && C == 0) {
          cur++;
          mas[i][j] = cur;
        }
        if (B != 0 && C == 0) {
          mas[i][j] = B;
        }
        if (B == 0 && C != 0) {
          mas[i][j] = C;
        }
        if (B != 0 && C != 0) {
          if (B == C) {
            mas[i][j] = B;
          } else {
            mas[i][j] = B;

            tbb::parallel_for(
              tbb::blocked_range<int>(0, i),
              [=](const tbb::blocked_range<int>& t) {
              int begin = t.begin(), end = t.end();
              for (int k = begin; k != end; k++)
                for (int kk = 0; kk < m; kk++)
                  if (mas[k][kk] == C) {
                    mas[k][kk] = B;
                  }
            });
          }
        }
      }
    }

  init.terminate();

  (*convex_mas) = new int*[*num];
  for (int i = 0; i < *num; i++)
    (*convex_mas)[i] = new int[3];

  int tmp = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (mas[i][j] != 0) {
        (*convex_mas)[tmp][0] = j;
        (*convex_mas)[tmp][1] = i;
        (*convex_mas)[tmp][2] = mas[i][j];
        tmp++;
      }
    }

  std::vector<int*> res;
  res = Jarvis(*convex_mas, *num);
  return res;
}
