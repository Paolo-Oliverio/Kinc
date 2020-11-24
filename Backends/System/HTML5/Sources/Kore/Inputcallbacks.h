#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/input/surface.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

/*
void onKeyPressed(int key, int action) {
		if (action == GLFW_PRESS) {
			switch (key) {
			case 87:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_W);
				break;
			case 65:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_A);
				break;
			case 83:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_S);
				break;
			case 68:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_D);
				break;
			case 32:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_SPACE);
				break;
			case 262:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_RIGHT);
				break;
			case 263:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_LEFT);
				break;
			case 265:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_UP);
				break;
			case 264:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_DOWN);
				break;
			case 256:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_ESCAPE);
				break;
			case 82:
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_R);
				break;
			}
		}
		else {
			switch (key) {
			case 87:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_W);
				break;
			case 65:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_A);
				break;
			case 83:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_S);
				break;
			case 68:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_D);
				break;
			case 32:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_SPACE);
				break;
			case 262:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_RIGHT);
				break;
			case 263:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_LEFT);
				break;
			case 265:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_UP);
				break;
			case 264:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_DOWN);
				break;
			case 256:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_ESCAPE);
				break;
			case 82:
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_R);
				break;
			}
		}
	}

	int mouseX = 0;
	int mouseY = 0;*/
/*
	void onMouseClick(int button, int action) {
		if (action == GLFW_PRESS) {
			if (button == 0) {
				kinc_internal_mouse_trigger_press(0, 0, mouseX, mouseY);
			}
			else if (button == 1) {
				kinc_internal_mouse_trigger_press(0, 1, mouseX, mouseY);
			}
		}
		else {
			if (button == 0) {
				kinc_internal_mouse_trigger_release(0, 0, mouseX, mouseY);
			}
			else if (button == 1) {
				kinc_internal_mouse_trigger_release(0, 1, mouseX, mouseY);
			}
		}
	}

	void onMouseMove(int x, int y) {
		mouseX = x;
		mouseY = y;
		kinc_internal_mouse_trigger_move(0, x, y);
	}
*/




EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
{ 
   // auto x = emscripten_get_device_pixel_ratio();
    switch (eventType) {
        case EMSCRIPTEN_EVENT_MOUSEDOWN:
            kinc_internal_mouse_trigger_press(0, e->button, e->targetX, e->targetY);
        break;
        case EMSCRIPTEN_EVENT_MOUSEUP:
            kinc_internal_mouse_trigger_release(0, e->button, e->targetX, e->targetY);
        break;
        case EMSCRIPTEN_EVENT_MOUSEMOVE:
            kinc_internal_mouse_trigger_move(0, e->targetX, e->targetY);
        break;
        default:
        break;
    }
    return 0;
}

/*
EM_BOOL touch_callbackEM_BOOL (int eventType, const EmscriptenTouchEvent *touchEvent, void *userData);
   // auto x = emscripten_get_device_pixel_ratio();
    switch (eventType) {
        case EMSCRIPTEN_EVENT_MOUSEDOWN:
            kinc_internal_mouse_trigger_press(0, e->button, e->targetX, e->targetY);
        break;
        case EMSCRIPTEN_EVENT_MOUSEUP:
            kinc_internal_mouse_trigger_release(0, e->button, e->targetX, e->targetY);
        break;
        case EMSCRIPTEN_EVENT_MOUSEMOVE:
            kinc_internal_mouse_trigger_move(0, e->targetX, e->targetY);
        break;
        default:
        break;
    }
    return 0;
}*/


EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent *e, void *userData)
{
    kinc_internal_mouse_trigger_scroll(0, e->deltaY * 0.01);
    return 0;
}


void setup_mouse_callbacks()
{
    const char * c = "#canvas";
    emscripten_set_mousedown_callback(c,    0, 1, mouse_callback);
    emscripten_set_mouseup_callback(c,      0, 1, mouse_callback);
    emscripten_set_mousemove_callback(c,    0, 1, mouse_callback);
   // emscripten_set_touchstart_callback(c,   0, 1, touch_callback);
   // emscripten_set_touchend_callback(c,     0, 1, touch_callback);
   // emscripten_set_touchmove_callback(c,    0, 1, touch_callback);
    emscripten_set_wheel_callback(c,        0, 1, wheel_callback);
}

