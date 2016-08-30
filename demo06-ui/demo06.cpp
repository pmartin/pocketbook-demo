#include "inkview.h"

static ifont *font;


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



char *submenu2_text;
char *submenu1_text;
char *menu_text;

icontext_menu *context_menu;
icontext_menu_properties *properties = NULL;

struct imenu_s *submenu1 = NULL, *submenu2 = NULL;
imenu *menu;

irect *rect_pos_selected_item;
irect *rect_pos_menu;


static void contextmenu_01_handler(int index)
{
	char buffer[2048];
	snprintf(buffer, 2048, "Context Menu: index %d has been selected!", index);
	Message(ICON_INFORMATION, "Debug", buffer, 3*1000);

	free(properties);

	free(submenu2);
	free(submenu1);
	free(menu);

	free(rect_pos_selected_item);
	free(rect_pos_menu);

	free(submenu2_text);
	free(submenu1_text);
	free(menu_text);
}


static void context_menu_01()
{
	rect_pos_selected_item = (irect *)malloc(sizeof(irect));
	rect_pos_selected_item->x = 200;
	rect_pos_selected_item->y = 200;
	rect_pos_selected_item->w = 10;
	rect_pos_selected_item->h = 50;
	rect_pos_selected_item->flags = 0;

	rect_pos_menu = (irect *)malloc(sizeof(irect));
	rect_pos_menu->x = 100;
	rect_pos_menu->y = 100;
	rect_pos_menu->w = 400;
	rect_pos_menu->h = 600;
	rect_pos_menu->flags = 0;

	submenu2_text = (char *)malloc(strlen("Text of entry 2 (type=2)") + 1);
	strcpy(submenu2_text, "Text of entry 2 (type=2)");
	submenu2 = (imenu_s *)malloc(sizeof(struct imenu_s));
	submenu2->type = 2;
	submenu2->index = 2;
	submenu2->text = submenu2_text;
	submenu2->submenu = NULL;

	submenu1_text = (char *)malloc(strlen("Text of entry 1 (type=2)") + 1);
	strcpy(submenu1_text, "Text of entry 1 (type=2)");
	submenu1 = (imenu_s *)malloc(sizeof(struct imenu_s));
	submenu1->type = 2;
	submenu1->index = 1;
	submenu1->text = submenu1_text;
	submenu1->submenu = submenu2;

	menu_text = (char *)malloc(strlen("My menu?") + 1);
	strcpy(menu_text, "My menu?");
	menu = (imenu *)malloc(sizeof(imenu));
	menu->type = 1;
	menu->index = 0;
	menu->text = menu_text;
	menu->submenu = submenu1;

	properties = (icontext_menu_properties *)malloc(sizeof(icontext_menu_properties));
	properties->font_normal = font;
	properties->font_selected = font;
	properties->font_disabled = font;
	properties->background_color_normal = WHITE;
	properties->background_color_disabled = LGRAY;
	properties->blackout = 0;
	properties->item_height = 30;
	properties->item_border_indent_horizontal = 2;
	properties->separator_indent_horizontal = 5;
	properties->separator_indent_vertical = 3;
	properties->separator_height = 6;
	properties->separator_style = 1;
	properties->separator_color = DGRAY;
	properties->separator_frequency = 1;
	properties->marker_height = 10;
	properties->marker_indent_horizontal = 5;
	properties->marker_style = 1;
	properties->marker_color = LGRAY;
	properties->border_size = 2;
	properties->border_radius = 5;
	properties->title_height = 60;
	properties->enable_pointer = 1;
	properties->font_normal_color = BLACK;
	properties->font_selected_color = DGRAY;
	properties->font_disabled_color = LGRAY;
	properties->bullet_show = 1;
	properties->bullet_style = 1;
	properties->item_border_indent_vertical = 4;
	properties->background_style = 2;

	context_menu = CreateContextMenu("menu");
	context_menu->hproc = (iv_menuhandler)contextmenu_01_handler;

	context_menu->menu = menu;

	context_menu->pos_selected_item = *rect_pos_selected_item;
	context_menu->pos_menu = *rect_pos_menu;

	context_menu->properties = properties;
	context_menu->active_element = 2;
	context_menu->enable_pointer = 1;
	context_menu->enable_aura = 0;
	context_menu->use_own_font = 0;
	context_menu->update_after_close = 1;

	OpenContextMenu(context_menu);
	SetContextMenu(context_menu);
}



int progressbar_01_percent;

static void progressbar_01_timer()
{
	char buffer[2048];

	progressbar_01_percent += 5;
	snprintf(buffer, 2048, "Progress is now %d%%", progressbar_01_percent);
	UpdateProgressbar(buffer, progressbar_01_percent);

	if (progressbar_01_percent < 100) {
		SetWeakTimer("My 1st timer", progressbar_01_timer, 250);
	}
	else {
		CloseProgressbar();
	}
}

static void progressbar_01_handler(int button)
{
	char buffer[2048];

	CloseProgressbar();

	snprintf(buffer, 2048, "Progress: button %d has been selected!\nStopping progress!", button);
	Message(ICON_INFORMATION, "Debug", buffer, 3*1000);
}

static void progressbar_01()
{
	const char *title = "Title of progress";
	const char *text = "There is a lot of progress going on, here!";
	progressbar_01_percent = 0;

	OpenProgressbar(ICON_WARNING, title, text, progressbar_01_percent, progressbar_01_handler);

	// Progressbar dialog is opened but non-blocking
	// => use a timer to call a function in 250ms, to update the progress
	// => kind-of simulating a background action taking some time to complete...
	SetWeakTimer("My 1st timer", progressbar_01_timer, 250);
}



static int main_handler(int event_type, int param_one, int param_two)
{
	// 0 == event not taken into account by the application. The ereader will deal with it.
	// non-0 == the application has taken the event into account. It will not be dealt with by the ereader.
	int result = 0;

	static int step = 0;

	switch (event_type) {
	case EVT_INIT:
		font = OpenFont("LiberationSans", 24, 1);
		SetMenuFont(font);
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
			else if (step == 5) {
				progressbar_01();
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
