#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iostream>

#include "s21_queue.h"

namespace s21 {
template <typename T>
class Stack {
 public:
  class ConNode;
  using value_type = T;
  using size_type = size_t;
  using reference = T &;
  using const_reference = const T &;
  using Nodes = s21::ConNode<value_type>;

 public:
  Stack();
  ~Stack();
  Stack(std::initializer_list<value_type> const &items);
  Stack(Stack const &other);
  Stack(Stack &&other);
  Stack(const Stack &&other);
  Stack<T> &operator=(Stack const &other);
  Stack<T> &operator=(Stack &&other);
  Stack<T> &operator=(const Stack &&other);
  bool operator==(const Stack &other) const;
  bool operator!=(const Stack &other) const;
  const_reference front();
  const_reference back();
  bool empty() const;
  size_type size() const;
  void push(const_reference value);
  value_type pop();
  reference top();
  void swap(Stack &other);
  void clear();
  Nodes *getFirst();
  Nodes *getLast();
  void setFirst(Nodes *ptr);
  void setLast(Nodes *ptr);
  void setSize(size_type size);
  template <class... Args>
  void emplaceFront(Args &&...args);

  friend std::ostream &operator<<(std::ostream &out, const Stack<T> &other) {
    Nodes *f = other.first_;
    for (size_type i = 0; i < other.size_; ++i) {
      out << f->data_;
      f = f->next_;
      if (i < other.size_ - 1) out << ", ";
    }
    return out;
  }

 private:
  Nodes *last_;
  Nodes *first_;
  size_type size_;
  void stackCopy_(const Stack &other);
  void pushCopy(const_reference value);
  bool identity(const Stack &other) const;
};
};  // namespace s21

#include "s21_stack.tpp"

#endif  // S21_STACK_H
