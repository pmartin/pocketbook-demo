#include "inkview.h"

static const int kFontSize = 42;

static int main_handler(int event_type, int param_one, int param_two)
{
    if (EVT_INIT == event_type) {
        ifont *font = OpenFont("LiberationSans", kFontSize, 0);

        ClearScreen();

        // Everything here is done to a buffer
        SetFont(font, BLACK);
        DrawLine(0, 25, ScreenWidth(), 25, 0x00333333);
        DrawLine(0, ScreenHeight() - 25, ScreenWidth(), ScreenHeight() - 25, 0x00666666);
        FillArea(50, 250, ScreenWidth() - 50*2, ScreenHeight() - 250*2, 0x00E0E0E0);
        FillArea(100, 300, ScreenWidth() - 100*2, ScreenHeight() - 300*2, 0x00A0A0A0);
        DrawTextRect(0, ScreenHeight()/2 - kFontSize/2, ScreenWidth(), kFontSize, "Hello, world!", ALIGN_CENTER);

        // Copies the buffer to the real screen
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
