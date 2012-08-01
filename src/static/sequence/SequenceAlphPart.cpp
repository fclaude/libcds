/*  SequenceAlphaPart.cpp
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <SequenceAlphPart.h>

#include <iostream>
#include <algorithm>

using namespace std;

namespace cds_static
{

	/*    inline uint group(uint pos, uint cut) {
			uint ret = 0;
			if(pos<=cut)
				ret = pos;
			else
				ret = bits(pos-cut)+cut;
			return ret;
		}

		inline uint offset(uint pos, uint cut, uint group) {
			//cout << "pos=" << pos << " cut=" << cut << " grp=" << group << endl;
			uint ret = 0;
			if(pos<=cut)
				ret = 0;
			else
				ret = pos-cut-(1<<(group-cut-1));
			//cout << "ret=" << ret << endl;
			return ret;
		}
		*/

	inline uint group(uint pos, uint cut) {
		uint ret = 0;
		if(pos<=cut)
			ret = pos;
		else
			ret = bits(pos)+cut-bits(cut);
		return ret;
	}

	inline uint offset(uint pos, uint cut, uint group) {
		uint ret = 0;
		if(pos<=cut)
			ret = 0;
		else
			ret = pos-((1<<(group-cut+bits(cut)-1)));
		return ret;
	}

	SequenceAlphPart::SequenceAlphPart(const Array & seq, uint _cut, SequenceBuilder * lenIndexBuilder, SequenceBuilder * seqsBuilder) : Sequence(0) {

		size_t n = seq.getLength();
		length = n;
		// Say we are using the builders
		lenIndexBuilder->use();
		seqsBuilder->use();

		this->cut = (1<<_cut)-1;
		// Compute the size of the alphabet
		sigma = 0;
		for(uint i=0;i<n;i++) sigma=max(sigma,seq[i]);

		// Compute the frequency of each symbol
		size_t * occ = new size_t[sigma+1];
		for(uint i=0;i<=sigma;i++) {
			assert(i<=sigma);
			occ[i] = 0;
		}
		for(uint i=0;i<n;i++) {
			assert(seq[i]<=sigma);
			occ[seq[i]]++;
		}

		// Create pairs (frequency,symbol) and then sort by frequency
		pair<size_t,uint> * pairs = new pair<size_t,uint>[sigma+2];
		for(uint i=0;i<=sigma;i++) {
			assert(i<=sigma+1);
			pairs[i] = pair<size_t,uint>(occ[i],i);
		}
		pairs[sigma+1] = pair<size_t,uint>(0,sigma+1);
		sort(pairs,pairs+sigma+2,greater<pair<size_t,uint> >());

		revPermFreq = new uint[sigma+1];
		for(uint i=0;i<=sigma;i++) {
			assert(i<=sigma);
			revPermFreq[i] = 0;
		}
		origsigma = sigma;
		sigma = 0;
		while(pairs[sigma].first>0) {
			assert(sigma<=origsigma);
			assert(pairs[sigma].second<=origsigma);
			revPermFreq[pairs[sigma].second]=sigma;
			sigma++;
		}

		// We don't need occ anymore
		delete [] occ;

		// Fill alphSortedbyFreq and lengthForSymb
		alphSortedByFreq = new uint[sigma+1];
		for(uint i=0;i<sigma+1;i++) {
			assert(i<=sigma);
			alphSortedByFreq[i] = 0;
		}

		uint * groupForSymb = new uint[origsigma+1];
		for(uint i=0;i<origsigma+1;i++) {
			assert(i<=origsigma);
			groupForSymb[i] = 0;
		}

		// We estimate maxLen, it may be smaller if many symbols have 0 frequency
		maxLen = group(sigma,cut);

		// Initialize the lengths of each sequence in indexesByLength
		size_t * lenLength = new size_t[maxLen+1];
		for(uint i=0;i<=maxLen;i++) {
			assert(i<=maxLen);
			lenLength[i] = 0;
		}

		// Compute the actual value for lenLengths and maxLen
		for(uint i=0;i<=sigma;i++) {
			if(pairs[i].first==0) break;
			assert(i<sigma);
			alphSortedByFreq[i] = pairs[i].second;
			uint sl = group(i,cut);
			assert(pairs[i].second<=origsigma);
			groupForSymb[pairs[i].second] = sl;
			//cout << "groupForSymb[" << pairs[i].second << "]=" << sl << endl;
			assert(sl<=maxLen);
			lenLength[sl]+=pairs[i].first;
			//maxLen = sl;
		}

		// Now we build lengthsIndex
		uint * tmpSeq = new uint[n];
		for(uint i=0;i<n;i++) {
			assert(i<n);
			assert(seq[i] <= origsigma);
			tmpSeq[i] = groupForSymb[seq[i]];
		}
		groupsIndex = lenIndexBuilder->build(tmpSeq,n);
		//delete [] tmpSeq;

		// Now we build the other sequences
		size_t sum = 0;
		uint ** seqs = new uint*[(maxLen<=cut)?0:maxLen-cut+1];
		for(uint i=0;(maxLen>cut) && i<maxLen-cut;i++) {
			assert(i<maxLen-cut);
			assert(i+cut+1<=maxLen);
			seqs[i] = new uint[lenLength[i+cut+1]];
			sum += lenLength[i+cut+1];
			//cout << "len=" << lenLength[i+cut+1] << " sum=" << sum << endl;
		}

		// Lets compute the offsets
		uint * lenLength2 = new uint[maxLen+1];
		for(uint i=0;i<maxLen+1;i++) {
			assert(i<=maxLen);
			lenLength2[i] = 0;
		}

		for(uint i=0;i<n;i++) {
			if(groupForSymb[seq[i]]>cut) {
				assert(seq[i]<=origsigma);
				assert(groupForSymb[seq[i]]<=maxLen);
				assert(groupForSymb[seq[i]]-cut-1<=maxLen-cut);
				assert(lenLength2[groupForSymb[seq[i]]]<lenLength[groupForSymb[seq[i]]]);
				seqs[groupForSymb[seq[i]]-cut-1][lenLength2[groupForSymb[seq[i]]]++] = offset(revPermFreq[seq[i]],cut,groupForSymb[seq[i]]);
				//cout << "Group=" << groupForSymb[seq[i]] << " offset=" << offset(revPermFreq[seq[i]],cut,groupForSymb[seq[i]]) << endl;
			}
		}

		indexesByLength = new Sequence*[(maxLen<=cut)?0:maxLen-cut+1];
		for(uint i=0;maxLen>cut && i<maxLen-cut;i++) {
			indexesByLength[i] = seqsBuilder->build(seqs[i],lenLength[i+cut+1]);
			delete [] seqs[i];
		}

		delete [] seqs;
		delete [] groupForSymb;
		delete [] lenLength;

		// We are done with the builders
		lenIndexBuilder->unuse();
		seqsBuilder->unuse();

		this->maxLen = maxLen;
		this->sigma = sigma;
	}

	SequenceAlphPart::SequenceAlphPart(uint * seq, size_t n, uint cut, SequenceBuilder * lenIndexBuilder, SequenceBuilder * seqsBuilder) : Sequence(n) {

		// Say we are using the builders
		lenIndexBuilder->use();
		seqsBuilder->use();

		// Compute the size of the alphabet
		sigma = 0;
		for(uint i=0;i<n;i++) sigma=max(sigma,seq[i]);

		// Compute the frequency of each symbol
		size_t * occ = new size_t[sigma+1];
		for(uint i=0;i<=sigma;i++) occ[i] = 0;
		for(uint i=0;i<n;i++) occ[seq[i]]++;

		// Create pairs (frequency,symbol) and then sort by frequency
		pair<size_t,uint> * pairs = new pair<size_t,uint>[sigma+2];
		for(uint i=0;i<=sigma;i++)
			pairs[i] = pair<size_t,uint>(occ[i],i);
		pairs[sigma+1] = pair<size_t,uint>(0,sigma+1);
		sort(pairs,pairs+sigma+2,greater<pair<size_t,uint> >());

		uint * groupForSymb = new uint[sigma+2];
		revPermFreq = new uint[sigma+2];

		origsigma = sigma;
		sigma = 0;
		while(pairs[sigma].first>0) {
			revPermFreq[pairs[sigma].second]=sigma;
			sigma++;
		}

		// We don't need occ anymore
		delete [] occ;

		// Fill alphSortedbyFreq and lengthForSymb
		alphSortedByFreq = new uint[sigma+1];

		// We estimate maxLen, it may be smaller if many symbols have 0 frequency
		maxLen = group(sigma,cut);

		// Initialize the lengths of each sequence in indexesByLength
		uint * lenLength = new uint[maxLen+1];
		for(uint i=0;i<maxLen;i++)
			lenLength[i] = 0;

		// Compute the actual value for lenLengths and maxLen
		for(uint i=0;i<=sigma;i++) {
			if(pairs[i].first==0) break;
			alphSortedByFreq[i] = pairs[i].second;
			uint sl = group(i,cut);
			groupForSymb[pairs[i].second] = sl;
			lenLength[sl]+=pairs[i].first;
			maxLen = sl;
		}

		// Now we build lengthsIndex
		uint * tmpSeq = new uint[n];
		for(uint i=0;i<n;i++)
			tmpSeq[i] = groupForSymb[seq[i]];
		groupsIndex = lenIndexBuilder->build(tmpSeq,n);
		delete [] tmpSeq;

		// Now we build the other sequences
		uint ** seqs = new uint*[maxLen-cut+1];
		for(uint i=0;i<maxLen-cut;i++)
			seqs[i] = new uint[lenLength[i+cut+1]];

		// Lets compute the offsets
		for(uint i=0;i<maxLen+1;i++)
			lenLength[i] = 0;

		for(uint i=0;i<n;i++) {
			if(groupForSymb[seq[i]]>cut) {
				seqs[groupForSymb[seq[i]]-cut-1][lenLength[groupForSymb[seq[i]]]++] = offset(revPermFreq[seq[i]],cut,groupForSymb[seq[i]]);
				//cout << "Group=" << groupForSymb[seq[i]] << " offset=" << offset(revPermFreq[seq[i]],cut,groupForSymb[seq[i]]) << endl;
			}
		}

		indexesByLength = new Sequence*[maxLen-cut+1];
		for(uint i=0;i<maxLen-cut;i++) {
			indexesByLength[i] = seqsBuilder->build(seqs[i],lenLength[i+cut+1]);
			delete [] seqs[i];
		}

		delete [] seqs;
		delete [] groupForSymb;
		delete [] lenLength;
		delete [] pairs;

		// We are done with the builders
		lenIndexBuilder->unuse();
		seqsBuilder->unuse();

		this->cut = cut;
		this->maxLen = maxLen;
		this->sigma = sigma;
	}

	SequenceAlphPart::SequenceAlphPart() : Sequence(0) {
		length = 0;
		sigma = 0;
	}

	SequenceAlphPart::~SequenceAlphPart() {
		for(uint i=0;maxLen>cut && i<maxLen-cut;i++)
			delete indexesByLength[i];
		delete [] indexesByLength;
		delete groupsIndex;
		delete [] revPermFreq;
		delete [] alphSortedByFreq;
	}

	size_t SequenceAlphPart::rank(uint c, size_t i) const
	{
		uint pos = revPermFreq[c];
		if(pos<=cut) {
			return groupsIndex->rank(pos,i);
		}
		uint g = group(pos,cut);
		uint o = offset(pos,cut,g);
		size_t ind = groupsIndex->rank(g,i);
		if(ind==0) return 0;
		return indexesByLength[g-cut-1]->rank(o,ind-1);
	}

	size_t SequenceAlphPart::select(uint c, size_t i) const
	{
		uint pos = revPermFreq[c];
		if(pos<=cut) {
			return groupsIndex->select(pos,i);
		}
		uint g = group(pos,cut);
		uint o = offset(pos,cut,g);
		return groupsIndex->select(g,indexesByLength[g-cut-1]->select(o,i)+1);
	}

	uint SequenceAlphPart::access(size_t i) const
	{
		uint g = groupsIndex->access(i);
		if(g<=cut) return alphSortedByFreq[g];
		uint o = indexesByLength[g-cut-1]->access(groupsIndex->rank(g,i)-1);
		return alphSortedByFreq[(1<<(g-cut+bits(cut)-1))+o];
	}

	size_t SequenceAlphPart::getSize() const
	{
		size_t ret = 0;
		for(uint i=0;maxLen>cut && i<maxLen-cut;i++) {
			//cout << "i=" << i << " len=" << indexesByLength[i]->getLength() << " size=" << indexesByLength[i]->getSize() << endl;
			ret += indexesByLength[i]->getSize();
		}
		ret += groupsIndex->getSize();
		//cout << "groupsIndex->getSize()=" << groupsIndex->getSize() << endl;
		ret += sizeof(SequenceAlphPart);
		return ret;
	}

	void SequenceAlphPart::save(ofstream & fp) const
	{
		uint type = ALPHPART_HDR;
		saveValue(fp,type);
		saveValue(fp,length);
		saveValue(fp,sigma);
		saveValue(fp,origsigma);
		saveValue(fp,maxLen);
		saveValue(fp,cut);
		saveValue(fp,revPermFreq,sigma+1);
		saveValue(fp,alphSortedByFreq,sigma+1);
		groupsIndex->save(fp);
		for(uint i=0;maxLen>cut && i<maxLen-cut;i++)
			indexesByLength[i]->save(fp);
	}

	SequenceAlphPart * SequenceAlphPart::load(ifstream & fp) {
		uint type = loadValue<uint>(fp);
		if(type!=ALPHPART_HDR)
			return NULL;
		SequenceAlphPart * ret = new SequenceAlphPart();
		ret->length = loadValue<size_t>(fp);
		ret->sigma = loadValue<uint>(fp);
		ret->origsigma = loadValue<uint>(fp);
		ret->maxLen = loadValue<uint>(fp);
		ret->cut = loadValue<uint>(fp);
		ret->revPermFreq = loadValue<uint>(fp,ret->sigma+1);
		ret->alphSortedByFreq = loadValue<uint>(fp,ret->sigma+1);
		ret->groupsIndex = Sequence::load(fp);
		ret->indexesByLength = new Sequence*[(ret->maxLen<=ret->cut)?0:(ret->maxLen-ret->cut)];
		for(uint i=0;ret->maxLen>ret->cut && i<ret->maxLen-ret->cut;i++)
			ret->indexesByLength[i] = Sequence::load(fp);
		return ret;
	}

};
