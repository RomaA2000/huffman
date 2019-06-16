//
// Created by Роман Агеев on 2019-06-07.
//
#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP
#include <memory>
#include <cstdint>
#include <cstddef>
#include <array>
#include <queue>
#include "../additional/consts/consts.hpp"
#include "bit_set/bit_set.hpp"
#include "../additional/additional.hpp"
#include "../counter/counter.hpp"

struct node;
typedef node *node_ptr;

struct node {
  std::unique_ptr<node> left = nullptr;
  std::unique_ptr<node> right = nullptr;
  node_ptr parent = nullptr;
  uint8_t inf = 0;
  size_t counter = 0;

  node() = default;
  node(uint8_t, size_t);
  node(node_ptr, node_ptr, node_ptr, uint8_t, size_t);
  ~node() = default;

  bool leaf() const;
};

struct tree_encoder {
 public:
  tree_encoder(counter const &in);

  std::string internal_data() const;

  ~tree_encoder() = default;

  template<typename I, typename =
  typename std::enable_if<std::is_trivially_copyable<typename std::iterator_traits<I>::value_type>::value &&
      (sizeof(typename std::iterator_traits<I>::value_type) == 1)>::type>
  std::string encode(I i1, I i2) {
    std::string ans;
    if (!empty()) {
      bit_set<> buff;
      size_t i = 0;
      for (; i1 != i2; ++i1, ++i) {
        buff.concat(map[*i1 & huffman_consts::MAX_BYTE]);
      }
      ans += to_char(buff.size());
      std::move(buff.data().begin(), buff.data().end(), std::back_inserter(ans));
    }
    return ans;
  }
 private:
  std::unique_ptr<node> root = nullptr;
  bit_set<> code_input;
  bit_set<> code_tree;
  std::array<bit_set<>, huffman_consts::ALPHABET> map;

  static std::string to_char(size_t);

  void build(std::array<huff_node, huffman_consts::ALPHABET> &&);

  void code_calc(std::unique_ptr<node> const &, bit_set<> &);

  bool empty() const;
};

struct tree_decoder {
  tree_decoder() = default;

  ~tree_decoder() = default;

  template<typename I, typename =
  typename std::enable_if<std::is_trivially_copyable<typename std::iterator_traits<I>::value_type>::value &&
      (sizeof(typename std::iterator_traits<I>::value_type) == 1)>::type>
  std::string decode(I i1, I i2) {
    std::string ans;
    while (i1 != i2) {
      if (now.size == 0 || now.size_byte) {
        i1 = decode_size(i1, i2);
        if (now.size_byte) {
          return ans;
        }
      }
      if (now.inf == START) {
        i1 = decode_start(i1, i2);
        if (decoder_alpha.size() == now.size) {
          now.inf = INF;
          now.size = 0;
          now.size_byte = huffman_consts::SIZE_T_MAX_SIZE;
        } else {
          return ans;
        }
      } else if (now.inf == INF) {
        i1 = decode_inf(i1, i2);
        if (dfs.size() >= now.size) {
          if (now.size % huffman_consts::SIZE_OF_BIT != 0) {
            dfs.cut(huffman_consts::SIZE_OF_BIT -
                now.size % huffman_consts::SIZE_OF_BIT);
          }
          build();
          now.inf = COUNT;
          now.size = 0;
          now.size_byte =
              huffman_consts::SIZE_T_MAX_SIZE;
        } else {
          return ans;
        }
      } else if (now.inf == COUNT) {
        return decode_block(i1, i2);
      }
    }
    return ans;
  }
 private:
  enum state { START, INF, COUNT };
  struct decoder {
    node_ptr now = nullptr;
    state inf = START;
    size_t size = 0;
    uint8_t size_byte = huffman_consts::SIZE_T_MAX_SIZE;
  } now;
  bit_set<> dfs;
  std::queue<uint8_t> decoder_alpha;
  std::unique_ptr<node> root = nullptr;
  std::string buffer;
  void build();

  template<typename I>
  I decode_size(I i1, I i2) {
    for (; i1 != i2 && now.size_byte; ++i1) {
      now.size |= (size_t) (huffman_consts::MAX_BYTE & *i1) << (huffman_consts::SIZE_OF_BIT * (now.size_byte - 1));
      --now.size_byte;
    }
    return i1;
  }

  template<typename I>
  I decode_start(I i1, I i2) {
    for (; (i1 != i2) && (decoder_alpha.size() < now.size); ++i1) {
      decoder_alpha.push(*i1);
    }
    return i1;
  }

  template<typename I>
  I decode_inf(I i1, I i2) {
    for (; (i1 != i2) && (dfs.size() < now.size); ++i1) {
      dfs.concat(bit_set<>(*i1 & huffman_consts::MAX_BYTE));
    }
    return i1;
  }

  void decode_block(bit_set<> const &code);

  template<typename I>
  std::string const &decode_block(I i1, I i2) {
    buffer.clear();
    while (i1 != i2) {
      bit_set<> block_code;
      if (now.size == 0 || now.size_byte) {
        i1 = decode_size(i1, i2);
        if (now.size_byte) {
          continue;
        }
      }
      for (; (i1 != i2) && (block_code.size() < now.size); ++i1) {
        block_code.concat(bit_set<>(*i1 & huffman_consts::MAX_BYTE));
      }
      if (block_code.size() >= now.size) {
        if ((now.size % huffman_consts::SIZE_OF_BIT) != 0) {
          block_code.cut(huffman_consts::SIZE_OF_BIT -
              (now.size % huffman_consts::SIZE_OF_BIT));
        }
        now.size_byte = huffman_consts::SIZE_T_MAX_SIZE;
        now.size = 0;
      } else {
        now.size -= block_code.size();
      }
      decode_block(block_code);
    }
    return buffer;
  }
};
#endif //HUFFMAN_TREE_HPP


