// Copyright 2022 Popov Andrey
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>


#include "./from_one_to_all_broadcats.h"
TEST(From_one_to_all, Rand_Arr_n_Int) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int Arrcnt = 50;
  int max;
  int maxown;
  int res;

  int* arrayInt = reinterpret_cast<int*>(malloc(sizeof(int) * Arrcnt));

  if (rank == 0) {
    arrayInt = getRandArrInt(Arrcnt);

    maxown = getMaxArrInt(arrayInt, Arrcnt);
    max = maxown - 1;  // fake max of Array

    Broadcast(arrayInt, Arrcnt, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(arrayInt, Arrcnt, MPI_INT, 0, MPI_COMM_WORLD);
    max = getMaxArrInt(arrayInt, Arrcnt);
  }

  MPI_Reduce(&max, &res, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  free(arrayInt);
  if (rank == 0) {
    ASSERT_GE(maxown, res);
  }
}

TEST(From_one_to_all, Test_With_Double_n_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int Arrcnt = 50;
  double max;
  double maxown;
  double res;

  double* arrayDouble =
      reinterpret_cast<double*>(malloc(sizeof(double) * Arrcnt));

  if (rank == 0) {
    arrayDouble = getRandArrDouble(Arrcnt);

    maxown = getMaxArrDouble(arrayDouble, Arrcnt);
    max = maxown - 1;  // fake max of Array

    Broadcast(arrayDouble, Arrcnt, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(arrayDouble, Arrcnt, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    max = getMaxArrDouble(arrayDouble, Arrcnt);
  }

  MPI_Reduce(&max, &res, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  free(arrayDouble);
  if (rank == 0) {
    ASSERT_GE(maxown, res);
  }
}

TEST(From_one_to_all, Rand_Arr_n_Float) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int Arrcnt = 50;
  float max;
  float maxzero;
  float res;

  float* arrayFloat = reinterpret_cast<float*>(malloc(sizeof(float) * Arrcnt));

  if (rank == 0) {
    arrayFloat = getRandArrFloat(Arrcnt);

    maxzero = getMaxArrFloat(arrayFloat, Arrcnt);
    max = maxzero - 1;  // fake max of Array

    Broadcast(arrayFloat, Arrcnt, MPI_FLOAT, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(arrayFloat, Arrcnt, MPI_FLOAT, 0, MPI_COMM_WORLD);
    max = getMaxArrFloat(arrayFloat, Arrcnt);
  }

  MPI_Reduce(&max, &res, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

  free(arrayFloat);
  if (rank == 0) {
    ASSERT_GE(maxzero, res);
  }
}

TEST(From_one_to_all, Size_Arr_one_Int) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int buff;
  if (rank == 0) {
    buff = 67;
    Broadcast(&buff, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(&buff, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ASSERT_EQ(buff, 67);
  }
}

TEST(From_one_to_all, Size_Arr_one_Double) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double buff;
  if (rank == 0) {
    buff = 150.5;
    Broadcast(&buff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(&buff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    ASSERT_EQ(buff, 150.5);
  }
}

TEST(From_one_to_all, Size_Arr_one_Float) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  float buff;
  if (rank == 0) {
    buff = 82.4;
    Broadcast(&buff, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(&buff, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    ASSERT_EQ(buff, static_cast<float>(82.4));
  }
}

TEST(From_one_to_all, Consistence_Arr) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  int Arrcnt = 70;
  int arrayInt[70];

  if (rank == 0) {
    for (int i = 0; i < Arrcnt; i++) {
      arrayInt[i] = i + 5;
    }

    Broadcast(arrayInt, Arrcnt, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    Broadcast(arrayInt, Arrcnt, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < Arrcnt; i++) {
      ASSERT_EQ(arrayInt[i], i + 5);
    }
  }
}

TEST(From_one_to_all, Arr_any_throw) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  int Arrcnt = 0;
  double arrayDouble[3];
  arrayDouble[0] = 3.5;
  arrayDouble[1] = 12.7;
  arrayDouble[2] = 21.3;

  if (rank == 0) {
    ASSERT_ANY_THROW(
        Broadcast(arrayDouble, Arrcnt, MPI_DOUBLE, 0, MPI_COMM_WORLD));
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
