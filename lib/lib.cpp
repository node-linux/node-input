#include <ranges>
#include <fcntl.h>
#include <iostream>
#include <libinput.h>
#include <libudev.h>
#include <napi.h>
#include <poll.h>
#include <string>
#include <thread>
#include <unistd.h>

#include "./lib.h"

const static struct libinput_interface interface = {.open_restricted = [](const char *path, int flags, void *user_data) -> int {
                                                        int fd = open(path, flags);
                                                        return fd < 0 ? -errno : fd;
                                                    },
                                                    .close_restricted = [](int fd, void *user_data) { close(fd); }};

static void init_event(const Napi::CallbackInfo &info) {
    udev *udev_ctx = udev_new();
    libinput *li = libinput_udev_create_context(&interface, NULL, udev_ctx);
    int fd = -1;

    if (libinput_udev_assign_seat(li, "seat0") != 0)
        std::cerr << "An error occurred initialising libinput\n";

    else if ((fd = libinput_get_fd(li)) < 1)
        std::cerr << "An error occurred opening filedescriptor\n";

    Napi::ThreadSafeFunction callback = Napi::ThreadSafeFunction::New(info.Env(), info[0].As<Napi::Function>(), "callback", 0, 1);

    std::thread([=]() {

        struct InputState *ctx = new InputState {};
        auto buildState = [=](Napi::Env env) -> Napi::Object {

            Napi::Object state = Napi::Object::New(env);

            state.Set("mouse_x", Napi::Number::New(env, ctx->mouse_x));
            state.Set("mouse_y", Napi::Number::New(env, ctx->mouse_y));

            state.Set("left", Napi::Boolean::New(env, ctx->left));
            state.Set("middle", Napi::Boolean::New(env, ctx->middle));
            state.Set("right", Napi::Boolean::New(env, ctx->right));

            state.Set("scroll_x", Napi::Number::New(env, ctx->scrollOffset_x));
            state.Set("scroll_y", Napi::Number::New(env, ctx->scrollOffset_y));

            Napi::Object keys = Napi::Object::New(env);

            for (const auto& [key, value] : ctx->keys)
                keys.Set(key, Napi::Boolean::New(env, value));

            state.Set("keys", keys);

            return state;

        };

        while (true) {
            std::array<struct pollfd, 1> pollFds{{fd, POLLIN, 0}};

            if (poll(pollFds.data(), pollFds.size(), -1) < 0)
                return;

            libinput_dispatch(li);

            for (struct libinput_event *event = libinput_get_event(li); event != nullptr; event = libinput_get_event(li))
                callback.NonBlockingCall(event, [=](Napi::Env env, Napi::Function callback, struct libinput_event *event) {
                    libinput_event_type eventType = libinput_event_get_type(event);

                    if (eventType == LIBINPUT_EVENT_POINTER_MOTION)
                        handlers::move(ctx, event);
                    else if (eventType == LIBINPUT_EVENT_POINTER_BUTTON)
                        handlers::mousebtn(ctx, event);
                    else if (eventType == LIBINPUT_EVENT_POINTER_AXIS)
                        handlers::wheel(ctx, event);
                    else if (eventType == LIBINPUT_EVENT_KEYBOARD_KEY)
                        handlers::key(ctx, event);

                    callback.Call({ Napi::Number::New(env, eventType), buildState(env) });
                    libinput_event_destroy(event);
                });
        }
    }).detach();
}

void poll_events(libinput *li, int fd, Napi::Function callback) {
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("init", Napi::Function::New(env, init_event));

    return exports;
}

NODE_API_MODULE(input, Init)
