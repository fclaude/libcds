
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

    template <typename T> T * loadValue(const char * name, size_t & slen) {
        ifstream in(name);
        assert(in.good());
        in.seekg(0,ios_base::end);
        size_t len = in.tellg();
        slen = len/sizeof(T);
        if(len%sizeof(T)) slen--;
        in.seekg(0,ios_base::beg);
				T * ret = loadValue<T>(in,slen);
        in.close();
        return ret;
    }

};
#endif
