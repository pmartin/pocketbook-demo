#include "inkview.h"

static bool is_touched;
static int x, y;
static int old_x, old_y;

static int main_handler(int event_type, int param_one, int param_two)
{
	iv_mtinfo *touch_info;
	bool must_redraw = false;

    if (EVT_INIT == event_type) {
    	old_x = x = ScreenWidth() / 2;
    	old_y = y = ScreenHeight() / 2;
    	must_redraw = true;
    	is_touched = false;

    	ClearScreen();
    	FullUpdate();
    }
    else if (EVT_POINTERDOWN == event_type || EVT_POINTERMOVE == event_type || EVT_POINTERUP == event_type) {
    	old_x = x;
    	old_y = y;

    	touch_info = GetTouchInfo();
    	x = touch_info->x;
    	y = touch_info->y;

    	is_touched = EVT_POINTERDOWN == event_type || EVT_POINTERMOVE == event_type;
    	must_redraw = true;
    }
    else if (EVT_KEYPRESS == event_type) {
        CloseApp();
    }

    if (must_redraw == true) {
    	DrawCircle(old_x, old_y, 15, WHITE);
    	PartialUpdate(old_x - 20, old_y - 20, 20 * 2, 20 * 2);

		DrawCircle(x, y, 15, is_touched ? BLACK : LGRAY);
		PartialUpdate(x - 20, y - 20, 20 * 2, 20 * 2);
    }

    return 0;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);
    return 0;
}
