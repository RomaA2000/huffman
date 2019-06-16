//
// Created by Роман Агеев on 2019-06-07.
//

#ifndef HUFFMAN_BIT_SET_HPP
#define HUFFMAN_BIT_SET_HPP
#include <iostream>
#include <vector>
#include "../../additional/consts/consts.hpp"

template<typename T = uint8_t>
struct bit_set {
 public:
  typedef uint8_t bit;

  bit_set() : last_(0), data_() {}

  explicit bit_set(bit const &in) : last_(0) {
    data_.push_back(in);
  }

  bit_set(bit_set const &) = default;

  ~bit_set() = default;

  void concat(bit_set const &in) {
    if (!in.empty()) {
      if (empty()) {
        *this = in;
      } else {
        size_t start_size = data_.size();
        increase_to_size(in.data_.size());
        std::copy(in.data_.begin(), in.data_.end(), data_.begin() + start_size);
        if (last_ == 0) {
          last_ = in.last_;
        } else {
          data_[start_size - 1] |= data_[start_size] >> last_;
          for (size_t i = start_size; i < data_.size() - 1; ++i) {
            data_[i] = (data_[i + 1] >> last_) | (data_[i] << (huffman_consts::SIZE_OF_BIT - last_));
          }
          if ((in.last_ > (huffman_consts::SIZE_OF_BIT - last_)) || (in.last_ == 0)) {
            data_.back() <<= (huffman_consts::SIZE_OF_BIT - last_);
          } else {
            data_.pop_back();
          }
          last_ = (last_ + in.last_) % huffman_consts::SIZE_OF_BIT;
        }
      }
    }
  }

  void push_back(T in) {
    if (last_ == 0) {
      data_.push_back(0);
    }
    uint8_t temp = (huffman_consts::MAX_BIT - last_);
    if (in == 0) {
      data_.back() &= huffman_consts::MAX_BYTE - (1u << temp);
    } else {
      data_.back() |= (in << temp);
    }
    ++last_;
    last_ %= huffman_consts::SIZE_OF_BIT;
  }

  void pop_back() {
    if (last_ == 1) {
      data_.pop_back();
    }
    --last_;
    last_ %= huffman_consts::SIZE_OF_BIT;
  }

  size_t size() const {
    return data_.size() * huffman_consts::SIZE_OF_BIT -
        (bit) ((huffman_consts::SIZE_OF_BIT - last_) % huffman_consts::SIZE_OF_BIT);
  }

  bool empty() const {
    return data_.empty();
  }

  bit back() const {
    return last_;
  }

  bit operator[](size_t i) const {
    return (data_[i / huffman_consts::SIZE_OF_BIT ] >>
                                                    (huffman_consts::MAX_BIT - i % huffman_consts::SIZE_OF_BIT)) & 1u;
  }

  void cut(size_t i) {
    for (size_t j = 0; j < i; ++j) {
      pop_back();
    }
  }

  std::vector<T> const &data() const {
    return data_;
  }

  std::string to_string() const {
    std::string out;
    out.reserve(size());
    for (size_t i = 0; i < size(); ++i) {
      out += (char) ('0' + (*this)[i]);
    }
    return out;
  }

 private:
  bit last_;
  std::vector<T> data_;
  void increase_to_size(size_t in) {
    data_.resize(data_.size() + in);
  }
};

#endif //HUFFMAN_BIT_SET_HPP
