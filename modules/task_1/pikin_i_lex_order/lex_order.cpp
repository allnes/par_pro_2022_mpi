// Copyright 2022 Pikin Ilya
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include "../../../modules/task_1/pikin_i_lex_order/lex_order.h"

std::string getStrFromRes(Result result) {
    if (result == Result::equal)
        return std::string("equel");
    else if (result == Result::greater_than)
        return std::string("greater than");
    else
        return std::string("less than");
}

std::string getRandomString(const int strSize) {
    std::string str;
    str.reserve(strSize);
    std::mt19937 engine;
    std::random_device device;
    engine.seed(device());
    std::uniform_int_distribution<int> char_distribution(
        97, 122);  // ASCII codes from a to z

    for (int i = 0; i < strSize; i++) {
        char c = static_cast<char>(char_distribution(engine));
        str += c;
    }
    return str;
}

std::string changeSomeChars(std::string str,
    int numChars) {  // change ~numChars of str chars
    int size = str.size();
    if (numChars > size) {
        numChars = size;
    }
    std::mt19937 engine;
    std::random_device device;
    engine.seed(device());
    std::uniform_int_distribution<int> char_distribution(
        97, 122);  // ASCII codes from a to z
    std::uniform_int_distribution<int> char_position(0,
        size - 1);  // index of str

    for (int i = 0; i < numChars; i++) {
        char c = static_cast<char>(char_distribution(engine));
        int pos = char_position(engine);
        str[pos] = c;
    }
    return str;
}

Result getSequentialCompare(const std::string& a, const std::string& b) {
    const int minLen = a.size() < b.size() ? a.size() : b.size();
    for (int i = 0; i < minLen; i++) {
        if (a[i] < b[i]) return Result::less_than;
        if (a[i] > b[i]) return Result::greater_than;
    }
    if (a.size() < b.size()) return Result::less_than;
    if (a.size() > b.size()) return Result::greater_than;
    return Result::equal;
}

Result getParallelCompare(const std::string& a, const std::string& b) {
    int procNum, procRank;
    int minLen, copyLen, delta;
    Result result;
    char* a_copy, * b_copy, * a_part, * b_part;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::cout << "Mark 1: procRank = " << procRank << std::endl;

    if (procRank == 0) {
        minLen = a.size() < b.size() ? a.size() : b.size();
        int remainder = minLen % procNum;

        // addition to the original strings to
        // divide them into equal parts
        int additionLen = (procNum - remainder) % procNum;

        copyLen = minLen + additionLen;
        delta = copyLen / procNum;

        a_copy = new char[copyLen];
        b_copy = new char[copyLen];
        // initializing copies of strings
        for (int i = 0; i < copyLen; i++) {
            if (i < minLen) {
                a_copy[i] = a[i];
                b_copy[i] = b[i];
            } else {
                a_copy[i] = 'a';  // neutral addition
                b_copy[i] = 'a';  // neutral addition
            }
        }
    }
    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "Mark 2: procRank = " << procRank << std::endl;

    a_part = new char[delta];
    b_part = new char[delta];

    MPI_Scatter(a_copy, delta, MPI_CHAR, a_part, delta, MPI_CHAR, 0,
        MPI_COMM_WORLD);
    MPI_Scatter(b_copy, delta, MPI_CHAR, b_part, delta, MPI_CHAR, 0,
        MPI_COMM_WORLD);
    std::cout << "Mark 3: procRank = " << procRank << std::endl;

    // processing local parts of strings
    for (int i = 0; i < delta; i++) {
        if (a_part[i] < b_part[i]) {
            result = Result::less_than;
            goto next;  // just skipping "equel" init
        }
        if (a_part[i] > b_part[i]) {
            result = Result::greater_than;
            goto next;  // just skipping "equel" init
        }
    }
    result = Result::equal;
next:
    int int_result = static_cast<int>(result);
    int* allResultsPtr;

    if (procRank == 0) {
        allResultsPtr = new int[procNum];
    }

    MPI_Gather(&int_result, 1, MPI_INT, allResultsPtr, 1, MPI_INT, 0,
        MPI_COMM_WORLD);
    std::cout << "Mark 4: procRank = " << procRank << std::endl;

    if (procRank == 0) {
        std::cout << "Array allResults:\n";
        for (int i = 0; i < procNum; i++) std::cout << allResultsPtr[i] << " ";
        std::cout << std::endl;

        // delete copies
        delete[] a_copy;
        delete[] b_copy;
        delete[] a_part;
        delete[] b_part;

        for (int i = 0; i < procNum; i++) {
            if (allResultsPtr[i] == 0) {
                delete[] allResultsPtr;
                return Result::less_than;
            }
            if (allResultsPtr[i] == 2) {
                delete[] allResultsPtr;
                return Result::greater_than;
            }
        }
        if (a.size() < b.size()) {
            delete[] allResultsPtr;
            return Result::less_than;
        }
        if (a.size() > b.size()) {
            delete[] allResultsPtr;
            return Result::greater_than;
        }
        delete[] allResultsPtr;
        return Result::equal;
    } else {
        delete[] a_part;
        delete[] b_part;
        return result;
    }
}
