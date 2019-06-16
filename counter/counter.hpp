//
// Created by Роман Агеев on 2019-06-12.
//

#ifndef HUFFMAN_COUNTER_HPP
#define HUFFMAN_COUNTER_HPP
#include <iostream>
#include <array>
#include "../additional/additional.hpp"
#include "../additional/consts/consts.hpp"

struct counter {
 private:
  std::array<huff_node, huffman_consts::ALPHABET> data_{};
 public:
  counter();

  ~counter() = default;

  std::array<huff_node, huffman_consts::ALPHABET> data() const {
    return data_;
  }

  template <typename I, typename = typename std::enable_if<std::is_trivially_copyable<typename
      std::iterator_traits<I>::value_type>::value>::type>
  void update(I i1, I i2) {
    for (; i1 != i2; ++i1) {
      data_[*i1 & huffman_consts::MAX_BYTE].counter++;
    }
  }
};

#endif //HUFFMAN_COUNTER_HPP
