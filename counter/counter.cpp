//
// Created by Роман Агеев on 2019-06-12.
//

#include "counter.hpp"

counter::counter() {
  for (size_t i = 0; i < huffman_consts::ALPHABET; ++i) {
    data_[i] = {static_cast<uint8_t>(i), 0};
  }
}
