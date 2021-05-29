#pragma once

#define DBG_SECTION_NAME "my.dhcp"

#include "app.h"
#include <lwip/dhcp.h>
#include "lwip/prot/dhcp.h"

void test_dhcp_set_options(struct netif *netif, struct dhcp *dhcp, dhcp_state_enum_t _DHCP_STATE_SELECTING, struct dhcp_msg *msg_out, int _DHCP_DISCOVER, u16_t *options_out_len);
