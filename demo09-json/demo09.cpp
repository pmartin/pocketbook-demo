#include "inkview.h"
#include "json-c/json.h"


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



static void json_01()
{
	char buffer[2048];

	const char *json_string = "{\"a_string\":\"plop!\",\"my_int\":123}";
	log_message(json_string);

	json_object *obj = json_tokener_parse(json_string);

	snprintf(buffer, 2048, "type=%d (%s)", json_object_get_type(obj), json_type_to_name(json_object_get_type(obj)));
	log_message(buffer);

	if (json_object_get_type(obj) == json_type_object) {
		json_object_object_foreach(obj, key, val) {
			json_type type = json_object_get_type(val);
			if (type == json_type_int) {
				snprintf(buffer, 2048, "  > %s :: type=%d (%s) -> %d", key, type, json_type_to_name(type), json_object_get_int(val));
			}
			else if (type == json_type_string) {
				snprintf(buffer, 2048, "  > %s :: type=%d (%s) -> %s", key, type, json_type_to_name(type), json_object_get_string(val));
			}
			// ... here, deal with the other types of data ; including array and object (recursive ;-) )
			log_message(buffer);
		}
	}
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
				json_01();
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
