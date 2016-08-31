#include "inkview.h"


static ifont *font;
static const int kFontSize = 16;
static int y_log;

static void log_message(const char *msg)
{
	if (strlen(msg) == 0) {
		return;
	}
	DrawTextRect(0, y_log, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
	PartialUpdate(0, y_log, ScreenWidth(), y_log + kFontSize + 2);
	y_log += kFontSize + 2;
}


// Opens the usual reader application, on our epub
// To quit: same as usual: touch in top-left corner of the screen
// Note: the EPUB is not added to "recent books" on the home-screen.
static void test_01_epub()
{
	char buffer[1024];

	const char *filepath = "/mnt/ext1/system/tmp/demo10/my-ebook.epub";
	const char *parameters = "r";
	int flags = 0;

	snprintf(buffer, sizeof(buffer), "Opening %s...", filepath);
	log_message(buffer);

	OpenBook(filepath, parameters, flags);
}


// Opens the usual reader application, on our HTML file,
// and it seems to work -- even if we don't have a full HTML document (only some content).
// Note: remote content, such as images, are not downloaded / displayed
static void test_02_html()
{
	char buffer[1024];

	const char *filepath = "/mnt/ext1/system/tmp/demo10/my-page.html";
	const char *parameters = "r";
	int flags = 0;

	snprintf(buffer, sizeof(buffer), "Opening %s...", filepath);
	log_message(buffer);

	OpenBook(filepath, parameters, flags);
}



static int main_handler(int event_type, int param_one, int param_two)
{
	int result = 0;

	static int step = 0;

	switch (event_type) {
	case EVT_INIT:
		font = OpenFont("LiberationSans", 16, 1);
		SetFont(font, BLACK);
		y_log = 0;
		ClearScreen();
		FullUpdate();
		break;
	case EVT_SHOW:

		break;
	case EVT_KEYPRESS:
		if (param_one == KEY_PREV) {
			CloseApp();
			return 1;
		}
		else if (param_one == KEY_NEXT) {
			//*
			if (step == 0) {
				test_01_epub();
			}
			else if (step == 1) {
				test_02_html();
			}
			else {
				CloseApp();
			}
			//*/

			step++;
			return 1;
		}

		break;
	case EVT_EXIT:
		CloseFont(font);
		break;
	default:
		break;
	}

    return result;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);

    return 0;
}
