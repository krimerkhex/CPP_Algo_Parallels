#include "s21_tensor.h"

namespace s21 {
template <class T>
Tensor<T>::Tensor()
    : rows_(0), collumns_(0), size_(rows_ * collumns_), data_(nullptr) {
  ;
}

template <class T>
Tensor<T>::Tensor(const size_t rows, const size_t collumns)
    : rows_(rows), collumns_(collumns), size_(rows * collumns) {
  if (!rows_ || !collumns_) {
    rows_ = 0;
    collumns_ = 0;
    size_ = 0;
    data_ = nullptr;
  } else {
    newData(data_, size_);
  }
}

template <class T>
Tensor<T>::Tensor(const size_t collumns) : Tensor(1, collumns) {}

template <class T>
Tensor<T>::Tensor(const Tensor& other) : Tensor() {
  if (data_ != other.data_ && other.data_ != nullptr) {
    rows_ = other.rows_;
    collumns_ = other.collumns_;
    size_ = other.size_;
    newData(data_, size_);
    copyData(data_, size_, other.data_, size_);
  }
}

template <class T>
Tensor<T>::Tensor(Tensor&& other) noexcept : Tensor() {
  if (data_ != other.data_) {
    rows_ = other.rows_;
    collumns_ = other.collumns_;
    size_ = other.size_;
    data_ = other.data_;
    other.collumns_ = other.rows_ = other.size_ = 0;
    other.data_ = nullptr;
  }
}

template <class T>
Tensor<T>::~Tensor() {
  clear();
}

template <class T>
const typename Tensor<T>::Tensor& Tensor<T>::operator=(const Tensor& other) {
  if (data_ != other.data_ && other.data_ != nullptr) {
    if (size_ != other.size_) {
      if (size_) clearData(data_);
      newData(data_, other.size_);
    }
    rows_ = other.rows_;
    collumns_ = other.collumns_;
    size_ = other.size_;
    copyData(data_, size_, other.data_, other.size_);
  }
  return *this;
}

template <class T>
const typename Tensor<T>::Tensor& Tensor<T>::operator=(
    Tensor&& other) noexcept {
  if (data_ != other.data_ && other.data_ != nullptr) {
    clearData(data_);
    rows_ = other.rows_;
    collumns_ = other.collumns_;
    size_ = other.size_;
    data_ = other.data_;
    other.collumns_ = other.rows_ = other.size_ = 0;
    other.data_ = nullptr;
  }
  return *this;
}

template <class T>
void Tensor<T>::pushBack(const value_type value) {
  if (!size_) {
    rows_ = collumns_ = size_ = 1;
    newData(data_, size_);
    data_[0] = value;
  } else if (rows_ == 1) {
    data_type tmp(data_);
    data_ = nullptr;
    collumns_ = size_ += 1;
    newData(data_, size_);
    copyData(data_, size_, tmp, size_ - 1);
    clearData(tmp);
    data_[size_ - 1] = value;
  } else {
    std::cout << "Error, tensor must be a vector" << std::endl;
  }
}

template <class T>
void Tensor<T>::pushBack(const Tensor& other) {
  if (other.rows_ == 1) {
    if (!size_) {
      *this = other;
    } else if (rows_ == 1) {
      data_type tmp(data_);
      data_ = nullptr;
      newData(data_, size_ + other.size_);
      copyData(data_, size_ + other.size_, tmp, size_);
      for (size_t i(0), j(size_); i < other.size_; ++i, ++j) {
        data_[j] = other.data_[i];
      }
      collumns_ = size_ += other.size_;
    } else {
      std::cout << "Error, tensor must be a vector" << std::endl;
    }
  } else {
    std::cout << "Error, tensor must be a vector" << std::endl;
  }
}

template <class T>
void Tensor<T>::pushBackM(const Tensor& other, const bool axis) {
  if (other.rows_ == 1) {
    if (!size_) {
      *this = other;
    } else if (collumns_ == other.collumns_ && !axis) {
      data_type tmp(data_);
      data_ = nullptr;
      rows_ += 1;
      size_ = rows_ * collumns_;
      newData(data_, size_);
      for (size_t i(0); i < rows_ - 1; ++i) {
        for (size_t j(0); j < collumns_; ++j) {
          data_[indexEs(i, j, collumns_)] = tmp[indexEs(i, j, collumns_)];
        }
      }
      for (size_t i(0); i < collumns_; ++i) {
        data_[indexEs(rows_ - 1, i, collumns_)] = other[i];
      }
      clearData(tmp);
    } else if (rows_ == other.collumns_ && axis) {
      data_type tmp(data_);
      data_ = nullptr;
      collumns_ += 1;
      size_ = rows_ * collumns_;
      newData(data_, size_);
      for (size_t i(0); i < rows_; ++i) {
        for (size_t j(0); j < collumns_ - 1; ++j) {
          data_[indexEs(i, j, collumns_)] = tmp[indexEs(i, j, collumns_ - 1)];
        }
        data_[indexEs(i, collumns_ - 1, collumns_)] = other[i];
      }
      clearData(tmp);
    } else {
      std::cout << "Error, tensor must be a vector" << std::endl;
    }
  } else {
    std::cout << "Error, tensor must be a vector" << std::endl;
  }
}

template <class T>
void Tensor<T>::operator()(const size_t indx, const Tensor& other,
                           const bool axis) {
  if (!axis) {
    if (rows_ > 1 && indx < rows_ && other.rows_ == 1 &&
        collumns_ == other.collumns_) {
      for (size_t i(0); i < collumns_; ++i) {
        data_[indexEs(indx, i, collumns_)] = other.data_[i];
      }
    } else {
      std::cout << "Error ()\n";
    }
  } else {
    if (collumns_ > 1 && indx < collumns_ && other.rows_ == 1 &&
        rows_ == other.collumns_) {
      for (size_t i(0); i < rows_; ++i) {
        data_[indexEs(i, indx, collumns_)] = other.data_[i];
      }
    } else {
      std::cout << "Error ()\n";
    }
  }
}

template <class T>
void Tensor<T>::clear() {
  clearData(data_);
  rows_ = collumns_ = size_ = 0;
}

template <class T>
typename Tensor<T>::value_type& Tensor<T>::operator()(long long rows,
                                                      long long collumns) {
  if (rows < 0) rows += rows_;
  if (collumns < 0) collumns += collumns_;
  return data_[indexEs(rows, collumns, collumns_)];
}

template <class T>
typename Tensor<T>::value_type& Tensor<T>::operator[](long long collumns) {
  if (collumns < 0) collumns += collumns_;
  return data_[collumns];
}

template <class T>
typename Tensor<T>::value_type Tensor<T>::operator()(long long rows,
                                                     long long collumns) const {
  if (rows < 0) rows += rows_;
  if (collumns < 0) collumns += collumns_;
  return data_[indexEs(rows, collumns, collumns_)];
}

template <class T>
typename Tensor<T>::value_type Tensor<T>::operator[](long long collumns) const {
  if (collumns < 0) collumns += collumns_;
  return data_[collumns];
}

template <class T>
size_t Tensor<T>::rows() const {
  return rows_;
}

template <class T>
size_t Tensor<T>::colls() const {
  return collumns_;
}

template <class T>
size_t Tensor<T>::size() const {
  return size_;
}

template <class T>
bool Tensor<T>::empty() const {
  return size_ ? 0 : 1;
}

template <class T>
size_t Tensor<T>::maxIndx(const size_t indx, const size_t axis) const {
  value_type max(0);
  size_t result(0);
  if (size_) {
    size_t x = axis ? rows_ : collumns_;
    for (size_t i(0); i < x; ++i) {
      size_t j(0), k(0);
      if (!axis) {
        j = indx;
        k = i;
      } else {
        j = i;
        k = indx;
      }
      value_type t(data_[indexEs(j, k, collumns_)]);
      if (t > max) {
        max = t;
        result = i;
      }
    }
  }
  return result;
}

template <class T>
size_t Tensor<T>::maxIndx() const {
  value_type max(data_[0]);
  size_t result(0);
  if (size_ && rows_ == 1) {
    for (size_t i(0); i < size_; ++i)
      if (max < data_[i]) {
        max = data_[i];
        result = i;
      }
  }
  return result;
}

template <class T>
bool Tensor<T>::contain(const value_type value) const {
  bool result(false);
  for (size_t i(0); i < size_ && !result; ++i) {
    result = data_[i] == value;
  }
  return result;
}

template <class T>
typename Tensor<T>::value_type Tensor<T>::front() const {
  return size_ ? data_[0] : value_type();
}

template <class T>
typename Tensor<T>::value_type Tensor<T>::back() const {
  return size_ ? data_[size_ - 1] : value_type();
}

template <class T>
void Tensor<T>::resize(const size_t rows, const size_t collumns) {
  if (rows && collumns) {
    rows_ = rows;
    collumns_ = collumns;
    if (!size_) {
      size_ = rows_ * collumns_;
      newData(data_, size_);
    } else if (size_ != (rows * collumns)) {
      data_type temp(data_);
      data_ = nullptr;
      newData(data_, (rows * collumns));
      copyData(data_, (rows * collumns), temp, size_);
      size_ = rows * collumns;
      clearData(temp);
    }
  }
}

template <class T>
void Tensor<T>::resize(const size_t collumns) {
  if (collumns) {
    rows_ = 1;
    collumns_ = collumns;
    if (size_ != collumns) {
      data_type temp(data_);
      data_ = nullptr;
      newData(data_, collumns);
      copyData(data_, collumns, temp, size_);
      size_ = collumns;
      clearData(temp);
    }
  }
}

template <class T>
void Tensor<T>::transposeSet() {
  if (size_) {
    for (size_t i(0); i < rows_; ++i) {
      for (size_t j(0); j < collumns_; ++j) {
        value_type t(data_[indexEs(i, j, collumns_)]);
        data_[indexEs(i, j, collumns_)] = data_[indexEs(j, i, rows_)];
        data_[indexEs(j, i, rows_)] = t;
      }
    }
    size_t t(rows_);
    rows_ = collumns_;
    collumns_ = t;
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::transpose() const {
  Tensor result(*this);
  result.transposeSet();
  return result;
}

template <class T>
void Tensor<T>::reverseSet() {
  if (size_ && rows_ == 1) {
    data_type tmp(data_);
    data_ = nullptr;
    newData(data_, size_);
    for (size_t i(0); i < size_; ++i) {
      data_[(size_ - 1) - i] = tmp[i];
    }
    clearData(tmp);
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::reverse() const {
  Tensor result(*this);
  result.reverseSet();
  return result;
}

template <class T>
void Tensor<T>::operator-=(const Tensor& other) {
  if (size_ && rows_ == other.rows_ && collumns_ == other.collumns_) {
    for (size_t i(0); i < size_; ++i) data_[i] -= other.data_[i];
  } else {
    std::cout << "Error minus shape 1: " << rows_ << " " << collumns_
              << "\nshape 2: " << other.rows_ << " " << other.collumns_ << "\n";
  }
}

template <class T>
void Tensor<T>::operator-=(const value_type value) {
  if (size_) {
    for (size_t i(0); i < rows_; ++i)
      for (size_t j(0); j < collumns_; ++j)
        data_[indexEs(i, j, collumns_)] -= value;
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator-(const Tensor& other) const {
  Tensor result(*this);
  result -= other;
  return result;
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator-(const value_type value) const {
  Tensor result(*this);
  result -= value;
  return result;
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator-() const {
  Tensor result(*this);
  result *= -1;
  return result;
}

template <class T>
void Tensor<T>::operator+=(const Tensor& other) {
  if (size_ && rows_ == other.rows_ && collumns_ == other.collumns_) {
    for (size_t i(0); i < size_; ++i) data_[i] += other.data_[i];
  } else {
    std::cout << "Error sum shape 1: " << rows_ << " " << collumns_
              << "\nshape 2: " << other.rows_ << " " << other.collumns_ << "\n";
  }
}

template <class T>
void Tensor<T>::operator+=(const value_type value) {
  if (size_) {
    for (size_t i(0); i < rows_; ++i)
      for (size_t j(0); j < collumns_; ++j)
        data_[indexEs(i, j, collumns_)] += value;
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator+(const Tensor& other) const {
  Tensor result(*this);
  result += other;
  return result;
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator+(const value_type value) const {
  Tensor result(*this);
  result += value;
  return result;
}

template <class T>
void Tensor<T>::operator*=(const value_type value) {
  for (size_t i(0); i < size_; ++i) data_[i] *= value;
}

template <class T>
void Tensor<T>::operator*=(const Tensor& other) {
  if ((size_ && rows_ == 1 && other.rows_ == 1 &&
       collumns_ == other.collumns_)) {
    for (size_t i(0); i < size_; ++i) data_[i] *= other.data_[i];
  } else if (size_ && collumns_ == other.rows_) {
    data_type temp(data_);
    size_t a(rows_), b(collumns_), c(other.collumns_);
    data_ = nullptr;
    size_t sizeI(a * c);
    newData(data_, sizeI);
    for (size_t i(0); i < sizeI; ++i)
      for (size_t j(0); j < b; ++j) {
        data_[indexEs(i / c, i % c, c)] +=
            temp[indexEs(i / c, j, b)] * other.data_[indexEs(j, i % c, c)];
      }
    clearData(temp);
    temp = nullptr;
    collumns_ = c;
    size_ = rows_ * collumns_;
  } else {
    std::cout << "Error mul shape 1: " << rows_ << " " << collumns_
              << "\nshape 2: " << other.rows_ << " " << other.collumns_ << "\n";
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator*(const Tensor& other) const {
  Tensor result(*this);
  result *= other;
  return result;
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator*(const value_type value) const {
  Tensor result(*this);
  result *= value;
  return result;
}

template <class T>
void Tensor<T>::operator/=(const value_type value) {
  for (size_t i(0); i < size_; ++i) data_[i] /= value;
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::operator/(const value_type value) const {
  Tensor result(*this);
  result /= value;
  return result;
}

template <class T>
void Tensor<T>::powSet(const size_t x) {
  if (size_) {
    for (size_t i(0); i < size_; ++i) data_[i] = pow(data_[i], x);
  } else {
    std::cout << "Error pow\n";
  }
}

template <class T>
typename Tensor<T>::value_type Tensor<T>::sum() const {
  value_type result(0);
  for (size_t i(0); i < size_; ++i) result += data_[i];
  return result;
}

template <class T>
void Tensor<T>::sumMSet() {
  if (size_) {
    data_type temp(data_);
    data_ = nullptr;
    newData(data_, rows_);
    for (size_t i(0); i < rows_; ++i)
      for (size_t j(0); j < collumns_; ++j)
        data_[i] += temp[indexEs(i, j, collumns_)];
    clearData(temp);
    temp = nullptr;
    collumns_ = 1;
    size_ = rows_;
  } else {
    std::cout << "Error sum M\n";
  }
}

template <class T>
void Tensor<T>::noise(const size_t max) {
  srand(static_cast<unsigned>(time(0)));
  for (size_t i(0); i < rows_; ++i) {
    for (size_t j(0); j < collumns_; ++j) {
      data_[indexEs(i, j, collumns_)] = rand() % max;
    }
  }
}

template <class T>
void Tensor<T>::noiseDouble(const bool flag) {
  srand(static_cast<unsigned>(time(0)));
  for (size_t i(0); i < size_; ++i) {
    if (flag)
      data_[i] = (double)((rand() % 100) * 0.03) / (rows_ + 35);
    else
      data_[i] = (double)((rand() % 50) * 0.06) / (collumns_ + 35);
  }
}

template <class T>
void Tensor<T>::noiseGraph(const size_t max) {
  srand(static_cast<unsigned>(time(0)));
  for (size_t i(0); i < rows_; ++i) {
    for (size_t j(0); j < collumns_; ++j) {
      if (i != j && !data_[indexEs(i, j, collumns_)]) {
        value_type r(rand() % max);
        data_[indexEs(i, j, collumns_)] = r;
        data_[indexEs(j, i, collumns_)] = r;
      }
    }
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::activation(
    const std::function<double(double)>& func) {
  Tensor result(*this);
  result.activationSet(func);
  return result;
}

template <class T>
void Tensor<T>::activationSet(const std::function<double(double)>& func) {
  if (size_) {
    for (size_t i(0); i < size_; ++i) data_[i] = func(data_[i]);
  } else {
    std::cout << "Error activation\n";
  }
}

template <class T>
typename Tensor<T>::Tensor Tensor<T>::slice(long long start, long long end,
                                            size_t step) const {
  Tensor<T> result;
  if (rows_ > 1) {
    checkStartEnd(start, end, step, rows_);
    size_t i(start), count(0);
    for (; static_cast<long long>(i) < end; i += step, ++count)
      ;
    data_type res;
    newData(res, count * collumns_);
    result.rows_ = count;
    result.collumns_ = collumns_;
    result.size_ = count * collumns_;
    count = 0;
    i = start;
    while (static_cast<long long>(i) < end) {
      size_t j(0);
      while (j < collumns_) res[count++] = data_[indexEs(i, j++, collumns_)];
      i += step;
    }
    result.data_ = res;
    res = nullptr;
  } else {
    checkStartEnd(start, end, step, size_);
    size_t i(start), count(0);
    for (; static_cast<long long>(i) < end; i += step, ++count)
      ;
    data_type res;
    newData(res, count);
    result.rows_ = 1;
    result.collumns_ = count;
    result.size_ = count;
    count = 0;
    i = start;
    while (static_cast<long long>(i) < end) res[count++] = data_[i++];
    result.data_ = res;
    res = nullptr;
  }
  return result;
}

template <class T>
void Tensor<T>::checkStartEnd(long long& start, long long& end, size_t& step,
                              const size_t size) const {
  if (!end) end = size;
  if (end < 0) end += size;
  if (start < 0) start += size;
  if (end > (static_cast<long long>(size) - 1)) end -= size;
  if (start > (static_cast<long long>(size) - 1)) start -= size;
  if (start > end) {
    long int t(start);
    start = end;
    end = t;
  }
  if (start == end) ++end;
  if (end - start < static_cast<long long>(step)) step = end - start;
}

template <class T>
inline size_t Tensor<T>::indexEs(const size_t rows, const size_t collumns,
                                 const size_t coll) const {
  return (rows * coll) + collumns;
}

template <class T>
void Tensor<T>::newData(data_type& data, const size_t size) const {
  if (size) data = new value_type[size]{0};
}

template <class T>
void Tensor<T>::clearData(data_type& data) {
  if (data != nullptr) {
    delete[] data;
    data = nullptr;
  }
}

template <class T>
void Tensor<T>::copyData(data_type& data, const size_t size,
                         const data_type& otherData, const size_t otherSize) {
  size_t size_i(size < otherSize ? size : otherSize);
  for (size_t i(0); i < size_i; ++i) data[i] = otherData[i];
}

template <class T>
std::string Tensor<T>::toString() {
  std::ostringstream out;
  out << rows_ << "," << collumns_ << ",";
  for (size_t i(0); i < size_; ++i) {
    out << data_[i];
    if (i < size_ - 1) out << ",";
  }
  return out.str();
}

template <class T>
void Tensor<T>::fromString(const std::string& str) {
  if (size_) {
    clearData(data_);
    rows_ = collumns_ = size_ = 0;
  }
  std::string::const_iterator i(str.begin());
  std::string::const_iterator back(str.end());
  rows_ = std::stoull(&(*i));
  nextNum(i, back);
  collumns_ = std::stoull(&(*i));
  nextNum(i, back);
  size_ = rows_ * collumns_;
  newData(data_, size_);
  for (size_t j(0); j < size_; ++j) {
    data_[j] = std::stold(&(*i));
    nextNum(i, back);
  }
}

template <class T>
void Tensor<T>::nextNum(std::string::const_iterator& i,
                        const std::string::const_iterator& back) {
  for (; i != back && *i != ','; ++i) {
  }
  if (i != back) ++i;
}
};  // namespace s21