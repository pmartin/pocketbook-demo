#include "inkview.h"
#include "curl/curl.h"
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


// Requête HTTP et download vers un fichier
static void http_request_02()
{
	char buffer[2048];

	int session = NewSession();

	// On récupére toujours un pointeur vers le même espace
	// Non initialisé par la liseuse au départ => on initialise à 0 pour identifier, plus tard, quand le download sera fini
	iv_sessioninfo *sinf = GetSessionInfo(session);
	sinf->response = 0;

	const char *url = "https://blog.pascal-martin.fr/post/directives-ini-c-est-le-mal.html";
	//const char *url = "http://bit.ly/2bPtNry";
	const char *postdata = NULL;
	const char *filename = USERDATA TEMPDIR "/debug.log";

	// Pour la forme, on efface le fichier avant que le download ne le (re-)crée
	int result_unlink = iv_unlink(filename);
	sprintf(buffer, "Suppression fichier %s => %d", filename, result_unlink);
	log_message(buffer);

	SetUserAgent(session, "Mon joli user-agent");

	// On lance le download
	int result_download = DownloadTo(session, url, postdata, filename, 15);
	int session_status = GetSessionStatus(session);
	sinf = GetSessionInfo(session);

	// Polling tant que le download n'est pas fini.
	// session_status : doit être un indicateur "download vraiment en cours ou pas"
	//    - 0 au tout début du download
	//    - passe à 2 pendant une partie du download
	//    - vaut à nouveau 0 à la fin du download, si téléchargement effectué (que ce soit en 200 ou en 404 !) ; ou reste à 2 en fait ???
	//    - reste à 2 si la requête part en 301
	//    - passe à -21 si erreur de DNS
	// sinf->response :
	//    même si on avait initialisé à autre chose que 0,
	//    la liseuse le bascule à 0 lorsque session_status passe à 2
	//    Vaut le code statut HTTP, à la fin de la requête
	// sinf->url :
	//    - vaut l'URL en résultat de la requête : celle de la page chargée
	//    - ou l'URL du redirect (qui n'est pas suivi !) si le statut est 301
	int i = 0;
	while (i<5000 && session_status >= 0 && sinf->response == 0) {
		if (i%250 == 0) {
			sprintf(buffer, "  Waiting... #%d ; settion_status=%d ; sinf->response=%ld ; length=%d ; progress=%d", i, session_status, sinf->response, sinf->length, sinf->progress);
			log_message(buffer);
		}

		// J'espérais que ça demande à la liseuse d'attendre un moment...
		// Mais ça n'a pas l'air de faire grand chose ;-(
		GoSleep(250, 1);

		session_status = GetSessionStatus(session);
		sinf = GetSessionInfo(session);
		i++;
	}

	session_status = GetSessionStatus(session);
	sinf = GetSessionInfo(session);

	sprintf(buffer, "Session #%d (status=%d) ; result_download=%d", session, session_status, result_download);
	log_message(buffer);

	// Espérons que ça rentre dans le buffer... sinon, buffer overflow ;-(
	sprintf(buffer, "Infos :: url=%s", sinf->url);
	log_message(buffer);
	sprintf(buffer, "Infos :: ctype=%s response=%ld length=%d progress=%d", sinf->ctype, sinf->response, sinf->length, sinf->progress);
		log_message(buffer);

	// On a fini avec le download !
	CloseSession(session);

	// Lecture du fichier résultat du download
	log_message("Contenu du fichier créé par le download :");
	FILE *fp = iv_fopen(filename, "rb");
	if (fp) {
		int count_read;
		int nb_lignes = 0;
		while ((count_read = iv_fread(buffer, 1, 2048-1, fp)) > 0 && ++nb_lignes < 5) {
			buffer[(int)fmin(count_read, 2048-1)] = '\0';
			log_message(buffer);
		}
		iv_fclose(fp);
	}

	log_message("Fin second essai.");
}



static size_t curl_03_header_callback(char *ptr, size_t size, size_t nitems, void *userdata)
{
	char buffer[1024];

	int data_size = size * nitems;
	snprintf(buffer, sizeof(buffer), "  Header : %d bytes : %.128s", data_size, ptr);
	log_message(buffer);

	return data_size;
}

static size_t curl_03_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	// Attention : la donnée reçue n'est pas \0-terminated
	char buffer[1024];

	int data_size = size * nmemb;
	snprintf(buffer, sizeof(buffer), "  Data %d bytes : %.128s", data_size, ptr);
	log_message(buffer);

	// Même si on n'a pas tout affiché, on indique qu'on a tout géré
	return data_size;
}

static void http_request_03()
{
	char buffer[2048];

	log_message("Début troisième essai (curl).");

	CURL *curl = curl_easy_init();
	if (!curl) {
		log_message("Echec initialisation curl");
		return;
	}

	//const char *url = "http://checkip.amazonaws.com/";
	const char *url = "https://blog.pascal-martin.fr/post/directives-ini-c-est-le-mal.html";
	//const char *url = "http://bit.ly/2bPtNry";

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, curl_03_header_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_03_write_callback);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		sprintf(buffer, "Erreur %d : %s", res, curl_easy_strerror(res));
		log_message(buffer);

		goto end;
	}

	end:
	curl_easy_cleanup(curl);

	log_message("Fin troisième essai (curl).");
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
    	http_request_02();
    	http_request_03();
    	log_message("Fin du programme.");

    	FullUpdate();
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
