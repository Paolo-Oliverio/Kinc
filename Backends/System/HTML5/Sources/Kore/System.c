#include "pch.h"
#ifdef KORE_A2
#include <kinc/audio2/audio.h>
#endif
#include <kinc/graphics4/graphics.h>
#include <kinc/system.h>
#include <kinc/window.h>
#include <emscripten/emscripten.h>

	//int argc;
	//char** argv;
bool initialized = false;

EMSCRIPTEN_KEEPALIVE
void js_loop() {
	if (!initialized) return;
	kinc_internal_update_callback();
#ifdef KORE_A2
	kinc_a2_update();
#endif
}

EM_JS(void, js_init, (int width, int height), {
	kjs_init(width,height);
});

extern int kinc_internal_window_width;
extern int kinc_internal_window_height;

int kinc_init(const char* name, int width, int height, kinc_window_options_t *win, kinc_framebuffer_options_t *frame) {
	kinc_window_options_t defaultWin;
	if (win == NULL) {
		kinc_internal_init_window_options(&defaultWin);
		win = &defaultWin;
	}
	kinc_framebuffer_options_t defaultFrame;
	if (frame == NULL) {
		kinc_internal_init_framebuffer_options(&defaultFrame);
		frame = &defaultFrame;
	}
	win->width = width;
	win->height = height;
	js_init(width, height);
#ifdef KORE_OPENGL
	EM_ASM(kjs_init_gfx_api(););
#endif
	kinc_internal_window_width = width;
	kinc_internal_window_height = height;
	kinc_g4_init(0, frame->depth_bits, frame->stencil_bits, true);
	return 0;

}

bool kinc_internal_handle_messages() {
	return true;
}

void kinc_set_keep_screen_on(bool on) {}

EM_JS(double, kinc_time, (void), {
	return window.performance.now()/1000;
});

double kinc_frequency(void) {
	return 1000.0;
}

kinc_ticks_t kinc_timestamp(void) {
	return (kinc_ticks_t) kinc_time();
}

extern int kickstart(int argc, char** argv);

#ifdef KORE_WEBGPU
EMSCRIPTEN_KEEPALIVE void kinc_internal_webgpu_initialized() {
	kickstart(argc, argv);
	initialized = true;
}
#endif

#include <stdio.h>

EMSCRIPTEN_KEEPALIVE
void js_event_resize(int w,int h){
}

int main(int argc, char** argv) {
#ifdef KORE_WEBGPU
	char* code = "(async () => {\
		const adapter = await navigator.gpu.requestAdapter();\
		const device = await adapter.requestDevice();\
		Module.preinitializedWebGPUDevice = device;\
		_kinc_internal_webgpu_initialized();\
	})();";
	emscripten_run_script(code);
#else
	kickstart(argc, argv);
	initialized = true;
#endif
	EM_ASM(kjs_start_loop(););
}
