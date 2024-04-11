#include "s21_console_menu.h"

namespace s21 {
void ConsoleMenu::start() {
  while (choise()) {
  }
}

bool ConsoleMenu::choise() {
  bool error(true);
  std::string in;
  mainMenu();
  std::cin >> in;
  if (in.size() == 1) {
    switch (in[0]) {
      case '1':
        error = solveTravelingSalesmanProblem();
        break;
      case '2':
        error = SLAU();
        break;
      case '3':
        error = mulMatrix();
        break;
      case '0':
        error = false;
        break;
      default:
        errorChoise();
        break;
    }
  } else if (in.empty()) {
    error = false;
  } else {
    errorChoise();
  }
  return error;
}

void ConsoleMenu::mainMenu() {
  std::cout << "_____________Thread algorithms_____________\n"
            << "1. Solving the traveling salesman problem\n"
            << "2. SLAU\n"
            << "3. Multiplication matrix\n"
            << "0. Exit" << std::endl;
}

std::pair<bool, size_t> ConsoleMenu::enterNumber() {
  int number(0);
  std::cout << "Enter the number of operations" << std::endl;
  std::string in;
  std::cin >> in;
  bool error(!in.empty());
  try {
    number = std::stoi(in);
  } catch (const std::exception&) {
  }
  if (number < 0) {
    std::cout << "The number must be positive" << std::endl;
    number = 0;
  }
  return std::make_pair(error, static_cast<size_t>(number));
}

bool ConsoleMenu::solveTravelingSalesmanProblem() {
  bool error(true);
  std::string in;
  std::cout << "1. Load graph\n"
            << "2. Create graph" << std::endl;
  std::cin >> in;
  error = !in.empty();
  if (in.size() == 1) {
    Graph graph;
    switch (in[0]) {
      case '1':
        error = loadGraph(graph);
        break;
      case '2':
        error = createGraph(graph);
        break;
      default:
        errorChoise();
        break;
    }
    sTSPSecond(graph);
  } else {
    errorChoise();
  }
  return error;
}

bool ConsoleMenu::sTSPSecond(const Graph& graph) {
  std::pair<bool, size_t> res(enterNumber());
  bool error(res.first);
  size_t number(res.second);
  if (error && number && !graph.empty() && res.second) {
    std::cout << "Graph: " << graph << std::endl;
    GraphAlgorithms ga;
    std::pair<bool, TsmResult> res1, res2;
    timerFoo("SolveTravelingSalesmanProblem not thread", [&] {
      for (size_t i(0); i < number; ++i) {
        res1 = std::move(ga.solveTravelingSalesmanProblem(graph));
      }
    });
    timerFoo("SolveTravelingSalesmanProblem yeat thread", [&] {
      for (size_t i(0); i < number; ++i) {
        res2 = (ga.solveTravelingSalesmanProblemThread(graph));
      }
    });
    std::cout << "Result work not thread:"
              << "  Solution found: " << (res1.first ? "true" : "false")
              << "  Path: " << res1.second.vertices
              << "\n  Distance: " << res1.second.distance << std::endl;
    std::cout << "Result work yeat thread:"
              << "  Solution found: " << (res2.first ? "true" : "false")
              << "  Path: " << res2.second.vertices
              << "\n  Distance: " << res2.second.distance << std::endl;
  } else if (graph.empty()) {
    std::cout << "Incorrect graph" << std::endl;
  }
  return error;
}

bool ConsoleMenu::loadGraph(Graph& graph) {
  std::cout << "Enter the path to the file" << std::endl;
  std::string path;
  std::cin >> path;
  bool error(!path.empty());
  graph.loadGraphFromFile(path);
  if (graph.empty()) {
    std::cout << "Not correct file or path" << std::endl;
  } else {
    std::cout << "Graph loading successful" << std::endl;
  }
  return error;
}

bool ConsoleMenu::createGraph(Graph& graph) {
  std::cout << "Enter the number vertices" << std::endl;
  std::string in;
  std::cin >> in;
  bool error(!in.empty());
  try {
    graph.create(std::stoi(in), 50);
  } catch (const std::exception&) {
    std::cout << "Enter the correct data" << std::endl;
  }
  return error;
}

bool ConsoleMenu::SLAU() {
  Tensor<double> cof, ref;
  bool error(createOrRandomSlau(cof, ref));
  std::pair<bool, size_t> res(enterNumber());
  error = !res.first ? false : error;
  if (error && res.second) {
    if (!cof.empty() && !ref.empty()) {
      std::cout << "Matrix coef: " << cof << "\nMatrix ref: " << ref
                << std::endl;
      SlauAlgorithms sl;
      Tensor<double> res1, res2;
      timerFoo("SLAU not thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          res1 = std::move(sl.calculate(cof, ref));
        }
      });
      timerFoo("SLAU yeat thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          res2 = std::move(sl.calculateThread(cof, ref));
        }
      });
      std::cout << "Result work SLAU not thread: " << res1
                << "\nResult work SLAU yeat thread: " << res2 << std::endl;
    } else {
      std::cout << "Incorrect matrix" << std::endl;
    }
  }
  return error;
}

bool ConsoleMenu::createOrRandomSlau(Tensor<double>& cof, Tensor<double>& ref) {
  std::string in;
  std::cout << "Enter size matrix" << std::endl;
  std::cin >> in;
  bool error(!in.empty());
  if (error) {
    size_t num(0);
    try {
      num = std::stoull(in);
    } catch (const std::exception&) {
      std::cout << "Chislo!" << std::endl;
    }
    if (num > 0) {
      cof.resize(num, num);
      ref.resize(num);
    } else {
      std::cout << "Normalnoe chislo!" << std::endl;
    }
    in.clear();
    std::cout << "Create or random? 1/0" << std::endl;
    std::cin >> in;
    error = !in.empty();
    unsigned ch(0);
    try {
      ch = std::stoull(in);
    } catch (const std::exception&) {
    }
    if (error && ch == 0) {
      error = random(cof) && random(ref);
    } else if (error && ch == 1) {
      error = create(cof) && create(ref);
    } else {
      std::cout << "1/0!!!! Da?!" << std::endl;
    }
  }
  return error;
}

bool ConsoleMenu::create(Tensor<double>& matrix) {
  std::cout << "Enter " << matrix.size() << " number for matrix cof"
            << std::endl;
  bool error(true);
  for (size_t i(0); i < matrix.size() && error; ++i) {
    std::cout << i << "/" << matrix.size() << " Enter number" << std::endl;
    std::string in;
    std::cin >> in;
    error = !in.empty();
    try {
      matrix[i] = std::stod(in);
    } catch (const std::exception&) {
      std::cout << "Chislo" << std::endl;
      matrix.clear();
    }
  }
  return error;
}

bool ConsoleMenu::random(Tensor<double>& matrix) {
  std::cout << "Enter max num greater than zero for random" << std::endl;
  std::string in;
  std::cin >> in;
  bool error(!in.empty());
  try {
    size_t n(std::stoull(in));
    if (n == 0) n = 1;
    matrix.noise(n);
  } catch (const std::exception&) {
    std::cout << "Chilso!" << std::endl;
    matrix.clear();
  }
  return error;
}

bool ConsoleMenu::mulMatrix() {
  Tensor<double> matrix1, matrix2;
  bool error(createOrRandomMatrix(matrix1, matrix2));
  std::pair<bool, size_t> res(enterNumber());
  error = res.first;
  if (error && res.second) {
    if (!matrix1.empty() && !matrix2.empty()) {
      std::cout << "Matrix 1: " << matrix1 << "\nMatrix 2: " << matrix2
                << std::endl;
      Tensor<double> res1, res2;
      timerFoo("mulMatrix not thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          res1 = std::move(matrix1 * matrix2);
        }
      });
      timerFoo("mulMatrix base thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          res2 = std::move(MatrixAlgorithms::mulMatrix(matrix1, matrix2, 0));
        }
      });
      timerFoo("mulMatrix 2 thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          MatrixAlgorithms::mulMatrix(matrix1, matrix2, 2);
        }
      });
      timerFoo("mulMatrix 4 thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          MatrixAlgorithms::mulMatrix(matrix1, matrix2, 4);
        }
      });
      timerFoo("mulMatrix 8 thread", [&] {
        for (size_t i(0); i < res.second; ++i) {
          MatrixAlgorithms::mulMatrix(matrix1, matrix2, 8);
        }
      });
      size_t cop(std::thread::hardware_concurrency());
      timerFoo("mulMatrix 4 * (number of logical computer processors) thread",
               [&] {
                 for (size_t i(0); i < res.second; ++i) {
                   MatrixAlgorithms::mulMatrix(matrix1, matrix2, cop * 4);
                 }
               });
      timerFoo("mulMatrix conveir", [&] {
        for (size_t i(0); i < res.second; ++i) {
          MatrixAlgorithms::mulMatrix(matrix1, matrix2, 100);
        }
      });
      std::cout << "Result mul matrix not thread: " << res1
                << "\nResult mul matrix yeat thread: " << res2 << std::endl;
    } else {
      std::cout << "Incorrect matrix" << std::endl;
    }
  }
  return error;
}

bool ConsoleMenu::createOrRandomMatrix(Tensor<double>& matrix1,
                                       Tensor<double>& matrix2) {
  std::string num1, num2, num3;
  std::cout << "Enter 3 matrix shape numbers" << std::endl;
  std::cin >> num1;
  std::cin >> num2;
  std::cin >> num3;
  bool error(!num1.empty() && !num2.empty() && !num3.empty());
  size_t n1(0), n2(0), n3(0);
  try {
    n1 = std::stoull(num1);
    n2 = std::stoull(num2);
    n3 = std::stoull(num3);
  } catch (const std::exception&) {
    n1 = n2 = n3 = 0;
    std::cout << "Enter normal numbers, yes" << std::endl;
  }
  if (n1 && n2 && n3 && error) {
    matrix1.resize(n1, n2);
    matrix2.resize(n2, n3);
    std::string in;
    std::cout << "Create or random? 1/0" << std::endl;
    std::cin >> in;
    error = !in.empty();
    unsigned ch(0);
    try {
      ch = std::stoull(in);
    } catch (const std::exception&) {
    }
    if (ch == 0 && error) {
      error = random(matrix1) && random(matrix2);
    } else if (ch == 1 && error) {
      error = create(matrix1) && create(matrix2);
    } else {
      std::cout << "1/0!!!! Da?!" << std::endl;
    }
  }
  return error;
}

void ConsoleMenu::errorChoise() {
  std::cout << "Wrong menu item selected" << std::endl;
}

void ConsoleMenu::timerFoo(const std::string& alg,
                           const std::function<void(void)>& fn) {
  auto begin_(std::chrono::steady_clock::now());
  fn();
  auto end_(std::chrono::steady_clock::now());
  auto elapsed_ms(
      std::chrono::duration_cast<std::chrono::milliseconds>(end_ - begin_));
  std::cout << "Time work " + alg + ": " << elapsed_ms.count() << std::endl;
}
};  // namespace s21