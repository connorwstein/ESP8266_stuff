#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "user_config.h"

#define LOOP_QUEUE_LEN 1
os_event_t loop_queue[LOOP_QUEUE_LEN];

void ICACHE_FLASH_ATTR print_station_config(const struct station_config *config)
{
	ets_uart_printf("SSID: %s\nPASSWORD: %s\nBSSID_SET: %d\n"
		  "BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n",
		  config->ssid, config->password, config->bssid_set,
		  config->bssid[0], config->bssid[1], config->bssid[2],
		  config->bssid[3], config->bssid[4], config->bssid[5]);
}

const char *print_ip_from_uint32(uint32 addr)
{
	static char addr_str[17];
	uint8 *bytes = (uint8 *)&addr;

	os_sprintf(addr_str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
	return addr_str;
}

void ICACHE_FLASH_ATTR print_ip_info(const struct ip_info *info)
{
	ets_uart_printf("IP Address:\t%s\n", print_ip_from_uint32(info->ip.addr));
	ets_uart_printf("Netmask:\t%s\n", print_ip_from_uint32(info->netmask.addr));
	ets_uart_printf("Gateway:\t%s\n", print_ip_from_uint32(info->gw.addr));
}

void ICACHE_FLASH_ATTR init_done(void)
{
	const char *opmodes[] = {"NONE", "STATION_MODE", "SOFTAP_MODE", "STATIONAP_MODE"};
	const char *statuses[] = {"STATION_IDLE", "STATION_CONNECTING", "STATION_WRONG_PASSWORD", "STATION_NO_AP_FOUND", "STATION_CONNECT_FAIL", "STATION_GOT_IP"};
	struct station_config config;
	struct ip_info info;
	uint8 opmode, status, auto_conn;

	ets_uart_printf("\n---------------\n");
	ets_uart_printf("System init done.\n");

	opmode = wifi_get_opmode();
	ets_uart_printf("Opmode: 0x%02x (%s)\n", opmode, (opmode < 4 ? opmodes[opmode] : "???"));

	if (!wifi_station_get_config(&config))
		ets_uart_printf("Failed to get wifi station config.\n");
	else
		print_station_config(&config);

	ets_uart_printf("\n");
	status = wifi_station_get_connect_status();
	ets_uart_printf("Connection status: %d (%s)\n", status, (status < 6 ? statuses[status] : "???"));

	auto_conn = wifi_station_get_auto_connect();
	ets_uart_printf("Auto connect: %d (%s)\n", auto_conn, (auto_conn == 0 ? "No": "Yes"));
	ets_uart_printf("\n\n");
}

void wifi_handler(System_Event_t *event)
{
	struct ip_info info;

	switch (event->event) {
		case EVENT_STAMODE_CONNECTED:
			ets_uart_printf("Connected to Access Point!\n");
			ets_uart_printf("SSID: %s\n", event->event_info.connected.ssid);
			ets_uart_printf("BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n",
					event->event_info.connected.bssid[0],
					event->event_info.connected.bssid[1],
					event->event_info.connected.bssid[2],
					event->event_info.connected.bssid[3],
					event->event_info.connected.bssid[4],
					event->event_info.connected.bssid[5]);
			ets_uart_printf("Channel: %d\n", event->event_info.connected.channel);
			ets_uart_printf("\n");
			break;

		case EVENT_STAMODE_GOT_IP:
			ets_uart_printf("Got IP!\n");
			print_ip_info((struct ip_info *)&(event->event_info.got_ip));
			ets_uart_printf("\n");
			break;
		default:
			break;
	}

}

void ICACHE_FLASH_ATTR loop(os_event_t *e)
{
	os_delay_us(100);
	system_os_post(USER_TASK_PRIO_0, 0, 0);
}

void ICACHE_FLASH_ATTR user_init()
{
	struct station_config config;

	os_strcpy((char *)&config.ssid, SSID);
	os_strcpy((char *)&config.password, PASSWORD);
	config.bssid_set = 0;

	system_restore();
	system_set_os_print(0);
	uart_div_modify(0, UART_CLK_FREQ / 115200);

	wifi_set_opmode(STATION_MODE);
	wifi_station_set_config(&config);
	
	system_init_done_cb(&init_done);
	wifi_set_event_handler_cb(wifi_handler);

	system_os_task(loop, USER_TASK_PRIO_0, loop_queue, LOOP_QUEUE_LEN);
	system_os_post(USER_TASK_PRIO_0, 0, 0);
}
