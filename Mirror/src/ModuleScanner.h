#pragma once
#include "B3L/AOBScanner.h"
#include "B3L/ImageView.h"
#include <optional>

namespace ModuleScanner {

    std::optional<uintptr_t> find(const B3L::ImageView& imageView, const B3L::AOBPattern& pattern);

};
