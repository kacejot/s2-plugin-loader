#ifndef DISPATCH_H
#define DISPATCH_H

#include "pch.h"

#include "console.h"
#include "loader.h"
#include "proxy/proxy.h"
#include "payload.h"

enum class loading_role {
    LOADER,
    PLUGIN,
    STANDALONE
};

#if defined(PLUGIN_ROLE)
    inline constexpr loading_role MODE = loading_role::PLUGIN;
#elif defined(STANDALONE_ROLE)
    inline constexpr loading_role MODE = loading_role::STANDALONE;
#else
    inline constexpr loading_role MODE = loading_role::LOADER;
#endif

namespace pluginloader::dispatch {

template<loading_role M>
inline void on_process_attach(HMODULE) {
    static_assert(M != M, "Specialization missing for this loading_role");
}

template<>
inline void on_process_attach<loading_role::LOADER>(HMODULE h_module) {
    pluginloader::console::create_if_needed();
    pluginloader::proxy::init(h_module);
    pluginloader::loader::load(h_module);
}

template<>
inline void on_process_attach<loading_role::PLUGIN>(HMODULE h_module) {
    pluginloader::payload::init(h_module);
}

template<>
inline void on_process_attach<loading_role::STANDALONE>(HMODULE h_module) {
    pluginloader::console::create_if_needed();
    pluginloader::proxy::init(h_module);
    pluginloader::payload::init(h_module);
}


template<loading_role M>
inline void on_process_detach() {
}

template<>
inline void on_process_detach<loading_role::LOADER>() {
    pluginloader::loader::free();
    pluginloader::proxy::free();
}

template<>
inline void on_process_detach<loading_role::PLUGIN>() {
     pluginloader::payload::free();
}

template<>
inline void on_process_detach<loading_role::STANDALONE>() {
    pluginloader::proxy::free();
    pluginloader::payload::free();
}

}

#endif /* DISPATCH_H */ 
