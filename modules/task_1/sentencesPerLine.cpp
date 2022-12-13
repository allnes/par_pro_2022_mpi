#include "sentencesPerLine.h"

int sentencesPerLine::countSentences(std::string str) {
    int count(0);
    for(int i =0; i < str.length(); i++)
    {
        if ((str[i] == '.') || (str[i] == '!') || (str[i] == '?'))
            count++;
    }
    return count;
}