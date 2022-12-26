// Copyright 2022 Yashina Anastasia

#include <tbb/tbb.h>

#include "../../../modules/task_3/yashina_a_convex_bin/convex_bin.h"


int** SideClass::RandomPic(const int a, const int b) {
  if ((a <= 0) || (b <= 0)) throw "\tError_In_Rows_&_Columns\n";
  int** arr = new int*[a];
  std::mt19937 seed;
  std::uniform_real_distribution<> urd(0, 100);
  for (int i = 0; i < a; i++) arr[i] = new int[b];
  for (int i = 0; i < a; i++) {
    for (int j = 0; j < b; j++) {
      arr[i][j] = static_cast<int>(urd(seed)) % 2;
    }
  }
  return arr;
}

double SideClass::length(int* p1, int* p2) {
  double result;
  result = (sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) +
                 (p2[1] - p1[1]) * (p2[1] - p1[1])));
  return result;
}

double SideClass::cosinus(int* p1, int* p2, int* p3) {
  double result;
  double ax = p2[0] - p1[0];
  double ay = p2[1] - p1[1];
  double bx = p3[0] - p1[0];
  double by = p3[1] - p1[1];
  result = ((ax * bx + ay * by) /
            (sqrt(ax * ax + ay * ay) * sqrt(bx * bx + by * by)));
  return result;
}

std::vector<int*> MainClass::jarvis(int** convex_arr, int n) {
  SideClass sc;
  std::vector<int*> res;
  if (n == 1) {
    res.push_back(convex_arr[0]);
  } else if (n == 2) {
    res.push_back(convex_arr[0]);
    res.push_back(convex_arr[1]);
  } else {
    double* buf = new double[2];
    buf[0] = convex_arr[0][0];
    buf[1] = convex_arr[0][1];
    int count = 0;

    for (int i = 1; i < n; i++) {
      if (convex_arr[i][1] < buf[1]) {
        count = i;
      } else {
        if ((convex_arr[i][1] == buf[1]) && (convex_arr[i][0] < buf[0])) {
          count = i;
        }
      }
    }

    res.push_back(convex_arr[count]);

    int* last = new int[2];
    int* beforelast = new int[2];
    last = convex_arr[count];
    beforelast[0] = convex_arr[count][0] - 2;
    beforelast[1] = convex_arr[count][1];

    double mincos, cos;
    int minind = 0;
    double maxlen = 0;
    while (1) {
      mincos = 2;
      for (int i = 0; i < n; i++) {
        cos = round(sc.cosinus(last, beforelast, convex_arr[i]) * 10000000) /
              10000000;
        if (cos < mincos) {
          minind = i;
          mincos = cos;
          maxlen = sc.length(last, convex_arr[i]);
        } else if (cos == mincos) {
          double len = sc.length(last, convex_arr[i]);
          if (len > maxlen) {
            minind = i;
            maxlen = len;
          }
        }
      }

      beforelast = last;
      last = convex_arr[minind];
      if (last == convex_arr[count]) break;
      res.push_back(convex_arr[minind]);
    }
  }
  return res;
}

std::vector<int*> MainClass::getComp(int** arr, int n, int m,
                                          int*** convex_arr, int* num) {
  MainClass mc;
  int x = 0, y = 0, bf = 1;
  int A, B, C;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      x = j - 1;
      if (x + 1 <= 0) {
        x = 1;
        B = 0;
      } else {
        B = arr[i][x];
      }
      y = i - 1;
      if (y + 1 <= 0) {
        y = 1;
        C = 0;
      } else {
        C = arr[y][j];
      }
      A = arr[i][j];

      if (A == 0) {
      } else {
        *num = *num + 1;
        if (B == 0 && C == 0) {
          bf++;
          arr[i][j] = bf;
        }
        if (B != 0 && C == 0) {
          arr[i][j] = B;
        }
        if (B == 0 && C != 0) {
          arr[i][j] = C;
        }
        if (B != 0 && C != 0) {
          if (B == C) {
            arr[i][j] = B;
          } else {
            arr[i][j] = B;
            for (int i_s = 0; i_s < i; i_s++)
              for (int i_t = 0; i_t < m; i_t++)
                if (arr[i_s][i_t] == C) arr[i_s][i_t] = B;
          }
        }
      }
    }

  (*convex_arr) = new int*[*num];
  for (int i = 0; i < *num; i++) (*convex_arr)[i] = new int[3];

  int tmp = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (arr[i][j] != 0) {
        (*convex_arr)[tmp][0] = j;
        (*convex_arr)[tmp][1] = i;
        (*convex_arr)[tmp][2] = arr[i][j];
        tmp++;
      }
    }

  std::vector<int*> res;
  res = mc.jarvis(*convex_arr, *num);
  return res;
}

std::vector<int*> MainClass::jarvis_TBB(int** convex_arr, int n) {
  SideClass sc;
  std::vector<int*> res;
  if (n == 1) {
    res.push_back(convex_arr[0]);
  } else if (n == 2) {
    res.push_back(convex_arr[0]);
    res.push_back(convex_arr[1]);
  } else {
    double* buf = new double[2];
    buf[0] = convex_arr[0][0];
    buf[1] = convex_arr[0][1];
    int count = 0;

    for (int i = 1; i < n; i++) {
      if (convex_arr[i][1] < buf[1]) {
        count = i;
      } else {
        if ((convex_arr[i][1] == buf[1]) && (convex_arr[i][0] < buf[0])) {
          count = i;
        }
      }
    }

    res.push_back(convex_arr[count]);

    int* last = new int[2];
    int* beforelast = new int[2];
    last = convex_arr[count];
    beforelast[0] = convex_arr[count][0] - 2;
    beforelast[1] = convex_arr[count][1];

    double mincosinus, cosinus;
    int minid = 0;
    double maxlen = 0;
    while (1) {
      mincosinus = 2;
      for (int i = 0; i < n; i++) {
        cosinus =
            round(sc.cosinus(last, beforelast, convex_arr[i]) * 10000000) /
            10000000;
        if (cosinus < mincosinus) {
          minid = i;
          mincosinus = cosinus;
          maxlen = sc.length(last, convex_arr[i]);
        } else if (cosinus == mincosinus) {
          double len = sc.length(last, convex_arr[i]);
          if (len > maxlen) {
            minid = i;
            maxlen = len;
          }
        }
      }

      beforelast = last;
      last = convex_arr[minid];
      if (last == convex_arr[count]) break;
      res.push_back(convex_arr[minid]);
    }
  }
  return res;
}

std::vector<int*> MainClass::getComponent_TBB(int** arr, int n, int m,
                                              int*** convex_arr, int* num,
                                              int num_thr) {
  MainClass mc;
  tbb::task_scheduler_init init(num_thr);
  *num = 0;
  int x = 0, y = 0, cur = 1, A, B, C;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      x = j - 1;
      if (x + 1 <= 0) {
        x = 1;
        B = 0;
      } else {
        B = arr[i][x];
      }
      y = i - 1;
      if (y + 1 <= 0) {
        y = 1;
        C = 0;
      } else {
        C = arr[y][j];
      }
      A = arr[i][j];

      if (A == 0) {
      } else {
        *num = *num + 1;
        if (B == 0 && C == 0) {
          cur++;
          arr[i][j] = cur;
        }
        if (B != 0 && C == 0) {
          arr[i][j] = B;
        }
        if (B == 0 && C != 0) {
          arr[i][j] = C;
        }
        if (B != 0 && C != 0) {
          if (B == C) {
            arr[i][j] = B;
          } else {
            arr[i][j] = B;

            tbb::parallel_for(tbb::blocked_range<int>(0, i),
                              [=](const tbb::blocked_range<int>& t) {
                                int begin = t.begin(), end = t.end();
                                for (int i_t = begin; i_t != end; i_t++)
                                  for (int i_s = 0; i_s < m; i_s++)
                                    if (arr[i_t][i_s] == C) {
                                      arr[i_t][i_s] = B;
                                    }
                              });
          }
        }
      }
    }

  init.terminate();

  (*convex_arr) = new int*[*num];
  for (int i = 0; i < *num; i++) (*convex_arr)[i] = new int[3];

  int tmp = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (arr[i][j] != 0) {
        (*convex_arr)[tmp][0] = j;
        (*convex_arr)[tmp][1] = i;
        (*convex_arr)[tmp][2] = arr[i][j];
        tmp++;
      }
    }

  std::vector<int*> res;
  res = mc.jarvis(*convex_arr, *num);
  return res;
}

std::vector<int*> MainClass::inside(std::vector<int*> root, int** convex_arr,
                                    int n) {
  std::vector<int*> v_first;
  std::vector<int> v_sec;
  std::vector<int> v_t;

  int s = root.size();
  for (int i = 0; i < s; i++) {
    int* tmp = root[i];
    v_t.push_back(tmp[2]);
  }

  for (int i = 0; i < n; i++) {
    auto result = std::find(v_t.begin(), v_t.end(), convex_arr[i][2]);
    auto result_sec = std::find(v_sec.begin(), v_sec.end(), convex_arr[i][2]);
    if (result_sec != v_sec.end()) {
      continue;
    } else {
      if (result != v_t.end()) {
        v_first.push_back(convex_arr[i]);
        int* tmp = convex_arr[i];
        v_sec.push_back(tmp[2]);
      }
    }
  }
  return v_first;
}
