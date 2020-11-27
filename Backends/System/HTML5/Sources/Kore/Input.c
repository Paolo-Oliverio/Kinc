#include "pch.h"
#include <kinc/input/mouse.h>
#include <kinc/input/keyboard.h>
#include <kinc/input/surface.h>
#include <kinc/input/gamepad.h>
#include <emscripten/emscripten.h>

EM_JS(bool, js_gamepad_connected, (int id), {
	return kjs_gamepad_connected(id);
});

EM_JS(const char *, js_gamepad_product_name, (int gamepad), {
	return kjs_gamepad_product_name(gamepad);
});

//Mouse Api
EMSCRIPTEN_KEEPALIVE
void js_event_mouse(int event, int button, int x, int y)
{
	switch (event)
	{
		case 0:
			kinc_internal_mouse_trigger_press(0, button, x, y);
			break;
		case 1:
			kinc_internal_mouse_trigger_release(0, button, x, y);
			break;
		case 2:
			kinc_internal_mouse_trigger_move(0, x, y);
			break;
	}
}

EMSCRIPTEN_KEEPALIVE
void js_event_wheel(int delta)
{
	kinc_internal_mouse_trigger_scroll(0, delta);
}
void kinc_internal_mouse_lock(int window) {}
void kinc_internal_mouse_unlock(int window) {}
bool kinc_mouse_can_lock(int window) {
	return false;
}
void kinc_mouse_show() {}
void kinc_mouse_hide() {}
void kinc_mouse_set_position(int window, int x, int y) {}
void kinc_mouse_get_position(int window, int *x, int *y) {}

//Keyboard Api
EMSCRIPTEN_KEEPALIVE
void js_event_key(int event,int key)
{
	switch (event){
		case 0:kinc_internal_keyboard_trigger_key_down(key);break;
		case 1:kinc_internal_keyboard_trigger_key_up(key);break;	
		case 2:kinc_internal_keyboard_trigger_key_press(key);break;
	}
}

//Touch Api
EMSCRIPTEN_KEEPALIVE
void js_event_touch(int event, int index, int x, int y)
{
	switch (event)
	{
		case 0:
			kinc_internal_surface_trigger_touch_start(index, x, y);
			if(index == 0){
				kinc_internal_mouse_trigger_press(0,0,x,y);
			}
			break;
		case 1:
			kinc_internal_surface_trigger_move(index, x, y);
			if(index == 0){
				kinc_internal_mouse_trigger_release(0,0,x,y);
			}
			break;
		case 2:
			kinc_internal_surface_trigger_touch_end(index, x, y);
			if(index == 0){
				kinc_internal_mouse_trigger_move(0,x,y);
			}
			break;
	}
}

//Gamepad Api.
EMSCRIPTEN_KEEPALIVE
void js_event_gamepad_axis (int pad, int axis, float value){
	kinc_internal_gamepad_trigger_axis(pad, axis, value);
}
EMSCRIPTEN_KEEPALIVE
void js_event_gamepad_button (int pad, int button, float value){
	kinc_internal_gamepad_trigger_button(pad, button, value);
}


bool kinc_gamepad_connected(int gamepad){
	return js_gamepad_connected(gamepad);
}
//TODO check if they can be implemented in js directly.
const char *kinc_gamepad_vendor(int gamepad){
	return "";
}

const char *kinc_gamepad_product_name(int gamepad){
	return js_gamepad_product_name(gamepad);
}
