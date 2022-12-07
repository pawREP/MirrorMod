#include "Configuration.h"
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>

Configuration::Configuration(const std::string& gameName, uint32_t versionId) {
    const auto configFilePath = std::filesystem::current_path() / configFile;
    if(!std::filesystem::exists(configFilePath) || !std::filesystem::is_regular_file(configFilePath))
        throw std::runtime_error("Config file not found");

    std::ifstream ifs(configFilePath);
    if(!ifs.is_open())
        throw std::runtime_error("Failed to open config file");

    std::stringstream buffer;
    buffer << ifs.rdbuf();

    root = nlohmann::json::parse(buffer.str());

    if(!root.contains(gameName))
        throw std::runtime_error("Game name not found");
    if(!root[gameName].contains("Version"))
        throw std::runtime_error("Version not found");

    auto versionIdString = std::to_string(versionId);
    if(!root[gameName]["Version"].contains(versionIdString))
        versionIdString = "latest";
    if(!root[gameName]["Version"].contains(versionIdString))
        throw std::runtime_error("Version ID not found and forward compatibility not supported");

    auto version = root[gameName]["Version"][versionIdString].get<std::string>();
    config       = &root[gameName]["Configuration"][version];
}