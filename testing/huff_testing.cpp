//
// Created by Роман Агеев on 2019-06-07.
//
#include <iostream>
#include <fstream>
#include <random>
#include <stdio.h>
#include "../additional/consts/consts.hpp"
#include "../testing/gtest/gtest.h"
#include "../testing/file/reader.hpp"
#include "../testing/file/writer.hpp"
#include "../testing/file/myfile.hpp"
#include "../tree/bit_set/bit_set.hpp"

const std::string file_name1 = "test1";
const std::string file_name2 = "test2";
const std::string file_name3 = "test3";
const std::string for_timer = "[   TIME   ]";
const std::string for_memory = "[  MEMORY  ]";

size_t get_size(std::string const &in) {
  std::fstream file(in);
  size_t size = 0;
  file.seekg (0, std::ios::end);
  size = file.tellg();
  file.close();
  return size;
}

void TIME(std::string const &in, std::chrono::duration<double> const &time) {
  std::cout << for_timer << " " << in << " (" << floor(time.count() * 1000) << " ms)" << std::endl;
}

void MEM(std::string const &in, std::string const &file) {
  std::cout << for_memory << " " << in << " (" << (get_size(file) / 1000000) << " Mb)" << std::endl;
}

void GET_INF(std::chrono::duration<double> const &time1, std::chrono::duration<double> const &time2) {
  TIME("Encoding time", time1);
  TIME("Decoding time", time2);
  MEM("Input", file_name1);
  MEM("Output", file_name2);
}

TEST(Tester, small_files) {
  string_generator(file_name1, "  ");
  string_generator(file_name2, " ");
  myfile a(file_name1);
  myfile b(file_name2);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == a);
  EXPECT_EQ(a, a);
  EXPECT_EQ(b, b);
}

TEST(Tester, random_small_files) {
  for (size_t i = 0; i < 100; ++i) {
    random_file_generator(file_name1, 1);
    random_file_generator(file_name2, 2);
    myfile a(file_name1);
    myfile b(file_name2);
    EXPECT_FALSE(a == b);
    EXPECT_FALSE(b == a);
    EXPECT_EQ(a, a);
    EXPECT_EQ(b, b);
  }
}

TEST(Tester, random_big_files) {
  for (size_t i = 1024; i < 1026; ++i) {
    random_file_generator(file_name1, i);
    random_file_generator(file_name2, i + 1);
    myfile a(file_name1);
    myfile b(file_name2);
    EXPECT_FALSE(a == b);
    EXPECT_FALSE(b == a);
    EXPECT_EQ(a, a);
    EXPECT_EQ(b, b);
  }
}


TEST(Bit_set, to_string) {
  bit_set set(128);
  EXPECT_EQ(set.to_string(), "10000000");
  set = bit_set(255);
  EXPECT_EQ(set.to_string(), "11111111");
  set = bit_set(0);
  EXPECT_EQ(set.to_string(), "00000000");
  set = bit_set(1);
  EXPECT_EQ(set.to_string(), "00000001");
  set = bit_set(2);
  EXPECT_EQ(set.to_string(), "00000010");
}

TEST(Bit_set, small_strings) {
  bit_set set;
  for (size_t i = 0; i < 10; ++i) {
    set.push_back(1);
    set.push_back(0);
  }
  EXPECT_EQ(set.to_string(), "10101010101010101010");
}

TEST(Bit_set, random_small_strings) {
  for (size_t i = 0; i < 25; ++i) {
    bit_set set;
    std::string ans = random_bit_string_generator(set, 25);
    EXPECT_EQ(set.to_string(), ans);
  }
}

TEST(Bit_set, small_concat) {
  bit_set set1;
  for (size_t i = 0; i < 5; ++i) {
    set1.push_back(0);
  }
  bit_set set2;
  for (size_t i = 0; i < 5; ++i) {
    set2.push_back(1);
  }
  set1.concat(set2);
  EXPECT_EQ(set1.to_string(), "0000011111");
}

TEST(Bit_set, random_small_concat) {
  for (size_t i = 0; i < 25; ++i) {
    bit_set set1;
    std::string ans1 = random_bit_string_generator(set1, 25);
    bit_set set2;
    std::string ans2 = random_bit_string_generator(set2, 25);
    set1.concat(set2);
    ans1 += ans2;
    EXPECT_EQ(set1.to_string(), ans1);
  }
}

TEST(Bit_set, random_big_concat) {
  for (size_t i = 0; i < 10; ++i) {
    bit_set set1;
    std::string ans1 = random_bit_string_generator(set1, 10000);
    bit_set set2;
    std::string ans2 = random_bit_string_generator(set2, 10000);
    set1.concat(set2);
    ans1 += ans2;
    EXPECT_EQ(set1.to_string(), ans1);
  }
}

TEST(Huffman, random_small_strings) {
  for (size_t i = 0; i < 1000; ++i) {
    std::string input;
    random_string_generator(input, i);
    counter cnt;
    cnt.update(input.begin(), input.end());
    tree_encoder tree(cnt);
    tree.encode(input.begin(), input.end());
    std::string gen;
    gen.append(tree.internal_data());
    gen.append(tree.encode(input.begin(),input.end()));
    tree_decoder decode_tree;
    std::string result;
    result.append(decode_tree.decode(gen.begin(),gen.end()));
    EXPECT_EQ(input, result);
  }
}

TEST(Huffman, random_big_strings) {
  for (size_t j = 1; j < 10; ++j) {
    std::string input;
    counter cnt;
    random_string_generator(input, huffman_consts::BUFF_SIZE * j);
    cnt.update(input.begin(), input.end());
    tree_encoder tree(cnt);
    std::string gen = tree.internal_data();
    for (size_t k = 0; k < j; ++k) {
      gen.append(tree.encode(input.begin() + k * huffman_consts::BUFF_SIZE,
          input.begin() + (k + 1) * huffman_consts::BUFF_SIZE));
    }
    tree_decoder decode_tree;
    std::string result;
    for (size_t k = 0; gen.begin() + k * huffman_consts::BUFF_SIZE < gen.end(); ++k) {
      auto l = (gen.begin() + (k + 1) * huffman_consts::BUFF_SIZE > gen.end()) ? gen.end()
          : gen.begin() + (k + 1) * huffman_consts::BUFF_SIZE;
        result.append(decode_tree.decode(gen.begin() + k * huffman_consts::BUFF_SIZE, l));
    }
    std::vector<std::vector<std::vector<int>>> a;
    EXPECT_EQ(input, result);
  }
}

TEST(Huffman, empty) {
  random_file_generator(file_name1, 0);
  file_encode(file_name1, file_name2);
  file_decode(file_name2, file_name3);
  myfile a(file_name1);
  myfile b(file_name3);
  EXPECT_EQ(a, b);
}

TEST(Huffman, random_small_file) {
  for (size_t i = 140; i < 150; ++i) {
    random_file_generator(file_name1, i);
    file_encode(file_name1, file_name2);
    file_decode(file_name2, file_name3);
    myfile a(file_name1);
    myfile b(file_name3);
    EXPECT_EQ(a, b);
  }
}

TEST(Huffman, random_big_file) {
  for (size_t i = 1000; i < 1002; ++i) {
    random_file_generator(file_name1, i);
    file_encode(file_name1, file_name2);
    file_decode(file_name2, file_name3);
    myfile a(file_name1);
    myfile b(file_name3);
    EXPECT_EQ(a, b);
  }
}

TEST(Huffman, one_mb_file) {
  generate_big_file(file_name1, 1);
  file_encode(file_name1, file_name2);
  file_decode(file_name2, file_name3);
  myfile a(file_name1);
  myfile b(file_name3);
  EXPECT_EQ(a, b);
}

TEST(Huffman, hundred_mb_file) {
  generate_big_file(file_name1, 100);
  auto time_in = std::chrono::high_resolution_clock::now();
  file_encode(file_name1, file_name2);
  std::chrono::duration<double> encoding = std::chrono::high_resolution_clock::now() - time_in;
  time_in = std::chrono::high_resolution_clock::now();
  file_decode(file_name2, file_name3);
  std::chrono::duration<double> decoding = std::chrono::high_resolution_clock::now() - time_in;
  myfile a(file_name1);
  myfile b(file_name3);
  EXPECT_EQ(a, b);
  GET_INF(encoding, decoding);
}

TEST(Testing_system, file_closing) {
  EXPECT_TRUE(remove(file_name1.data()) == 0);
  EXPECT_TRUE(remove(file_name2.data()) == 0);
  EXPECT_TRUE(remove(file_name3.data()) == 0);
}