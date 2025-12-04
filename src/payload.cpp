#include "payload.h"

#if defined PLUGIN_ROLE || STANDALONE_ROLE
extern "C" void init(HMODULE h_module);
extern "C" void free();
#endif

namespace pluginloader::payload {

void init(HMODULE h_module) {
#if defined PLUGIN_ROLE || STANDALONE_ROLE
    ::init(h_module);
#else
    UNREFERENCED_PARAMETER(h_module);
#endif
}

void free(void) {
#if defined PLUGIN_ROLE || STANDALONE_ROLE
    ::free();
#endif
}
}
