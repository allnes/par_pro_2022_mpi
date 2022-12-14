#include <iostream>
using namespace std;
#include "simpsonMethod.h"

int main() {
    cout << simpson::SimpsonMethod(1, 2, 3, simpson::Square);
}
