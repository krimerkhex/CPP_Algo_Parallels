#ifndef S21_CONSOLE_MENU_H
#define S21_CONSOLE_MENU_H

#include <chrono>
#include <iostream>

#include "s21_graph_algorithms.h"
#include "s21_mul_algorithms.h"
#include "s21_slau_algoritms.h"

namespace s21 {
class ConsoleMenu {
 public:
  ConsoleMenu() = default;
  ~ConsoleMenu() = default;
  static void start();

 private:
  static bool choise();
  static void mainMenu();
  static std::pair<bool, size_t> enterNumber();
  static bool solveTravelingSalesmanProblem();
  static bool sTSPSecond(const Graph&);
  static bool loadGraph(Graph&);
  static bool createGraph(Graph&);
  static bool SLAU();
  static bool createOrRandomSlau(Tensor<double>&, Tensor<double>&);
  static bool create(Tensor<double>&);
  static bool random(Tensor<double>&);
  static bool mulMatrix();
  static bool createOrRandomMatrix(Tensor<double>&, Tensor<double>&);
  static void errorChoise();
  static void timerFoo(const std::string&, const std::function<void(void)>&);
};
};  // namespace s21

#endif  // S21_CONSOLE_MENU_H
