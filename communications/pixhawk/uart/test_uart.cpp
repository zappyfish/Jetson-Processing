//
// Created by Liam Kelly on 11/12/18.
//

#include "test_uart.h"

test_uart::test_uart() : m_last_read("") {}

test_uart::~test_uart() {}

void test_uart::write_buffer(std::vector<char> &data) {
    for (auto const &c : data) {
        m_last_read += c;
    }
}

void test_uart::read_buffer(std::vector<char> &read_data) {
    for (auto const &c: m_last_read) {
        read_data.push_back(c);
    }
    m_last_read.clear(); // empty out
}

void test_uart::write_single_char(char c) {
    m_last_read += c;
}