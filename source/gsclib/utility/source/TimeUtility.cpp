
#include "../include/TimeUtility.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace GSC
{
    namespace Utility
    {

        std::string to_string(std::chrono::nanoseconds ns)
        {
            typedef std::chrono::duration<int, std::ratio<86400>> days;
            std::ostringstream os;
            os.fill('0');
            auto d = std::chrono::duration_cast<days>(ns);
            ns -= d;
            auto h = std::chrono::duration_cast<std::chrono::hours>(ns);
            ns -= h;
            auto m = std::chrono::duration_cast<std::chrono::minutes>(ns);
            ns -= m;
            auto s = std::chrono::duration_cast<std::chrono::seconds>(ns);
            os << std::setw(2) << d.count() << "d:"
               << std::setw(2) << h.count() << "h:"
               << std::setw(2) << m.count() << "m:"
               << std::setw(2) << s.count() << 's';
            return os.str();
        };

    } // namespace Utility
} // namespace GSC