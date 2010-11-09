
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <libcdsBasics.h>
#include <Array.h>
#include <cppUtils.h>

using namespace std;
using namespace cds_utils;

bool compare(Array * a1, Array * a2) {
    if(a1==NULL || a2==NULL)
        return a1==a2;
    if(a1->getMax() != a2->getMax())
        return false;
    if(a1->getLength() != a2->getLength())
        return false;
    if(a1->getSize() != a2->getSize())
        return false;
    for(size_t i=0;i<a1->getLength();i++)
        if(a1->getField(i)!=(*a2)[i])
            return false;
    return true;
}

void test(const string & function, Array *a1, Array *a2) {
    if(!compare(a1,a2)) {
        cout << "Error in ()" << function << endl;
        abort();
    }
}

void testSaveLoad(Array * a) {
    string dir = "/tmp/";
    string pfx = "lcds";
    char * fname = tempnam(dir.c_str(),pfx.c_str()); //tmpnam(NULL);
    ofstream outfs(fname);
    a->save(outfs);
    outfs.close();
    ifstream infs(fname);
    Array * a_copy = new Array(infs);
    infs.close();
    remove(fname);
    //delete [] fname; // C function uses malloc
    free(fname);
    test("testSaveLoad",a,a_copy);
    delete a_copy;
}

void testConstructors(Array * a) {
    vector<uint> vals_vector;
    uint * vals_array = new uint[a->getLength()];
    for(size_t i=0;i<a->getLength();i++) {
        vals_vector.push_back(a->getField(i));
        vals_array[i] = a->getField(i);
    }
    Array * a2 = new Array(vals_vector);
    test("testConstructor",a,a2);
    delete  a2;
    a2 = new Array(vals_vector.begin(),vals_vector.end());
    test("testConstructor",a,a2);
    delete  a2;
    a2 = new Array(vals_array,a->getLength());
    test("testConstructor",a,a2);
    delete  a2;
    if(a->getLength()>0) {
        a2 = new Array(vals_array,(size_t)0,(size_t)a->getLength()-1,0);
        test("testConstructor",a,a2);
        delete  a2;
    }
    a2 = new Array(a->getLength(),a->getMax());
    for(size_t i=0;i<a->getLength();i++)
        a2->setField(i,a->getField(i));
    test("testConstructor",a,a2);
    delete  a2;
    delete [] vals_array;
}

Array * buildArray(uint seed, size_t len, uint maxv) {
    Array * a = new Array(len,maxv);
    srand(seed);
    for(size_t i=0;i<len;i++) {
        uint val = rand()%(maxv+1);
        a->setField(i,val);
    }
    return a;
}

int main(int argc, char ** argv) {

    Array * a;

    // Test 1: empty array
    cout << "Testing empty array" << endl;
    a = buildArray(57921, 0, 0);
    testSaveLoad(a);
    testConstructors(a);
    delete a;

    // Test 2: binary array
    cout << "Testing binary array" << endl;
    a = buildArray(57921, 100000, 1);
    testSaveLoad(a);
    testConstructors(a);
    delete a;

    // Test 1: 7 bits array
    cout << "Testing 7 bits array" << endl;
    a = buildArray(8647, 100000, 126);
    testSaveLoad(a);
    testConstructors(a);
    delete a;

    // Test 1: big array
    cout << "Testing big array" << endl;
    a = buildArray(8647, 10000000, (1<<23));
    testSaveLoad(a);
    testConstructors(a);
    delete a;

    cout << endl << "Tests passed!" << endl;
    return 0;
}

