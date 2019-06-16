//
// Created by Роман Агеев on 2019-06-07.
//
#include "myfile.hpp"
void file_encode(std::string const &input_file, std::string const &output_file) {
  reader file1(input_file);
  writer file2(output_file);
  if (!file1.is_open()) {
    throw std::runtime_error("no such file :" + input_file);
  } if (!file2.is_open()) {
    throw std::runtime_error("error in writing in :" + output_file);
  } else {
    counter cnt;
    size_t file1_size = 1;
    while (file1_size != 0) {
      std::vector<char> buf1(huffman_consts::BUFF_SIZE);
      file1_size = file1.get_buf(buf1.data(), huffman_consts::BUFF_SIZE);
      cnt.update(buf1.begin(), buf1.begin() + file1_size);
    }
    file1.close();
    file1.open(input_file);
    tree_encoder tree(cnt);
    auto data = tree.internal_data();
    file1_size = file2.write_buf(data.data(), data.size());
    if (file1_size !=  data.size()) {
      throw std::runtime_error("error in writing in :" + output_file);
    }
    size_t file2_size = 1;
    while (file1_size != 0) {
      std::vector<char> buf1(huffman_consts::BUFF_SIZE);
      file1_size = file1.get_buf(buf1.data(), huffman_consts::BUFF_SIZE);
      auto str = tree.encode(buf1.begin(), buf1.begin() + file1_size);
      file2_size = file2.write_buf(str.data(), str.size());
      if (file2_size != str.size()) {
        throw std::runtime_error("error in writing in :" + output_file);
      }
    }
    file2.close();
  }
}

void file_decode(std::string const &input_file, std::string const &output_file) {
  reader file1(input_file);
  writer file2(output_file);
  if (!file1.is_open()) {
    throw std::runtime_error("no such file : " + input_file);
  } if (!file2.is_open()) {
    throw std::runtime_error("error in writing in :" + output_file);
  } else {
    tree_decoder tree;
    size_t file1_size = 1;
    size_t file2_size = 1;
    while (file1_size != 0) {
      std::vector<char> buf1(huffman_consts::BUFF_SIZE);
      file1_size = file1.get_buf(buf1.data(), huffman_consts::BUFF_SIZE);
      auto str = tree.decode(buf1.begin(), buf1.begin() + file1_size);
      file2_size = file2.write_buf(str.data(), str.size());
      if (file2_size != str.size()) {
        throw std::runtime_error("error in writing in :" + output_file);
      }
    }
  }
}


void string_generator(std::string const &out, std::string const & in) {
  std::vector<char> buffer;
  buffer.resize(in.size());
  std::copy(in.begin(), in.end(), buffer.begin());
  writer file(out);
  file.write_buf(buffer.data(), buffer.size());
}


void random_file_generator(std::string const &out, size_t size = 1024) {
  writer file(out);
  std::random_device random;
  std::mt19937 rand(random());
  std::uniform_int_distribution<uint32_t> range(0, 255);
  for (size_t i = 0; i < size; i++) {
    std::vector<char> buffer(size);
    for (size_t j = 0; j < size; j++) {
      buffer[j] = range(rand);
    }
    file.write_buf(buffer.data(), size);
  }
  file.close();
}

std::string random_bit_string_generator(bit_set<uint8_t> & in, size_t size = 256) {
  std::random_device random;
  std::mt19937 rand(random());
  std::uniform_int_distribution<uint8_t> range(0, 1);
  std::string ans;
  uint8_t tmp = 0;
  for (size_t i = 0; i < size; ++i) {
    tmp = range(rand);
    in.push_back(tmp);
    ans += tmp == 1 ? "1" : "0";
  }
  return ans;
}

void random_string_generator(std::string &in, size_t size) {
  in.clear();
  std::random_device random;
  std::mt19937 rand(random());
  std::uniform_int_distribution<char> range;
  in.resize(size);
  for (size_t i = 0; i < size; ++i) {
    in[i] = range(rand);
  }
}

void generate_big_file(std::string const & in, size_t m_number = 1) {
  writer r(in);
  const size_t size = 1000;
  for (size_t i = 0; i < m_number; i++) {
    for (size_t j = 0; j < size; j++) {
      char buffer[size];
      r.write_buf(buffer, size);
    }
  }
}


bool operator==(myfile const &a, myfile const &b) {
  const size_t size = 4096;
  reader file1(a.name), file2(b.name);
  file1.open(a.name);
  file2.open(b.name);
  if (!file1.is_open()) {
    throw std::runtime_error("There is no such file : " + a.name);
  }
  if (!file2.is_open()) {
    throw std::runtime_error("There is no such file : " + b.name);
  }
  while (true) {
    std::vector<char> buf1(size), buf2(size);
    size_t file1_size = file1.get_buf(buf1.data(), size);
    size_t file2_size = file2.get_buf(buf2.data(), size);
    if ((file1_size != file2_size) || (buf1 != buf2)) {
      return false;
    }
    if (file1_size == 0) {
      break;
    }
  }
  return true;
}
myfile::myfile(std::string in) : name(in) {}
