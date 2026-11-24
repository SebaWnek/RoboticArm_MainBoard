#ifndef HELPERS_H
#define HELPERS_H

typedef enum {
	IP_ADDRESS_SET_FAILED = 0,
	UDP_SOCKET_BIND_FAILED,
	UDP_SOCKET_RECEIVE_FAILED
} error_codes_t;

// Send data to the blackbox via UART4 to be written in FRAM
int log_blackbox(error_codes_t error_code);

#endif /* HELPERS_H */
