/*******************************************************************************
@ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
@ filename:	   log.cc
@ author:	   Connor
@ version:	   1.0.0
@ date:		   2017-05-01
@ description: log.
@ others:
@ history:
1.date:
author:
modification:
********************************************************************************/

#include "log.h"

#ifdef _WIN32
#include "windows.h"
#endif // _WIN32

namespace gamer {

void log::printfgreen(const char* format, ...) {
	va_list args;
	va_start(args, format);
	char buf[256];
	vsnprintf(buf, sizeof(buf), format, args);
#ifdef _WIN32
	auto h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | (int)log::Colors::COLOR_GREEN);
	//std::cout << buf << std::endl;
	std::cout << buf;
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | (int)log::Colors::COLOR_WHITE);
#else
	// TODO : other platform
#endif
	va_end(args);
}

void log::printferr(const char* format, ...) {
	va_list args;
	va_start(args, format);
	char buf[256];
	vsnprintf(buf, sizeof(buf), format, args);
#ifdef _WIN32
	auto h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | (int)log::Colors::COLOR_RED);
	//std::cout << buf << std::endl;
	std::cout << buf;
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | (int)log::Colors::COLOR_WHITE);
#else
	// TODO : other platform
#endif
	va_end(args);
}

void log::printfc(Colors c, const char* format, ...) {
	va_list args;
	va_start(args, format);
	char buf[256];
	vsnprintf(buf, sizeof(buf), format, args);
#ifdef _WIN32
	auto h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | (int)c);
	//std::cout << buf << std::endl;
	std::cout << buf;
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | (int)Colors::COLOR_WHITE);
#else
	// TODO : other platform
#endif
	va_end(args);
}

void log::printf(const char* s) {
	while (*s) {
		if (*s == '%' && *(++s) != '%')
			throw std::runtime_error("invalid format string: missing arguments");
		std::cout << *s++;
	}
}

} // namespace gamer