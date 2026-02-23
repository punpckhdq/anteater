#include "anteater_private_pch.hh"

void debug_log(const char* format, ...)
{
	char buffer[4096] = { 0 };

	va_list args;
	__crt_va_start(args, format);
	vsnprintf(buffer, _countof(buffer), format, args);
	__crt_va_end(args);

	OutputDebugStringA(buffer);
}
