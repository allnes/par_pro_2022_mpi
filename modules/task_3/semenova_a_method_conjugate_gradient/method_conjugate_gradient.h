// Copyright 2022 Semenova Veronika
#ifndef MODULES_TASK_3_SEMENOVA_A_METHOD_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_H_
#define MODULES_TASK_3_SEMENOVA_A_METHOD_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_H_

#include <vector>

#define Vector std::vector < double >

  Vector RandVec(int n);
Vector RandMat(int n);

double scalar_mult(const Vector & x,
  const Vector & y);
Vector mult_MxV(const Vector & M,
  const Vector & V);

Vector Serial_method_gradient(const Vector & M,
  const Vector & V, int n);
Vector Paralle_method_gradient(const Vector & M,
  const Vector & V, int n);

#endif  // MODULES_TASK_3_SEMENOVA_A_METHOD_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_H_
