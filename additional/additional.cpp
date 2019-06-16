//
// Created by Роман Агеев on 2019-06-12.
//

#include "additional.hpp"

bool operator<(huff_node const& a, huff_node const& b) {
  return a.counter < b.counter;
}
bool operator>(huff_node const& a, huff_node const& b) {
  return a.counter > b.counter;
}
bool operator<=(huff_node const& a, huff_node const& b) {
  return a.counter <= b.counter;
}
bool operator>=(huff_node const& a, huff_node const& b) {
  return a.counter >= b.counter;
}
bool operator==(huff_node const& a, huff_node const& b) {
  return a.counter == b.counter;
}
bool operator!=(huff_node const& a, huff_node const& b) {
  return a.counter != b.counter;
}
