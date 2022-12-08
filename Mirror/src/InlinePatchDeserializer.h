#pragma once
#include "B3L/Cast.h"
#include "B3L/ImageView.h"
#include "B3L/InlinePatch.h"
#include "ModuleScanner.h"
#include "nlohmann/json.hpp"
#include <B3L/AOBScanner.h>
#include <B3L/DeepPointer.h>

struct PatchDescriptor {
    int64_t offset{};
    std::optional<B3L::AOBPattern> pattern;
    std::optional<std::string> assembly;
    std::optional<std::vector<uint8_t>> bytes;
};

template <typename PatchType>
std::unique_ptr<PatchType> fromPatchDesc(const B3L::ImageView& image, const PatchDescriptor& desc) {
    intptr_t target = 0;

    if(desc.pattern) {
        auto match = ModuleScanner::find(image, desc.pattern.value());
        if(!match)
            throw std::runtime_error("Failed to find PatchDescriptor pattern");
        target = match.value() + desc.offset;
    } else {
        target = image.baseAddress() + desc.offset;
    }

    if(desc.bytes)
        throw std::runtime_error("Bytes unsupported.");

    assert(desc.assembly);

    return std::make_unique<PatchType>(B3L::rcast<uint8_t*>(target), desc.assembly.value());
}


NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<PatchDescriptor> {
    static void from_json(const json& j, PatchDescriptor& desc) {

        if(!j.contains("Offset") || !j["Offset"].is_number_integer())
            throw std::runtime_error("adl_serializer<PatchDescriptor>: Offset key missing or invalid");

        desc.offset = j["Offset"].get<int64_t>();

        if(j.contains("Pattern") && j["Pattern"].is_string()) {
            auto patternString = j["Pattern"].get<std::string>();
            desc.pattern       = B3L::AOBPattern::fromString(patternString).value();
        }

        if(j.contains("Assembly") && j["Assembly"].is_string())
            desc.assembly = j["Assembly"].get<std::string>();

        if(j.contains("Bytes") && j["Bytes"].is_string()) {
            auto bytesStr = j["Bytes"].get<std::string>();
            if(!bytesStr.empty()) {
                auto bytes = B3L::parseByteArrayString(bytesStr);
                if(!bytes)
                    throw std::runtime_error("adl_serializer<PatchDescriptor>: invalid byte string");

                desc.bytes = bytes.value();
            }
        }

        if(!desc.assembly && !desc.bytes) // Need at least one
            throw std::runtime_error("adl_serializer<PatchDescriptor>: neither assembly nor bytes key present");
    }
};
NLOHMANN_JSON_NAMESPACE_END