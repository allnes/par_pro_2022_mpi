#include "simpsonMethod.h";

//Любая математическая функция
double simpson::Square(double a) {
    return a * a;
}

//Метод симпсона (a - , b - , n - , )
double simpson::SimpsonMethod(double a, double b, unsigned n, double (*f)(double x)) {
    //Длина интервала
    double h = (b - a) / n;
    //Сумма
    double sum = 0;

    //Начальная позиция интервала
    double x0 = a;
    //Конечная позиция интервала
    double x1 = a + h;

    //Подсчёт суммы для каждого отрезка
    for (unsigned i=0; i<=n-1; i++) {
        sum += f(x0) + 4*f(x0 + h/2) + f(x1);

        x0 += h;
        x1 += h;
    }

    return (h/6)*sum;
}