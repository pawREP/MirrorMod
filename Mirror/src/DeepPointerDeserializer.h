#pragma once
#include "B3L/AOBScanner.h"
#include "B3L/DeepPointer.h"
#include "B3L/ImageView.h"
#include "B3L/Process.h"
#include "ModuleScanner.h"
#include "nlohmann/json.hpp"
#include <string>

struct DeepPointerDescriptor {
    std::vector<int> offsets;
    std::optional<B3L::AOBPattern> pattern;
    std::optional<int> patternOffset;
};

template <typename T>
B3L::DeepPointer<T> fromPatchDesc(const B3L::ImageView& image, const DeepPointerDescriptor& desc) {
    std::vector<int> offsets = desc.offsets;

    if(desc.pattern) {
        auto basematch = ModuleScanner::find(image, desc.pattern.value());
        if(!basematch)
            throw std::runtime_error("Failed to find DeepPointerDescriptor pattern");

        auto match      = basematch.value() + desc.patternOffset.value();
        auto baseOffset = match + sizeof(int) + *reinterpret_cast<int*>(match) - image.baseAddress();

        offsets.insert(offsets.begin(), B3L::domain_cast<int>(baseOffset));
    }

    return B3L::DeepPointer<T>(B3L::rcast<void*>(image.baseAddress()), offsets);
}

NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<DeepPointerDescriptor> {
    static void from_json(const json& j, DeepPointerDescriptor& desc) {
        if(j.contains("Pattern") && j["Pattern"].is_object()) {
            if(!j["Pattern"].contains("Bytes") || !j["Pattern"]["Bytes"].is_string())
                throw std::runtime_error("adl_serializer<B3L::DeepPointer<T>>: Invalid or missing Pattern: Bytes key");
            if(!j["Pattern"].contains("Offset") || !j["Pattern"]["Offset"].is_number_integer())
                throw std::runtime_error("adl_serializer<B3L::DeepPointer<T>>: Invalid or missing Pattern: Offset key");

            auto patternString = j["Pattern"]["Bytes"].get<std::string>();
            auto pattern       = B3L::AOBPattern::fromString(patternString);
            if(!pattern)
                throw std::runtime_error("adl_serializer<B3L::DeepPointer<T>>: Invalid Pattern Byte string");

            desc.pattern       = pattern.value();
            desc.patternOffset = j["Pattern"]["Offset"].get<int>();
        }

        if(!j.contains("Offsets") || !j["Offsets"].is_array())
            throw std::runtime_error("adl_serializer<B3L::DeepPointer<T>>: invalid Offset key");

        desc.offsets = j["Offsets"].get<std::vector<int>>();
    }
};
NLOHMANN_JSON_NAMESPACE_END
