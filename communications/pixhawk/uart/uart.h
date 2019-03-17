//
// Created by Liam Kelly on 11/12/18.
//

#ifndef NVIDIA_UART_MANAGER_H
#define NVIDIA_UART_MANAGER_H

#include <vector>
#include <string>

class uart {

public:

    virtual ~uart() {}

    /**
     * @return the number of bytes read
     */
    virtual void read_buffer(std::vector<char> &read_data) = 0;

    virtual void write_buffer(std::vector<char> &data) = 0;

private:


};


#endif //NVIDIA_UART_MANAGER_H
