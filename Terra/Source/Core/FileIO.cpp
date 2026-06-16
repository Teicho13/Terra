#include "FileIO.h"
#include <filesystem>

namespace Terra
{
    namespace fs = std::filesystem;
    std::string FileIO::GetEngineFile(const std::string& path)
    {
        return fs::current_path().string() + R"(\Assets\Terra\)" + path;
    }

    std::string FileIO::GetGameFile(const std::string& path)
    {
        return fs::current_path().string() + R"(\Assets\Game\)" + path;
    }

    std::string FileIO::GetScuffedMonFile(const std::string& path)
    {
        return fs::current_path().string() + R"(\Assets\ScuffedMon\)" + path;
    }
}
