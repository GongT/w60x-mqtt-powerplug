#pragma once

#include <mbedtls/config.h>
#undef MBEDTLS_SELF_TEST
#define mbedtls_platform_set_calloc_free(a, b)
#define DBG_LEVEL DBG_LOG
