#pragma once

namespace orion
{

//
// Debug build
//
#ifdef BUILD_DEBUG
#define OE_ENABLE_ASSERTIONS
#define OE_ENABLE_LOGGING
#define OE_ENABLE_LOG_TRACE
#define OE_ENABLE_LOG_DEBUG
#define OE_ENABLE_LOG_INFO
#define OE_ENABLE_LOG_WARN
#define OE_ENABLE_LOG_ERROR
#define OE_ENABLE_LOG_FATAL
#endif

//
// Release build
//
#ifdef BUILD_RELEASE
#define OE_ENABLE_ASSERTIONS
#define OE_ENABLE_LOGGING
#define OE_ENABLE_LOG_INFO
#define OE_ENABLE_LOG_WARN
#define OE_ENABLE_LOG_ERROR
#define OE_ENABLE_LOG_FATAL
#endif

//
// Distribution build
//
#ifdef BUILD_DISTRIBUTION

#endif
}
