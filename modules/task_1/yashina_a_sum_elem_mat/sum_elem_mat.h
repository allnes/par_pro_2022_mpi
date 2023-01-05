// Copyright 2022 Yashina Anastasia
#ifndef MODULES_TASK_1_YASHINA_A_SUM_ELEM_MAT_SUM_ELEM_MAT_H_
#define MODULES_TASK_1_YASHINA_A_SUM_ELEM_MAT_SUM_ELEM_MAT_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

// основная работа
int Sum(int size, std::vector<int> matrix);

// работа с отдельной частью, вычисление суммы
int SumPart(std::vector<int> matrix);

std::vector<int> getMat(int size);

#endif  // MODULES_TASK_1_YASHINA_A_SUM_ELEM_MAT_SUM_ELEM_MAT_H_
