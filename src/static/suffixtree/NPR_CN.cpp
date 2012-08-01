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

#include <NPR_CN.h>

namespace cds_static
{

	NPR_CN::NPR_CN() {
		npr_type =  CN_NPR;
		n = 0;
		b = 0;
		bits_b = 0;
		l = 0;
		min_level = NULL;
		level_size = NULL;
		min_pos = NULL;
	}

	NPR_CN::NPR_CN(LCP *lcp, size_t block_Size, TextIndex *csa) {
		npr_type = CN_NPR;
		n = csa->index_length();
		b = block_Size;
		bits_b = bits(b);
		l =(size_t)(log(n)/log(b));
		level_size = new uint[l];
		for(size_t i=0; i<l; i++)
			level_size[i] = 0;
		min_level = new uint*[l];
		min_pos = new uint*[l];

		create_first_level(lcp,csa);
		for(size_t r=1; r<l; r++) {
			create_next_level(r);
		}
	}

	void NPR_CN::create_first_level(LCP *lcp, TextIndex *csa) {
		level_size[0] = (n+b-1)/b;
		min_level[0] = new uint[level_size[0]];
		for(size_t i=0; i< level_size[0]; i++)
			min_level[0][i] = 0;
		min_pos[0] = new uint[((level_size[0]*bits_b+W-1)/W)];
		for(size_t i=0; i<((level_size[0]*bits_b+W-1)/W); i++)
			min_pos[0][i] = 0;
		size_t min, min_aux;
		size_t ini;
		size_t pos;

		/*calculate the min for each block except the last one*/
		for(size_t i=0; i<level_size[0]-1; i++) {
			min = n;
			ini = i*b;
			pos = 0;
			for(size_t j=0; j < b; j++) {
				min_aux = lcp->get_LCP(ini+j, csa);
				if(min_aux < min) {
					min = min_aux;
					pos = j;
				}
			}
			min_level[0][i] = min;
			set_field_64(min_pos[0], bits_b, i, pos);
		}

		/*calculate min for the last block*/
		min = n;
		pos = 0;
		ini = (level_size[0]-1)*b;
		for(size_t j=ini; j < n; j++) {
			min_aux = lcp->get_LCP(j, csa);
			if(min_aux < min) {
				min = min_aux;
				pos = j - ini;
			}
		}
		min_level[0][level_size[0]-1] = min;
		set_field_64(min_pos[0], bits_b, level_size[0]-1, pos);
	}

	void NPR_CN::create_next_level(size_t r) {
		level_size[r] = (level_size[r-1]+b-1)/b;
		min_level[r] = new uint[level_size[r]];
		for(size_t i=0; i< level_size[r]; i++)
			min_level[r][i] = 0;
		min_pos[r] = new uint[((level_size[r]*bits_b+W-1)/W)];
		for(size_t i=0; i<((level_size[r]*bits_b+W-1)/W); i++)
			min_pos[r][i] = 0;
		size_t min, min_aux;
		size_t ini;
		size_t pos;

		/*calculate the min for each block except the last one*/
		for(size_t i=0; i<level_size[r]-1; i++) {
			min = n;
			ini = i*b;
			pos = 0;
			for(size_t j=0; j < b; j++) {
				min_aux = min_level[r-1][ini+j];
				if(min_aux < min) {
					min = min_aux;
					pos = j;
				}
			}
			min_level[r][i] = min;
			set_field_64(min_pos[r], bits_b, i, pos);
		}

		/*calculate min for the last block*/
		min = n;
		pos = 0;
		ini = (level_size[r]-1)*b;
		for(size_t j=ini; j < level_size[r-1]; j++) {
			min_aux = min_level[r-1][j];
			if(min_aux < min) {
				min = min_aux;
				pos = j - ini;
			}
		}
		min_level[r][level_size[r]-1] = min;
		set_field_64(min_pos[r], bits_b, level_size[r]-1, pos);
	}

	NPR_CN::~NPR_CN() {
		for(size_t i=0; i<l; i++) {
			delete [] min_level[i];
			delete [] min_pos[i];
		}
		delete [] min_level;
		delete [] level_size;
		delete [] min_pos;
	}

	size_t NPR_CN::find_NSV(size_t v,  TextIndex *csa, LCP *lcp) const
	{
		uint value_v = lcp->get_LCP(v, csa);
		return fwd_NSV(v,  csa, lcp, value_v);
	}

	size_t NPR_CN::find_NSV_level_r(size_t value_v, size_t next_pos, size_t r) const
	{
		size_t lim;
		size_t end_block = b*((next_pos+b)/b);
		if(end_block > level_size[r])
			end_block = level_size[r];
		/*search in the same block*/
		for(size_t i=next_pos+1; i < end_block; i++) {
			if(min_level[r][i] < value_v)
				return i;
		}
		if(end_block==level_size[r])
			return level_size[r];
		/*search in the next levels*/
		lim = find_NSV_level_r(value_v, next_pos/b, r+1);
		/*search in the same block than lim*/
		if(lim==level_size[r+1])
			return level_size[r];
		/*this must return something*/
		for(size_t i= b*lim; i< b*(lim+1); i++) {
			if(min_level[r][i] < value_v)
				return i;
		}
		/*if here not exist a return yet -> error!!*/
		return (size_t)-1;
	}

	size_t NPR_CN::fwd_NSV(size_t v,  TextIndex *csa, LCP *lcp, size_t d) const
	{
		size_t aux_nsv;
		size_t lim;
		if(d == 0)
			return n;
		size_t *next;
		size_t n_next = 0;
		/*search in the same block*/
		size_t end_block = b*((v+b)/b);
		if(end_block>n)
			end_block = n;
		for(size_t i=v+1; i < end_block; i++) {
			aux_nsv = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
			if(aux_nsv < d)
				return i;
		}
		if(end_block==n)
			return n;
		/*search in the next levels*/
		lim = find_NSV_level_r(d, v/b, 0);
		/*search in the same block than lim*/
		if(lim==level_size[0]) {
			return n;
		}
		/*this must return something*/
		n_next = 0;
		for(size_t i= b*lim; i< b*(lim+1); i++) {
			aux_nsv = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
			if(aux_nsv < d) {
				return i;
			}
		}
		/*this never happen*/
		return (size_t)-1;
	}

	size_t NPR_CN::find_PSV(size_t v,  TextIndex *csa, LCP *lcp) const
	{
		size_t value_v = lcp->get_LCP(v, csa);
		return bwd_PSV(v,  csa, lcp, value_v);
	}

	size_t NPR_CN::find_PSV_level_r(size_t value_v, size_t next_pos, size_t r) const
	{
		size_t lim;
		size_t ini_block = b*((next_pos)/b);
		if(next_pos == 0)
			return 0;
		/*search in the same block*/
		for(size_t i=next_pos-1; i > ini_block; i--) {
			if(min_level[r][i] < value_v)
				return i+1;
		}
		/*last one is a special case so we do it separate*/
		if(next_pos != ini_block) {
			if(value_v > min_level[r][ini_block])
				return ini_block+1;
		}
		if(ini_block==0)
			return 0;
		/*search in the next levels*/
		lim = find_PSV_level_r(value_v, next_pos/b, r+1);
		/*search in the same block than lim*/
		if(lim==0)
			return 0;
		/*this must return something*/
		for(size_t i= (b*lim-1); i >= b*(lim-1); i--) {
			if(min_level[r][i] < value_v)
				return i+1;
		}
		/*if here not exist a return yet -> error!!*/
		return (size_t)-1;
	}

	size_t NPR_CN::bwd_PSV(size_t v,  TextIndex *csa, LCP *lcp, size_t d) const
	{
		size_t aux_psv;
		size_t lim;
		if(d == 0)
			return 0;
		size_t *next;
		size_t n_next = 0;
		/*search in the same block*/
		size_t ini_block = b*(v/b);
		for(size_t i=v-1; i > ini_block; i--) {
			aux_psv = lcp->get_seq_LCP(i,csa, &next, &n_next, 0);
			if(aux_psv < d)
				return i+1;
		}
		/*last one is a special case so we do it separeted*/
		if(ini_block < v) {
			if(d > lcp->get_LCP(ini_block,csa))
				return ini_block+1;
		}

		if(ini_block==0)
			return 0;

		/*search in the next levels*/
		lim = find_PSV_level_r(d, v/b, 0);
		/*search in the same block than lim*/
		if(lim==0)
			return 0;

		/*this must return something*/
		n_next = 0;
		for(size_t i= b*lim-1; i >= b*(lim-1); i--) {
			aux_psv = lcp->get_seq_LCP(i,csa, &next, &n_next, 0);
			if(aux_psv < d)
				return i+1;
		}
		return (size_t)-1;
	}

	size_t NPR_CN::find_RMQ(size_t x, size_t y, TextIndex *csa, LCP *lcp) const
	{
		if(x>y){ size_t z = y;      y = x;      x = z;}
		size_t ret = x;
		size_t next_ret, min_r;
		if(x==y)
			return ret;
		size_t *next;
		size_t n_next = 0;
		size_t min_val = lcp->get_seq_LCP(x,csa, &next, &n_next, 1);
		size_t aux_min;
		size_t x_r, y_r;
		/*if x and y are in the same block  O(b)*/
		if(x/b == y/b) {
			for(size_t i=x+1; i<=y; i++) {
				aux_min = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
				if(aux_min < min_val) {
					min_val = aux_min;
					ret = i;
				}
			}
			return ret;
		}
		/*if x is the begin of the block*/
		if(x%b == 0)
			x_r = x/b;
		else {
			/*view in the same block*/
			for(size_t i=x+1; i < b*((x+b-1)/b) ; i++) {
				aux_min = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
				if(aux_min < min_val) {
					min_val = aux_min;
					ret = i;
				}
			}
			x_r = (x+b-1)/b;
		}
		/*if y is the end of a block*/
		if(y%b == b-1)
			y_r = y/b;
		else {
			/*view in the same block*/
			n_next = 0;
			for(size_t i= b*(y/b); i <= y ; i++) {
				aux_min = lcp->get_seq_LCP(i,csa, &next, &n_next, 1);
				if(aux_min < min_val) {
					min_val = aux_min;
					ret = i;
				}
			}
			y_r = (y/b) - 1;
		}
		if(x_r > y_r) {
			return ret;
		}
		next_ret = find_RMQ(x_r, y_r, 0, &min_r);
		if( (min_val < min_r) || (min_val == min_r && ret < b*next_ret))
			return ret;
		else
			return next_ret*b + get_field_64(min_pos[0], bits_b, next_ret);

		/*this never happen*/
		cout << "Error" << endl;
		return 0;
	}

	size_t NPR_CN::find_RMQ(size_t x, size_t y, size_t r, size_t *min_r) const
	{
		size_t ret = x;
		size_t next_ret, next_min;
		*min_r = min_level[r][x];
		size_t aux_min;
		size_t x_r, y_r;
		if(x==y)
			return ret;
		/*if x and y are in the same block  O(b)*/
		if(x/b == y/b) {
			for(size_t i=x+1; i<=y; i++) {
				aux_min = min_level[r][i];
				if(aux_min < *min_r) {
					*min_r = aux_min;
					ret = i;
				}
			}
			return ret;
		}
		/*if x is the begin of the block*/
		if(x%b == 0)
			x_r = x/b;
		else {
			/*view in the same block*/
			for(size_t i=x+1; i < b*((x+b-1)/b) ; i++) {
				aux_min = min_level[r][i];
				if(aux_min < *min_r) {
					*min_r = aux_min;
					ret = i;
				}
			}
			x_r = (x+b-1)/b;
		}
		/*if y is the end of a block*/
		if(y%b == b-1)
			y_r = y/b;
		else {
			/*view in the same block*/
			for(size_t i=b*(y/b); i <= y ; i++) {
				aux_min = min_level[r][i];
				if(aux_min < *min_r) {
					*min_r = aux_min;
					ret = i;
				}
			}
			y_r = y/b - 1;
		}
		if(x_r > y_r)
			return ret;
		next_ret = find_RMQ(x_r, y_r, r+1, &next_min);
		if( (*min_r < next_min) || (*min_r == next_min && ret < next_ret*b))
			return ret;
		else {
			*min_r = next_min;
			return next_ret*b + get_field_64(min_pos[r+1], bits_b, next_ret);
		}
		/*Error if the code reach this return*/
		//cout << "Error" << endl;
		return (size_t)-1;
	}

	size_t NPR_CN::getSize() const
	{
		size_t mem = sizeof(NPR_CN);
		mem += sizeof(uint)*l;	 //level size
		for(size_t i=0; i<l; i++) {
								 //min_level[i]
			mem += sizeof(uint)*level_size[i];
			mem += sizeof(uint)*((level_size[i]*bits_b+W-1)/W);
		}
		return mem;
	}

	void NPR_CN::save(ofstream & fp) const
	{
		saveValue(fp, npr_type);
		saveValue(fp, n);
		saveValue(fp, b);
		saveValue(fp, bits_b);
		saveValue(fp, l);
		saveValue(fp, level_size, l);
		for(size_t i=0; i<l; i++)
			saveValue(fp, min_level[i], level_size[i]);
		for(size_t i=0; i<l; i++)
			saveValue(fp, min_pos[i], (level_size[i]*bits_b+W-1)/W);
	}

	NPR_CN * NPR_CN::load(ifstream & fp) {
		NPR_CN *npr = new NPR_CN();
		size_t type = loadValue<size_t>(fp);
		if(type!=CN_NPR) {
			abort();
		}
		npr->n = loadValue<size_t>(fp);
		npr->b = loadValue<size_t>(fp);
		npr->bits_b = loadValue<size_t>(fp);
		npr->l = loadValue<size_t>(fp);
		npr->level_size = loadValue<uint>(fp, npr->l);
		npr->min_level = new uint*[npr->l];
		for(size_t i=0; i<npr->l; i++)
			npr->min_level[i] = loadValue<uint>(fp, npr->level_size[i]);
		npr->min_pos = new uint*[npr->l];
		for(size_t i=0; i<npr->l; i++)
			npr->min_pos[i] = loadValue<uint>(fp, (npr->level_size[i]*npr->bits_b+W-1)/W);
		return npr;
	}

};
