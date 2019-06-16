//
// Created by Роман Агеев on 2019-06-07.
//
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "reader.hpp"

reader::reader(std::string const &in) {
  opener(in);
}
reader::~reader() {
  close();
}
size_t reader::get_buf(char *buf, size_t size) {
  return ::read(inf, buf, size);
}
bool reader::is_open() {
  return inf != -1;
}
void reader::close() {
  if (inf != -1) {
    ::close(inf);
    inf = -1;
  }
}
void reader::open(std::string const &in) {
  close();
  opener(in);
}
void reader::opener(std::string const &in) {
  inf = ::open(in.c_str(), O_RDONLY);
}
