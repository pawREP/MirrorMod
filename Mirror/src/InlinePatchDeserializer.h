#pragma once
#include "B3L/Cast.h"
#include "B3L/ImageView.h"
#include "B3L/InlinePatch.h"
#include "ModuleScanner.h"
#include "nlohmann/json.hpp"
#include <B3L/AOBScanner.h>
#include <B3L/DeepPointer.h>

struct InlinePatchDescriptor {
    int64_t offset{};
    std::optional<B3L::AOBPattern> pattern;
    std::optional<std::string> assembly;
    std::optional<std::vector<uint8_t>> bytes;
};

std::unique_ptr<B3L::InlinePatch> fromPatchDesc(const B3L::ImageView& image, const InlinePatchDescriptor& desc);

NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<InlinePatchDescriptor> {
    static void from_json(const json& j, InlinePatchDescriptor& desc) {

        if(!j.contains("Offset") || !j["Offset"].is_number_integer())
            throw std::runtime_error("adl_serializer<InlinePatchDescriptor>: Offset key missing or invalid");

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
                    throw std::runtime_error("adl_serializer<InlinePatchDescriptor>: invalid byte string");

                desc.bytes = bytes.value();
            }
        }

        if(!desc.assembly && !desc.bytes) // Need at least one
            throw std::runtime_error("adl_serializer<InlinePatchDescriptor>: neither assembly nor bytes key present");
    }
};
NLOHMANN_JSON_NAMESPACE_END