#pragma once

namespace orion
{
	#define I8_MIN -128
	#define I8_MAX 127
	#define I16_MIN -32768
	#define I16_MAX 32767
	#define I32_MIN -2'147'483'648
	#define I32_MAX 2'147'483'647
	#define I64_MIN -9'223'372'036'854'775'808
	#define I64_MAX 9'223'372'036'854'775'807

	#define U8_MIN 0
	#define U8_MAX 255
	#define U16_MIN 0
	#define U16_MAX 65535
	#define U32_MIN 0
	#define U32_MAX 4'294'967'295
	#define U64_MIN 0
	#define U64_MAX 18'446'744'073'709'551'615

	#define F32_MIN -3.40282347e+38
	#define F32_MAX 3.40282347e+38
	#define F32_INFINITY 1.0f / 0.0f
	#define F32_NAN 0.0f / 0.0f
	#define F32_EPSILON 1.1920929e-7

	#define F64_MIN -1.7976931348623157e+308
	#define F64_MAX 1.7976931348623157e+308
	#define F64_INFINITY 1.0 / 0.0
	#define F64_NAN 0.0 / 0.0
	#define F64_EPSILON 2.2204460492503131e-16
}
