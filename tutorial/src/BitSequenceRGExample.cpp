/**
 *    BitSequenceRGExample.cpp
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

#include <libcdsBasics.h>
#include <BitString.h>
#include <BitSequence.h>
#include <BitSequenceRG.h>
#include <BitSequenceRRR.h>

using namespace std;
using namespace cds_utils;
using namespace cds_static;

int main(int argc, char **argv) {
  size_t N;
  cout << "Length of the bitmap: ";
  cin >> N;
  uint * bs = new uint[uint_len(N,1)];
  for(uint i=0;i<N;i++) {
    uint b;
    cout << "bit at position " << i << ": ";
    cin >> b;
    if(b==0) bitclean(bs,i);
    else bitset(bs,i);
  }
  BitSequenceRG * bsrg = new BitSequenceRG(bs,N,20);
  cout << "rank(" << N/2 << ")=" << bsrg->rank1(N/2) << endl;
  cout << "select(1) = " << bsrg->select1(1) << endl;
  cout << "size = " << bsrg->getSize() << endl;
  delete bsrg;
  delete [] bs;
  return 0;
}
