#pragma once

#include <iosfwd>
#include <chrono>

namespace GSC { namespace Utility {

std::string to_string(std::chrono::nanoseconds ns);
  
}}