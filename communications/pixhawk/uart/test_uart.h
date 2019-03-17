//
// Created by Liam Kelly on 11/12/18.
//

#ifndef NVIDIA_TEST_UART_H
#define NVIDIA_TEST_UART_H

#include "uart.h"

class test_uart : public uart {

public:

    test_uart();
    ~test_uart();

    void write_buffer(std::vector<char> &data);
    void read_buffer(std::vector <char> &read_data);

    // For testing
    void write_single_char(char c);

private:

    std::string m_last_read;

};


#endif //NVIDIA_TEST_UART_H
