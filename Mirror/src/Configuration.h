#pragma once
#include "nlohmann/json.hpp"
#include <string>

class Configuration {
public:
    Configuration() = default;
    Configuration(const std::string& gameName, uint32_t versionId);

    const nlohmann::json& operator[](const std::string& key) const {
        return config->operator[](key);
    }

private:
    nlohmann::json root;
    const nlohmann::json* config = nullptr;

    static inline const char* configFile = "MirrorModConfig.json";
};
