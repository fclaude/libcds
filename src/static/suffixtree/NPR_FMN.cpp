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

#include <NPR_FMN.h>

namespace cds_static
{

	NPR_FMN::NPR_FMN() {
		npr_type = FMN_NPR;
		nsv = NULL;
		psv = NULL;
		rmq = NULL;
	}

	NPR_FMN::NPR_FMN(LCP *lcp, size_t block_Size, TextIndex *csa, size_t levels) {
		npr_type = FMN_NPR;
		nsv = new NSV(lcp,levels, block_Size, csa);
		psv = new PSV(lcp,levels, block_Size, csa);
		rmq = new RMQ_succinct_lcp(lcp, csa);
	}

	NPR_FMN::~NPR_FMN() {
		delete (NSV *)nsv;
		delete (PSV *)psv;
		delete (RMQ_succinct_lcp *)rmq;
	}

	size_t NPR_FMN::find_NSV(size_t i, TextIndex *csa, LCP *lcp) const
	{
		return nsv->find_NSV(i, csa, lcp);
	}

	size_t NPR_FMN::find_PSV(size_t i, TextIndex *csa, LCP *lcp) const
	{
		return psv->find_PSV(i,csa,lcp);
	}

	size_t NPR_FMN::bwd_PSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const
	{
		if(d==0)
			return 0;
		size_t pos = i;
		size_t val_lcp;
		while(pos!=0) {
			val_lcp = lcp->get_LCP(pos, csa);
			if(val_lcp<d)
				return pos;
			pos = find_PSV(pos,csa,lcp);
		}
		return pos;
	}

	size_t NPR_FMN::fwd_NSV(size_t i, TextIndex *csa, LCP *lcp, size_t d) const
	{
		if(d==0)
			return csa->index_length();;
		size_t pos = i;
		size_t val_lcp;
		while(pos!=csa->index_length()) {
			val_lcp = lcp->get_LCP(pos, csa);
			if(val_lcp<d)
				return pos;
			pos = find_NSV(pos,csa,lcp);
		}
		return pos;
	}

	size_t NPR_FMN::find_RMQ(size_t x, size_t y, TextIndex *csa, LCP *lcp) const
	{
		return rmq->query(x, y, csa, lcp);
	}

	size_t NPR_FMN::getSize() const
	{
		size_t mem = sizeof(NPR_FMN);
		mem += nsv->getSize();
		mem += psv->getSize();
		mem += rmq->getSize();
		return mem;
	}

	void NPR_FMN::save(ofstream & fp) const
	{
		saveValue(fp, npr_type);
		nsv->save(fp);
		psv->save(fp);
		rmq->save(fp);
	}

	NPR_FMN * NPR_FMN::load(ifstream & fp) {
		NPR_FMN *npr = new NPR_FMN();
		size_t type = loadValue<size_t>(fp);
		if(type!=FMN_NPR) {
			abort();
		}
		npr->nsv = NSV::load(fp);
		npr->psv = PSV::load(fp);
		npr->rmq = RMQ_succinct_lcp::load(fp);
		return npr;
	}

};
