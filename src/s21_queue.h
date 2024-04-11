#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <initializer_list>
#include <iostream>
#include <sstream>

namespace s21 {
template <typename T>
class ConNode {
 public:
  ConNode<T> *next_;
  T data_;
  ConNode<T> *getNext() { return next_; }
  ConNode(T data = T()) : next_(nullptr), data_(data) {}
  ~ConNode() = default;
};

template <typename T>
class Queue {
 public:
  class ConNode;
  using value_type = T;
  using size_type = size_t;
  using Nodes = s21::ConNode<T>;
  using reference = T &;
  using const_reference = const T &;

  Queue();
  Queue(std::initializer_list<value_type> const &items);
  Queue(const Queue &other);
  Queue(Queue &&other);
  Queue(const Queue &&other);
  ~Queue();
  Queue<T> &operator=(const Queue &other);
  Queue<T> &operator=(Queue &&other);
  Queue<T> &operator=(const Queue &&other);
  bool operator==(const Queue &other) const;
  bool operator!=(const Queue &other) const;
  const_reference front();
  const_reference back();
  bool empty() const;
  size_type size() const;
  void push(const_reference value);
  value_type pop();
  value_type top() const;
  void swap(Queue &other);
  void clear();
  Nodes *getFirst();
  Nodes *getLast();
  void setFirst(Nodes *ptr);
  void setLast(Nodes *ptr);
  void setSize(size_type size);
  template <class... Args>
  void emplace_back(Args &&...args);

  friend std::ostream &operator<<(std::ostream &out, const Queue<T> &other) {
    Nodes *f = other.first_;
    for (size_type i = 0; i < other.size_; ++i) {
      out << f->data_;
      f = f->next_;
      if (i < other.size_ - 1) out << ", ";
    }
    return out;
  }

 private:
  Nodes *first_;
  Nodes *last_;
  size_type size_;
  void QueueCopy(const Queue &other);
  bool identity(const Queue &other) const;
};
};  // namespace s21
#include "s21_queue.tpp"

#endif  // S21_QUEUE_H
