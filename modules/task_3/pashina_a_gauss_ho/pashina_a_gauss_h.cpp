// Copyright 2022 Pashina Alina

#include "../../../modules/task_3/pashina_a_gauss_ho/pashina_a_gauss_h.h"
#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

std::vector<unsigned char> generateNewImage(int rows_num, int columns_num) {
  std::mt19937 generator;
  std::vector<unsigned char> generated_resimage(columns_num * rows_num);
  generator.seed(static_cast<unsigned int>(time(0)));
  int k = 0;
  while (k < rows_num * columns_num) {
    generated_resimage[k] = static_cast<unsigned char>(generator() % 256);
    k += 1;
  }
  return generated_resimage;
}

unsigned char getNewColor(const std::vector<unsigned char> &myimg, int startX,
                          int startY, int rows_n, int columns_n) {
  int resCol = 0;
  constexpr unsigned char gaussMatrix[3][3]{{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
  int coord_x = 0;
  int coord_y = 0;
  constexpr unsigned char gaussDenom{16};
  for (int k = -1; k < 2; ++k) {
    coord_x = startX + k;
    for (int m = -1; m < 2; ++m) {
      coord_y = startY + m;
      if (coord_y > columns_n - 1 || coord_y < 0) {
        coord_y = startY;
      }
      if (coord_x > rows_n - 1 || coord_x < 0) {
        coord_x = startX;
      }
      if (coord_x * columns_n + coord_y >= columns_n * rows_n) {
        coord_y = startY;
        coord_x = startX;
      }
      resCol += static_cast<int>(myimg[coord_x * columns_n + coord_y] *
                                 (gaussMatrix[k + 1][m + 1]));
    }
  }
  return resCol / gaussDenom;
}

std::vector <unsigned char> gauss_filt(
const std::vector<unsigned char>& mystart_image,
                                      int rows_num, int columns_num) {
  std::vector<unsigned char> result(columns_num * rows_num);
  int yin = 0;
  while (yin < rows_num) {
    int xin = 0;
    while (xin < columns_num) {
      result[xin + yin * columns_num] =
          getNewColor(mystart_image, yin, xin, rows_num, columns_num);
      xin += 1;
    }
    yin += 1;
  }
  return result;
}

std::vector<unsigned char> DoParallel_fGauss(
    std::vector<unsigned char> mystart_im, int rows_num, int columns_num) {
  std::vector<unsigned char> finalResultIm(columns_num * rows_num);
  int th_size;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &th_size);
  const int th_part = rows_num / th_size;
  std::vector<unsigned char> startImg(th_part * columns_num + 2 * columns_num);
  std::vector<unsigned char> partRes(th_part * columns_num);
  const int th_finish_part = rows_num % th_size;
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

  if (rows_num < th_size) {
    return gauss_filt(mystart_im, rows_num, columns_num);
  }

  if (th_rank != 0) {
    if (th_size - 1 == th_rank) {
      startImg.resize((th_part + 1) * columns_num);
      MPI_Recv(&startImg[0], (th_part + 1) * columns_num, MPI_UNSIGNED_CHAR, 0,
               0, MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(&startImg[0], (th_part + 2) * columns_num + 2, MPI_UNSIGNED_CHAR,
               0, 0, MPI_COMM_WORLD, &status);
    }
  } else {
    if (th_size != 1) {
      startImg.resize((th_part + th_finish_part + 1) * columns_num);
      for (int i = 0; i < (th_part + th_finish_part + 1) * columns_num; ++i) {
        startImg[i] = mystart_im[i];
      }
    }
    for (int num_th = 1; num_th < th_size; num_th++) {
      int startAdress =
          (num_th * th_part * columns_num) + (th_finish_part - 1) * columns_num;
      int countSend;

      if (num_th == (th_size - 1)) {
        countSend = th_part * columns_num + columns_num;
      } else {
        countSend = th_part * columns_num + 2 * columns_num;
      }
      MPI_Send(&mystart_im[0] + startAdress, countSend, MPI_UNSIGNED_CHAR,
               num_th, 0, MPI_COMM_WORLD);
    }
  }

  if (th_rank == th_size - 1 && th_size != 1) {
    int i = 1;
    while (i < th_part) {
      for (int j = 0; j < columns_num; ++j)
        partRes[(i - 1) * columns_num + j] =
            getNewColor(startImg, i, j, th_part + 1, columns_num);
      i += 1;
    }
  } else if (th_rank != 0) {
    int r = 1;
    while (r < th_part + 1) {
      for (int j = 0; j < columns_num; ++j)
        partRes[(r - 1) * columns_num + j] =
            getNewColor(startImg, r, j, th_part + 2, columns_num);
      r += 1;
    }
  } else {
    if (th_size != 1) {
      int z = 1;
      while (z < th_part + th_finish_part) {
        for (int j = 0; j < columns_num; ++j)
          finalResultIm[z * columns_num + j] = getNewColor(
              startImg, z, j, th_part + th_finish_part + 1, columns_num);
        z += 1;
      }
    } else {
      int f = 1;
      while (f < rows_num) {
        for (int j = 0; j < columns_num; ++j)
          finalResultIm[f * columns_num + j] =
              getNewColor(mystart_im, f, j, rows_num, columns_num);
        f += 1;
      }
    }
  }

  if (th_rank == 0) {
    for (int th_num = 1; th_num < th_size; ++th_num) {
      int start = ((th_part + th_finish_part) * columns_num) +
                  ((th_num - 1) * th_part * columns_num);
      MPI_Recv(&finalResultIm[0] + start, th_part * columns_num,
               MPI_UNSIGNED_CHAR, th_num, 0, MPI_COMM_WORLD, &status);
    }
  } else {
    MPI_Send(&partRes[0], th_part * columns_num, MPI_UNSIGNED_CHAR, 0, 0,
             MPI_COMM_WORLD);
  }
  return finalResultIm;
}

