//
// Created by Роман Агеев on 2019-06-07.
//

#ifndef HUFFMAN_WRITER_HPP
#define HUFFMAN_WRITER_HPP

#include <string>

struct writer {
 private:
  int32_t inf;
  static size_t const MINIMAL_SIZE = 1024;
  void opener(std::string const &);
 public:
  explicit writer(std::string const &);
  writer(writer const &) = delete;
  ~writer();
  writer operator=(writer const &) = delete;

  size_t write_buf(char *buf, size_t size = MINIMAL_SIZE);
  bool is_open();
  void close();
  void open(std::string const &);
};

#endif //HUFFMAN_WRITER_HPP
