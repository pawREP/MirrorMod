#include "ModuleScanner.h"
#include "B3L/Cast.h"

std::optional<uintptr_t> ModuleScanner::find(const B3L::ImageView& imageView, const B3L::AOBPattern& pattern) {

    for(int i = 0; i < imageView.sectionCount(); ++i) {
        const auto span = imageView.sectionData(i);
        auto it         = B3L::AOBScanner::find(span.begin(), span.end(), pattern.begin(), pattern.end());
        if(it != span.end())
            return B3L::rcast<uintptr_t>(span.data() + std::distance(span.begin(), it));
    }

    return std::nullopt;
}
