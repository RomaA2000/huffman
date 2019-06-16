//
// Created by Роман Агеев on 2019-06-07.
//

#ifndef HUFFMAN_READER_HPP
#define HUFFMAN_READER_HPP

#include <string>

struct reader {
 private:
  int32_t inf;
  static size_t const MINIMAL_SIZE = 1024;
 public:
  explicit reader(std::string const &);
  reader(reader const &) = delete;
  ~reader();
  reader operator=(reader const &) = delete;

  size_t get_buf(char *buf, size_t size = MINIMAL_SIZE);
  bool is_open();
  void close();
  void open(std::string const &);

  void opener(std::string const &);
};

#endif //HUFFMAN_READER_HPP
