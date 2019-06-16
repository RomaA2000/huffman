//
// Created by Роман Агеев on 2019-06-07.
//

#ifndef HUFFMAN_MYFILE_HPP
#define HUFFMAN_MYFILE_HPP
#include <iostream>
#include <random>
#include <fstream>
#include <fstream>
#include <stdio.h>
#include "reader.hpp"
#include "writer.hpp"
#include "../../counter/counter.hpp"
#include "../../tree/tree.hpp"
void file_encode(std::string const &, std::string const &);

void file_decode(std::string const &, std::string const &);

void string_generator(std::string const &, std::string const &);

void generate_big_file(std::string const &, size_t);

void random_file_generator(std::string const &, size_t);

std::string random_bit_string_generator(bit_set<uint8_t> &, size_t);

void random_string_generator(std::string &, size_t);

struct myfile {
    std::string name;
    explicit myfile(std::string in);
    friend bool operator==(myfile const &, myfile const &);
};

#endif //HUFFMAN_MYFILE_HPP
