#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "ip_addr.h"
#include "user_interface.h"

#define USER_FLASH_ADDRESS 0x3D000
#define USER_FLASH_SECTOR 0x3D

char * ICACHE_FLASH_ATTR separate(char *str, char sep)
{
	size_t len;
	size_t i;

	len = strlen(str);

	for (i = 0; i < len; i++) {
		if (str[i] == sep) {
			str[i] = '\0';
			return str + i + 1;
		}
	}

	return NULL;
}

void ICACHE_FLASH_ATTR strip_newline(char *str)
{
	int i;

	i = strlen(str) - 1;

	while ((i >= 0) && (str[i] == '\n' || str[i] == '\r')) {
		str[i] = '\0';
		--i;
	}
}

void ICACHE_FLASH_ATTR print_softap_config(const struct softap_config *config)
{
	ets_uart_printf("SSID:\t\t\t%s\n", config->ssid);
	ets_uart_printf("Password:\t\t%s\n", config->password);
	ets_uart_printf("Channel:\t\t%d\n", config->channel);
	ets_uart_printf("Authmode:\t\t%d\n", config->authmode);
	ets_uart_printf("Hidden:\t\t\t%d\n", config->ssid_hidden);
	ets_uart_printf("Max connection:\t\t%d\n", config->max_connection);
	ets_uart_printf("Beacon interval:\t%d\n", config->beacon_interval);
}

const char * ICACHE_FLASH_ATTR str_bssid(uint8 *bssid)
{
	static char bssid_str[20];

	os_sprintf(bssid_str, "%02x:%02x:%02x:%02x:%02x:%02x",
			bssid[0], bssid[1], bssid[2],
			bssid[3], bssid[4], bssid[5]);

	return bssid_str;
}

const char * ICACHE_FLASH_ATTR inet_ntoa(uint32 addr)
{
	static char addr_str[17];
	uint8 *bytes = (uint8 *)&addr;

	os_sprintf(addr_str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
	return addr_str;
}

void ICACHE_FLASH_ATTR print_ip_info(const struct ip_info *info)
{
	ets_uart_printf("IP Address:\t\t%s\n", inet_ntoa(info->ip.addr));
	ets_uart_printf("Netmask:\t\t%s\n", inet_ntoa(info->netmask.addr));
	ets_uart_printf("Gateway:\t\t%s\n", inet_ntoa(info->gw.addr));
}

int ICACHE_FLASH_ATTR start_station(const char *ssid, const char *password)
{
	struct station_config config;

	if (!wifi_set_opmode(STATION_MODE)) {
		ets_uart_printf("Failed to set as station mode.\n");
		return -1;
	}

	memset(&config.ssid, 0, 32);
	memset(&config.password, 0, 64);

	os_memcpy(&config.ssid, ssid, strlen(ssid));
	os_memcpy(&config.password, password, strlen(password));
	config.bssid_set = 0;
	memset(config.bssid, 0, sizeof config.bssid);

	if (!wifi_station_set_config(&config)) {
		ets_uart_printf("Failed to configure station.\n");
		return -1;
	}

	if (!wifi_station_connect()) {
		ets_uart_printf("Failed to connect to router.\n");
		return -1;
	}

	return 0;
}

int ICACHE_FLASH_ATTR start_access_point(const char *ssid, const char *password, uint8 channel)
{
	struct softap_config config;
	ets_uart_printf("Starting access point");
	//Set access point mode
	if (!wifi_set_opmode(SOFTAP_MODE)) {
		ets_uart_printf("Failed to set as access point mode.\n");
		return -1;
	}

	//Set ap settings
	os_memcpy(&config.ssid, ssid, 32);
	os_memcpy(&config.password, password, 64);
	config.ssid_len = strlen(ssid);
	config.channel = 6;
	config.authmode = 0;
	config.ssid_hidden = 0;
	config.max_connection = 10;
	config.beacon_interval = 100;

	if (!wifi_softap_set_config(&config)) {
		ets_uart_printf("Failed to configure access point.\n");
		return -1;
	}

	return 0;
}

int ICACHE_FLASH_ATTR write_to_flash(uint32 *data, uint32 size){
	int erase,write,read_back;
	//must erase whole sector before writing
	erase=spi_flash_erase_sector(USER_FLASH_SECTOR);
	if(erase){
		ets_uart_printf("Erase failed.");
		return -1;
	}
	write=spi_flash_write(USER_FLASH_ADDRESS, (uint32*) data, size);
	if(write){
		ets_uart_printf("Write failed.");
		return -1;
	}
	uint32 read_back_test;
	read_back=spi_flash_read(USER_FLASH_ADDRESS, &read_back_test, size);
	if(read_back){
		ets_uart_printf("Read back failed.");
		return -1;
	}
	else if(read_back_test!=*data){
		ets_uart_printf("Read back does not match.");
		return -1;
	}
	return 0;

} 

// int ICACHE_FLASH_ATTR save_ap(void){
// 	struct station_config config;
// 	bool get_config_result=wifi_station_get_config(&config);
// 	ets_uart_printf("Saved configuration: SSID %s, PASS %s, BYTES: %d\n\n", config.ssid, config.password,sizeof(config.ssid)+sizeof(config.password));
// 	uint32 test=3;
// 	write_to_flash(&test,(uint32)sizeof(uint32));
// 	ets_uart_printf("Trying to write ssid now... \n\n");
// 	write_to_flash((uint32*)&config,(uint32)(sizeof(config.ssid)));
// }	