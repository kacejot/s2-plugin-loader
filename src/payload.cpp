#include "payload.h"

#if defined PLUGIN_ROLE || STANDALONE_ROLE
extern "C" void init_plugin(HMODULE h_module);
extern "C" void deinit_plugin();
#endif

namespace pluginloader::payload {

void init(HMODULE h_module) {
#if defined PLUGIN_ROLE || STANDALONE_ROLE
    ::init_plugin(h_module);
#else
    UNREFERENCED_PARAMETER(h_module);
#endif
}

void free(void) {
#if defined PLUGIN_ROLE || STANDALONE_ROLE
    ::deinit_plugin();
#endif
}

}
