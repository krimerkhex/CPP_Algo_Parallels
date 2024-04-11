#ifndef S21_GRAPH_H
#define S21_GRAPH_H

#include <climits>
#include <fstream>
#include <sstream>
#include <string>

#include "s21_tensor.h"

namespace s21 {

class Graph {
 public:
  Graph();
  Graph(const Graph&);
  Graph(Graph&&);
  Graph(const std::string&);
  const Graph& operator=(const Graph&);
  const Graph& operator=(Graph&&) noexcept;
  ~Graph();

  size_t size() const;
  bool empty() const;
  unsigned& operator()(const size_t, const size_t);
  unsigned operator()(const size_t, const size_t) const;

  void loadGraphFromFile(const std::string&);
  void exportGraphToDot(const std::string&) const;
  void create(const int, const size_t);

  friend std::ostream& operator<<(std::ostream& out, const Graph& other) {
    out << other.data_;
    return out;
  }

 private:
  Tensor<unsigned> data_;

  size_t parsNum(size_t&, const std::string&, bool&);
};
};  // namespace s21

#endif  // S21_GRAPH_H