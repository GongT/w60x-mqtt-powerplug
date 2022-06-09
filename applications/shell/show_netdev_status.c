#include "shell.h"
#include <gongt/config_tool.h>
#include <sys/socket.h>
#include <netdev.h>

#if defined(NETDEV_USING_IFCONFIG)

int netdev_ifconfig(int argc, char **argv);
static char *netdev_status_map[] = {
	[NETDEV_CB_ADDR_IP] = "NETDEV_CB_ADDR_IP",
	[NETDEV_CB_ADDR_NETMASK] = "NETDEV_CB_ADDR_NETMASK",
	[NETDEV_CB_ADDR_GATEWAY] = "NETDEV_CB_ADDR_GATEWAY",
	[NETDEV_CB_ADDR_DNS_SERVER] = "NETDEV_CB_ADDR_DNS_SERVER",
	[NETDEV_CB_STATUS_UP] = "NETDEV_CB_STATUS_UP",
	[NETDEV_CB_STATUS_DOWN] = "NETDEV_CB_STATUS_DOWN",
	[NETDEV_CB_STATUS_LINK_UP] = "NETDEV_CB_STATUS_LINK_UP",
	[NETDEV_CB_STATUS_LINK_DOWN] = "NETDEV_CB_STATUS_LINK_DOWN",
	[NETDEV_CB_STATUS_INTERNET_UP] = "NETDEV_CB_STATUS_INTERNET_UP",
	[NETDEV_CB_STATUS_INTERNET_DOWN] = "NETDEV_CB_STATUS_INTERNET_DOWN",
	[NETDEV_CB_STATUS_DHCP_ENABLE] = "NETDEV_CB_STATUS_DHCP_ENABLE",
	[NETDEV_CB_STATUS_DHCP_DISABLE] = "NETDEV_CB_STATUS_DHCP_DISABLE",
};

static void handle_change_status(struct netdev *netdev, enum netdev_cb_type type) {
	KPRINTF_COLOR(14, "netdev status changed %s(%d)", netdev_status_map[type], type);
	netdev_ifconfig(1, NULL);
}
static void handle_change_addr(struct netdev *netdev, enum netdev_cb_type type) {
	KPRINTF_COLOR(14, "netdev address changed %s(%d)", netdev_status_map[type], type);
	netdev_ifconfig(1, NULL);
}

static int init_debug_netdev() {
	return 0;
	netdev_set_status_callback(CONFIG_INTERFACE_NETDEV, handle_change_status);
	netdev_set_addr_callback(CONFIG_INTERFACE_NETDEV, handle_change_addr);
}
INIT_COMPONENT_EXPORT(init_debug_netdev);

#endif
