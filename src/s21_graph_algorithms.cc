#include "s21_graph_algorithms.h"

namespace s21 {

std::pair<bool, TsmResult> GraphAlgorithms::solveTravelingSalesmanProblem(
    const Graph& graph) {
  Tensor<int> mapVertices(graph.size());
  std::vector<TsmResult> pathDeadlock(graph.size());
  AntColony colony(graph, 0, 0, 0, AntColony::Ant(), &mapVertices,
                   &pathDeadlock);
  TsmResult result = colony.makePatch();
  if (!result.vertices.empty()) {
    result.vertices.reverseSet();
    result.vertices += 1;
  }
  bool error = !result.vertices.empty();
  return std::make_pair(error, result);
}

std::pair<bool, TsmResult> GraphAlgorithms::solveTravelingSalesmanProblemThread(
    const Graph& graph) {
  Tensor<int> mapVertices(graph.size());
  std::vector<TsmResult> pathDeadlock(graph.size());
  AntColony colony(graph, 0, 0, 0, AntColony::Ant(), &mapVertices,
                   &pathDeadlock);
  TsmResult result = colony.makePatchThread();
  if (!result.vertices.empty()) {
    result.vertices += 1;
  }
  bool error = !result.vertices.empty();
  return std::make_pair(error, result);
}

GraphAlgorithms::AntColony::AntColony(const Graph& Graph,
                                      const unsigned startPlace,
                                      const unsigned currentPlace,
                                      const unsigned backPlace, Ant queen,
                                      Tensor<int>* mapVertices,
                                      std::vector<TsmResult>* pathDeadlock)
    : currentPlace_(currentPlace),
      startPlace_(startPlace),
      backPlace_(backPlace),
      graph_(Graph),
      sizeGraph_(Graph.size()),
      queen_(queen),
      mapVertices_(mapVertices),
      pathDeadlock_(pathDeadlock) {
  if (queen_.path.vertices.empty()) queen_.death = true;
}

GraphAlgorithms::AntColony::Ant::Ant() : path(), death(false) {}

GraphAlgorithms::AntColony::Ant::Ant(const unsigned currentPlace) : Ant() {
  path.vertices.pushBack(currentPlace);
  path.distance = 0;
}

GraphAlgorithms::AntColony::Ant::Ant(const Ant& queen) : Ant() {
  path.vertices = queen.path.vertices;
  path.distance = queen.path.distance;
}

void GraphAlgorithms::AntColony::Ant::nextVertices(const unsigned nextPlace,
                                                   const unsigned distance) {
  path.vertices.pushBack(nextPlace);
  path.distance += distance;
}

TsmResult GraphAlgorithms::AntColony::makePatch() {
  TsmResult result;
  result.distance = INT_MAX;
  antCrusade();
  if (!ants_.empty()) {
    colonyNotDead(result);
  } else {
    colonyDead(result);
  }
  return result;
}

TsmResult GraphAlgorithms::AntColony::makePatchThread() {
  TsmResult result;
  result.distance = INT_MAX;
  antCrusade();
  if (!ants_.empty()) {
    colonyNotDeadThread(result);
  } else {
    colonyDead(result);
  }
  return result;
}

void GraphAlgorithms::AntColony::antCrusade() {
  if (numberEdges(currentPlace_) < 2 && currentPlace_ != startPlace_) {
    exitDeadlock();
    currentPlace_ = queen_.path.vertices.back();
  }
  for (size_t i(0); i < sizeGraph_; ++i) {
    if (graph_(currentPlace_, i) && (*mapVertices_)[i] >= 0) {
      if (!queen_.death) {
        ants_.push(Ant(queen_));
      } else {
        ants_.push(Ant(currentPlace_));
      }
      if (!ants_.top().path.vertices.contain(i)) {
        if ((*mapVertices_)[i] == 1) {
          sumTsm(ants_.top().path, (*pathDeadlock_)[i]);
        } else {
          ants_.top().nextVertices(i, graph_(currentPlace_, i));
        }
      } else {
        ants_.pop();
      }
    }
  }
}

void GraphAlgorithms::AntColony::exitDeadlock() {
  long i(3);
  (*mapVertices_)[currentPlace_] = -1;
  unsigned z(queen_.path.vertices[-2]);
  unsigned sumD(graph_(currentPlace_, z));
  queen_.nextVertices(z, sumD);
  while (numberEdges(z) < 3 && z != startPlace_) {
    (*mapVertices_)[z] = -1;
    unsigned g(queen_.path.vertices[-i]);
    unsigned d(graph_(z, g));
    z = g;
    queen_.nextVertices(g, d);
    i += 2;
    sumD += d;
  }
  sumD *= 2;
  Tensor<unsigned>& tmp(queen_.path.vertices);
  Tensor<unsigned> sliceT(tmp.slice(tmp.size() - i));
  if ((*mapVertices_)[z] == 1) {
    (*pathDeadlock_)[z].vertices.pushBack(sliceT);
    (*pathDeadlock_)[z].distance += sumD;
  } else {
    (*mapVertices_)[z] = 1;
    TsmResult t{sliceT, sumD};
    (*pathDeadlock_)[z] = t;
  }
}

unsigned GraphAlgorithms::AntColony::numberEdges(const unsigned n) {
  unsigned result(0);
  for (unsigned i(0); i<sizeGraph_; result += graph_(n, i++)> 0) {
  }
  return result;
}

void GraphAlgorithms::AntColony::sumTsm(TsmResult& thise,
                                        const TsmResult other) {
  thise.vertices.pushBack(other.vertices);
  thise.distance += other.distance;
}

bool GraphAlgorithms::AntColony::checkIsFullPath(
    const Tensor<unsigned>& path) const {
  bool result(true);
  for (unsigned i(0); i < sizeGraph_ && result; ++i) {
    result = path.contain(i);
  }
  return result;
}

void GraphAlgorithms::AntColony::colonyNotDead(TsmResult& result) {
  Stack<Ant>::Nodes* an(ants_.getFirst());
  while (an != nullptr) {
    Ant& at(an->data_);
    if (startPlace_ == at.path.vertices.back()) {
      if (result.distance > at.path.distance &&
          checkIsFullPath(at.path.vertices)) {
        result = at.path;
      }
      at.death = true;
    }
    if (!at.death) {
      TsmResult tmp(AntColony(graph_, startPlace_, at.path.vertices.back(),
                              currentPlace_, at, mapVertices_, pathDeadlock_)
                        .makePatch());
      if (result.distance > tmp.distance) result = tmp;
    }
    if (an != ants_.getLast())
      an = an->next_;
    else
      an = nullptr;
  }
}

void GraphAlgorithms::AntColony::colonyNotDeadThread(TsmResult& result) {
  Stack<Ant>::Nodes* an(ants_.getFirst());
  std::vector<std::thread> th;
  std::vector<TsmResult> tr;
  while (an != nullptr) {
    Ant& at(an->data_);
    if (startPlace_ == at.path.vertices.back()) {
      if (result.distance > at.path.distance &&
          checkIsFullPath(at.path.vertices)) {
        result = at.path;
      }
      at.death = true;
    }
    if (!at.death) {
      th.push_back(std::thread([&] {
        TsmResult res(AntColony(graph_, startPlace_, at.path.vertices.back(),
                                currentPlace_, at, mapVertices_, pathDeadlock_)
                          .makePatch());
        mut_.lock();
        tr.push_back(res);
        mut_.unlock();
      }));
    }
    if (an != ants_.getLast())
      an = an->next_;
    else
      an = nullptr;
  }
  for (unsigned i(0); i < th.size(); ++i) {
    th[i].join();
    mut_.lock();
    if (tr[i].distance < result.distance) result = tr[i];
    mut_.unlock();
  }
}

void GraphAlgorithms::AntColony::colonyDead(TsmResult& result) {
  if (!queen_.death && graph_(currentPlace_, startPlace_)) {
    queen_.nextVertices(startPlace_, graph_(currentPlace_, startPlace_));
    if (queen_.path.distance < result.distance &&
        checkIsFullPath(queen_.path.vertices)) {
      result = queen_.path;
    }
  }
}
};  // namespace s21