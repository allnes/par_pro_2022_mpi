#ifndef SIMPSON_SIMPSONMETHOD_H
#define SIMPSON_SIMPSONMETHOD_H
namespace simpson
{
    double Square(double);
    double SimpsonMethod(double, double, unsigned, double (*)(double));
}

#endif //SIMPSON_SIMPSONMETHOD_H
