#include <iostream>
#include <math.h>
#include <napi.h>

#include "./lib.h"

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    return exports;
}

NODE_API_MODULE(input, Init)
