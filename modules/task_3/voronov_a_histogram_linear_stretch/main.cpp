// Copyright 2022 Voronov Alexander

#include <gtest/gtest.h>
#include <algorithm>
#include "./histogram_linear_stretch.h"
#include <gtest-mpi-listener.hpp>

TEST(Histogram_linear_stretch_MPI, Test_SequentialMinValue_returns_correct_min_value) {
  std::vector<uint8_t> vector = { 1, 2, 3, 4, 5 };
  ASSERT_EQ(*std::min_element(vector.begin(), vector.end()), SequentialMinValue(&vector));
}

TEST(Histogram_linear_stretch_MPI, Test_SequentialMaxValue_returns_correct_max_value) {
  std::vector<uint8_t> vector = { 1, 2, 3, 4, 5 };
  ASSERT_EQ(*std::max_element(vector.begin(), vector.end()), SequentialMaxValue(&vector));
}

TEST(Histogram_linear_stretch_MPI, Test_GetRandomPixelArray_wrong_size_value_throws_exception) {
  ASSERT_ANY_THROW(GetRandomPixelArray(-1, 0, 255));
}

TEST(Histogram_linear_stretch_MPI, Test_GetRandomPixelArray_min_is_greater_than_max_throws_exception) {
  ASSERT_ANY_THROW(GetRandomPixelArray(10, 200, 100));
}

TEST(Histogram_linear_stretch_MPI, Test_GetRandomPixelArray_returns_vector_with_correct_value_range) {
  std::vector<uint8_t> pixelArray = GetRandomPixelArray(10, 100, 200);
  uint8_t min = SequentialMinValue(&pixelArray);
  uint8_t max = SequentialMaxValue(&pixelArray);
  ASSERT_TRUE(min >= 100 && max <= 200);
}

TEST(Histogram_linear_stretch_MPI, Test_Segmentation_throw_exception_on_negative_size_value) {
  int start, end;
  ASSERT_ANY_THROW(Segmentation(-1, 0, 0, &start, &end));
}

TEST(Histogram_linear_stretch_MPI, Test_Stretch_returns_vector_with_correct_new_range) {
  std::vector<uint8_t> pixelArray = GetRandomPixelArray(10, 100, 200);
  Stretch(&pixelArray, 100, 200, 0, 255);
  uint8_t min = SequentialMinValue(&pixelArray);
  uint8_t max = SequentialMaxValue(&pixelArray);
  ASSERT_TRUE(min >= 0 && max <= 255);
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
