#include "s21_slau_algoritms.h"

namespace s21 {

Tensor<double> SlauAlgorithms::calculate(const Tensor<double>& coff,
                                         const Tensor<double>& reff) {
  Tensor<double> cof(coff), ref(reff);
  cof.pushBackM(ref, 1);
  trianAlg(cof);
  return calculateResult(cof);
}

Tensor<double> SlauAlgorithms::calculateThread(const Tensor<double>& coff,
                                               const Tensor<double>& reff) {
  Tensor<double> cof(coff), ref(reff);
  cof.pushBackM(ref, 1);
  trianAlgThread(cof);
  return calculateResult(cof);
}

bool SlauAlgorithms::check(const double a, const double b) {
  return (a - b) < 0.000001;
}

void SlauAlgorithms::searchAndSwap(Tensor<double>& cof, const size_t it) {
  size_t i(0);
  for (; i < cof.rows(); ++i) {
    if (cof(i, it)) break;
  }
  if (i) {
    for (size_t j(0); j < cof.colls(); ++j) {
      double tmp(cof(it, j));
      cof(it, j) = cof(i, j);
      cof(i, j) = tmp;
    }
  }
}

void SlauAlgorithms::trianAlg(Tensor<double>& cof) {
  onesSet(cof, 0);
  for (size_t i(0); i < cof.colls() - 1; ++i) {
    if (!cof(i, i)) searchAndSwap(cof, i);
    for (size_t j(0); j < cof.colls() - 1; ++j) {
      if (j != i) {
        double coef(cof(j, i) / cof(i, i));
        for (size_t k(0); k < cof.colls(); ++k) {
          cof(j, k) -= cof(i, k) * coef;
        }
      }
    }
    onesSet(cof, i);
  }
}

void SlauAlgorithms::trianAlgThread(Tensor<double>& cof) {
  std::mutex mut;
  onesSet(cof, 0);
  for (size_t i(0); i < cof.colls() - 1; ++i) {
    if (!cof(i, i)) searchAndSwap(cof, i);
    threadFirst(i, cof);
    onesSet(cof, i);
  }
}

void SlauAlgorithms::threadFirst(const size_t i, Tensor<double>& cof) {
  size_t j(0);
  std::mutex mut;
  std::thread th1(threadSecond, std::ref(j), i, std::ref(mut), std::ref(cof));
  std::thread th2(threadSecond, std::ref(j), i, std::ref(mut), std::ref(cof));
  std::thread th3(threadSecond, std::ref(j), i, std::ref(mut), std::ref(cof));
  std::thread th4(threadSecond, std::ref(j), i, std::ref(mut), std::ref(cof));
  std::thread th5(threadSecond, std::ref(j), i, std::ref(mut), std::ref(cof));
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  th5.join();
}

void SlauAlgorithms::threadSecond(size_t& j, const size_t i, std::mutex& mut,
                                  Tensor<double>& cof) {
  while (j < cof.colls() - 1) {
    mut.lock();
    size_t jj(j);
    ++j;
    mut.unlock();
    if (jj != i && jj < cof.colls() - 1) {
      double coef(cof(jj, i) / cof(i, i));
      for (size_t k(0); k < cof.colls(); ++k) {
        cof(jj, k) -= cof(i, k) * coef;
      }
    }
  }
}

void SlauAlgorithms::onesSet(Tensor<double>& cof, const size_t it) {
  double coef(1. / cof(it, it));
  for (size_t i(0); i < cof.colls(); ++i) {
    cof(it, i) *= coef;
  }
}

Tensor<double> SlauAlgorithms::calculateResult(Tensor<double>& cof) {
  Tensor<double> result(cof.colls() - 1);
  for (size_t i(0); i < cof.rows(); ++i) {
    result[i] = cof(i, -1);
  }
  return result;
}

};  // namespace s21