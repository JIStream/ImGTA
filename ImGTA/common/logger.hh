#pragma once

#include <cstdio>
#include <string>

namespace ImGTA {

    class Logger
    {
        static inline FILE* mFile = nullptr;
        static FILE* GetLogFile();

    public:
        static void LogMessage(const char* format, ...);
        static void AddEmptyLine(int count);

        static uint32_t GetGameBuild();
    };

} // namespace Rainbomizer