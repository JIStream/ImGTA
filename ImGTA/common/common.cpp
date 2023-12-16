#include "../utils.h"
#include "common.hh"
#include <cstring>
#include <filesystem>
#include "logger.hh"
#include <cstdlib>
#include <system_error>
#include <array>

void (*gameSkeleton__Init) (gameSkeleton*, uint32_t);

namespace ImGTA {

    struct GamePathA
    {
        char _path[160 + 1];
        char _temp_path[160 + 1];

        GamePathA()
        {
            _path[0] = '\0';
            HMODULE h = NULL;
            h = GetModuleHandleA(NULL);
            GetModuleFileNameA(h, _path, MAX_PATH);
            char* bslp = strrchr(_path, '\\');
            char* fslp = strrchr(_path, '/');
            char* slp = (std::max)(bslp, fslp);
            if (slp)
                slp[1] = '\0';
        }
    };

    /*******************************************************/
    char*
        GetGameDirRelativePathA(const char* subpath)
    {
        static GamePathA gamePath;
        strcpy(gamePath._temp_path, gamePath._path);
        strcat(gamePath._temp_path, subpath);
        return gamePath._temp_path;
    }

    /*******************************************************/
    std::string
        Common::GetRainbomizerFileName(const std::string& name,
            const std::string& subdirs, bool temp)
    {
        std::string baseDir
            = GetGameDirRelativePathA(("ImGTA/" + subdirs).c_str());

        if (!std::filesystem::exists(GetGameDirRelativePathA("ImGTA/")))
        {
            MessageBox(NULL,
                "Failed to locate Rainbomizer Data "
                "Directory.\n\nPlease reinstall the mod following "
                "the instructions in the README.",
                "Installation Validation Error", MB_ICONHAND);
            abort();
        }

        if (temp)
            baseDir = std::filesystem::temp_directory_path().string()
            + "/ImGTA/" + subdirs;

        std::error_code ec;
        std::filesystem::create_directories(baseDir, ec);

        return baseDir + name;
    }

    /*******************************************************/
    FILE*
        Common::GetRainbomizerFile(const std::string& name, const std::string& mode,
            const std::string& subdirs, bool tempFallback)
    {
        FILE* file = fopen(GetRainbomizerFileName(name, subdirs).c_str(),
            mode.c_str());

        if (!file && tempFallback)
            return fopen(GetRainbomizerFileName(name, subdirs, true).c_str(),
                mode.c_str());

        return file;
    }

    /*******************************************************/
    FILE*
        Common::GetRainbomizerDataFile(const std::string& name,
            const std::string& mode)
    {
        FILE* f = GetRainbomizerFile(name, mode, "data/");

        if (!f)
            Logger::LogMessage("Failed to read Rainbomizer data file: data/%s: %s",
                name.c_str(), std::strerror(errno));

        return f;
    }
} // namespace ImGTA