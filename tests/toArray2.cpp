/*  toArray.cpp
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<stack>
#include<cassert>
#include<cmath>

using namespace std;

#include <Array.h>
#include <cppUtils.h>

using namespace cds_utils;

int main(int argc, char ** argv) {
    if(argc!=3) {
        cout << "usage: " << argv[0] << " <input> <output>" << endl;
        return 0;
    }

    size_t len = 0;
    int * content = loadValue<int>(argv[1],len);
    uint * content2 = new uint[len];
    size_t pos = 0;
    for(size_t i=3;i<len;i++)
        if(content[i]>0) content2[pos++]=content[i];

    Array a(content2,pos);
    ofstream out(argv[2]);
    a.save(out);
    out.close();

    delete [] content;
    delete [] content2;
    return 0;
}



