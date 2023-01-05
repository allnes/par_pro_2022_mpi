// Copyright 2022 Panov Alexey
#ifndef MODULES_TASK_3_PANOV_A_INT_MERGE_SORT_INT_MERGE_SORT_H_
#define MODULES_TASK_3_PANOV_A_INT_MERGE_SORT_INT_MERGE_SORT_H_

#include <vector>

using Vector = std::vector<int>;

const int CATEGORIES_COUNT = 10;

Vector mergeSort(const Vector& first, const Vector& second, int sbegin, int send);

int getMaxCategoryForVector(Vector* arr);
int getMaxCategoryForInt(int num);
int getCategoryValue(int num, int category);
void intSortForCategory(Vector* arr_, int category);
void intSort(Vector* arr);

void sortParallel(Vector* arr);

#endif  // MODULES_TASK_3_PANOV_A_INT_MERGE_SORT_INT_MERGE_SORT_H_
