#include "inkview.h"
#include "inkinternal.h"
#include <math.h>


static int g_argc;
static char **g_argv;


static ifont *font;
static const int kFontSize = 15;
static int y_log;

static void start_logging()
{
	font = OpenFont("LiberationSans", kFontSize, 0);
	SetFont(font, BLACK);

	ClearScreen();
	FullUpdate();

	y_log = 0;
}


static void end_logging()
{
	FullUpdate();
	CloseFont(font);
}


static void log_message(const char *msg)
{
	DrawTextRect(0, y_log, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
	PartialUpdate(0, y_log, ScreenWidth(), y_log + kFontSize + 2);
	y_log += kFontSize + 2;
}


static void test_network()
{
	char buffer[2048];

	snprintf(buffer, 2048, "hw_wifi_status = %d", hw_wifi_status());
	log_message(buffer);

	snprintf(buffer, 2048, "QueryNetwork = %d", QueryNetwork());
	log_message(buffer);

	// Pour forcer la déconnexion (ça aide à tester la connexion ensuite)
	snprintf(buffer, 2048, "NetDisconnect = %d", NetDisconnect());
	log_message(buffer);

	iv_netinfo *netinfo = NetInfo();
	if (netinfo->connected) {
		snprintf(buffer, 2048, "Connecté au réseau : %s", netinfo->name);
		log_message(buffer);
	}
	else {
		log_message("Non connecté => connexion");

		// Je pense que si on on met NULL comme nom de réseau, la liseuse se connecte à celui "par défaut"
		//const char *network_name = "TEA-GUEST";
		const char *network_name = NULL;
		int result = NetConnect2(network_name, 1);

		snprintf(buffer, 2048, "Resultat connexion : %d", result);
		log_message(buffer);

		netinfo = NetInfo();
		if (netinfo->connected) {
			snprintf(buffer, 2048, "Connecté au réseau : %s", netinfo->name);
			log_message(buffer);
		}
		else {
			log_message("Toujours pas connecté ;-(");
		}
	}

	network_interface_info *nii = GetNetInfo(NULL);
	snprintf(buffer, 2048, "IP : %s / %s ; br = %s ; hw = %s", nii->ip_addr.addr, nii->mask.addr, nii->br_addr.addr, nii->hw_addr.hw_addr);
	log_message(buffer);
}


static int main_handler(int event_type, int param_one, int param_two)
{
	if (EVT_INIT == event_type) {
    	font = OpenFont("LiberationSans", kFontSize, 0);
    	SetFont(font, BLACK);

    	ClearScreen();
    	FullUpdate();

    	y_log = 0;

    	log_message("Lancement de l'application...");

    	test_network();

    	log_message("Fin du programme.");

    	FullUpdate();
    }
    else if (EVT_KEYPRESS == event_type) {
    	CloseFont(font);
        CloseApp();
    }

    return 0;
}


int main (int argc, char* argv[])
{
	g_argc = argc;
	g_argv = argv;

    InkViewMain(main_handler);
    return 0;
}
