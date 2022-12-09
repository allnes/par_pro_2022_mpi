// Copyright 2022 Artemev Alexej
#ifndef MODULES_TASK_1_ARTEMEV_A_RECT_INTEGRAL_RECT_INTG_H_
#define MODULES_TASK_1_ARTEMEV_A_RECT_INTEGRAL_RECT_INTG_H_


float lrect_intg_par(float l, float r, int n, float(*f)(float));
float mp_intg_par(float l, float r, int n, float(*f)(float));

float lrect_intg_seq(float l, float r, int n, float(*f)(float));
float mp_intg_seq(float l, float r, int n, float(*f)(float));


#endif  // MODULES_TASK_1_ARTEMEV_A_RECT_INTEGRAL_RECT_INTG_H_
