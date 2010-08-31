
#ifndef _CPPUTILS
#define _CPPUTILS

#include <libcdsBasics.h>
#include <cppUtils.h>

namespace cds_utils
{

    uint transform(const string & s);

    template <typename T> void saveValue(ofstream & out, const T val) {
        assert(out.good());
        out.write((char*)&val,sizeof(T));
    }

    template <typename T> T loadValue(ifstream & in) {
        assert(in.good());
        T ret;
        in.read((char*)&ret,sizeof(T));
        return ret;
    }

    template <typename T> void saveValue(ofstream & out, const T * val, const size_t len) {
        assert(out.good());
        out.write((char*)val,len*sizeof(T));
    }

    template <typename T> T * loadValue(ifstream & in, const size_t len) {
        assert(in.good());
        T * ret = new T[len];
        in.read((char*)ret,len*sizeof(T));
        return ret;
    }

};
#endif
