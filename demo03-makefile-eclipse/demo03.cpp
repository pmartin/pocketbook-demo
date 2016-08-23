#include "inkview.h"

static const int kFontSize = 42;

static int main_handler(int event_type, int param_one, int param_two)
{
    if (EVT_INIT == event_type) {
        ifont *font = OpenFont("LiberationSans", kFontSize, 0);

        ClearScreen();
        SetFont(font, BLACK);

        DrawTextRect(0, 0, ScreenWidth(), kFontSize, "Hello, World!", ALIGN_LEFT);
        DrawTextRect(0, 30, ScreenWidth(), kFontSize, "Application compiled from Eclipse!", ALIGN_LEFT);
        FullUpdate();

        CloseFont(font);
    }
    else if (EVT_KEYPRESS == event_type) {
        CloseApp();
    }
    return 0;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);
    return 0;
}
