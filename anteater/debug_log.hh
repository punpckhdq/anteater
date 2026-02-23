#pragma once

#define DEBUG_LOG(...) debug_log("anteater -> " __VA_ARGS__)

void debug_log(const char* format, ...);
