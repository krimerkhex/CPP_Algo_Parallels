#include "s21_graph.h"

namespace s21 {
Graph::Graph() { ; }

Graph::Graph(const Graph& other) : data_(other.data_) { ; }

Graph::Graph(Graph&& other) : data_(std::move(other.data_)) { ; }

Graph::Graph(const std::string& filename) { loadGraphFromFile(filename); }

Graph::~Graph() { ; }

const typename Graph::Graph& Graph::operator=(const Graph& other) {
  if (this != &other) {
    data_ = other.data_;
  }
  return *this;
}

const typename Graph::Graph& Graph::operator=(Graph&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
  }
  return *this;
}

size_t Graph::size() const { return data_.rows(); }

bool Graph::empty() const { return data_.empty(); }

unsigned& Graph::operator()(const size_t n, const size_t m) {
  return data_(n, m);
}

unsigned Graph::operator()(const size_t n, const size_t m) const {
  return data_(n, m);
}

void Graph::loadGraphFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (file.is_open()) {
    bool error(false);
    std::string line;
    std::getline(file, line);
    size_t size(0);
    try {
      size = std::stoull(line);
    } catch (const std::exception&) {
      error = true;
    }
    data_.resize(size, size);
    for (size_t i(0); std::getline(file, line) && i < size && !error; ++i) {
      if (line.size() > 1) {
        for (size_t j(0), it = 0; it < line.size() && j < size && !error; ++j) {
          data_(i, j) = parsNum(it, line, error);
        }
      }
    }
    if (error) data_.clear();
  }
  file.close();
}

void Graph::exportGraphToDot(const std::string& filename) const {
  std::ofstream file(filename);
  if (file.is_open()) {
    std::ostringstream out;
    out << "Graph " + filename + " {" << std::endl;
    for (size_t i(0); i < data_.rows(); ++i) {
      for (size_t j(0); j < data_.colls(); ++j) {
        if (data_(i, j) != 0 || data_(j, i) != 0) {
          out << "  ";
          if (data_(i, j) == data_(j, i)) {
            out << std::to_string(i + 1) + " -- " + std::to_string(j + 1)
                << std::endl;
          } else {
            out << std::to_string(std::min(i, j) + 1) + " -> " +
                       std::to_string(std::max(i, j) + 1)
                << std::endl;
          }
        }
      }
    }
    out << "}" << std::endl;
    file << out.str();
  }
  file.close();
}

void Graph::create(const int number, const size_t max) {
  if (number > 0) {
    data_.resize(number, number);
    data_.noiseGraph(max);
  }
}

size_t Graph::parsNum(size_t& it, const std::string& line, bool& error) {
  size_t result(0);
  if (line[it] >= '0' && line[it] <= '9') {
    result = std::stoull(&(line[it]));
    for (; (line[it] >= '0' && line[it] <= '9') && it < line.size(); ++it) {
    }
    for (; (line[it] == ' ' || line[it] == ',') && it < line.size(); ++it) {
    }
  } else {
    error = true;
  }
  return result;
}
};  // namespace s21