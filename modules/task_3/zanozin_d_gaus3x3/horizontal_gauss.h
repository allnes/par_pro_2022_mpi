// Copyright 2022 Zanozin Danila
#include <vector>

// #define DEBUG

constexpr unsigned char gaussFilter[3][3]{ {1, 2, 1},
                                           {2, 4, 2},
                                           {1, 2, 1} };

constexpr unsigned char sumMask{ 16 };

std::vector<unsigned char> getRandomImage(int _cols, int _rows);

unsigned char changePixel(const std::vector<unsigned char>& _source, int x, int y, int rows, int cols);

std::vector<unsigned char> filterImageParallel(std::vector<unsigned char> source, int rows, int cols);
std::vector<unsigned char> filterImageSequential(std::vector<unsigned char> source, int rows, int cols);



#endif
