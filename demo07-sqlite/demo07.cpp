#include "inkview.h"
#include "sqlite3.h"



#define DB_FILE USERDATA TEMPDIR "/demo07.sqlite3"


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



static int callback_01(void *not_used, int argc, char **argv, char **col_name){
	char buffer[128*5];
	char row_buffer[128];

	strcpy(buffer, " > ");
	for(int i=0; i<argc && i<5 ; i++) {
		snprintf(row_buffer, 128, "%s=%s, ", col_name[i], argv[i] ? argv[i] : "NULL");
		strcat(buffer, row_buffer);
	}

	if (strlen(buffer) > 3) {
		log_message(buffer);
	}
	return 0;
}


static void database_01()
{
	char buffer[2048];

	sqlite3 *db;
	int result;
	char *err_msg;

	// Ensure the DB doesn't already exist
	iv_unlink(DB_FILE);

	result = sqlite3_open(DB_FILE, &db);
	if (result) {
		snprintf(buffer, 2048, "Fail opening DB : %s", sqlite3_errmsg(db));
		log_message(buffer);
		goto exit;
	}

	log_message("Create table...");
	result = sqlite3_exec(db, "create table plop (id integer primary key, nom text)", callback_01, 0, &err_msg);
	if (result != SQLITE_OK) {
		snprintf(buffer, 2048, "Fail creating table : %s", err_msg);
		log_message(buffer);
		goto exit;
	}

	log_message("Insert(s)...");
	result = sqlite3_exec(db, "insert into plop (id, nom) values (1, 'Pascal')", callback_01, 0, &err_msg);
	if (result != SQLITE_OK) {
		snprintf(buffer, 2048, "Fail inserting : %s", err_msg);
		log_message(buffer);
		goto exit;
	}

	result = sqlite3_exec(db, "insert into plop (id, nom) values (2, 'John')", callback_01, 0, &err_msg);
	if (result != SQLITE_OK) {
		snprintf(buffer, 2048, "Fail inserting : %s", err_msg);
		log_message(buffer);
		goto exit;
	}

	log_message("Select...");
	result = sqlite3_exec(db, "select * from plop", callback_01, 0, &err_msg);
	if (result != SQLITE_OK) {
		snprintf(buffer, 2048, "Fail selecting : %s", err_msg);
		log_message(buffer);
		goto exit;
	}

	exit:
	log_message("All done.");
	sqlite3_close(db);
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
				database_01();
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
