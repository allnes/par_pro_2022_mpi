  // Copyright 2022 Ermolaev Danila
#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <random>
#include <iostream>
#include "./Linear_filter_3x3.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_MPI, Random_Matrix_test_10x10) {
    char* filename, * ofilename;

    int size, rank;
    MPI_Status status;
    const int tag = 0;
    bool success = true;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pix_dist(0, 255);

    const int Tsize = 1;
    int Ws[] = { 10 };
    int Hs[] = { 10 };
    Image TData[Tsize];
    for (int I = 0; I < Tsize; I++) {
        Image& im = TData[I];
        int width = Ws[I];
        int height = Hs[I];
        int fsize = 3;

        im.w = Ws[I];
        im.h = Hs[I];

        unsigned char* pic;

        Image im_seq, om_seq, im_par, om_par;
        im_seq.w = im_par.w = width;
        im_seq.h = im_par.h = height;
        if (rank == 0) {
            pic = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
            unsigned char* pi = pic;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++, pi++) {
                    *pi = pix_dist(gen);
                }
            }

            im_seq.data = pic;
            im_par.data = pic;

            /* Sequential section */
            gauss_filter(&im_seq, &om_seq, fsize);
        }

        /* Parallel section */

        mpi_gauss_filter(&im_par, &om_par, fsize);

        /* Comparation */
        if (rank == 0) {
            if ((om_par.w != om_seq.w) || (om_par.h != om_seq.h)) {
                std::cerr << "Dimensions mismatch" << std::endl;
            } else {
                unsigned char* pi1 = om_seq.data;
                unsigned char* pi2 = om_par.data;
                for (int i = 0; i < om_par.w * om_par.h; i++, pi1++, pi2++) {
                    if (*pi1 != *pi2) {
                        success = false;
                        break;
                    }
                }
            }

            free(om_par.data);
            free(om_seq.data);
            free(pic);
        }
    }
    EXPECT_EQ(success, 1);
}
TEST(Parallel_MPI, Random_Matrix_test_50x50) {
    char* filename, * ofilename;

    int size, rank;
    MPI_Status status;
    const int tag = 0;
    bool success = true;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pix_dist(0, 255);

    const int Tsize = 1;
    int Ws[] = { 50 };
    int Hs[] = { 50 };
    Image TData[Tsize];
    for (int I = 0; I < Tsize; I++) {
        Image& im = TData[I];
        int width = Ws[I];
        int height = Hs[I];
        int fsize = 3;

        im.w = Ws[I];
        im.h = Hs[I];

        unsigned char* pic;

        Image im_seq, om_seq, im_par, om_par;
        im_seq.w = im_par.w = width;
        im_seq.h = im_par.h = height;
        if (rank == 0) {
            pic = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
            unsigned char* pi = pic;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++, pi++) {
                    *pi = pix_dist(gen);
                }
            }

            im_seq.data = pic;
            im_par.data = pic;

            /* Sequential section */
            gauss_filter(&im_seq, &om_seq, fsize);
        }

        /* Parallel section */

        mpi_gauss_filter(&im_par, &om_par, fsize);

        /* Comparation */
        if (rank == 0) {
            if ((om_par.w != om_seq.w) || (om_par.h != om_seq.h)) {
                std::cerr << "Dimensions mismatch" << std::endl;
            } else {
                unsigned char* pi1 = om_seq.data;
                unsigned char* pi2 = om_par.data;
                for (int i = 0; i < om_par.w * om_par.h; i++, pi1++, pi2++) {
                    if (*pi1 != *pi2) {
                        success = false;
                        break;
                    }
                }
            }

            free(om_par.data);
            free(om_seq.data);
            free(pic);
        }
    }
    EXPECT_EQ(success, 1);
}
TEST(Parallel_MPI, Random_Matrix_test_100x100) {
    char* filename, * ofilename;

    int size, rank;
    MPI_Status status;
    const int tag = 0;
    bool success = true;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pix_dist(0, 255);

    const int Tsize = 1;
    int Ws[] = { 100 };
    int Hs[] = { 100 };
    Image TData[Tsize];
    for (int I = 0; I < Tsize; I++) {
        Image& im = TData[I];
        int width = Ws[I];
        int height = Hs[I];
        int fsize = 3;

        im.w = Ws[I];
        im.h = Hs[I];

        unsigned char* pic;

        Image im_seq, om_seq, im_par, om_par;
        im_seq.w = im_par.w = width;
        im_seq.h = im_par.h = height;
        if (rank == 0) {
            pic = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
            unsigned char* pi = pic;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++, pi++) {
                    *pi = pix_dist(gen);
                }
            }

            im_seq.data = pic;
            im_par.data = pic;

            /* Sequential section */
            gauss_filter(&im_seq, &om_seq, fsize);
        }

        /* Parallel section */

        mpi_gauss_filter(&im_par, &om_par, fsize);

        /* Comparation */
        if (rank == 0) {
            if ((om_par.w != om_seq.w) || (om_par.h != om_seq.h)) {
                std::cerr << "Dimensions mismatch" << std::endl;
            } else {
                unsigned char* pi1 = om_seq.data;
                unsigned char* pi2 = om_par.data;
                for (int i = 0; i < om_par.w * om_par.h; i++, pi1++, pi2++) {
                    if (*pi1 != *pi2) {
                        success = false;
                        break;
                    }
                }
            }

            free(om_par.data);
            free(om_seq.data);
            free(pic);
        }
    }
    EXPECT_EQ(success, 1);
}
TEST(Parallel_MPI, Random_Matrix_test_1000x1000) {
    char* filename, * ofilename;

    int size, rank;
    MPI_Status status;
    const int tag = 0;
    bool success = true;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pix_dist(0, 255);

    const int Tsize = 1;
    int Ws[] = { 10 };
    int Hs[] = { 10 };
    Image TData[Tsize];
    for (int I = 0; I < Tsize; I++) {
        Image& im = TData[I];
        int width = Ws[I];
        int height = Hs[I];
        int fsize = 3;

        im.w = Ws[I];
        im.h = Hs[I];

        unsigned char* pic;

        Image im_seq, om_seq, im_par, om_par;
        im_seq.w = im_par.w = width;
        im_seq.h = im_par.h = height;
        if (rank == 0) {
            pic = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
            unsigned char* pi = pic;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++, pi++) {
                    *pi = pix_dist(gen);
                }
            }

            im_seq.data = pic;
            im_par.data = pic;

            /* Sequential section */
            gauss_filter(&im_seq, &om_seq, fsize);
        }

        /* Parallel section */
        mpi_gauss_filter(&im_par, &om_par, fsize);

        /* Comparation */
        if (rank == 0) {
            if ((om_par.w != om_seq.w) || (om_par.h != om_seq.h)) {
                std::cerr << "Dimensions mismatch" << std::endl;
            } else {
                unsigned char* pi1 = om_seq.data;
                unsigned char* pi2 = om_par.data;
                for (int i = 0; i < om_par.w * om_par.h; i++, pi1++, pi2++) {
                    if (*pi1 != *pi2) {
                        success = false;
                        break;
                    }
                }
            }

            free(om_par.data);
            free(om_seq.data);
            free(pic);
        }
    }
    EXPECT_EQ(success, 1);
}
TEST(Parallel_MPI, Random_Matrix_test_2500x2500) {
    char* filename, * ofilename;

    int size, rank;
    MPI_Status status;
    const int tag = 0;
    bool success = true;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pix_dist(0, 255);

    const int Tsize = 1;
    int Ws[] = { 25 };
    int Hs[] = { 25 };
    Image TData[Tsize];
    for (int I = 0; I < Tsize; I++) {
        Image& im = TData[I];
        int width = Ws[I];
        int height = Hs[I];
        int fsize = 3;

        im.w = Ws[I];
        im.h = Hs[I];

        unsigned char* pic;

        Image im_seq, om_seq, im_par, om_par;
        im_seq.w = im_par.w = width;
        im_seq.h = im_par.h = height;
        if (rank == 0) {
            pic = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
            unsigned char* pi = pic;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++, pi++) {
                    *pi = pix_dist(gen);
                }
            }

            im_seq.data = pic;
            im_par.data = pic;

            /* Sequential section */
            gauss_filter(&im_seq, &om_seq, fsize);
        }

        /* Parallel section */

        mpi_gauss_filter(&im_par, &om_par, fsize);

        /* Comparation */
        if (rank == 0) {
            if ((om_par.w != om_seq.w) || (om_par.h != om_seq.h)) {
                std::cerr << "Dimensions mismatch" << std::endl;
            } else {
                unsigned char* pi1 = om_seq.data;
                unsigned char* pi2 = om_par.data;
                for (int i = 0; i < om_par.w * om_par.h; i++, pi1++, pi2++) {
                    if (*pi1 != *pi2) {
                        success = false;
                        break;
                    }
                }
            }

            free(om_par.data);
            free(om_seq.data);
            free(pic);
        }
    }
    EXPECT_EQ(success, 1);
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
