// Copyright 2022 Kandrin Alexey
#ifndef MODULES_TASK_2_KANDRIN_A_READERS_WRITERS_READERS_WRITERS_H_
#define MODULES_TASK_2_KANDRIN_A_READERS_WRITERS_READERS_WRITERS_H_

#include <array>
#include <cassert>
#include <cstring>
#include <vector>

#ifdef DEBUG_OUTPUT
#include <ostream>
#endif

class ByteSpan {
  char* m_begin;  ///< non-owning pointer
  size_t m_size;  ///< span size

 public:
  ByteSpan(char* begin, size_t size);

  template <class T>
  explicit ByteSpan(const T& value) : m_begin(&value), m_size(sizeof(T)) {}

  template <class T>
  operator T() const {
    const T* t = reinterpret_cast<const T*>(m_begin);
    return *t;
  }

  const char* GetData() const;

  size_t GetSize() const;

#ifdef DEBUG_OUTPUT
  friend std::ostream& operator<<(std::ostream& out, const ByteSpan& byteSpan) {
    for (int i = 0; i < byteSpan.m_size; ++i) {
      out << static_cast<int>(byteSpan.m_begin[i]) << ' ';
    }
    return out;
  }
#endif
};

class Memory {
  std::array<char, 1024> m_buffer = {0};

 public:
  Memory();

  constexpr size_t GetSize() const { return m_buffer.size(); }

  // Write buffer "data" with size "size" to memory at index "index".
  void Write(ByteSpan span, size_t index);

  ByteSpan Read(size_t size, size_t index);

  char* data() { return m_buffer.data(); }
};

template <class T>
class Operation {
 public:
  enum class OperationType : int {
    operator_add,  // reinterpret_cast<T*>(memory.data())[index] += argument
    operator_dif,  // reinterpret_cast<T*>(memory.data())[index] -= argument
    read,
  };

 private:
  Memory* m_memory;
  size_t m_index;
  OperationType m_operationType;
  T m_argument;

 public:
  // memory - What memory is being operated on?
  // index - what index is being operated on?
  // operationType - type of operation (e.g. plus/minus)
  // argument - argument for operation
  Operation(Memory* memory, size_t index, OperationType operationType,
            const T& argument)
      : m_memory(memory),
        m_index(index),
        m_operationType(operationType),
        m_argument(argument) {}

  Operation(size_t index, OperationType operationType, const T& argument)
      : Operation(nullptr, index, operationType, argument) {}

  Operation() : Operation(nullptr, -1, OperationType{}, T{}) {}

  void SetMemory(Memory* memory) { m_memory = memory; }

  OperationType GetOperationType() const { return m_operationType; }

  const T& GetArgument() const { return m_argument; }

  T Perform() {
    assert(m_index < m_memory->GetSize() / sizeof(T));
    T& variable = reinterpret_cast<T*>(m_memory->data())[m_index];
    switch (m_operationType) {
      case OperationType::operator_add: {
        variable += m_argument;
        break;
      }
      case OperationType::operator_dif: {
        variable -= m_argument;
        break;
      }
      case OperationType::read: {
        break;
      }
      default: {
        assert(false && "unknown enumeration value");
        break;
      }
    }
    return variable;
  }

#ifdef DEBUG_OUTPUT
  friend std::ostream& operator<<(std::ostream& out,
                                  const Operation& operation) {
    out << "Operation: " << operation.m_memory << ' ' << operation.m_index
        << ' ' << static_cast<int>(operation.m_operationType) << ' '
        << operation.m_argument;
    return out;
  }
#endif
};

using OperationInt = Operation<int>;

// function for master process
void masterProcessFunction(Memory* memory, int requestsCount);

// function for "reader" process
std::vector<int> readerProcessFunction(int readingCount);

// function for "writer" process
void writerProcessFunction(std::vector<OperationInt>* operations);

#endif  // MODULES_TASK_2_KANDRIN_A_READERS_WRITERS_READERS_WRITERS_H_
