
#include <libcdsBasics.h>
#include <cppUtils.h>

namespace cds_utils
{

    uint transform(const string & s) {
        stringstream ss;
        ss << s;
        uint ret;
        ss >> ret;
        return ret;
    }

};
