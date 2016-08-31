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

	snprintf(buffer, sizeof(buffer), "type=%d (%s)", json_object_get_type(obj), json_type_to_name(json_object_get_type(obj)));
	log_message(buffer);

	if (json_object_get_type(obj) == json_type_object) {
		json_object_object_foreach(obj, key, val) {
			json_type type = json_object_get_type(val);
			if (type == json_type_int) {
				snprintf(buffer, sizeof(buffer), "  > %s :: type=%d (%s) -> %d", key, type, json_type_to_name(type), json_object_get_int(val));
			}
			else if (type == json_type_string) {
				snprintf(buffer, sizeof(buffer), "  > %s :: type=%d (%s) -> %s", key, type, json_type_to_name(type), json_object_get_string(val));
			}
			// ... here, deal with the other types of data ; including array and object (recursive ;-) )
			log_message(buffer);
		}
	}
}


// Serialize some C/C++ data structures to JSON
static void json_02()
{
	log_message("Serializing C/C++ data structures to JSON...");

	const char *json_string;
	json_object *obj, *sub_obj1;

	// Empty object: {}
	obj = json_object_new_object();
	json_string = json_object_to_json_string(obj);
	free(obj);
	log_message(json_string);
	free((void *)json_string);

	// Very simple object: {"my_int":123,"a_string":"Plop!"}
	obj = json_object_new_object();
	json_object_object_add(obj, "my_int", json_object_new_int(123));
	json_object_object_add(obj, "a_string", json_object_new_string("Plop!"));
	json_string = json_object_to_json_string(obj);
	free(obj);
	log_message(json_string);
	free((void *)json_string);

	// An array that contains an object and an integer: [{"key":"some val"},123456]
	obj = json_object_new_array();
	sub_obj1 = json_object_new_object();
	json_object_object_add(sub_obj1, "key", json_object_new_string("some val"));
	json_object_array_add(obj, sub_obj1);
	json_object_array_add(obj, json_object_new_int(123456));
	json_string = json_object_to_json_string(obj);
	free(obj);
	log_message(json_string);
	free((void *)json_string);
}


// A bit of error-handling + (basic) parsing with a tokener
static void json_03_errors()
{
	log_message("Error-handling");

	char buffer[2048];
	const char *json_string;
	json_object *obj;
	json_tokener_error error;

	// An invalid not-JSON-string (missing a quote before "plop")
	// Error 4 -> unexpected character
	json_string = "{\"a_string\":plop!\"}";
	obj = json_tokener_parse_verbose(json_string, &error);
	if (!obj) {
		snprintf(buffer, sizeof(buffer), "Error %d parsing '%s' -> %s", error, json_string, json_tokener_error_desc(error));
		log_message(buffer);
	}

	// Another invalid not-JSON-string, parsing with a tokener
	// Error 9 -> quoted object property name expected
	json_tokener *tok = json_tokener_new();
	json_string = "{this is not the sound }[of] JSON!";
	obj = json_tokener_parse_ex(tok, json_string, strlen(json_string));
	if (!obj) {
		error = json_tokener_get_error(tok);
		snprintf(buffer, sizeof(buffer), "Error %d parsing '%s' -> %s", error, json_string, json_tokener_error_desc(error));
		log_message(buffer);
	}
	json_tokener_free(tok);
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
			else if (step == 1) {
				json_02();
			}
			else if (step == 2) {
				json_03_errors();
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
