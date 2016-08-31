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


// HTTP request + result stored in memory
static void http_request_01(void)
{
	char buffer[2048];

	const char *url = "http://checkip.amazonaws.com/";
	int retsize;
	char *cookie = NULL;
	char *post = NULL;

	snprintf(buffer, sizeof(buffer), "HTTP Request to %s", url);
	log_message(buffer);

	void *result = QuickDownloadExt(url, &retsize, 15, cookie, post);

	snprintf(buffer, sizeof(buffer), "Response size: %d", retsize);
	log_message(buffer);

	log_message("Response content:");
	snprintf(buffer, sizeof(buffer), "Response content: %.1024s", (char *)result);
	log_message(buffer);

	free(result);
}


// HTTP request + result written to a file
static void http_request_02()
{
	char buffer[2048];

	int session = NewSession();

	// We always get a pointer to the same memory space.
	// At first, it's not initialized by the ereader
	// => We initialize to 0 so we can detect changes later on (esp. when download is finished)
	iv_sessioninfo *sinf = GetSessionInfo(session);
	sinf->response = 0;

	const char *url = "https://blog.pascal-martin.fr/post/directives-ini-c-est-le-mal.html";
	//const char *url = "http://bit.ly/2bPtNry";
	const char *postdata = NULL;
	const char *filename = USERDATA TEMPDIR "/debug.log";

	// Delete the file (if exists) before it's recreated while downloading
	// (Makes it easier to debug: if the file is there later on, it's because of the download)
	int result_unlink = iv_unlink(filename);
	snprintf(buffer, sizeof(buffer), "Remove file %s => %d", filename, result_unlink);
	log_message(buffer);

	SetUserAgent(session, "My nice user-agent");

	// Start downloading!
	int result_download = DownloadTo(session, url, postdata, filename, 15);
	int session_status = GetSessionStatus(session);
	sinf = GetSessionInfo(session);

	// Polling, as long as the download is not finished (with a counter for security)
	// session_status : must be some kind of indicator "the download is really in progress or not"
	//    - 0 at first, when the download begins
	//    - 2 during part of the download
	//    - back to 0 at the end of the download, if it did something (be it 200 or 404) ; or remains at 2 actually ???
	//    - stays at 2 if the requests ends up with a 301
	//    - goes to -21 when there is a DNS error
	// sinf->response :
	//    even if initialized to something else than 0,
	//    the ereader sets it to 0 when session_status goes to 2
	//    Set to the HTTP status code, at the end of the request
	// sinf->url :
	//    - Set to the resulting URL: the URL of the loaded page
	//    - or the redirect URL (which is not followed!) if HTTP status is 301
	int i = 0;
	while (i<5000 && session_status >= 0 && sinf->response == 0) {
		if (i%250 == 0) {
			snprintf(buffer, sizeof(buffer), "  Waiting... #%d ; settion_status=%d ; sinf->response=%ld ; length=%d ; progress=%d", i, session_status, sinf->response, sinf->length, sinf->progress);
			log_message(buffer);
		}

		// I was hoping this would ask the ereader to wait for a bit...
		// But it doesn't seem to be doing much ;-(
		GoSleep(250, 1);

		session_status = GetSessionStatus(session);
		sinf = GetSessionInfo(session);
		i++;
	}

	session_status = GetSessionStatus(session);
	sinf = GetSessionInfo(session);

	snprintf(buffer, sizeof(buffer), "Session #%d (status=%d) ; result_download=%d", session, session_status, result_download);
	log_message(buffer);

	// Let's hope is fits in the buffer... Else, buffer overflow ;-(
	snprintf(buffer, sizeof(buffer), "Infos :: url=%s", sinf->url);
	log_message(buffer);
	snprintf(buffer, sizeof(buffer), "Infos :: ctype=%s response=%ld length=%d progress=%d", sinf->ctype, sinf->response, sinf->length, sinf->progress);
	log_message(buffer);

	// Yeah, we're done downloading!
	CloseSession(session);

	// Read the file to which the downloaded page has been saved
	log_message("Contenu du fichier créé par le download :");
	FILE *fp = iv_fopen(filename, "rb");
	if (fp) {
		int count_read;
		int nb_lignes = 0;
		while ((count_read = iv_fread(buffer, 1, 2048-1, fp)) > 0 && ++nb_lignes < 5) {
			buffer[count_read] = '\0';
			log_message(buffer);
		}
		iv_fclose(fp);
	}

	log_message("End of 2nd try.");
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
	// Warning: the received data it not NUL-terminated
	char buffer[1024];

	int data_size = size * nmemb;
	snprintf(buffer, sizeof(buffer), "  Data %d bytes : %.128s", data_size, ptr);
	log_message(buffer);

	// Even if we didn't display everything, we signal the system we used all received data
	return data_size;
}

static void http_request_03()
{
	char buffer[2048];

	log_message("Start 3rd try (curl).");

	CURL *curl = curl_easy_init();
	if (!curl) {
		log_message("Failed initializing curl");
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
		snprintf(buffer, sizeof(buffer), "Error %d : %s", res, curl_easy_strerror(res));
		log_message(buffer);

		goto end;
	}

	end:
	curl_easy_cleanup(curl);

	log_message("End 3rd try (curl).");
}


static int main_handler(int event_type, int param_one, int param_two)
{
	if (EVT_INIT == event_type) {
    	font = OpenFont("LiberationSans", kFontSize, 0);
    	SetFont(font, BLACK);

    	ClearScreen();
    	FullUpdate();

    	y_log = 0;

    	log_message("Starting application...");
    	http_request_01();
    	http_request_02();
    	http_request_03();
    	log_message("End of application.");

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
