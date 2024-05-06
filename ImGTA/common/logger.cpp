#include "logger.hh"
#include "common.hh"
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "../utils.h"

#ifndef NDEBUG
#define BUILD_INFO "Debug Build: " __DATE__ " " __TIME__
#else
#define BUILD_INFO "Release TODO CHANGE VERSION TO IMGTA VERSION: " __DATE__ " " __TIME__
#define BUILD_INFO_SHORT "Release TODO CHANGE VERSION TO IMGTA VERSION"
#endif

char* (*rage__formatf6eb9) (char*, char const*, ...);

#ifdef ENABLE_DEBUG_MENU

#include "debug/logger.hh"

#endif

namespace ImGTA {

	static uint32_t sg_GameBuild = 0;

	/*******************************************************/
	std::string
		GetTimeNow()
	{
		time_t currentTime;
		char   str[256];

		time(&currentTime);

		auto tm = std::localtime(&currentTime);
		sprintf(str, "%04d-%02d-%02d", 1900 + tm->tm_year, tm->tm_mon + 1,
			tm->tm_mday);

		return str;
	}

	/*******************************************************/
	FILE*
		Logger::GetLogFile()
	{
		if (!mFile)
		{
			mFile = Common::GetRainbomizerFile(GetTimeNow() + ".txt", "a+",
				"logs/", true);
			if (!mFile)
			{
				MessageBox(NULL, "Failed to open log file for writing.",
					"Error", MB_ICONHAND);
				mFile = stdout;
			}

			fprintf(mFile, "===========================================\n");
			fprintf(mFile, "%d\n", (int)time(NULL));
			fprintf(mFile, "ImGTA V Build: %s \n", BUILD_INFO);
			fprintf(mFile, "===========================================\n");
		}

		return mFile;
	}

	/*******************************************************/
	void
		Logger::LogMessage(const char* format, ...)
	{
		FILE* file = GetLogFile();
		fprintf(file, "[%d]: ", int(time(NULL)));

		va_list args;
		va_start(args, format);
		unsigned int size = vfprintf(file, format, args) + 1;
		va_end(args);

#ifdef ENABLE_DEBUG_MENU
		std::unique_ptr<char[]> buf(new char[size]);
		va_start(args, format);
		vsnprintf(buf.get(), size, format, args);
		va_end(args);

		LoggerDebugInterface::PublishLogMessage(buf.get());
#endif

		fputc('\n', file);
		fflush(file);
	}

	void Logger::AddEmptyLine(int count) {
		for (int i = 0; i < count; i++)
		{
			LogMessage("");
		}
	}

	uint32_t
		Logger::GetGameBuild()
	{
		return sg_GameBuild;
	}
}