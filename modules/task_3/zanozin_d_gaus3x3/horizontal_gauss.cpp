// Copyright 2022 Zanozin Danila
#include <mpi.h>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <ctime>
#include <list>

std::vector<unsigned char> getRandomImage(int _cols, int _rows) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<unsigned char> image(_cols * _rows);
    for (int i = 0; i < _cols; i++)
        for (int j = 0; j < _rows; j++)
            image[i * _rows + j] = static_cast<unsigned char>(gen() % 256);

    return image;
}

unsigned char changePixel(const std::vector<unsigned char>& source, int _x, int _y, int rows, int cols) {
    int sum = 0;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            int x = _x + i;
            int y = _y + j;

            if (x < 0 || x > rows - 1) {
                x = _x;
            }
            if (y < 0 || y > cols - 1) {
                y = _y;
            }
            if (x * cols + y >= cols * rows) {
                x = _x;
                y = _y;
            }
            sum += static_cast<int>(source[x * cols + y] * (gaussFilter[i + 1][j + 1]));
        }
    }
    return sum / sumMask;
}

std::vector<unsigned char> filterImageSequential(std::vector<unsigned char> source, int rows, int cols) {
    std::vector<unsigned char> result(cols * rows);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            result[i * cols + j] = changePixel(source, i, j, rows, cols);
        }
    return result;
}

std::vector<unsigned char> filterImageParallel(std::vector<unsigned char> source, int rows, int cols) {
    int comm_size, rank;
    MPI_Status status;

    std::vector<unsigned char> globalResult(cols * rows);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rows < comm_size) {
        return filterImageSequential(source, rows, cols);
    }

    const int interval = rows / comm_size;  // how many lines will send to processes != 0
    const int last_interval = rows % comm_size;  // residue to 0'th process

    if (rank == 0) {
#ifdef DEBUG
        std::cout << "Interval: " << interval << std::endl;
        std::cout << "Last Interval: " << last_interval << std::endl;
#endif  // DEBUG
    }

    std::vector<unsigned char> localImage(interval * cols + 2 * cols);
    std::vector<unsigned char> localResult(interval * cols);

    if (rank == 0) {
#ifdef DEBUG
        std::cout << "Source: " << "Size: " << source.size() << std::endl;
        for (int i = 0; i < source.size(); ++i) {
            std::cout << (unsigned int)source[i] << " ";
            if ((i + 1) % cols == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
#endif  // DEBUG
        if (comm_size != 1) {
            localImage.resize((interval + last_interval + 1) * cols);
            for (int i = 0; i < (interval + last_interval + 1) * cols; ++i) {
                localImage[i] = source[i];
            }
        }

        for (int proc = 1; proc < comm_size; proc++) {
            int startAdress = (proc * interval * cols) + (last_interval - 1) * cols;
            int countSend;

            if (proc != (comm_size - 1)) {
                countSend = interval * cols + 2 * cols;
            }
            else {
                countSend = interval * cols + cols;
            }
            MPI_Send(&source[0] + startAdress, countSend, MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD);
        }

    }
    else if (rank != 0) {
        if (rank != comm_size - 1) {
            MPI_Recv(&localImage[0], (interval + 2) * cols + 2, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        }
        else {
            localImage.resize((interval + 1) * cols);
            MPI_Recv(&localImage[0], (interval + 1) * cols, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        }
    }

#ifdef DEBUG
    std::cout << "{" << rank << "}" << "Local Image" << " | Size: " << localImage.size() << std::endl;
    for (int i = 0; i < localImage.size(); ++i) {
        std::cout << (unsigned int)localImage[i] << " ";
        if ((i + 1) % cols == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
#endif  // DEBUG

    if (rank == comm_size - 1 && comm_size != 1) {
        for (int i = 1; i < interval + 1; ++i)
            for (int j = 0; j < cols; ++j)
                localResult[(i - 1) * cols + j] = changePixel(localImage, i, j, interval + 1, cols);

#ifdef DEBUG
        std::cout << "{" << rank << "}" << "Local result" << " | Size: " << localResult.size() << std::endl;
        for (int i = 0; i < localResult.size(); ++i) {
            std::cout << (unsigned int)localResult[i] << " ";
            if ((i + 1) % cols == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
#endif  // DEBUG
    }
    else if (rank != 0) {
        for (int i = 1; i < interval + 1; ++i)
            for (int j = 0; j < cols; ++j)
                localResult[(i - 1) * cols + j] = changePixel(localImage, i, j, interval + 2, cols);

#ifdef DEBUG
        std::cout << "{" << rank << "}" << "Local result" << " | Size: " << localResult.size() << std::endl;
        for (int i = 0; i < localResult.size(); ++i) {
            std::cout << (unsigned int)localResult[i] << " ";
            if ((i + 1) % cols == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
#endif  // DEBUG
    }
    else {
        if (comm_size != 1) {
            for (int i = 0; i < interval + last_interval; ++i)
                for (int j = 0; j < cols; ++j)
                    globalResult[i * cols + j] = changePixel(localImage, i, j, interval + last_interval + 1, cols);
        }
        else {
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    globalResult[i * cols + j] = changePixel(source, i, j, rows, cols);
        }
    }

    if (rank != 0) {
        MPI_Send(&localResult[0], interval * cols, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else {
        for (int proc = 1; proc < comm_size; ++proc) {
            int start = ((interval + last_interval) * cols) + ((proc - 1) * interval * cols);
            MPI_Recv(&globalResult[0] + start, interval * cols, MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD, &status);
        }
    }

    return globalResult;
}