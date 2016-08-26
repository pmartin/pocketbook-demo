#include "devutils.h"


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





static int main_handler(int event_type, int param_one, int param_two)
{
	if (EVT_INIT == event_type) {
		if (g_argc <= 1) {
			start_logging();
			log_message("No command specified");
			log_message("Usage: devutils.app 'command'");
			log_message("Press a key, any key, to exit ;-)");
			end_logging();
			return 0;
    	}

		char *command = g_argv[1];

		if (iv_strcmp(command, "wifi:activate") == 0) {
			wifi_activate();
			CloseApp();
		}
		else if (iv_strcmp(command, "wifi:deactivate") == 0) {
			wifi_deactivate();
			CloseApp();
		}
		else {
			start_logging();

			char buffer[2048];
			snprintf(buffer, 2048, "Unknown command '%s'", command);
			log_message(buffer);

			end_logging();
		}
    }
    else if (EVT_KEYPRESS == event_type) {
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
