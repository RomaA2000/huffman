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
#include "writer.hpp"
writer::writer(std::string const &in) {
  opener(in);
}
writer::~writer() {
  close();
}
size_t writer::write_buf(char *buf, size_t size) {
  return ::write(inf, buf, size);
}
bool writer::is_open() {
  return inf != -1;
}
void writer::close() {
  if (inf != -1) {
    ::close(inf);
    inf = -1;
  }
}
void writer::open(std::string const &in) {
  close();
  opener(in);
}
void writer::opener(std::string const &in) {
  inf = ::open(in.c_str(),  O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}
