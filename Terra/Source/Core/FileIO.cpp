#include "FileIO.h"
#include <filesystem>

namespace Terra
{
    namespace fs = std::filesystem;
    std::string FileIO::GetEngineFile(const std::string& path)
    {
        return fs::current_path().string() + "\\Assets\\Terra\\" + path;
    }

    std::string FileIO::GetGameFile(const std::string& path)
    {
        return fs::current_path().string() + "\\Assets\\Game\\" + path;
    }
}
