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

#ifndef SUFFIXY_H
#define SUFFIXY_H

#include <LCP.h>
#include <NPR.h>
#include <SuffixTree.h>

namespace cds_static
{

	class SuffixTreeY : public SuffixTree
	{
		private:
			SuffixTreeY();

			TextIndex *csa;
			LCP *lcp;
			NPR *npr;

		public:
			/*Create the compressed suffix tree data structure base on Canovas and Navarro "Practical Compressed Suffix Trees"
			 * @param text The text from where the suffix tree will be create
			 * @param n  The length of the text
			 * @param lcp_type The lcp data structure that will be use (NAIVE|SAD_GON_OS|FMN_RRR_OS|PT|PHI|DAC|DAC_VAR).
			 * @param npr_type The npr data structute that will be use (FMN_NPR|CN_NPR).
			 * @param b  The block size used in NPR
			 * */
			SuffixTreeY(char *text, size_t n, size_t lcp_type = DAC, size_t npr_type = CN_NPR, size_t b = 32);

			/*Create the compressed suffix tree data structure base on Canovas and Navarro "Practical Compressed Suffix Trees"
			 *  @param _lcp The LCP array data structure
			 *  @param _npr The NPR data structure
			 *  @param csa  The Compressed suffix array data structure
			 *  */
			SuffixTreeY(LCP *_lcp, NPR *_npr, TextIndex *csa);

			/*Operations*/

			//Return true is [vl,vr] is the root
			virtual bool isRoot(size_t vl,size_t vr) const;

			//Return, in [vl,vr], the Root
			virtual void Root(size_t *vl, size_t *vr) const;

			//Return vr-vl+1
			virtual size_t Count(size_t vl, size_t vr) const;

			//Return true if [wl,wr] is ancestor of [vl,vr]
			virtual bool Ancestor(size_t wl, size_t wr, size_t vl, size_t vr) const;

			//Return the String Depth of the node [vl,vr]
			virtual size_t SDepth(size_t vl, size_t vr) const;

			//If [vl,vr] is a leaf (vr == vl), then return the suxffix array value at position v_l, Otherwise NULL.
			virtual size_t Locate(size_t vl, size_t vr) const;

			//Return, in [parent_l, parent_r], the parent of the node [vl,vr]
			virtual void Parent(size_t vl, size_t vr, size_t *parent_l, size_t *parent_r) const;

			//Return, in [fchild_l, fchild_r], the first child of the node [vl,vr]
			virtual void FChild(size_t vl, size_t vr, size_t *fchild_l, size_t *fchild_r) const;

			//Return, in [nsibling_l, nsibling_r], the next sibling of the node [vl,vr]
			virtual void NSibling(size_t vl, size_t vr, size_t *nsibling_l, size_t *nsibling_r) const;

			//Return, in [slink_l, slink_r], the suffix link of the node [vl,vr]
			virtual void SLink(size_t vl, size_t vr, size_t *slink_l, size_t *slink_r) const;

			//Return, in [slinki_l, slinki_r], the suffix link applied i times over the node [vl,vr]
			virtual void SLink_i(size_t vl, size_t vr, size_t i, size_t *slinki_l, size_t *slinki_r) const;

			//Return, in [lca_l, lca_r], the lower common ancestor between nodes [vl,vr] and [wl,wr]
			virtual void LCA(size_t vl, size_t vr, size_t wl, size_t wr, size_t *lca_l, size_t *lca_r) const;

			//Return, in [child_l, child_r], the child of the node [vl,vr] which label path start with the char a
			virtual void Child(size_t vl, size_t vr, uchar a, size_t *child_l, size_t *child_r) const;

			//Return the i-th letter of the label-path of the node [vl,vr]
			virtual uchar Letter(size_t vl, size_t vr, int i) const;

			//Return the tree depth of the node [vl,vr]
			virtual size_t TDepth(size_t vl, size_t vr) const;

			//Return, in [laq_s_l, laq_s_r], the highest ancestor of [vl,vr] with string-depth >=d
			virtual void LAQs(size_t vl, size_t vr, size_t d, size_t *laq_s_l, size_t *laq_s_r) const;

			//Return, in [laq_t_l, laq_t_r], the highest ancestor of [vl,vr] with tree-depth >=d
			virtual void LAQt(size_t vl, size_t vr, size_t d, size_t *laq_t_l, size_t *laq_t_r) const;

			//Memory use in bytes
			virtual size_t getSize() const;

			virtual void save(ofstream & fp) const;

			static SuffixTreeY * load(ifstream & fp);

			virtual ~SuffixTreeY();
	};

};
#endif
