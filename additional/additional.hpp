//
// Created by Роман Агеев on 2019-06-12.
//

#ifndef ADDITIONAL_HPP
#define ADDITIONAL_HPP

#include <cstddef>
#include <cstdint>

struct huff_node {
  typedef uint8_t my_symbol;
  my_symbol symbol;
  size_t counter = 0;

  huff_node() = default;

  ~huff_node() = default;

  friend bool operator<(huff_node const& a, huff_node const& b);
  friend bool operator>(huff_node const& a, huff_node const& b);
  friend bool operator<=(huff_node const& a, huff_node const& b);
  friend bool operator>=(huff_node const& a, huff_node const& b);
  friend bool operator==(huff_node const& a, huff_node const& b);
  friend bool operator!=(huff_node const& a, huff_node const& b);
};

#endif //ADDITIONAL_HPP