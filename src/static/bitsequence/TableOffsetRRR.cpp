/* table_offset.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * Table for offsets.
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

#include "TableOffsetRRR.h"

namespace cds_static
{
	// Interface for old implementation
	void genera(ushort * bch, uint u, ushort * F, uint lF);
	uint generaClase(ushort * bch, uint u, uint clase, uint puestos, uint pos_ini, uint generado);
	uint offset_func(uint u, uint busca);
	uint offsetRecursivo(uint u, uint busca, uint clase, uint puestos, uint pos_ini, uint generado);
	uint __indiceFunc;
	uint __indAcumulado;
	ushort * __Lis;
	// End interface old implementation

	table_offset::table_offset(uint u) {
		this->u = u;
		users_count = 0;
		short_bitmaps = new ushort[((1<<u)+1)];
		offset_class = new ushort[u+2];
		binomial = new uint*[u+1];
		log2binomial = new ushort*[u+1];
		for(uint i=0;i<u+1;i++) {
			binomial[i] = new uint[u+1];
			log2binomial[i] = new ushort[u+1];
			for(uint j=0;j<u+1;j++) {
				binomial[i][j] = 0;
				log2binomial[i][j] = 0;
			}
		}
		for(uint i=0;i<u+1;i++) {
			binomial[i][0] = 1;
			binomial[i][1] = 1;
			binomial[i][i] = 1;
			log2binomial[i][0] = 0;
			log2binomial[i][1] = 0;
			log2binomial[i][i] = 0;
		}
		for(uint j=1;j<u+1;j++) {
			for(uint i=j+1;i<u+1;i++) {
				binomial[i][j] = binomial[i-1][j-1]+binomial[i-1][j];
				log2binomial[i][j] = bits(binomial[i][j]-1);
			}
		}
		fill_tables();
	}

	void table_offset::fill_tables() {
		genera(short_bitmaps, u, offset_class, u);
		rev_offset = __Lis;
		//delete [] __Lis;
	}

	table_offset::~table_offset() {
		delete [] short_bitmaps;
		delete [] offset_class;
		for(uint i=0;i<u+1;i++) {
			delete [] binomial[i];
			delete [] log2binomial[i];
		}
		delete [] binomial;
		delete [] log2binomial;
		delete [] rev_offset;
	}

	uint table_offset::size() {
		uint ret = sizeof(ushort)*(((1<<u)+1)+u+1);
		ret += (sizeof(uint)+sizeof(ushort))*((u+1)*(u+1));
		ret += sizeof(ushort)*(2<<(u+1));
		ret += sizeof(ushort)*(u+2);
		return ret;
	}

	// OLD implementation, replace

	void genera(ushort * bch, uint u, ushort * F, uint lF) {
		__indAcumulado=0;
		__indiceFunc=0;
		F[0]=0;
								 //(uint *)malloc((2<<u+1)*sizeof(uint));
		__Lis = new ushort[(2<<(u+1))];
		for (uint i=0;i<=u;i++) {
			__indAcumulado += generaClase(bch, u, i, 0, 0, 0);
			F[i+1] = __indiceFunc;
		}
	}

	uint generaClase(ushort * bch, uint u, uint clase, uint puestos, uint pos_ini, uint generado) {
		uint ret=0;
		if (clase==puestos) {
			bch[__indiceFunc] = generado;
			__Lis[generado] = __indiceFunc-__indAcumulado;
			__indiceFunc++;
			return 1;
		}
		if (clase<puestos)
			return 0;
		for (uint i=pos_ini;i<u;i++) {
			uint tmp = generado | (1<<i);
			ret += generaClase(bch, u, clase, puestos+1, i+1, tmp);
		}
		return ret;
	}
};
