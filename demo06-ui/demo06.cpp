#include "inkview.h"


// Seems like DialogSynchro() is blocking : code execution will only continue after
// a button has been clicked
static void ui_dialog_synchro_01()
{
	int result = DialogSynchro(ICON_QUESTION, "My Tile", "Content of the dialog", "1st button", "2nd button", NULL);
	char buffer[2048];
	snprintf(buffer, 2048, "Result of DialogSynchro() = %d", result);
	Message(ICON_INFORMATION, "Debug", buffer, 3*1000);
}


static void *ui_simple_dialog_handler(int button)
{
	char buffer[2048];
	snprintf(buffer, 2048, "Dialog: button %d has been selected!", button);
	Message(ICON_INFORMATION, "Debug", buffer, 3*1000);
	return NULL;
}

// Seems Dialog() is non-blocking : code execution will continue immediately
// and a callback will be called when a button is clicked
static void ui_dialog_01()
{
	const char *title = "This is a dialog";
	const char *text = "This is the text of the dialog";
	const char *button1 = "1st button!";
	const char *button2 = "And 2nd one ;-)";
	Dialog(ICON_INFORMATION, title, text, button1, button2, (iv_dialoghandler)ui_simple_dialog_handler);
	Message(ICON_INFORMATION, "Debug", "Dialog() is non-blocking ;-)", 3*1000);
}


static int main_handler(int event_type, int param_one, int param_two)
{
	// 0 == événement non géré par l'application ; et sera donc géré par la liseuse
	// non-0 == événement géré par l'application ; et ne sera donc pas géré par la liseuse
	int result = 0;

	static int step = 0;

	switch (event_type) {
	case EVT_INIT:

		break;
	case EVT_SHOW:

		break;
	case EVT_KEYPRESS:
		if (param_one == KEY_PREV) {
			CloseApp();
			return 1;
		}
		else if (param_one == KEY_NEXT) {
			if (step == 0) {
				Message(ICON_INFORMATION, "Debug", "Key right!\n(message will disappear after a while...)", 3*1000);
			}
			else if (step == 1) {
				ui_dialog_synchro_01();
			}
			else if (step == 2) {
				ui_dialog_01();
			}
			else {
				CloseApp();
			}

			step++;
			return 1;
		}

		break;
	case EVT_EXIT:

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
