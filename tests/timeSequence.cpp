/*  timeSequence.cpp
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
#include<set>

#include<Sequence.h>
#include<Array.h>
#include<BitSequence.h>
#include<timing.h>

using namespace std;
using namespace cds_static;
using namespace cds_utils;

uint numqueries = 100000;
uint seed = 8647;

Array extract_alphabet(const Array & values) {
    set<uint> sigma;
    for(size_t i=0;i<values.getLength();i++) {
        sigma.insert(values[i]);
    }
    return Array(sigma.begin(),sigma.end());
}

void measure_select(const Array & sigma, Sequence * seq) {
    pair<uint,size_t> *queries = new pair<uint,size_t>[numqueries];
    for(uint i=0;i<numqueries;i++) {
        queries[i].first = sigma[rand()%sigma.getLength()];
        queries[i].second = rand()%seq->rank(queries[i].first,seq->getLength()-2);
    }
    size_t acc = 0;
    start_timing();
    for(uint i=0;i<numqueries;i++) {
        acc += seq->select(queries[i].first,queries[i].second+1);
    }
    double time = get_timing();
    cout << "*************************" << endl;
    cout << " SELECT QUERIES" << endl;
    cout << " time: " << time << "ms" << endl;
    cout << " acc:  " << acc << endl;
}

void measure_access(const Array & sigma, Sequence * seq) {
    size_t *queries = new size_t[numqueries];
    for(uint i=0;i<numqueries;i++) {
        queries[i] = rand()%seq->getLength();
    }
    size_t acc = 0;
    start_timing();
    for(uint i=0;i<numqueries;i++) {
        acc += seq->access(queries[i]);
    }
    double time = get_timing();
    cout << "*************************" << endl;
    cout << " ACCESS QUERIES" << endl;
    cout << " time: " << time << "ms" << endl;
    cout << " acc:  " << acc << endl;
}


void measure_rank(const Array & sigma, Sequence * seq) {
    pair<uint,size_t> *queries = new pair<uint,size_t>[numqueries];
    for(uint i=0;i<numqueries;i++) {
        queries[i].first = sigma[rand()%sigma.getLength()];
        queries[i].second = rand()%seq->getLength();
    }
    size_t acc = 0;
    start_timing();
    for(uint i=0;i<numqueries;i++) {
        acc += seq->rank(queries[i].first,queries[i].second);
    }
    double time = get_timing();
    cout << "*************************" << endl;
    cout << " RANK QUERIES" << endl;
    cout << " time: " << time << "ms" << endl;
    cout << " acc:  " << acc << endl;
}

int main(int argc, char **argv) {
    int c;
    char *fname=NULL, *builderopt=NULL;
    SequenceBuilder * sb = NULL;
    while((c=getopt(argc,argv,"f:b:q:"))!=-1) {
        switch(c) {
            case 'f': fname = optarg; break;
            case 'b': builderopt = optarg; break;
            case 'q': numqueries = atoi(optarg); break;
        }
    }
    if(fname==NULL || builderopt==NULL) {
        cout << "usage: " << argv[0] << " -f fname -b buildstr [-q numqueries]" << endl;
        return 0;
    }
    ifstream input(fname);
    if(!input.good()) {
        cerr << "Error opening file: " << fname << endl;
    }
    Array values(input);
    sb = new SequenceBuilderStr(builderopt);
    Sequence * seq = sb->build(values);
    cout << "builderopt=" << builderopt << endl;
    cout << "size=" << seq->getSize() << endl;
    cout << "plain size=" << values.getSize() << endl;
    cout << "sigma=" << values.getMax() << endl;
    Array sigma = extract_alphabet(values);
    measure_access(sigma,seq);
    measure_rank(sigma,seq);
    measure_select(sigma,seq);
    return 0;
}

