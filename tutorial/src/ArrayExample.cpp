/**
 *    ArrayExample.cpp
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

#include<iostream>

#include <libcdsBasics.h>
#include <Array.h>
#include <cppUtils.h>

using namespace std;
using namespace cds_utils;

int main(int argc, char **argv) {
  size_t N=0;
  cout << "Enter array length: ";
  cin >> N;
  uint * A = new uint[N];
  for(size_t i=0;i<N;i++) {
    cout << "Enter element at position " << i << ": ";
    cin >> A[i];
  }

  Array * a = new Array(A,N);
  delete [] A;

  cout << "Size: " << a->getSize() << " bytes" << endl;
  for(uint i=0;i<N;i++)
    cout << "A[" << i << "]=" << a->getField(i) << endl;

  delete a;
  return 0;
}
