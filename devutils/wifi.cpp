#include "devutils.h"


int wifi_activate()
{
	iv_netinfo *netinfo = NetInfo();
	if (netinfo->connected) {
		// Already connected => nothing more to do
		return 0;
	}

	const char *network_name = NULL;
	int result = NetConnect2(network_name, 1);
	if (result != 0) {
		// Failed to connect
		return 1;
	}

	// Just to be sure: check if we are, now, connected
	netinfo = NetInfo();
	if (netinfo->connected) {
		return 0;
	}

	// Connection failed, I don't know why
	return 2;
}


int wifi_deactivate()
{
	iv_netinfo *netinfo = NetInfo();
	if (!netinfo->connected) {
		// Already not connected => nothing to do
		return 0;
	}

	return NetDisconnect();
}


