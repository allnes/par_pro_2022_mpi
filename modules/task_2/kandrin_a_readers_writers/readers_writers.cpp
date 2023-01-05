// Copyright 2022 Kandrin Alexey
#include "../../../modules/task_2/kandrin_a_readers_writers/readers_writers.h"

#include <mpi.h>

#include <algorithm>
#include <memory>
#include <vector>

#ifdef DEBUG_OUTPUT
#include <fstream>
extern std::vector<std::ofstream> outs;
#endif

ByteSpan::ByteSpan(char* begin, size_t size) : m_begin(begin), m_size(size) {}

const char* ByteSpan::GetData() const { return m_begin; }

size_t ByteSpan::GetSize() const { return m_size; }

Memory::Memory() { memset(m_buffer.data(), 0, m_buffer.size()); }

void Memory::Write(ByteSpan span, size_t index) {
  assert(index + span.GetSize() <= m_buffer.size());

  // we use "memmove" (instead of "memcpy") because span and buffer memory can
  // overlap
  memmove(m_buffer.data() + index, span.GetData(), span.GetSize());
}

ByteSpan Memory::Read(size_t size, size_t index) {
  assert(index + size <= m_buffer.size());
  return ByteSpan(m_buffer.data() + index, size);
}

void masterProcessFunction(Memory* memory, int requestsCount) {
  int procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);

  const int workerCount = procCount - 1;

  OperationInt operationBuffer;

  // Receive procCount - 1 requests
  for (int i = 0; i < requestsCount; ++i) {
    MPI_Status status;
    MPI_Recv(reinterpret_cast<char*>(&operationBuffer), sizeof(OperationInt),
             MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    // handle operation
    operationBuffer.SetMemory(memory);
    auto result = operationBuffer.Perform();
    if (operationBuffer.GetOperationType() ==
        OperationInt::OperationType::read) {
      // send response
      MPI_Send(&result, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }
  }
}

std::vector<int> readerProcessFunction(int readingCount) {
  std::vector<int> results(readingCount, 0);
  std::vector<OperationInt> operations(
      readingCount, OperationInt(0, OperationInt::OperationType::read, 0));

  for (int i = 0; i < readingCount; ++i) {
    auto& currentOperation = operations.at(i);

    MPI_Send(reinterpret_cast<char*>(&currentOperation), sizeof(OperationInt),
             MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }

  for (int i = 0; i < readingCount; ++i) {
    auto& currentResult = results[i];

    MPI_Status status;
    MPI_Recv(&currentResult, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  return results;
}

void writerProcessFunction(std::vector<OperationInt>* operations) {
  for (int i = 0; i < operations->size(); ++i) {
    auto& currentOperation = operations->at(i);
    MPI_Send(reinterpret_cast<char*>(&currentOperation), sizeof(OperationInt),
             MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
}
