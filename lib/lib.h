#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <vector>
#include <map>

#include <napi.h>

struct InputState {
    int mouse_x;
    int mouse_y;

    bool left;
    bool middle;
    bool right;

    double scrollOffset_x;
    double scrollOffset_y;

    std::map<unsigned int, bool> keys;
};

static void init_event(const Napi::CallbackInfo &info);
void poll_events(libinput *li, int fd, Napi::Function callback);

namespace handlers {

void move(InputState *ctx, libinput_event *event);
void mousebtn(InputState *ctx, libinput_event *event);
void wheel(InputState *ctx, libinput_event *event);
void key(InputState *ctx, libinput_event *event);

} // namespace handlers

#endif
