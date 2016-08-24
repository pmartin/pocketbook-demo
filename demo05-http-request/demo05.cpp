#include "inkview.h"
#include <math.h>

static ifont *font;
static const int kFontSize = 15;
static int y_log;

static void log_message(const char *msg)
{
	DrawTextRect(0, y_log, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
	PartialUpdate(0, y_log, ScreenWidth(), y_log + kFontSize + 2);
	y_log += kFontSize + 2;
}


static void http_request_01(void)
{
	char buffer[2048];

	const char *url = "http://checkip.amazonaws.com/";
	int retsize;
	char *cookie = NULL;
	char *post = NULL;

	sprintf(buffer, "HTTP Request to %s", url);
	log_message(buffer);

	void *result = QuickDownloadExt(url, &retsize, 15, cookie, post);

	sprintf(buffer, "Taille de la réponse : %d", retsize);
	log_message(buffer);

	log_message("Contenu de la réponse :");
	strncpy(buffer, (char *)result, fmax(sizeof(buffer) - 1, retsize));
	log_message(buffer);

	free(result);
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
    	http_request_01();
    	log_message("Fin du programme.");
    }
    else if (EVT_KEYPRESS == event_type) {
    	CloseFont(font);
        CloseApp();
    }

    return 0;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);
    return 0;
}
