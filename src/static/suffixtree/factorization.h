/* Copyright (C) 2010, Rodrigo Cnovas, all rights reserved.
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
 Factorizacion version
 ------------------------------------------------------------------------*/
#ifndef _FACTORIZATION
#define _FACTORIZATION

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <libcdsTrees.h>
#include <BitSequence.h>

namespace cds_static
{

	class factorization
	{
		private:
			factorization();
		public:
			uint listLength;	 //longitud de la lista
			byte nLevels;		 //numero de niveles de arrays
			uint * levelsIndex;	 //inicio de cada array
			byte * levels;		 //array con los bytes
			BitSequence * bS;	 //bitmap para indicar fin de codigo
			uint * rankLevels;	 //rank de 1s hasta el principio de cada array

			/*Susana implementation of Directly Addressable Variable-Length Codes with blocks of size 4
			 * @param list Array with the values
			 * @param l_Length Size of the array
			 * */
			factorization(uint *list,uint l_Length);
			virtual uint access(uint param);
			virtual uint access_seq(uint param, size_t *next_pos, bool dir);
			virtual void save(ofstream & fp);
			virtual uint getSize();
			static factorization* load(ifstream & fp);
			virtual ~factorization();
	};
};
#endif
