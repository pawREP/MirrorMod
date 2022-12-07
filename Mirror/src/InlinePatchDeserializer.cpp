#include "InlinePatchDeserializer.h"

std::unique_ptr<B3L::InlinePatch> fromPatchDesc(const B3L::ImageView& image, const InlinePatchDescriptor& desc) {
    intptr_t target = 0;

    if(desc.pattern) {
        auto match = ModuleScanner::find(image, desc.pattern.value());
        if(!match)
            throw std::runtime_error("Failed to find InlinePatchDescriptor pattern");
        target = match.value() + desc.offset;
    } else {
        target = image.baseAddress() + desc.offset;
    }

    if(desc.bytes)
        throw std::runtime_error("Bytes unsupported.");

    assert(desc.assembly);

    return std::make_unique<B3L::InlinePatch>(B3L::rcast<uint8_t*>(target), desc.assembly.value());
};