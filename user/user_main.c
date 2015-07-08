#include "c_types.h"
#include "osapi.h"
#include "ets_sys.h"
#include "user_interface.h"

// #include "ap_server.h"
// #include "device_config.h"
// #include "network_cmds.h"
// #include "helper.h"
// #include "mqtt.h"
// #include "wifi.h"
#include "camera.h"
#include "debug.h"

bool HAS_BEEN_CONNECTED_AS_STATION;
bool HAS_RECEIVED_CONNECT_INSTRUCTION;

void ICACHE_FLASH_ATTR init_done()
{
	DEBUG("enter init_done");
	// const char *opmodes[] = {"NONE", "STATION_MODE", "SOFTAP_MODE", "STATIONAP_MODE"};
	// struct softap_config config;
	// struct ip_info info;
	// uint8 opmode;
	// enum dhcp_status status;

	// opmode = wifi_get_opmode();

	// if (opmode != SOFTAP_MODE) {
	// 	DEBUG("exit init_done");
	// 	return;
	// }

	// ets_uart_printf("Current Opmode: 0x%02x (%s)\n", opmode, (opmode < 4 ? opmodes[opmode] : "???"));

	// ets_uart_printf("\n");

	// if (!wifi_softap_get_config(&config))
	// 	ets_uart_printf("Failed to get wifi softap config.\n");
	// else
	// 	print_softap_config(&config);

	// ets_uart_printf("\n");

	// if (!wifi_get_ip_info(SOFTAP_IF, &info))
	// 	ets_uart_printf("Failed to get ip info.\n");
	// else
	// 	print_ip_info(&info);

	// ets_uart_printf("\n");

	// status = wifi_softap_dhcps_status();

	// ets_uart_printf("DHCP server status:\t%d\n", status);
	// ets_uart_printf("\n");

	// if (ap_server_init() != 0)
	// 	ets_uart_printf("Failed to initialize ap server.\n");
	camera_init();
	camera_reset();
	DEBUG("exit init_done");
}

// void ICACHE_FLASH_ATTR wifi_timer_cb(void *timer_arg)
// {
// 	DEBUG("enter wifi_timer_cb");

// 	if (!HAS_BEEN_CONNECTED_AS_STATION) {
// 		ets_uart_printf("wifi timer timeout\n");
// 		go_back_to_ap(NULL);
// 	}

// 	DEBUG("exit wifi_timer_cb");
// }

void ICACHE_FLASH_ATTR user_init()
{
	DEBUG("enter user_init");
	//static ETSTimer wifi_connect_timer;

	//system_restore();
	system_set_os_print(0);
	uart_div_modify(0, UART_CLK_FREQ / 115200);
	ets_uart_printf("\n\n\n");
	
	// HAS_RECEIVED_CONNECT_INSTRUCTION = false;
	// HAS_BEEN_CONNECTED_AS_STATION = false;

	// if (DeviceConfig_already_exists()) {
	// 	struct DeviceConfig conf;

	// 	if (DeviceConfig_read_config(&conf) != 0) {
	// 		ets_uart_printf("Failed to read device config.\n");
	// 	} else {
	// 		ets_uart_printf("Current device config: device_name = %s, device_type = %d\n",
	// 				conf.name, conf.type);
	// 	}

	// 	DeviceConfig_delete(&conf);
	// } else {
	// 	ets_uart_printf("This device has not yet been configured.\n");
	// }

	// mqtt_init();

	// wifi_set_opmode(STATION_MODE);
	// wifi_set_event_handler_cb(sta_wifi_handler);

	// if (!wifi_station_set_auto_connect(1))
	// 	ets_uart_printf("Unable to set auto connect.\n");
	
	system_init_done_cb(&init_done);

	// os_timer_setfn(&wifi_connect_timer, wifi_timer_cb, NULL);
	// os_timer_arm(&wifi_connect_timer, AUTO_CONNECT_TIMEOUT, false); 
	// DEBUG("exit user_init");
}