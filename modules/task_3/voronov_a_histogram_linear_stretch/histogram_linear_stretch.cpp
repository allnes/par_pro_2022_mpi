// Copyright 2022 Voronov Alexander

#include "../../../modules/task_3/voronov_a_histogram_linear_stretch/histogram_linear_stretch.h"
#include <mpi.h>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>

// Reserved
std::vector<uint8_t> ImageRead(std::string fileName = "in") {
  std::vector<uint8_t> pixelArray;
  std::ifstream in(fileName, std::ios::in | std::ios::binary);
  if (in.is_open()) {
    uint32_t pixelCount;
    in.read(reinterpret_cast<char*>(&pixelCount), sizeof(pixelCount));
    pixelArray = std::vector<uint8_t>(pixelCount);
    in.read(reinterpret_cast<char*>(&pixelArray[0]), pixelCount);
    in.close();
  }
  return pixelArray;
}
// ----------------------------------------------------------------------------
void ImageWrite(std::vector<uint8_t> *vectorRef, std::string fileName = "out") {
  std::ofstream out(fileName, std::ios::out | std::ios::binary);
  if (out.is_open()) {
    uint32_t pixelCount = vectorRef->size();
    out.write(reinterpret_cast<char*>(&pixelCount), sizeof(pixelCount));
    out.write(reinterpret_cast<char*>(&vectorRef[0]), pixelCount);
    out.close();
  }
}
// ----------------------------------------------------------------------------

std::vector<uint8_t> GetRandomPixelArray(int size, uint8_t min, uint8_t max) {
  if (size < 0) {
    throw -1;
  }
  if (min > max) {
    throw -1;
  }
  std::mt19937 generator;
  generator.seed(static_cast<unsigned int>(time(0)));
  std::vector<uint8_t> pixelArray;
  for (int i = 0; i < size; i++) {
    pixelArray.push_back(generator() % (max - min + 1) + min);
  }
  return pixelArray;
}

uint8_t SequentialMaxValue(std::vector<uint8_t> *vectorRef) {
  return *std::max_element(vectorRef->begin(), vectorRef->end());
}

uint8_t SequentialMinValue(std::vector<uint8_t> *vectorRef) {
  return *std::min_element(vectorRef->begin(), vectorRef->end());
}

void Segmentation(int size, int worldSize, int rank, int *segmentStart, int *segmentSize) {
  if (size <= 0) {
    throw - 1;
  }
  *segmentStart = size / worldSize * rank;
  *segmentSize = size / worldSize;
  if (rank == worldSize - 1) {
    *segmentSize += size % worldSize;
  }
}

void Stretch(std::vector<uint8_t> *vectorRef, uint8_t minValue, uint8_t maxValue, uint8_t min, uint8_t max) {
  for (size_t i = 0; i < vectorRef->size(); i++) {
    (*vectorRef)[i] = static_cast<double>(((*vectorRef)[i] - minValue)) / (maxValue - minValue) * (max - min);
  }
}

void ParallelStretch(std::vector<uint8_t> *vectorRef, uint8_t min, uint8_t max) {
  std::vector<uint8_t> pixelArrayPart;
  int worldSize;
  int worldRank;
  int* displacement = NULL;
  int* receiveCounts = NULL;
  int segmentStart = 0;
  int segmentSize = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

  if (vectorRef->size() <= static_cast<size_t>(worldSize)) {
    Stretch(vectorRef, SequentialMinValue(vectorRef), SequentialMaxValue(vectorRef), min, max);
  }

  receiveCounts = new int[worldSize];
  displacement = new int[worldSize];

  for (int i = 0; i < worldSize; i++) {
    int start = 0;
    int size = 0;
    Segmentation(vectorRef->size(), worldSize, i, &start, &size);
    receiveCounts[i] = size;
    displacement[i] = i * ((i == worldSize - 1) ? receiveCounts[i - 1] : size);
    if (i == worldRank) {
      segmentStart = start;
      segmentSize = size;
    }
  }

  pixelArrayPart = std::vector<uint8_t>(vectorRef->begin() + segmentStart,
    vectorRef->begin() + segmentStart + segmentSize);

  int localMax = SequentialMaxValue(&pixelArrayPart);
  int localMin = SequentialMinValue(&pixelArrayPart);

  int parallelMax = 0;
  int parallelMin = 0;
  MPI_Allreduce(&localMax, &parallelMax, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&localMin, &parallelMin, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  Stretch(&pixelArrayPart, parallelMin, parallelMax, min, max);

  MPI_Gatherv(&pixelArrayPart[0], segmentSize, MPI_CHAR,
    &vectorRef[0], receiveCounts, displacement, MPI_CHAR, 0, MPI_COMM_WORLD);

  delete[] displacement;
  delete[] receiveCounts;
  displacement = nullptr;
  receiveCounts = nullptr;
}
