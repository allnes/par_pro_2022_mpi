  // Copyright 2022 Ermolaev Danila
#include <mpi.h>
#include <math.h>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/ermolaev_d_Linear_filter_3x3/Linear_filter_3x3.h"

unsigned char* getRandomMatrix(unsigned char* matrix, int x) {
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < x; i++) {
        matrix[i] = 0;
    }

    for (int i = 0; i < x; i++) {
        matrix[i] = gen() % 10 + 5;
    }

    return matrix;
}

void convolution(unsigned char* a, int aw, int ah, unsigned char* b, double* w,
    int ww, int wh) {
    unsigned char* pb = b;
    for (int i = 0; i < ah - wh; i++) {
        for (int j = 0; j < aw - ww; j++, pb++) {
            double p = 0.0;
            for (int ii = 0; ii < wh; ii++) {
                for (int jj = 0; jj < ww; jj++) {
                    p += w[ii * ww + jj] * a[(i + ii) * aw + j + jj];
                }
            }
            *pb = round(p);
        }
    }
}

void mpi_gauss_filter(Image* im, Image* om, int w) {
    unsigned char* b;

    int size, rank;
    MPI_Status status;
    const int tag = 0;
    double mypi = 3.14159265358979323846;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double* g = reinterpret_cast<double*>(malloc(sizeof(double) * w * w));
    double sigma2 = (static_cast<double>(w)) / 6.0;
    sigma2 *= sigma2;
    double x, y, r;
    double c = (static_cast<double>(w)) / 2;
    double* pg = g;
    for (int i = 0; i < w; i++) {
        x = i - c;
        for (int j = 0; j < w; j++, pg++) {
            y = j - c;
            r = x * x + y * y;
            *pg = 1 / (2 * mypi * sigma2) * exp(-r / (2 * sigma2));
        }
    }

    int psize = size == 1 ? 0 : (im->h - w) / (size - 1);

    unsigned char* data;
    unsigned char* pb;
    if (rank == 0) {
        b = (unsigned char*)malloc(sizeof(unsigned char) * (im->w - w) *
            (im->h - w));
        pb = b + (im->w - w) * psize * (size - 1);
        data = im->data + im->w * psize * (size - 1);

        unsigned char* pa = im->data;
        for (int i = 1; i < size; i++, pa += im->w * psize) {
            MPI_Send(pa, im->w * (psize + w), MPI_UNSIGNED_CHAR, i, tag,
                MPI_COMM_WORLD);
        }
        psize = (im->h - w) - psize * (size - 1);
    } else {
        b = (unsigned char*)malloc(sizeof(unsigned char) * (im->w - w) * psize);
        data = (unsigned char*)malloc(sizeof(unsigned char) * im->w * (psize + w));
        MPI_Recv(data, im->w * (psize + w), MPI_UNSIGNED_CHAR, 0, tag,
            MPI_COMM_WORLD, &status);
        pb = b;
    }

    convolution(data, im->w, psize + w, pb, g, w, w);
    free(g);

    if (rank == 0) {
        psize = size == 1 ? 0 : (im->h - w) / (size - 1);
        pb = b;
        for (int i = 1; i < size; i++, pb += (im->w - w) * psize) {
            MPI_Recv(pb, (im->w - w) * psize, MPI_UNSIGNED_CHAR, i, tag,
                MPI_COMM_WORLD, &status);
        }
        om->data = b;
        om->w = im->w - w;
        om->h = im->h - w;
    } else {
        MPI_Send(b, (im->w - w) * psize, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
        free(data);
        free(b);
    }
}

void gauss_filter(Image* im, Image* om, int w) {
    double mypi = 3.14159265358979323846;
    double* g = reinterpret_cast<double*>((malloc(sizeof(double) * w * w)));
    double sigma2 = (static_cast<double>(w)) / 6.0;
    sigma2 *= sigma2;
    double x, y, r;
    double c = (static_cast<double>(w)) / 2;
    double* pg = g;
    for (int i = 0; i < w; i++) {
        x = i - c;
        for (int j = 0; j < w; j++, pg++) {
            y = j - c;
            r = x * x + y * y;
            *pg = 1 / (2 * mypi * sigma2) * exp(-r / (2 * sigma2));
        }
    }

    unsigned char* b =
        (unsigned char*)malloc(sizeof(unsigned char) * (im->w - w) * (im->h - w));

    convolution(im->data, im->w, im->h, b, g, w, w);
    free(g);
    om->data = b;
    om->w = im->w - w;
    om->h = im->h - w;
}

