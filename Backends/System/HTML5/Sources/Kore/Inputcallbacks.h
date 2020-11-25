#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/input/surface.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>




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

EMSCRIPTEN_KEEPALIVE
void js_event_key(int event,int key)
{
	switch (event){
		case 0:kinc_internal_keyboard_trigger_key_down(key);break;
		case 1:kinc_internal_keyboard_trigger_key_up(key);break;	
		case 2:kinc_internal_keyboard_trigger_key_press(key);break;
	}
}


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