#include "inkview.h"


pthread_mutex_t mutex_y_log;

static ifont *font;
static const int kFontSize = 16;
static int y_log;

static void log_message(const char *msg)
{
	int y;

	// Si deux threads accèdent à y_log en même temps,
	// deux lignes de logs risquent de se chevaucher
	pthread_mutex_lock(&mutex_y_log);
	y = y_log;
	y_log += kFontSize + 2;
	pthread_mutex_unlock(&mutex_y_log);


	if (strlen(msg) == 0) {
		return;
	}
	DrawTextRect(0, y, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
	PartialUpdate(0, y, ScreenWidth(), y + kFontSize + 2);
}



static void *test01_start_routine_01(void *)
{
	char buffer[2048];

	for (int i=0 ; i<5 ; i++) {
		snprintf(buffer, 2048, " >> Thread n°%d -> %d / 5", 1, i);
		log_message(buffer);
		usleep(500*1000);
	}

	pthread_exit(NULL);
}


static void *test01_start_routine_02(void *)
{
	char buffer[2048];

	for (int i=0 ; i<4 ; i++) {
		snprintf(buffer, 2048, " >> Thread n°%d -> %d / 4", 2, i);
		log_message(buffer);
		usleep(400*1000);
	}

	pthread_exit(NULL);
}

static void test_threads_01()
{
	log_message("Lancement...");

	pthread_t thread1;
	pthread_attr_t *attr1 = NULL;
	void *arg1 = NULL;

	pthread_t thread2;
	pthread_attr_t *attr2 = NULL;
	void *arg2 = NULL;

	mutex_y_log = PTHREAD_MUTEX_INITIALIZER;

	log_message("Lancement thread n°1");
	if (pthread_create(&thread1, attr1, test01_start_routine_01, arg1) == -1) {
		log_message("Erreur lancement du thread n°1");
		goto exit;
	}

	usleep(500*1000);

	log_message("Lancement thread n°2");
	if (pthread_create(&thread2, attr2, test01_start_routine_02, arg2) == -1) {
		log_message("Erreur lancement du thread n°2");
		goto exit;
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&mutex_y_log);

	exit:
	log_message("Fin!");
}


static int main_handler(int event_type, int param_one, int param_two)
{
	// 0 == événement non géré par l'application ; et sera donc géré par la liseuse
	// non-0 == événement géré par l'application ; et ne sera donc pas géré par la liseuse
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
				test_threads_01();
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
