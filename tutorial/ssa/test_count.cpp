/**
 *    test_count.cpp
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

uint brute_check(uchar * text, uint n, uchar * pattern, uint m) {
  uint ret = 0;
  for(uint i=0;i<n-m+1;i++) {
    uint match_index=0;
    for(;match_index<m;match_index++)
      if(text[i+match_index]!=pattern[match_index])
        break;
    if(match_index==m) ret++;
  }
  return ret;
}


int main(int argc, char ** argv) {
  if(argc!=5) {
    cout << "usage: " << argv[0] << " <text> <index-file> <pattern-length> <repetitions>" << endl;
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
  text[n] = 0;

  ifstream ssainput(argv[2]);
  ssa * _ssa = new ssa(ssainput);
  ssainput.close();
  _ssa->print_stats();

  uint m;
  {
    stringstream ss;
    ss << argv[3];
    ss >> m;
  }

  uint rep;
  {
    stringstream ss;
    ss << argv[4];
    ss >> rep;
  }

  uint total_occ = 0;
  uchar * pattern = new uchar[m+1];
  for(uint i=0;i<rep;i++) {
    uint pos = rand()%(n-m-1);
    for(uint k=0;k<m;k++)
      pattern[k] = text[pos+k];
    pattern[m] = 0;
    uint occ = _ssa->count(pattern,m);
    uint real_occ = brute_check(text,n,pattern,m);
    if(occ!=real_occ) {
      cout << "Error for pattern " << i+1 << endl;
      cout << "ssa->count() returned " << occ << endl;
      cout << "expected value is " << real_occ << endl;
      break;
    }
    total_occ += occ;
  }

  cout << "Total occ: " << total_occ << endl;

  delete [] pattern;
  delete _ssa;
  delete [] text;

  return 0;
}
