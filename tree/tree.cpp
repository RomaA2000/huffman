//
// Created by Роман Агеев on 2019-06-07.
//

#include "tree.hpp"
bool node::leaf() const {
  return (left == nullptr) && (right == nullptr);
}

node::node(uint8_t inf, size_t counter) : inf(inf), counter(counter) {}

node::node(node_ptr l, node_ptr r, node_ptr p, uint8_t inf, size_t counter)
    :  left(l), right(r), parent(p), inf(inf), counter(counter) {
}

void tree_encoder::build(std::array<huff_node, huffman_consts::ALPHABET> &&in) {
  auto comparator = [](node_ptr const & a, node_ptr const & b) {
    return a->counter > b->counter;
  };
  std::priority_queue<node_ptr, std::vector<node_ptr>, decltype(comparator)> input(comparator);
  for (huff_node const &i : in) {
    if (i.counter > 0) {
      input.push(new node(i.symbol, i.counter));
    }
  }
  if (input.empty()) {
    root = nullptr;
  } else {
    while (input.size() > 1) {
      node_ptr l = input.top();
      input.pop();
      node_ptr r = input.top();
      input.pop();
      input.push(new node(l, r, nullptr, 0, l->counter + r->counter));
    }
    root.reset(input.top());
  }
}

std::string tree_encoder::to_char(size_t in) {
  std::string result;
  result.resize(huffman_consts::SIZE_T_MAX_SIZE);
  for (size_t i = 0; i < huffman_consts::SIZE_T_MAX_SIZE; ++i) {
    result[i] = (in >> ((huffman_consts::SIZE_T_MAX_SIZE - i - 1) * 8));
  }
  return result;
}

tree_encoder::tree_encoder(counter const &in) {
  build(in.data());
  if (!empty()) {
    if (root->leaf()) {
      code_tree.push_back(1);
      code_input.concat(bit_set(root->inf));
      map[root->inf] = code_tree;
    } else {
      bit_set<> buf;
      code_calc(root, buf);
    }
  }
}

bool tree_encoder::empty() const {
  return root == nullptr;
}

void tree_encoder::code_calc(std::unique_ptr<node> const & in, bit_set<> &inf) {
  if (in != nullptr) {
    if (in->leaf()) {
      map[in->inf] = inf;
      code_input.concat(bit_set(in->inf));
    } else {
      code_tree.push_back(1);
      inf.push_back(1);
      code_calc(in->left, inf);
      inf.pop_back();
      code_tree.push_back(0);
      inf.push_back(0);
      code_calc(in->right, inf);
      inf.pop_back();
    }
  }
}

std::string tree_encoder::internal_data() const {
  std::string result;
  if (!empty()) {
    auto code_2 = code_input.data();
    auto code_1 = code_tree.data();
    result.resize(2 * huffman_consts::SIZE_T_MAX_SIZE + code_1.size() + code_2.size());
    std::string size = to_char(code_2.size());
    std::copy(size.begin(), size.end(), result.begin());
    std::copy(code_2.begin(), code_2.end(), result.begin() + huffman_consts::SIZE_T_MAX_SIZE);
    size = to_char(code_tree.size());
    std::copy(size.begin(),
              size.end(),
              result.begin() + code_2.size() + huffman_consts::SIZE_T_MAX_SIZE);
    std::copy(code_1.begin(), code_1.end(),
              result.begin() + code_2.size() + 2 * huffman_consts::SIZE_T_MAX_SIZE);
  }
  return result;
}

void tree_decoder::build() {
  if (dfs.size() != 1) {
    root = std::make_unique<node>(0, 239239);
    node_ptr j = root.get();
    for (size_t i = 0; i < dfs.size(); ++i) {
      if (dfs[i] != 1) {
        j->inf = decoder_alpha.front();
        decoder_alpha.pop();
        j = j->parent;
        while ((j->right != nullptr) && (j->parent != nullptr)) {
          j = j->parent;
        }
        if (j->right != nullptr) {
          throw std::runtime_error("error in tree code");
        } else {
          j->right = std::make_unique<node>(nullptr, nullptr, j, 0, 239239);
          j = j->right.get();
        }
      } else {
        j->left = std::make_unique<node>(nullptr, nullptr, j, 0, 239239);
        j = j->left.get();
      }
    }
    if ((j->left != nullptr) || (j->right != nullptr)) {
      throw std::runtime_error("error in tree code");
    } else {
      j->inf = decoder_alpha.front();
      decoder_alpha.pop();
    }
  } else {
    root = std::make_unique<node>(decoder_alpha.front(), 239239);
    decoder_alpha.pop();
  }
}

void tree_decoder::decode_block(bit_set<> const &code) {
  if (dfs.size() != 1) {
    node_ptr j = (now.now == nullptr) ? root.get() : now.now;
    for (size_t i = 0; i < code.size(); ++i) {
      if (code[i] == 1) {
        j = j->left.get();
      } else {
        j = j->right.get();
      }
      if (j == nullptr) {
        throw std::runtime_error("error in data block");
      }
      if (j->leaf()) {
        buffer += static_cast<char>(j->inf);
        j = root.get();
      }
    }
    now.now = j;
  } else {
    for (size_t i = 0; i < code.size(); ++i) {
      if (code[i] == 0) {
        throw std::runtime_error("corrupted data block");
      }
      buffer += static_cast<char>(root->inf);
    }
  }
}
