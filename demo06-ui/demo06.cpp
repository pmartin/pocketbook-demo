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


static void menu_01_handler(int index)
{
	char buffer[2048];
	snprintf(buffer, 2048, "Menu: index %d has been selected!", index);
	Message(ICON_INFORMATION, "Debug", buffer, 3*1000);
}

static void menu_01()
{
	struct imenu_s submenu3 = {.type = 3, .index = 3, .text = (char *)"Text of entry 3 (type=3)", .submenu = NULL};
	struct imenu_s submenu2 = {.type = 2, .index = 2, .text = (char *)"Text of entry 2 (type=2)", .submenu = &submenu3};
	struct imenu_s submenu1 = {.type = 1, .index = 1, .text = (char *)"Text of entry 1 (type=1)", .submenu = &submenu2};
	imenu menu = {.type = 1, .index = 0, .text = (char *)"Text of menu (type=1)", .submenu = &submenu1};

	OpenMenu(&menu, 0, 100, 200, (iv_menuhandler)menu_01_handler);
}



static void context_menu_01()
{
	/*
	imenuex menuex;
	menuex.type = 0;
	menuex.index = 0;
	menuex.text = "1st menu item?";
	menuex.submenu = NULL;
	menuex.icon = NULL;
	menuex.font = font;
	*/

	imenu menu = {type: 0, index: 0, text: (char *)"1st menu item?", submenu: NULL};

	icontext_menu_properties properties;
	properties.font_normal = font;
	properties.font_selected = font;
	properties.font_disabled = font;
	// A voir : plein d'autres champs...


	icontext_menu *context_menu = CreateContextMenu("my_menu");
	context_menu->hproc = (iv_menuhandler)menu_01_handler;

	//context_menu->menuex = &menuex;
	context_menu->menuex = NULL;
	context_menu->menu = &menu;

	//context_menu.pos_selected_item = 0;
	//context_menu.pos_menu = 0;
	//context_menu->properties = &properties;
	//context_menu->active_element = 0;
	//context_menu->enable_pointer = 1;
	//context_menu->enable_aura = 1;
	//context_menu->use_own_font = 0;
	//context_menu->update_after_close = 0;


	OpenContextMenu(context_menu);
	SetContextMenu(context_menu);

	Message(ICON_INFORMATION, "Debug", "Menu ???", 3*1000);
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
			//*
			if (step == 0) {
				Message(ICON_INFORMATION, "Debug", "Key right!\n(message will disappear after a while...)", 3*1000);
			}
			else if (step == 1) {
				ui_dialog_synchro_01();
			}
			else if (step == 2) {
				ui_dialog_01();
			}
			else if (step == 3) {
				menu_01();
			}
			else if (step == 4) {
				context_menu_01();
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
