#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "request.hpp"

namespace response {
    std::vector<uint8_t> generate(const Request* req);
}
