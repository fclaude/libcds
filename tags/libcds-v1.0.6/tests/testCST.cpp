/* Copyright (C) 2010, Rodrigo Cánovas, all rights reserved.  
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <CST.h>

using namespace std;
using namespace cds_utils;
using namespace cds_static;

CST * saveLoad(CST * bs) {
	ofstream ofs("cst.tmp");
	bs->save(ofs);
	ofs.close();
	ifstream ifs("cst.tmp");
	CST * ret = CST::load(ifs);
	ifs.close();
	return ret;
}

bool testCST(CST *s1){
	/*add any test you want*/
	return true;
}


int main(int argc, char *argv[]){
	char *text;
	size_t length;
	
	if(argc!=2) {
		cout << "Checks if the CST of the file <arch> is save/load correctly" << endl << endl;
		cout << "usage: " << argv[0] << " <arch>" << endl;
		return 0;
	}
	
	if(loadText(argv[1], &text, &length))
		return 1;
	
	/*create index*/

	CST *cst;
	
	CST_Y csty(text, length, DAC, CN_NPR, 32); 
	cst	= saveLoad(&csty); 
	if(!testCST(cst)) {
		cerr << "ERROR TESTING CST_Y" << endl;
		return -1;
	}
	delete (CST_Y *)cst;
	cout << "CST_Y OK\n" << endl;
	
	delete [] text;
	return 0;
}

