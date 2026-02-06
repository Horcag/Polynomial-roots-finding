// Файл для сборки всех методов в кучу (будет реализован в (не)далеком будущем)

#pragma once

#include <chrono>
#include <iostream>
#include <string>

#ifdef _WIN32
// Избавляемся от конфликтов макросов и ускоряем подключение Windows SDK
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <psapi.h>
#include <crtdbg.h>


#endif

// генерируем полиномы и сравниваем полученные корни с исходными
#include <chrono>
#include <iostream>
#include <string>

// методы Грефе

// методы Сагралова
#include "methods/Sagralov/Sagralov_real_roots.h"

#ifdef _WIN32
// измерение затраченной памяти + измерение затраченного времени
// Реализация: Корастелин Никита, КМБО-01-23
class MemoryTracker {
public:
  static size_t GetAllocatedMemory() {
    _CrtMemState state;
    _CrtMemCheckpoint(&state);
    return state.lTotalCount;
  }
};

template <typename Func>
void MeasurePerformance(const std::string &method_name, Func &&func) {
  // 1. прогрев и сброс
  for (int i = 0; i < 3; ++i) {
    func();
    _CrtMemDumpAllObjectsSince(nullptr);
  }

  // 2. замер начального состояния
  size_t mem_before = MemoryTracker::GetAllocatedMemory();
  auto start = std::chrono::high_resolution_clock::now();

  // 3. выполнение функции
  func();

  // 4. замер конечного состояния
  auto end = std::chrono::high_resolution_clock::now();
  size_t mem_after = MemoryTracker::GetAllocatedMemory();

  // 5. принудительный сброс
  _CrtMemDumpAllObjectsSince(nullptr);

  // 6. вывод результатов
  std::cout << method_name << ":\n"
            << "  Время: " << std::chrono::duration<double>(end - start).count()
            << " сек\n"
            << "  Память: "
            << (mem_after > mem_before ? (mem_after - mem_before) : 0)
            << " Кбайт\n\n";
}
#else
class MemoryTracker {
public:
  static size_t GetAllocatedMemory() { return 0; }
};

template <typename Func>
void MeasurePerformance(const std::string &method_name, Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << method_name << ":\n"
            << "  Время: " << std::chrono::duration<double>(end - start).count()
            << " сек\n"
            << "  Память: недоступно на этой платформе\n\n";
}
#endif