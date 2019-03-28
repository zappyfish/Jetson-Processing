//
// Created by Liam Kelly on 11/12/18.
//

#ifndef NVIDIA_PACKET_MANAGER_H
#define NVIDIA_PACKET_MANAGER_H

#include "pixhawk_packet.h"
#include <queue>
#include <cstddef>
#include "../uart/uart.h"

class packet_manager {

public:

    typedef struct {
        void (*callback)(const char *packet_type, std::vector<const char *> keys, std::vector<const char *> values, void *args);
        const char * name;
        void *args;
    } packet_callback;

    ~packet_manager();

    static packet_manager& get_instance();

    void send_packet(pixhawk_packet *packet);
    void set_uart(uart* uart_bus);

    void set_packet_callback(packet_callback *callback);
    bool remove_packet_callback(packet_callback *callback);

    /**
     * Will parse ALL current packets in UART and make callbacks and return the number of packet_type packets available
     * @param packet_type
     * @return 0 for unknown packet_type, number of packets handled otherwise
     */
    size_t check_packets();

private:

    packet_manager();

    bool parse_for_packet_data();
    void handle_packet(const char* packet_type, std::vector<const char*> keys, std::vector<const char*> values);

    uart *m_uart;

    // This maps packet type to a queue of those packets
    std::vector<packet_callback*> m_packet_callbacks;

    std::queue<char> m_buffer_data;

    char* get_token_copy(std::vector<char> &next_token);

};


#endif //NVIDIA_PACKET_MANAGER_H
