  // Copyright 2022 Kudryashov Nikita
#include <mpi.h>
#include <random>
#include <ctime>
#include "../../../modules/task_3/kudryashov_n_sobel_operator/kudryashov_n_sobel_operator.h"

std::vector<std::vector<int>> generateRandomImage(int height, int width) {
    std::mt19937 rnd;
    rnd.seed(std::time(nullptr));
    std::vector<std::vector<int>> img(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            img[i][j] = rnd() % 256;
        }
    }
    return img;
}

int clamp(int value, int min, int max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

std::vector<std::vector<int>> vectorToMatrix(std::vector<int> vec, int height, int width) {
    std::vector<std::vector<int>> res(height, std::vector<int>(width));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            res[i][j] = vec[i * width + j];
    return res;
}

std::vector<int> matrixToVector(std::vector<std::vector<int>> mat, int height, int width) {
    std::vector<int> res(height * width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            res[i * width + j] = mat[i][j];
        }
    }
    return res;
}

int calcNewPixelColor(std::vector<std::vector<int>> image, int height, int width, int y, int x) {
    int kernelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int kernelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    int intensityX = 0;
    int intensityY = 0;
    int G;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int xn = clamp(x + j, 0, width - 1);
            int yn = clamp(y + i, 0, height - 1);
            intensityX += image[yn][xn] * kernelX[i + 1][j + 1];
            intensityY += image[yn][xn] * kernelY[i + 1][j + 1];
        }
    }
    G = sqrt(pow(intensityX, 2) + pow(intensityY, 2));
    G = clamp(G, 0, 255);
    return G;
}

std::vector<std::vector<int>> calcSobel(const std::vector<std::vector<int>>& image, int height, int width) {
    std::vector<std::vector<int>> resultImage(height, std::vector<int>(width));
    int kernelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int kernelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
            resultImage[y][x] = calcNewPixelColor(image, height, width, y, x);
        }
    }
    return resultImage;
}

std::vector<std::vector<int>> calcSobelParallel(const std::vector<std::vector<int>>& image, int height, int width) {
    int proc_num, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int shift = height / proc_num;
    int remain = height % proc_num;

    std::vector<int> sendbuf(height);
    std::vector<int> recvcounts(proc_num);
    std::vector<int> displs(proc_num);
    std::vector<int> vecImage(height * width);

    if (rank == 0) {
        vecImage = matrixToVector(image, height, width);
        for (int proc = 1; proc < proc_num; proc++) {
            MPI_Send(vecImage.data() + (proc - 1) * shift * width, shift * width, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Bcast(vecImage.data(), width* height, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> localRes(shift * width);

    if (rank == 0) {
        localRes.resize(vecImage.end() - (vecImage.begin() + width * shift * (proc_num - 1)));
        localRes = std::vector<int>(vecImage.begin() + width * shift * (proc_num - 1), vecImage.end());
    } else {
        MPI_Status status;
        MPI_Recv(localRes.data(), shift * width, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int row;
    if (rank == 0) {
        row = (proc_num - 1) * shift;
        recvcounts[0] = shift * width + remain * width;
        displs[0] = width * shift * (proc_num - 1);
        for (int i = 1; i < proc_num; i++) {
            recvcounts[i] = shift * width;
            if (i == 1) {
                displs[i] = 0;
            } else {
                displs[i] = displs[i - 1] + recvcounts[i];
            }
        }
    } else {
        row = (rank - 1) * shift;
    }
    for (int i = 0; i < (localRes.size() / width); i++) {
        for (int j = 0; j < width; j++) {
            localRes[i * width + j] = calcNewPixelColor(vectorToMatrix(vecImage, height, width)
                , height, width, row + i, j);
        }
    }
    std::vector<int> global_res(width * height);

    MPI_Gatherv(localRes.data(), localRes.size(), MPI_INT, global_res.data(),
        recvcounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    return vectorToMatrix(global_res, height, width);
}
