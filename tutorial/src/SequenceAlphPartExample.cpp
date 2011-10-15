/**
 *    SequenceAlphPartExample.cpp
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

  SequenceBuilder * sb1 = new SequenceBuilderWaveletTree(
				new BitSequenceBuilderRG(20),
				new MapperNone());
  SequenceBuilder * sb2 = new SequenceBuilderGMRChunk(
				new BitSequenceBuilderRG(20),
				new PermutationBuilderMRRR(
				   20, new BitSequenceBuilderRG(20)));
  SequenceBuilder * sb3 = new SequenceBuilderGMR(
				new BitSequenceBuilderRG(20),
				sb2);

  SequenceAlphPart * ap = new SequenceAlphPart(seq, N, 0u,
					       sb1, sb3);
  cout << "size = " << ap->getSize() << " bytes" << endl;

  delete ap;
  delete []seq;
  return 0;
}

