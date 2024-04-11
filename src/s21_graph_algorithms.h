#ifndef S21_GRAPH_ALGORITHMS_H
#define S21_GRAPH_ALGORITHMS_H

#include <list>
#include <mutex>
#include <thread>

#include "s21_graph.h"
#include "s21_stack.h"

namespace s21 {
struct TsmResult {
  Tensor<unsigned> vertices;
  unsigned distance;
};

class GraphAlgorithms {
 public:
  GraphAlgorithms() = default;
  ~GraphAlgorithms() = default;

  static std::pair<bool, TsmResult> solveTravelingSalesmanProblem(const Graph&);
  static std::pair<bool, TsmResult> solveTravelingSalesmanProblemThread(
      const Graph&);

 private:
  class AntColony;
};

class GraphAlgorithms::AntColony {
 public:
  struct Ant {
    Ant();
    Ant(const unsigned);
    Ant(const Ant&);
    TsmResult path;
    bool death;

    void nextVertices(const unsigned, const unsigned);
  };
  AntColony(const Graph&, const unsigned, const unsigned, const unsigned, Ant,
            Tensor<int>* mapVertices = nullptr,
            std::vector<TsmResult>* pathDeadlock = nullptr);
  ~AntColony() = default;

  TsmResult makePatch();
  TsmResult makePatchThread();

 private:
  std::mutex mut_;
  unsigned currentPlace_;
  unsigned startPlace_;
  unsigned backPlace_;
  const Graph& graph_;
  unsigned sizeGraph_;
  s21::Stack<Ant> ants_;
  Ant queen_;
  std::list<unsigned> vertices_;
  Tensor<int>* mapVertices_;
  std::vector<TsmResult>* pathDeadlock_;

  void antCrusade();
  void exitDeadlock();
  unsigned numberEdges(const unsigned);
  void sumTsm(TsmResult& thise, const TsmResult);
  bool checkIsFullPath(const Tensor<unsigned>&) const;
  void colonyNotDead(TsmResult&);
  void colonyNotDeadThread(TsmResult&);
  void colonyDead(TsmResult&);
};
};  // namespace s21

#endif  // S21_GRAPH_ALGORITHMS_H
