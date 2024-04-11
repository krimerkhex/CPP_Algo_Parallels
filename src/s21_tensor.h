#ifndef S21_TENSOR_H
#define S21_TENSOR_H

#include <ctime>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

namespace s21 {
template <class T>
class Tensor {
 public:
  using value_type = T;
  using data_type = value_type*;

  Tensor();
  Tensor(const size_t, const size_t);
  Tensor(const size_t);
  Tensor(const Tensor&);
  Tensor(Tensor&&) noexcept;
  ~Tensor();

  const Tensor& operator=(const Tensor&);
  const Tensor& operator=(Tensor&&) noexcept;
  void pushBack(const value_type);
  void pushBack(const Tensor&);
  void pushBackM(const Tensor&, const bool axis = 0);
  void operator()(const size_t, const Tensor&, const bool);
  void clear();

  value_type& operator()(long long, long long);
  value_type& operator[](long long);
  value_type operator()(long long, long long) const;
  value_type operator[](long long) const;
  size_t rows() const;
  size_t colls() const;
  size_t size() const;
  bool empty() const;
  size_t maxIndx() const;
  size_t maxIndx(const size_t, const size_t axis = 0) const;
  bool contain(const value_type) const;
  value_type front() const;
  value_type back() const;

  void resize(const size_t, const size_t);
  void resize(const size_t);
  void transposeSet();
  Tensor transpose() const;
  void reverseSet();
  Tensor reverse() const;

  void operator-=(const Tensor&);
  void operator-=(const value_type);
  Tensor operator-(const Tensor&) const;
  Tensor operator-(const value_type) const;
  friend Tensor operator-(const value_type value, const Tensor& other) {
    Tensor result(other);
    if (other.size_) {
      for (size_t i(0); i < other.size_; ++i)
        other.data_[i] = value - other.data_[i];
    }
    return result;
  }
  Tensor operator-() const;
  void operator+=(const Tensor&);
  void operator+=(const value_type);
  Tensor operator+(const Tensor&) const;
  Tensor operator+(const value_type) const;
  void operator*=(const value_type);
  void operator*=(const Tensor&);
  Tensor operator*(const Tensor&) const;
  Tensor operator*(const value_type) const;
  void operator/=(const value_type);
  Tensor operator/(const value_type) const;
  friend Tensor operator/(const value_type value, const Tensor& other) {
    Tensor result(other);
    if (other.size_) {
      for (size_t i(0); i < other.size_; ++i)
        result.data_[i] = value / other.data_[i];
    }
    return std::move(result);
  }
  void powSet(const size_t);
  value_type sum() const;
  void sumMSet();

  void noise(const size_t);
  void noiseDouble(const bool);
  void noiseGraph(const size_t);
  Tensor activation(const std::function<double(double)>&);
  void activationSet(const std::function<double(double)>&);
  Tensor slice(long long start = 0, long long end = 0, size_t step = 1) const;

  friend std::ostream& operator<<(std::ostream& out, const Tensor& other) {
    if (other.size_) {
      out << "rows: " << other.rows_ << ", collumns: " << other.collumns_
          << "\n";
      for (size_t i(0); i < other.size_; ++i) {
        if (other.rows_ > 1 && !i) out << "[";
        if (!(i % other.collumns_)) out << "[";
        out << other.data_[i];
        if ((i % other.collumns_) < other.collumns_ - 1) {
          out << ", ";
        } else {
          out << "]";
          if (other.rows_ > 1 && i < other.size_ - 1) out << "\n";
        }
      }
      if (other.rows_ > 1) out << "]";
    }
    return out;
  }

  std::string toString();
  void fromString(const std::string&);

  void nextNum(std::string::const_iterator&,
               const std::string::const_iterator&);

 private:
  size_t rows_;
  size_t collumns_;
  size_t size_;
  data_type data_;

  void newData(data_type&, const size_t) const;
  void clearData(data_type&);
  void copyData(data_type&, const size_t, const data_type&, const size_t);
  void checkStartEnd(long long&, long long&, size_t&, const size_t) const;
  inline size_t indexEs(const size_t, const size_t, const size_t) const;
};
};  // namespace s21
#include "s21_tensor.tpp"

#endif  // S21_TENSOR_H