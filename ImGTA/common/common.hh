#pragma once

#include <vector>
#include <functional>
#include <cstdio>
#include <string>

class gameSkeleton;

namespace ImGTA {
    class Common
    {
    public:
        static std::string GetRainbomizerFileName(const std::string& name,
            const std::string& subdirs = "",
            bool               temp = false);

        static FILE* GetRainbomizerFile(const std::string& name,
            const std::string& mode,
            const std::string& subdirs = "",
            bool               tempFallback = false);

        static FILE* GetRainbomizerDataFile(const std::string& name,
            const std::string& mode = "r");
    };
} // namespace ImGTA