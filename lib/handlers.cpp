#include <iostream>
#include <libinput.h>
#include <linux/input-event-codes.h>
#include <napi.h>

#include "lib.h"

void handlers::move(InputState* ctx, libinput_event *event) {
    auto pointer = libinput_event_get_pointer_event(event);

    ctx->mouse_x += libinput_event_pointer_get_dx(pointer);
    ctx->mouse_y += libinput_event_pointer_get_dy(pointer);
}

void handlers::mousebtn(InputState* ctx, libinput_event *event) {
    auto pointer = libinput_event_get_pointer_event(event);

    uint32_t button_id = libinput_event_pointer_get_button(pointer);
    bool isDown = libinput_event_pointer_get_button_state(pointer) == LIBINPUT_BUTTON_STATE_PRESSED;

    if (button_id == BTN_LEFT)
        ctx->left = isDown;
    else if (button_id == BTN_MIDDLE)
        ctx->middle = isDown;
    else if (button_id == BTN_RIGHT)
        ctx->right = isDown;
}

void handlers::wheel(InputState *ctx, libinput_event* event) {
    auto pointer = libinput_event_get_pointer_event(event);

    ctx->scrollOffset_x = libinput_event_pointer_get_axis_value(pointer, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL);
    ctx->scrollOffset_y = libinput_event_pointer_get_axis_value(pointer, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL);
}

void handlers::key(InputState *ctx, libinput_event* event) {
    auto key = libinput_event_get_keyboard_event(event);

    ctx->keys[libinput_event_keyboard_get_key(key)] = libinput_event_keyboard_get_key_state(key) == LIBINPUT_KEY_STATE_PRESSED;

}
