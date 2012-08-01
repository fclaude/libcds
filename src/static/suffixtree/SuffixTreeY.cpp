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

#include <SuffixTreeY.h>
#include <vector>

namespace cds_static
{

	SuffixTreeY::SuffixTreeY() {
		length = 0;
		csa = NULL;
		lcp = NULL;
		npr = NULL;
	}

	SuffixTreeY::SuffixTreeY(char *text, size_t n, size_t lcp_type, size_t npr_type, size_t b) {
		length = n;
		cout << "Text length: " << n << endl;
		/*create the index*/
		csa = new TextIndexCSA((uchar *)text, (ulong)length, NULL);
		cout << "TextIndex bits/c: " << csa->getSize()*8.0/length << endl;
		switch(lcp_type) {
			case NAIVE: lcp = new LCP_naive(csa, text, length); break;
			case SAD_GON_OS: lcp = new LCP_Sad(csa, text, length); break;
			case FMN_RRR_OS: lcp = new LCP_FMN(csa, text, length); break;
			case PT: lcp = new LCP_PT(csa, text, length); break;
			case PHI: lcp = new LCP_PhiSpare(csa,text,length); break;
			case DAC: lcp = new LCP_DAC(csa, text, length); break;
			case DAC_VAR: lcp = new LCP_DAC_VAR(csa, text,length); break;
			default: exit(0);
		}
		cout << "LCP bits/c: " << lcp->getSize()*8.0/length << endl;
		switch(npr_type) {
			case FMN_NPR: npr = new NPR_FMN(lcp, b, csa); break;
			case CN_NPR: npr = new NPR_CN(lcp, b, csa); break;
			default: exit(0);
		}
		cout << "NPR bits/c: " << npr->getSize()*8.0/length << endl;
		/*here you can free the text*/
	}

	SuffixTreeY::SuffixTreeY(LCP *_lcp, NPR *_npr, TextIndex *_csa) {
		csa = _csa;
		cout << "TextIndex bits/c: " << csa->getSize()*8.0/length << endl;
		lcp = _lcp;
		cout << "LCP bits/c: " << lcp->getSize()*8.0/length << endl;
		npr = _npr;
		cout << "NPR bits/c: " << npr->getSize()*8.0/length << endl;
	}

	bool SuffixTreeY::isRoot(size_t vl,size_t vr) const
	{
		if(vl==0 && vr==length-1)
			return true;
		return false;
	}

	void SuffixTreeY::Root(size_t *vl, size_t *vr) const
	{
		*vl = 0;
		*vr = length-1;
	}

	size_t SuffixTreeY::Count(size_t vl, size_t vr) const
	{
		return vr - vl+1;
	}

	bool SuffixTreeY::Ancestor(size_t wl, size_t wr, size_t vl, size_t vr) const
	{
		if((wl<=vl) && (wr >= vr))
			return true;
		return false;
	}

	size_t SuffixTreeY::SDepth(size_t vl, size_t vr) const
	{
		size_t h,k, val;
		if(vl!=vr) {
			k = npr->find_RMQ(vl+1, vr, csa, lcp);
			h = lcp->get_LCP(k, csa);
		}
		else {					 /*for leaf (->v.left==v.right)*/
			val = csa->getSA(vl);
			h = length - val +1;
		}
		return h;
	}

	size_t SuffixTreeY::Locate(size_t vl, size_t vr) const
	{
		if(vl==vr) {
			return csa->getSA(vl);
		}
		return (size_t)-1;
	}

	void SuffixTreeY::Parent(size_t vl, size_t vr, size_t *parent_l, size_t *parent_r) const
	{
		if(isRoot(vl, vr)) {
			*parent_l = (size_t)-1;
			*parent_r = (size_t)-1;
			return;
		}
		size_t k;
		if(vr == length-1)
			k = vl;
		else {
			if(lcp->get_LCP(vl, csa) > lcp->get_LCP(vr+1,csa))
				k = vl;
			else
				k = vr+1;
		}
		size_t p_l = npr->find_PSV(k,csa,lcp);
		if(p_l > 0)
			p_l--;
		size_t p_r = npr->find_NSV(k,csa,lcp)-1;
		*parent_l = p_l;
		*parent_r = p_r;
	}

	void SuffixTreeY::FChild(size_t vl, size_t vr, size_t *fchild_l, size_t *fchild_r) const
	{
		if(vl==vr) {
			*fchild_l = (size_t)-1;
			*fchild_r = (size_t)-1;
			return;
		}
		*fchild_l = vl;
		*fchild_r = npr->find_RMQ(vl+1,vr,csa,lcp)-1;
	}

	void SuffixTreeY::NSibling(size_t vl, size_t vr, size_t *nsibling_l, size_t *nsibling_r) const
	{
		size_t wl, wr;
		Parent(vl, vr, &wl, &wr);
		if(wl==(size_t)-1) {	 /*v is the root*/
			*nsibling_l = (size_t)-1;
			*nsibling_r = (size_t)-1;
			return;
		}
		if(wr == vr) {			 /*v does not have a next sibling*/
			*nsibling_l = (size_t)-1;
			*nsibling_r = (size_t)-1;
			return;
		}
		if(vr+1 == wr) {		 /*v's next sibling is a leaf*/
			*nsibling_l = wr;
			*nsibling_r = wr;
			return;
		}
		else {
			size_t left = vr+1;
			size_t lcp_rm = lcp->get_LCP(vr+1, csa);
			size_t rm =  npr->fwd_NSV(vr+1, csa, lcp, lcp_rm+1);
			*nsibling_l = left;
			*nsibling_r = rm-1;
			return;
		}
	}

	void SuffixTreeY::SLink(size_t vl, size_t vr, size_t *slink_l, size_t *slink_r) const
	{
		size_t x,y,k;
								 /*v is the root*/
		if(vl==0 && vr == length-1) {
			*slink_l = (size_t)-1;
			*slink_r = (size_t)-1;
			return;
		}
		x = csa->getPsi(vl);
		y = csa->getPsi(vr);
		k = npr->find_RMQ(x+1, y, csa, lcp);
		size_t p_l = npr->find_PSV(k, csa, lcp);
		if(p_l > 0)
			p_l--;
		size_t p_r = npr->find_NSV(k,csa,lcp)-1;
		*slink_l = p_l;
		*slink_r = p_r;
	}

	void SuffixTreeY::SLink_i(size_t vl, size_t vr, size_t i, size_t *slinki_l, size_t *slinki_r) const
	{
		size_t x,y,k;
		size_t seq = 1;			 /*set seq = 0 if you dont want to do it sequentialy*/
								 /*v is the root*/
		if(vl==0 && vr == length-1) {
			*slinki_l = (size_t)-1;
			*slinki_r = (size_t)-1;
			return;
		}

		if(seq) {
			while(!isRoot(vl,vr) && i>0) {
				SLink(vl, vr, &vl, &vr);
				i--;
			}
			if(i==0) {
				*slinki_l = vl;
				*slinki_r = vr;
			}
			else {
				*slinki_l = (uint)-1;
				*slinki_r = (uint)-1;
			}
		}
		else {
			/*psi_i(v->left)*/
			x = csa->getSA(vl);
			if(x+i >=length) {	 //then return the root
				Root(slinki_l, slinki_r);
				return;
			}
			x = csa->getISA(x);
			/*psi_i(v->right)*/
			y = csa->getSA(vr);
			if(y+i >=length) {	 //then return the root
				Root(slinki_l, slinki_r);
				return;
			}
			y = csa->getISA(y);
			/*first letter of x and y are diff*/
			if(csa->getT(x)!= csa->getT(y)) {
				Root(slinki_l, slinki_r);
				return;
			}
			k = npr->find_RMQ(x+1, y, csa, lcp);
			size_t p_l = npr->find_PSV(k, csa, lcp);
			if(p_l > 0)
				p_l--;
			size_t p_r = npr->find_NSV(k,csa,lcp)-1;
			*slinki_l = p_l;
			*slinki_r = p_r;
		}
	}

	void SuffixTreeY::LCA(size_t vl, size_t vr, size_t wl, size_t wr, size_t *lca_l, size_t *lca_r) const
	{
		size_t k;
		if(Ancestor(vl, vr, wl, wr)) {
			*lca_l = vl;
			*lca_r = vr;
			return;
		}
		if(Ancestor(wl, wr, vl, vr)) {
			*lca_l = wl;
			*lca_r = wr;
			return;
		}
		if(vr < wl)
			k =  npr->find_RMQ(vr+1, wl, csa, lcp);
		else					 //wr < vl
			k =  npr->find_RMQ(wr+1, vl, csa, lcp);
		/*there are no more possible case for calculate k*/
		uint p_l = npr->find_PSV(k, csa, lcp);
		if(p_l > 0)
			p_l--;
		size_t p_r = npr->find_NSV(k,csa,lcp)-1;
		*lca_l = p_l;
		*lca_r = p_r;
	}

	void SuffixTreeY::Child(size_t vl, size_t vr, uchar a, size_t *child_l, size_t *child_r) const
	{
		int x,y,m, pos_letter;
		uchar first_letter;
		size_t aux_l, aux_r;
		size_t res_l = (size_t)-1;
		size_t res_r = (size_t)-1;
		vector<size_t> children_l;
		vector<size_t> children_r;
		if(vl==vr) {			 //is a leaf
			*child_l = (size_t)-1;
			*child_r = (size_t)-1;
			return;
		}
		/*calculate the position where the letter must be*/
		pos_letter = SDepth(vl, vr)+1;
		/*get all the childs of v*/
		FChild(vl, vr, &aux_l, &aux_r);
		children_l.push_back(aux_l);
		children_r.push_back(aux_r);
		NSibling(aux_l, aux_r, &aux_l, &aux_r);
		while(aux_l != (size_t)-1) {
			children_l.push_back(aux_l);
			children_r.push_back(aux_r);
			NSibling(aux_l, aux_r, &aux_l, &aux_r);
		}
		x=0;
		y=children_l.size()-1;
		/*Binary search over children*/
		while(x<=y) {
			m = (x+y)/2;
			aux_l = children_l[m];
			aux_r = children_r[m];
			/*get letter*/
			first_letter = Letter(aux_l, aux_r, pos_letter);
			if(first_letter == a) {
				res_l = aux_l;
				res_r = aux_r;
				break;
			}
			else {
				if (first_letter < a)
					x=m+1;
				else
					y=m-1;
			}
		}
		/*free the space used*/
		children_l.clear();
		children_r.clear();
		*child_l = res_l;
		*child_r = res_r;
	}

	uchar SuffixTreeY::Letter(size_t vl, size_t vr, int i) const
	{
		uchar l;
		if(i==1) {
			l = (uchar)csa->getT(vl);
			return l;
		}
		/*else return Letter(psi_{i-1}(vl),1)*/
		size_t p1 = vl;
		for(size_t j=0; j<(size_t)(i-1); j++)
			p1 = csa->getPsi(p1);
		/*
		 uint v_sa, v_i;
		 v_sa = csa_lookup(csa, v->left+1)-1; //SA[v->left]
		 v_i = csa_inverse(csa, v_sa + i)-1;  // psi_{i-1} (v_sa);
		*/
		return Letter(p1, p1,1); //Letter(v_i,v_i, 1);
	}

	size_t SuffixTreeY::TDepth(size_t vl, size_t vr) const
	{
		size_t depth = 0;
		size_t aux_l = vl;
		size_t aux_r = vr;
		while(aux_l !=0 || aux_r != length-1) {
			Parent(aux_l, aux_r, &aux_l, &aux_r);
			depth++;
		}
		return depth;
	}

	void SuffixTreeY::LAQs(size_t vl, size_t vr, size_t d, size_t *laq_s_l, size_t *laq_s_r) const
	{
		size_t u_l, u_r=0;
		if(d==0) {
			*laq_s_l = 0;
			*laq_s_r = length-1;
			return;
		}
		u_l = npr->bwd_PSV(vl+1, csa, lcp, d);
		if(u_l>0)
			u_l--;
		if(vr==0)
			u_r=0;
		else {
			u_r = npr->fwd_NSV(vr-1, csa, lcp, d);
			if(u_r!=vr)
				u_r--;
		}
		*laq_s_l = u_l;
		*laq_s_r = u_r;
	}

	void SuffixTreeY::LAQt(size_t vl, size_t vr, size_t d, size_t *laq_t_l, size_t *laq_t_r) const
	{
		size_t depth = 0;
		size_t x;
		size_t string_depth = d;
		size_t res_l, res_r;
		size_t next_l, next_r;
		size_t aux_l, aux_r;
		size_t acum =1;
		if(d==0 || (vl==0 && vr==length-1)) {
			*laq_t_l = 0;
			*laq_t_r = length-1;
			return;
		}
		LAQs(vl, vr, d, &res_l, &res_r);
		depth = TDepth(res_l, res_r);
		x = d-depth;
		while(x!=0 && !(res_l==vl && res_r==vr) ) {
			if(acum==1)
								 //this can be improve
				string_depth = SDepth(res_l,res_r);
			LAQs(vl, vr, string_depth+x, &next_l, &next_r);
			aux_l = next_l;
			aux_r = next_r;

			while(aux_l != res_l || aux_r != res_r) {
				Parent(aux_l, aux_r, &aux_l, &aux_r);
				depth++;
			}

			if(res_l==next_l && res_r==next_r) {
				string_depth++;
				acum++;
			}
			else {
				acum = 1;
				res_l = next_l;
				res_r = next_r;
				x = d - depth;
			}
		}
		*laq_t_l = res_l;
		*laq_t_r = res_r;
	}

	size_t SuffixTreeY::getSize() const
	{
		size_t mem = sizeof(SuffixTreeY);
		mem += lcp->getSize();
		mem += npr->getSize();
		mem += csa->getSize();
		return mem;
	}

	void SuffixTreeY::save(ofstream & fp) const
	{
		size_t wr =  CSTY;
		saveValue(fp,wr);
		saveValue(fp, length);
		lcp->save(fp);
		npr->save(fp);
		csa->save(fp);
	}

	SuffixTreeY * SuffixTreeY::load(ifstream & fp) {
		SuffixTreeY *cst = new SuffixTreeY();
		size_t type = loadValue<size_t>(fp);
		if(type!=CSTY) {
			abort();
		}
		cst->length = loadValue<size_t>(fp);
		cst->lcp = LCP::load(fp);
		cst->npr = NPR::load(fp);
		cst->csa = TextIndex::load(fp);
		return cst;
	}

	SuffixTreeY::~SuffixTreeY() {
		delete (TextIndexCSA *)csa;
		size_t lcp_type = lcp->lcp_type;
		switch(lcp_type) {
			case NAIVE: delete (LCP_naive *)lcp; break;
			case SAD_GON_OS: delete (LCP_Sad *)lcp; break;
			case FMN_RRR_OS: delete (LCP_FMN *)lcp; break;
			case PT: delete (LCP_PT *)lcp; break;
			case PHI: delete (LCP_PhiSpare *)lcp; break;
			case DAC: delete (LCP_DAC *)lcp; break;
			case DAC_VAR: delete (LCP_DAC_VAR *)lcp; break;
			default: break;
		}
		size_t npr_type = npr->npr_type;
		switch(npr_type) {
			case FMN_NPR: delete (NPR_FMN *)npr; break;
			case CN_NPR: delete (NPR_CN *)npr; break;
			default: break;
		}
	}

};
