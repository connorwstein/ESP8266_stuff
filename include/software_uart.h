#ifndef SOFTWARE_UART_H
#define SOFTWARE_UART_H

struct rx_buffer;

typedef void (*rx_buffer_recv_cb)(struct rx_buffer *);

struct rx_buffer{
	uint16 size;
	uint8 *buf;
	rx_buffer_recv_cb recv_cb;
};

void ICACHE_FLASH_ATTR bit_bang_send(const char *data, uint16 len);

void ICACHE_FLASH_ATTR enable_interrupts();
void ICACHE_FLASH_ATTR disable_interrupts();

void ICACHE_FLASH_ATTR flush_read_buffer();
bool ICACHE_FLASH_ATTR read_buffer_full();

void ICACHE_FLASH_ATTR set_rx_buffer(struct rx_buffer *buf);
void ICACHE_FLASH_ATTR software_uart_config(uint32 baud, uint8 rx_pin, uint8 tx_pin);

struct rx_buffer* ICACHE_FLASH_ATTR create_rx_buffer(uint16 response_size, rx_buffer_recv_cb recv_cb);
void ICACHE_FLASH_ATTR destroy_rx_buffer(struct rx_buffer *buffer_to_kill);

#endif