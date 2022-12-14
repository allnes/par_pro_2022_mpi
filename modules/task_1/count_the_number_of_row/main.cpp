#include <iostream>
#include "sentencesPerLine.h"
using namespace std;


int main() {
    string str = "Строка, состоящая из 3 предложений. Воторе предложение тут. Проверка?";

    cout << sentencesPerLine::countSentences(str);
}