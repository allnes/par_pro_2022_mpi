// Copyright 2022 Voronov Alexander

#ifndef MODULES_TASK_3_VORONOV_A_HISTOGRAM_LINEAR_STRETCH_HISTOGRAM_LINEAR_STRETCH_H_
#define MODULES_TASK_3_VORONOV_A_HISTOGRAM_LINEAR_STRETCH_HISTOGRAM_LINEAR_STRETCH_H_

#include <vector>
#include <string>

std::vector<uint8_t> ImageRead(std::string fileName);
void ImageWrite(std::vector<uint8_t> *vectorRef, std::string fileName);
std::vector<uint8_t> GetRandomPixelArray(int size, uint8_t min, uint8_t max);
uint8_t SequentialMaxValue(std::vector<uint8_t> *vectorRef);
uint8_t SequentialMinValue(std::vector<uint8_t> *vectorRef);
void Segmentation(int size, int worldSize, int rank, int* segmentStart, int* segmentSize);
void Stretch(std::vector<uint8_t> *vectorRef, uint8_t minValue, uint8_t maxValue, uint8_t min, uint8_t max);
void ParallelStretch(std::vector<uint8_t> *vectorRef, uint8_t min, uint8_t max);

#endif  // MODULES_TASK_3_VORONOV_A_HISTOGRAM_LINEAR_STRETCH_HISTOGRAM_LINEAR_STRETCH_H_
