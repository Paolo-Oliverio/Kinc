#include "pch.h"
#include <kinc/display.h>>
#include <emscripten/emscripten.h>

kinc_display_mode_t actualMode;

EMSCRIPTEN_KEEPALIVE
void js_update_display(int x,int y,int width,int height, int ppi,int freq,int bbp)
{
    actualMode.x = x;
    actualMode.x = y;
    actualMode.width = width;
    actualMode.height = height;
    actualMode.pixels_per_inch = ppi;
    actualMode.frequency = freq;
    actualMode.bits_per_pixel = bbp;
}

void kinc_display_init(void){}

int kinc_primary_display(void){
    return 0;
}

int kinc_count_displays(void){ 
    return 1;
}

bool kinc_display_available(int display_index)
{
    return display_index == 0;
}

const char *kinc_display_name(int display_index)
{
    return "browser";
}

kinc_display_mode_t kinc_display_current_mode(int display_index)
{
    //TODO assertion
    return actualMode;
}

int kinc_display_count_available_modes(int display_index)
{
    return 1;
}

kinc_display_mode_t kinc_display_available_mode(int display_index, int mode_index)
{
    //TODO assetion
    return actualMode;
}