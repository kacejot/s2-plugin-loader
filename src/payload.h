#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "pch.h"

namespace pluginloader::payload {

/**
 * @brief Initializes payload. Payload must export next symbol:
 *        extern "C" void init(HMODULE h_module);

 * @param this_dll Handle to this dll's module.
 */
void init(HMODULE h_module);

/**
 * @brief  Calls payload's deinitialization routine. Payload must export next symbol:
 *         extern "C" void free();
 */
void deinit(void);

}

#endif /* #define PAYLOAD_H */ 
