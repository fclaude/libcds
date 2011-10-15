/**
 *    build_index.cpp
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ssa.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char ** argv) {
  if(argc!=3) {
    cout << "usage: " << argv[0] << " <input-file> <index-file>" << endl;
    return 0;
  }

  fstream input(argv[1],ios::in | ios::binary);
  if(!input.is_open()) {
    cout << "Error opening file: " << argv[1] << endl;
    return -1;
  }

  input.seekg(0,ios::end);
  uint n=input.tellg();
  uchar * text = new uchar[n+1];

  input.seekg(0,ios::beg);
  input.read((char*)text,sizeof(uchar)*n);
  input.close();
  //for(uint i=0;i<n;i++) text[i]++;
  text[n]=0;

  ssa * _ssa = new ssa(text,n);
  _ssa->set_samplepos(32);
  _ssa->set_samplesuff(32);
  _ssa->build_index();
  _ssa->print_stats();

  cout << "Index size: " << _ssa->size() << endl;

  ofstream fp(argv[2]);
  _ssa->save(fp);
  fp.close();

  //delete sbb;
  delete _ssa;
  delete [] text;

  return 0;
}

