// Copyright 2020 Silenko Dmitrii

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "../../../modules/task_3/silenko_d_convex_binary_tbb/convex_binary_tbb.h"

TEST(convex_binary_tbb, test_one_point) {
  std::vector<int*> result;
  int** convex_mas = NULL;
  int num = 0, n = 5, m = 20;

  int** mas = new int*[n];
  for (int i = 0; i < n; i++)
    mas[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      mas[i][j] = 0;
  mas[2][7] = 1;

  int num_thr = 2;
  result = getComponent_TBB(mas, n, m, &convex_mas, &num, num_thr);

  int s = result.size();
  int** tmp = new int*[s];
  for (int i = 0; i < s; i++)
    tmp[i] = new int[2];
  tmp[0][0] = 7;
  tmp[0][1] = 2;

  for (int i = 0; i < s; i++) {
    int* temp = result[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++)
    delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++)
    delete[] mas[i];
  delete[] mas;
  for (int i = 0; i < num; i++)
    delete[] convex_mas[i];
  delete[] convex_mas;
}

TEST(convex_binary_tbb, test_one_line) {
  std::vector<int*> result;
  int** convex_mas = NULL;
  int num = 0, n = 1, m = 20;

  int** mas = new int*[n];
  for (int i = 0; i < n; i++)
    mas[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      mas[i][j] = 0;
  mas[0][2] = 1;
  mas[0][11] = 1;
  mas[0][7] = 1;
  mas[0][19] = 1;
  mas[0][5] = 1;
  mas[0][17] = 1;

  int num_thr = 2;
  result = getComponent_TBB(mas, n, m, &convex_mas, &num, num_thr);

  int** tmp = new int*[2];
  for (int i = 0; i < 2; i++)
    tmp[i] = new int[2];
  tmp[0][0] = 2;
  tmp[0][1] = 0;
  tmp[1][0] = 19;
  tmp[1][1] = 0;

  int s = result.size();
  for (int i = 0; i < s; i++) {
    int* temp = result[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++)
    delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++)
    delete[] mas[i];
  delete[] mas;
  for (int i = 0; i < num; i++)
    delete[] convex_mas[i];
  delete[] convex_mas;
}

TEST(convex_binary_tbb, test_sq) {
  std::vector<int*> result;
  int** convex_mas = NULL;
  int num = 0, n = 10, m = 10;

  int** mas = new int*[n];
  for (int i = 0; i < n; i++)
    mas[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      mas[i][j] = 0;
  mas[0][2] = 1;
  mas[0][5] = 1;
  mas[0][8] = 1;
  mas[2][5] = 1;
  mas[2][8] = 1;
  mas[5][8] = 1;
  mas[5][5] = 1;
  mas[5][4] = 1;
  mas[3][2] = 1;
  mas[1][2] = 1;

  int num_thr = 2;
  result = getComponent_TBB(mas, n, m, &convex_mas, &num, num_thr);

  int s = result.size();
  int** tmp = new int*[s];
  for (int i = 0; i < s; i++)
    tmp[i] = new int[2];
  tmp[0][0] = 2;
  tmp[0][1] = 0;
  tmp[1][0] = 8;
  tmp[1][1] = 0;
  tmp[2][0] = 8;
  tmp[2][1] = 5;
  tmp[3][0] = 4;
  tmp[3][1] = 5;
  tmp[4][0] = 2;
  tmp[4][1] = 3;

  for (int i = 0; i < s; i++) {
    int* temp = result[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++)
    delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++)
    delete[] mas[i];
  delete[] mas;
  for (int i = 0; i < num; i++)
    delete[] convex_mas[i];
  delete[] convex_mas;
}

TEST(convex_binary_tbb, test_inside) {
  int** convex_mas = NULL;
  int num = 0, n = 6, m = 6;

  int** mas = new int*[n];
  for (int i = 0; i < n; i++)
    mas[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      mas[i][j] = 0;
  mas[0][1] = 1;
  mas[0][2] = 1;
  mas[1][1] = 1;
  mas[1][5] = 1;
  mas[2][4] = 1;
  mas[2][5] = 1;
  mas[3][5] = 1;
  mas[4][2] = 1;

  std::vector<int*> omp_res;
  int num_thr = 2;

  omp_res = getComponent_TBB(mas, n, m, &convex_mas, &num, num_thr);

  std::vector<int*> inl;
  inl = Inside(omp_res, convex_mas, num);

  int s = inl.size();
  int** tmp = new int*[s];
  for (int i = 0; i < s; i++)
    tmp[i] = new int[2];
  tmp[0][0] = 1;
  tmp[0][1] = 0;
  tmp[1][0] = 5;
  tmp[1][1] = 1;
  tmp[2][0] = 2;
  tmp[2][1] = 4;

  for (int i = 0; i < s; i++) {
    int* temp = inl[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++)
    delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++)
    delete[] mas[i];
  delete[] mas;
  for (int i = 0; i < num; i++)
    delete[] convex_mas[i];
  delete[] convex_mas;
}

TEST(convex_binary_tbb, test_rand) {
  std::vector<int*> result;
  int** convex_mas = NULL;
  int num = 0, n = 20, m = 20;

  int** mas = getRandomMas(n, m);
  int** newmas = new int*[n];
  for (int i = 0; i < n; i++)
    newmas[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      newmas[i][j] = mas[i][j];

  std::vector<int*> tbb_res;
  int** tbb_convex_mas = NULL;
  int num_thr = 2;

  result = getComponent(mas, n, m, &convex_mas, &num);

  tbb_res = getComponent_TBB(newmas, n, m, &tbb_convex_mas, &num, num_thr);


  int size = tbb_res.size();

  for (int i = 0; i < size; i++) {
    EXPECT_EQ(result[i][0], tbb_res[i][0]);
    EXPECT_EQ(result[i][1], tbb_res[i][1]);
  }

  std::vector<int*> inside;
  std::vector<int*> inside_omp;

  inside = Inside(result, convex_mas, num);
  inside_omp = Inside(tbb_res, tbb_convex_mas, num);

  size = inside_omp.size();

  for (int i = 0; i < size; i++) {
    EXPECT_EQ(inside[i][0], inside_omp[i][0]);
    EXPECT_EQ(inside[i][1], inside_omp[i][1]);
  }

  for (int i = 0; i < n; i++) {
    delete[] mas[i];
    delete[] newmas[i];
  }
  delete[] mas;
  delete[] newmas;
  for (int i = 0; i < num; i++) {
    delete[] convex_mas[i];
    delete[] tbb_convex_mas[i];
  }
  delete[] convex_mas;
  delete[] tbb_convex_mas;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
