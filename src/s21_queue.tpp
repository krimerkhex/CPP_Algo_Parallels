#include "s21_queue.h"

namespace s21 {

template <typename T>
Queue<T>::Queue() {
  this->first_ = nullptr;
  this->last_ = nullptr;
  this->size_ = 0;
}

template <typename T>
Queue<T>::Queue(std::initializer_list<value_type> const &items) : Queue() {
  for (auto it = items.begin(); it != items.end(); it++) {
    this->push(*it);
  }
}

template <typename T>
Queue<T>::Queue(Queue const &other) : Queue() {
  this->QueueCopy(other);
}

template <typename T>
Queue<T>::Queue(Queue &&other) {
  if (this != &other) {
    this->first_ = other.first_;
    this->last_ = other.last_;
    this->size_ = other.size_;
    other.first_ = other.last_ = nullptr;
    other.size_ = 0;
  }
}

template <typename T>
Queue<T>::Queue(const Queue &&other) {
  if (this != &other) {
    this->first_ = other.first_;
    this->last_ = other.last_;
    this->size_ = other.size_;
  }
}

template <typename T>
Queue<T>::~Queue() {
  this->clear();
  this->first_ = this->last_ = nullptr;
}

template <typename T>
bool Queue<T>::operator==(const Queue &other) const {
  return identity(other);
}

template <typename T>
bool Queue<T>::operator!=(const Queue &other) const {
  return !identity(other);
}

template <typename T>
bool Queue<T>::identity(const Queue &other) const {
  bool eq = false;
  if (this->size_ == other.size_) {
    if (this->first_ && this->last_ && other.first_ && other.last_) {
      eq = true;
      Nodes *ptr_this = this->first_;
      Nodes *ptr_other = other.first_;
      while (eq && ptr_this && ptr_other) {
        if (ptr_this->data_ != ptr_other->data_) {
          eq = false;
        } else {
          ptr_this = ptr_this->next_;
          ptr_other = ptr_other->next_;
        }
      }
    }
  }
  return eq;
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::front() {
  if (this->first_ == nullptr) {
    throw std::out_of_range("front: no any data");
  }
  return this->first_->data_;
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::back() {
  if (this->last_ == nullptr) {
    throw std::out_of_range("back: no any data");
  }
  return this->last_->data_;
}

template <typename T>
bool Queue<T>::empty() const {
  return this->first_ == nullptr;
}

template <typename T>
typename Queue<T>::size_type Queue<T>::size() const {
  return this->size_;
}

template <typename T>
void Queue<T>::clear() {
  while (this->size_) {
    this->pop();
  }
}

template <typename T>
void Queue<T>::push(const_reference value) {
  auto *temp = new s21::ConNode<T>(value);
  if (this->first_ == this->last_ && !this->size_) {
    this->first_ = this->last_ = temp;
  } else {
    this->last_->next_ = temp;
    this->last_ = this->last_->next_;
  }
  this->size_ += 1;
}

template <typename T>
typename Queue<T>::value_type Queue<T>::pop() {
  value_type result = this->first_->data_;
  if (this->size_) {
    auto ptr = this->first_->next_;
    delete this->first_;
    this->first_ = ptr;
    this->size_ -= 1;
  }
  if (!this->size_) {
    this->first_ = this->last_ = nullptr;
  }
  return result;
}

template <typename T>
typename Queue<T>::value_type Queue<T>::top() const {
  return this->first_->data_;
}

template <typename T>
void Queue<T>::swap(Queue &other) {
  auto size = this->size_;
  auto ptr = this->first_;

  this->size_ = other.size_;
  other.size_ = size;

  this->first_ = other.first_;
  other.first_ = ptr;

  ptr = this->last_;

  this->last_ = other.last_;
  other.last_ = ptr;
}

template <typename T>
Queue<T> &Queue<T>::operator=(const Queue &other) {
  this->QueueCopy(other);
  return *this;
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue &&other) {
  if (this != &other) {
    if (this->first_ != nullptr) {
      this->clear();
    }
    this->first_ = other.first_;
    this->last_ = other.last_;
    this->size_ = other.size_;
    other.first_ = other.last_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <typename T>
Queue<T> &Queue<T>::operator=(const Queue &&other) {
  if (this != &other) {
    if (this->first_ != nullptr) {
      this->clear();
    }
    this->first_ = other.first_;
    this->last_ = other.last_;
    this->size_ = other.size_;
  }
  return *this;
}

template <typename T>
typename Queue<T>::Nodes *Queue<T>::getFirst() {
  return this->first_;
}

template <typename T>
typename Queue<T>::Nodes *Queue<T>::getLast() {
  return this->last_;
}

template <typename T>
void Queue<T>::setFirst(Nodes *ptr) {
  if (ptr != nullptr) {
    ptr->next_ = this->first_->next_;
    delete this->first_;
    this->first_ = ptr;
  }
}

template <typename T>
void Queue<T>::setLast(Nodes *ptr) {
  if (ptr != nullptr) {
    auto temp = this->first_;
    for (size_t i = 0; i < size_ && temp->next_ != this->last_; ++i) {
      temp = temp->next_;
    }
    this->pop();
    temp->next_ = ptr;
    this->last_ = ptr;
  }
}

template <typename T>
void Queue<T>::setSize(size_type size) {
  if (size > 0) {
    if (size > this->size_) {
      for (size_t i = 0; i < size - this->size_; ++i) {
        this->push(0);
      }
    } else {
      for (size_t i = 0; i < this->size_ - size; ++i) {
        this->pop();
      }
    }
  }
}

template <typename T>
void Queue<T>::QueueCopy(const Queue &other) {
  this->clear();
  auto ptr = other.first_;
  while (ptr != nullptr) {
    this->push(ptr->data_);
    ptr = ptr->next_;
  }
}

template <typename T>
template <class... Args>
void Queue<T>::emplace_back(Args &&...args) {
  push(T(std::forward<Args>(args)...));
}
};  // namespace s21