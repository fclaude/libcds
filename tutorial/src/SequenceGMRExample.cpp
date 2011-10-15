/**
 *    SequenceGMRExample.cpp
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
#include <BitSequence.h>
#include <Mapper.h>
#include <Sequence.h>

using namespace std;
using namespace cds_static;

int main(int argc, char **argv) {

  size_t N;
  uint s;
  cout << "Length: ";
  cin >> N;
  uint * seq = new uint[N];
  for(size_t i=0;i<N;i++) {
    uint v;
    cout << "Element at position " << i << ": ";
    cin >> seq[i];
  }
  SequenceGMR * gmr = new SequenceGMR(seq, N, 5u,
			    new BitSequenceBuilderRG(20),
		            new SequenceBuilderGMRChunk(
			       new BitSequenceBuilderRG(20), 
			       new PermutationBuilderMRRR(
				 20,
				 new BitSequenceBuilderRG(20))));
  cout << "size = " << gmr->getSize() << " bytes" << endl;

  delete gmr;
  delete []seq;
  return 0;
}

