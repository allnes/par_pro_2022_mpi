// Copyright 2022 Mitin Roman
#include <gtest/gtest.h>
#include <cmath>
#include <gtest-mpi-listener.hpp>
#include "./multi_dim_integr.h"

#ifndef M_E
#define M_E 2.7182818284590452354
#endif  // M_E

constexpr double max_error = 1e-2;

TEST(integrate_rectan_metod, test_cube) {
    range_t ranges[3];

    for (int i = 0; i < 3; i++) {
        ranges[i].a = 0;
        ranges[i].b = 1.0;
        ranges[i].n = 10;
    }

    auto lambda = [](double x, double y, double z) {
        return 1.0;
    };

    double actual = integrate(lambda, ranges);

    EXPECT_FLOAT_EQ(1.0, actual);
}

TEST(integrate_rectan_metod, test_paralellepiped) {
    range_t ranges[3];

    for (int i = 0; i < 3; i++) {
        ranges[i].a = 0;
        ranges[i].b = 1.0;
        ranges[i].n = 10;
    }

    ranges[0].b *= 2;
    ranges[2].b *= 3;

    auto lambda = [](double x, double y, double z) {
        return 1.0;
    };

    double actual = integrate(lambda, ranges);

    EXPECT_FLOAT_EQ(6.0, actual);
}

TEST(integrate_rectan_metod, test_exp) {
    range_t ranges[3];

    for (int i = 0; i < 3; i++) {
        ranges[i].a = 0;
        ranges[i].b = 1.0;
        ranges[i].n = 5;
    }

    ranges[0].n = 1000;

    auto lambda = [](double x, double y, double z) {
        return std::exp(x);
    };

    double actual = integrate(lambda, ranges);
    EXPECT_LE(std::abs(actual - (M_E - 1.0)), max_error);
}

TEST(integrate_rectan_metod, test_polinom) {
    range_t ranges[3];

    for (int i = 0; i < 3; i++) {
        ranges[i].a = 0;
        ranges[i].b = 1.0;
        ranges[i].n = 180;
    }

    auto lambda = [](double x, double y, double z) {
        return x + y + z;
    };

    double actual = integrate(lambda, ranges);

    EXPECT_LE(std::abs(actual - 1.5), max_error);
}

TEST(integrate_rectan_metod, test) {
    range_t ranges[3];

    for (int i = 0; i < 3; i++) {
        ranges[i].a = 0;
        ranges[i].b = 1.0;
        ranges[i].n = 10;
    }

    auto lambda = [](double x, double y, double z) {
        return 1.0;
    };

    double actual = integrate(lambda, ranges);

    EXPECT_FLOAT_EQ(1.0, actual);
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
