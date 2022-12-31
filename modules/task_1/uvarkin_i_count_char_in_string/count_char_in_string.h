// Copyright 2022 Uvarkin Ilya
#pragma once
#include <cstdio>
#include <string>

int getCountCharInString(const char* str, char ch);
int getLenStringForRankProcess(int len, int rank, int procNum);
int getFirstIndexString(int len, int rank, int procNum);
int getCountCharInStringParallel(const std::string& str, char letter);
