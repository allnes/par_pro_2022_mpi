// Copyright 2022 Pashina Alina

#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "./pashina_a_lex_compare_strings.h";

int compareStrings(const std::string& first_str,
                   const std::string& second_str) {
  if (first_str == second_str)
    return 100;
  else if (first_str > second_str)
    return 101;
  else
    return 99;
}

std::string generateString(size_t len) {
  std::string allowed_symbols =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::string your_new_str;
  std::random_device randGen;
  std::mt19937 generator;
  generator.seed(randGen());
  your_new_str.reserve(len);
  int i = 0;
  while (i < len) {
    your_new_str.push_back(
        allowed_symbols[generator() % allowed_symbols.length()]);
    i++;
  }
  return your_new_str;
}

int doParallel(const std::string& first_str, const std::string& second_str,
               size_t lenstr1, size_t lenstr2) {
  if (!lenstr1 || !lenstr2) throw "Not enough symbols";
  size_t shortestLen = 1;
  if (lenstr1 < lenstr2) {
    shortestLen = lenstr1;
  } else {
    shortestLen = lenstr2;
  }
  int size = 1;
  std::string first_str_part;
  std::string second_str_part;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  size_t part_size = shortestLen / size;
  if (static_cast<int>(shortestLen) < size)
    return compareStrings(first_str, second_str);
  size_t reminder = shortestLen % size;
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int pr_turn = 1;
    while (pr_turn < size) {
      int addend = pr_turn * part_size + reminder;
      MPI_Send(first_str.c_str() + addend, static_cast<int>(part_size),
               MPI_CHAR, pr_turn, 0, MPI_COMM_WORLD);
      MPI_Send(second_str.c_str() + addend, static_cast<int>(part_size),
               MPI_CHAR, pr_turn, 1, MPI_COMM_WORLD);
      pr_turn = pr_turn + 1;
    }
  }
  std::vector<int> final_answers;
  int part_res = 0;
  if (rank != 0) {
    MPI_Status status;
    first_str_part.resize(part_size);
    second_str_part.resize(part_size);
    std::vector<char> bufstorage(part_size);
    MPI_Recv(&bufstorage[0], static_cast<int>(part_size), MPI_CHAR, 0, 0,
             MPI_COMM_WORLD, &status);
    MPI_Recv(&bufstorage[0], static_cast<int>(part_size), MPI_CHAR, 0, 1,
             MPI_COMM_WORLD, &status);
    first_str_part.assign(bufstorage.begin(), bufstorage.end());
    second_str_part.assign(bufstorage.begin(), bufstorage.end());
  } else {
    first_str_part = first_str.substr(0, part_size + reminder);
    second_str_part = second_str.substr(0, part_size + reminder);
  }
  part_res = compareStrings(first_str_part, second_str_part);
  if (rank == 0) final_answers.resize(size);
  MPI_Gather(&part_res, 1, MPI_INT, final_answers.data(), 1, MPI_INT, 0,
             MPI_COMM_WORLD);

  if (rank == 0) {
    bool startsEqually = true;
    bool endres = false;
    for (const auto& part_res : final_answers) {
      if (part_res == 101) {
        if (!endres) return 101;
        break;
      } else if (part_res == 99) {
        endres = true;
        startsEqually = false;
        return 99;
        break;
      } else {
        endres = true;
        if (startsEqually) {
          if (lenstr1 > lenstr2)
            return 101;
          else if (lenstr1 == lenstr2)
            return 100;
          else
            return 99;
        }
        break;
      }
    }
  }

  return part_res;
}

