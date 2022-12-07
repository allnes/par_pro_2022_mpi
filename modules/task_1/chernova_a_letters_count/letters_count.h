// Copyright 2022 Chernova Anna
#pragma once

#include <mpi.h>

#include <random>
#include <string>

using std::string;

int countLetters(string str);
int countLettersMPI(string str, int size);

string randomStr(int size);
