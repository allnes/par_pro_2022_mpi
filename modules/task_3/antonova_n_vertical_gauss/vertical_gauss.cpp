// Copyright 2022 Antonova Anastasya

#include "../../../modules/task_3/antonova_n_vertical_gauss/vertical_gauss.h"
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <random>

int* getRandomImage(int width, int height) {
  std::mt19937 gen(time(0));
  std::uniform_int_distribution<> uid(0, 255);
  int* img = new int[width * height];
  for (int i = 0; i < width * height; i++) {
    img[i] = uid(gen);
  }
  return img;
}

int clamp(int value, int max, int min) {
  if (value > max) return max;
  if (value < min) return min;
  return value;
}

float* getKernel() {
  int size = 3;
  int len = 9;

  float* kernel = new float[len];
  float sigma = 3.f;
  float norm = 0;
  int signed_radius = static_cast<int>(1);

  for (int x = -signed_radius; x <= signed_radius; x++)
    for (int y = -signed_radius; y <= signed_radius; y++) {
      std::size_t idx = (x + 1) * size + (y + 1);
      kernel[idx] = std::exp(-(x * x + y * y) / (sigma * sigma));
      norm += kernel[idx];
    }
  for (int i = 0; i < len; i++) {
    kernel[i] /= norm;
  }

  return kernel;
}

float calcNewPixelColor(const int* image, int x, int y, int width, int height,
                        const float* kernel) {
  float returnC = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int idx = (i + 1) * 3 + j + 1;
      int pixel = image[x + j + (y + i) * width];
      returnC += pixel * kernel[idx];
    }
  }
  return clamp(returnC, 255, 0);
}


int* getSequentialGauss(const int* image, int width, int height) {
  int* new_image = new int[(width - 2) * (height - 2)];

  const float* kernel = getKernel();

  for (int i = 1; i < height - 1; i++) {
    for (int j = 1; j < width - 1; j++) {
      new_image[j - 1 + (i - 1) * (width - 2)] =
          calcNewPixelColor(image, j, i, width, height, kernel);
    }
  }
  return new_image;
}

int* getParallelGauss(const int* image, int width, int height) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = (width - 2) / size;
  int rem = (width - 2) % size;

  if (delta) {
    int *counts = new int[size], *displs = new int[size];
    for (int i = 0; i < size; i++) {
      if (i == 0) {
        counts[i] = delta + rem + 2;
        displs[i] = 0;
      } else {
        counts[i] = delta + 2;
        displs[i] = delta * i + rem;
      }
    }

    int* local_matrix;

    int temp = delta;
    if (rank == 0) temp += rem;

    local_matrix = new int[(temp + 2) * height];

    for (int i = 0; i < height; i++) {
      MPI_Scatterv(image + i * width, counts, displs, MPI_INT,
                   local_matrix + i * (temp + 2), temp + 2, MPI_INT, 0,
                   MPI_COMM_WORLD);
    }

    local_matrix = getSequentialGauss(local_matrix, temp + 2, height);

    int* global_matrix = nullptr;

    for (int i = 0; i < size; i++) {
      counts[i] -= 2;
    }

    if (rank == 0) global_matrix = new int[(width - 2) * (height - 2)];

    for (int i = 0; i < height - 2; i++) {
      MPI_Gatherv(local_matrix + i * temp, temp, MPI_INT,
                  global_matrix + i * (width - 2), counts, displs, MPI_INT, 0,
                  MPI_COMM_WORLD);
    }

    return global_matrix;
  } else {
    if (rank == 0) {
      return getSequentialGauss(image, width, height);
    } else {
      return nullptr;
    }
  }
}
