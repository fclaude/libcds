/* Copyright (C) 2010, Rodrigo Cánovas, all rights reserved.
 *
 *This library is free software; you can redistribute it and/or
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


/*-----------------------------------------------------------------------
 Factorizacion with Variable block size
 ------------------------------------------------------------------------*/
#ifndef _FACTORIZATION_VAR
#define _FACTORIZATION_VAR

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <libcdsTrees.h>
#include <BitSequence.h>

namespace cds_static{

	class factorization_var{
		private:
			factorization_var();
		public:
			uint tamCode;
			uint * iniLevel;
			ushort * base;
			ushort * base_bits;
			uint * tablebase;
			uint tamtablebase;
			uint listLength;    //list length
			byte nLevels;       //number of levels 
			uint * levelsIndex; //start point of each array
			uint * levels;      //array with the bytes
			BitSequence * bS; //bitmap para indicar fin de codigo
			uint * rankLevels;  //rank de 1s hasta el principio de cada array

			
			/*Susana implementation of Directly Addressable Variable-Length Codes with variable length blocks
			 * @param list Array with the values
			 * @param l_Length Size of the array
			 * @param kvalues Array with the block length for each level
			 * @param nkvalues Length of the array kvalues
			 * */
			factorization_var(uint *list, uint l_Length, ushort* kvalues,  uint nkvalues);
			virtual uint access(uint param);
			virtual uint access_seq(uint param, size_t *next_pos, bool dir);
			virtual uint getSize();
			virtual void save(ofstream & fp);
			static factorization_var* load(ifstream & fp);
			virtual ~factorization_var();
	};
};

#endif
