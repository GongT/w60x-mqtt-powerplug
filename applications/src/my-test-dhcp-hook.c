#include "my-test-dhcp-hook.h"

#ifdef LWIP_HOOK_DHCP_APPEND_OPTIONS
void test_dhcp_set_options(struct netif *netif, struct dhcp *dhcp, dhcp_state_enum_t state, struct dhcp_msg *msg_out, int type, u16_t *options_out_len)
{
	if (type != DHCP_DISCOVER || state != DHCP_STATE_SELECTING)
	{
		return;
	}

	msg_out->options[(*options_out_len)++] = DHCP_OPTION_CLIENT_ID;
	msg_out->options[(*options_out_len)++] = 7;
	msg_out->options[*options_out_len] = 0x1;
	rt_wlan_get_mac(&(msg_out->options[1 + *options_out_len]));
	*options_out_len += 7;
}
#endif
