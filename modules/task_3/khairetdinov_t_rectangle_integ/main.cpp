// Copyright 2022 Khairetdinov Timur
#include <gtest/gtest.h>
#include <mpi.h>
#include <math.h>
#include <utility>
#include <vector>
#include "../../../modules/task_3/khairetdinov_t_rectangle_integ/rectang_integ.h"
#include <gtest-mpi-listener.hpp>

using std::vector;
using std::pair;

double f1(vector<double> a) {
  double x = a[0];
  double y = a[1];;
  return (x + y * y * y);
}

double f2(vector<double> a) {
  double x = a[0];
  double y = a[1];
  double z = a[2];
  return (x + y * y * 7 + 12 - z * z * z);
}

double f3(vector<double> a) {
  double x = a[0];
  double y = a[1];
  double z = a[2];
  return (log10(2 * x * x) + z + 5 * y);
}

double f4(vector<double> a) {
  double x = a[0];
  double y = a[1];
  double z = a[2];
  return (x - sqrt(36) * 5 * sin(y) + sin(z * 12));
}

double f5(vector<double> a) {
  double x = a[0];
  double y = a[1];
  return x + y;
}

TEST(RectangleIntegration, Double_Integral_0) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 100;
  vector <pair<double, double>> a(2);
  a = { {7, 11}, {5, 6} };
  double result = RectParall(f5, a, n);
  if (rank == 0) {
    double error = 0.01;
    EXPECT_NEAR(result, RectSequen(f5, a, n), error);
  }
}

TEST(RectangleIntegration, Double_Integral_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 100;
  vector <pair<double, double>> a(2);
  a = { {7, 19}, {12, 21} };
  double result = RectParall(f1, a, n);
  if (rank == 0) {
    double error = 0.01;
    EXPECT_NEAR(result, RectSequen(f1, a, n), error);
  }
}

TEST(RectangleIntegration, Triple_Integral_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 100;
  vector <pair<double, double>> a(3);
  a = { {11, 17}, {29, 48}, {17, 21} };
  double result = RectParall(f2, a, n);
  if (rank == 0) {
    double error = 0.01;
    EXPECT_NEAR(result, RectSequen(f2, a, n), error);
  }
}

TEST(RectangleIntegration, Triple_Integral_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 100;
  vector <pair<double, double>> a(3);
  a = { {54, 68}, {9, 18}, {7, 121} };
  double result = RectParall(f3, a, n);
  double error = 0.01;
  if (rank == 0) {
    EXPECT_NEAR(result, RectSequen(f3, a, n), error);
  }
}

TEST(RectangleIntegration, Triple_Integral_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 100;
  vector<pair<double, double>> a(3);
  a = { {41, 117}, {-10, 44}, {29, 72} };
  double result = RectParall(f4, a, n);
  if (rank == 0) {
    double error = 0.01;
    EXPECT_NEAR(result, RectSequen(f4, a, n), error);
  }
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

  return RUN_ALL_TESTS();
}
