#pragma once

//
// Debug build
//

#ifdef ORION_BUILD_DEBUG
#define OE_ENABLE_ASSSERTIONS

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

#ifdef ORION_BUILD_RELEASE
#define OE_ENABLE_LOGGING
#define OE_ENABLE_LOG_INFO
#define OE_ENABLE_LOG_WARN
#define OE_ENABLE_LOG_ERROR
#define OE_ENABLE_LOG_FATAL
#endif

//
// Distribution build
//

#ifdef ORION_BUILD_DIST
// Do not log information other than ERROR and FATAL.
#define OE_ENABLE_LOGGING
#define OE_ENABLE_LOG_ERROR
#define OE_ENABLE_LOG_FATAL
#endif
