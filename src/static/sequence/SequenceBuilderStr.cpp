/*  SequenceBuilderStr.cpp
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

#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<stack>
#include<cassert>
#include<cmath>

using namespace std;

#include <SequenceBuilderStr.h>
#include <PermutationBuilder.h>

namespace cds_static
{
	SequenceBuilderStr::SequenceBuilderStr(const string & str) {
		this->str = str;
	}

	SequenceBuilderStr::~SequenceBuilderStr() { }

	Sequence * SequenceBuilderStr::build(uint *seq, size_t len) {
		/*SequenceBuilder * b = getBuilder(seq,len);
		Sequence * ret = b->build(seq,len);
		delete b;
		return ret;
				*/
				return NULL;			 
	}

	Sequence * SequenceBuilderStr::build(const Array & seq) {
		SequenceBuilder * b = getBuilder(seq);
		Sequence * ret = b->build(seq);
		delete b;
		return ret;
	}

	SequenceBuilder * SequenceBuilderStr::getBuilder(const Array & seq) {
		vector<string> params;
		tokenize(str,params,';');
		map<string,string> values;
		for(uint i=1;i<params.size();i++) {
			vector<string> res;
			tokenize(params[i],res,'=');
			if(res.size()==2)
				values[res[0]]=res[1];
			else
				values[params[i]]="";
		}
		#ifdef VERBOSE
		cout << "params[0]=" << params[0] << endl;
		#endif
		if(params[0]=="WaveletTree") {
			BitSequenceBuilder * bsb = NULL;
			if(values.find("bitseq")==values.end()) return NULL;
			if(values.find("bitsamp")==values.end()) return NULL;
			uint samp = transform(values["bitsamp"]);
			if(values["bitseq"]=="BitSequenceRG") {
				bsb = new BitSequenceBuilderRG(samp);
				#ifdef VERBOSE
				cout << "bitseq=BitSequenceRG" << endl;
				#endif
			}
			else if(values["bitseq"]=="BitSequenceRRR") {
				bsb = new BitSequenceBuilderRRR(samp);
				#ifdef VERBOSE
				cout << "bitseq=BitSequenceRRR" << endl;
				#endif
			}
			else if(values["bitseq"]=="BitSequenceSDArray") {
				bsb = new BitSequenceBuilderSDArray();
				#ifdef VERBOSE
				cout << "bitseq=BitSequenceSDArray" << endl;
				#endif
			}
			else if(values["bitseq"]=="BitSequenceDArray") {
				bsb = new BitSequenceBuilderDArray();
				#ifdef VERBOSE
				cout << "bitseq=BitSequenceDArray" << endl;
				#endif
			}
			else {
				return NULL;
			}
			#ifdef VERBOSE
			cout << "samp=" << samp << endl;
			#endif
			if(values.find("mapper")==values.end()) return NULL;
			Mapper * am = NULL;
			if(values["mapper"]=="MapperCont") {
				am = new MapperCont(seq, *bsb);
				#ifdef VERBOSE
				cout << "mapper=MapperCont" << endl;
				#endif
			}
			else if(values["mapper"]=="MapperNone") {
				am = new MapperNone();
				#ifdef VERBOSE
				cout << "mapper=MapperNone" << endl;
				#endif
			}
			else {
				delete bsb;
				return NULL;
			}
			if(values.find("shape")==values.end()) return NULL;
			wt_coder * wc = NULL;
			if(values["shape"]=="Huffman") {
				wc = new wt_coder_huff(seq, am);
				#ifdef VERBOSE
				cout << "shape=Huffman" << endl;
				#endif
			}
			else if(values["shape"]=="Plain") {
				wc = new wt_coder_binary(seq, am);
				#ifdef VERBOSE
				cout << "shape=Plain" << endl;
				#endif
			}
			else {
				delete bsb;
				delete am;
				return NULL;
			}
			return new SequenceBuilderWaveletTree(bsb, am, wc);
		}
		else if(params[0]=="WaveletTreeNoptrs") {
			BitSequenceBuilder * bsb = NULL;
			if(values.find("bitseq")==values.end()) return NULL;
			if(values.find("bitsamp")==values.end()) return NULL;
			uint samp = transform(values["bitsamp"]);
			if(values["bitseq"]=="BitSequenceRG") {
				bsb = new BitSequenceBuilderRG(samp);
			}
			else if(values["bitseq"]=="BitSequenceRRR") {
				bsb = new BitSequenceBuilderRRR(samp);
			}
			else if(values["bitseq"]=="BitSequenceSDArray") {
				bsb = new BitSequenceBuilderSDArray();
			}
			else if(values["bitseq"]=="BitSequenceDArray") {
				bsb = new BitSequenceBuilderDArray();
			}
			else {
				return NULL;
			}
			if(values.find("mapper")==values.end()) return NULL;
			Mapper * am = NULL;
			if(values["mapper"]=="MapperCont") {
				am = new MapperCont(seq,*bsb);
			}
			else if(values["mapper"]=="MapperNone") {
				am = new MapperNone();
			}
			else {
				delete bsb;
				return NULL;
			}
			return new SequenceBuilderWaveletTreeNoptrs(bsb,am);
		}
		else if(params[0]=="WaveletMatrix") {
			BitSequenceBuilder * bsb = NULL;
			if(values.find("bitseq")==values.end()) return NULL;
			if(values.find("bitsamp")==values.end()) return NULL;
			uint samp = transform(values["bitsamp"]);
			if(values["bitseq"]=="BitSequenceRG") {
				bsb = new BitSequenceBuilderRG(samp);
			}
			else if(values["bitseq"]=="BitSequenceRRR") {
				bsb = new BitSequenceBuilderRRR(samp);
			}
			else if(values["bitseq"]=="BitSequenceSDArray") {
				bsb = new BitSequenceBuilderSDArray();
			}
			else if(values["bitseq"]=="BitSequenceDArray") {
				bsb = new BitSequenceBuilderDArray();
			}
			else {
				return NULL;
			}
			if(values.find("mapper")==values.end()) return NULL;
			Mapper * am = NULL;
			if(values["mapper"]=="MapperCont") {
				am = new MapperCont(seq,*bsb);
			}
			else if(values["mapper"]=="MapperNone") {
				am = new MapperNone();
			}
			else if(values["mapper"]=="MapperRev") {
				am = new MapperRev(seq);
			}
			else {
				delete bsb;
				return NULL;
			}
			return new SequenceBuilderWaveletMatrix(bsb,am);
		}
		else if(params[0]=="SequenceGMR") {
			BitSequenceBuilder * bsb = NULL;
			if(values.find("bitseq")==values.end()) return NULL;
			if(values.find("bitsamp")==values.end()) return NULL;
			uint samp = transform(values["bitsamp"]);
			if(values.find("chunklen")==values.end()) return NULL;
			uint chunklen = transform(values["chunklen"]);
			if(values.find("permsamp")==values.end()) return NULL;
			uint permsamp = transform(values["permsamp"]);
			if(values["bitseq"]=="BitSequenceRG") {
				bsb = new BitSequenceBuilderRG(samp);
			}
			else if(values["bitseq"]=="BitSequenceRRR") {
				bsb = new BitSequenceBuilderRRR(samp);
			}
			else if(values["bitseq"]=="BitSequenceSDArray") {
				bsb = new BitSequenceBuilderSDArray();
			}
			else if(values["bitseq"]=="BitSequenceDArray") {
				bsb = new BitSequenceBuilderDArray();
			}
			else {
				return NULL;
			}
			if(values.find("mapper")==values.end()) return NULL;
			Mapper * am = NULL;
			if(values["mapper"]=="MapperCont") {
				am = new MapperCont(seq,*bsb);
			}
			else if(values["mapper"]=="MapperNone") {
				am = new MapperNone();
			}
			else {
				delete bsb;
				return NULL;
			}
			PermutationBuilder * pb = new PermutationBuilderMRRR(permsamp,bsb);
			SequenceBuilder * sb1 = new SequenceBuilderGMRChunk(bsb,pb);
			return new SequenceBuilderGMR(bsb,sb1,chunklen);
		}
		else if(params[0]=="SequenceAlphPart") {
			BitSequenceBuilder * bsb = NULL;
			if(values.find("bitseq")==values.end()) return NULL;
			if(values.find("bitsamp")==values.end()) return NULL;
			uint samp = transform(values["bitsamp"]);
			if(values["bitseq"]=="BitSequenceRG") {
				bsb = new BitSequenceBuilderRG(samp);
			}
			else if(values["bitseq"]=="BitSequenceRRR") {
				bsb = new BitSequenceBuilderRRR(samp);
			}
			else if(values["bitseq"]=="BitSequenceSDArray") {
				bsb = new BitSequenceBuilderSDArray();
			}
			else if(values["bitseq"]=="BitSequenceDArray") {
				bsb = new BitSequenceBuilderDArray();
			}
			else {
				return NULL;
			}
			if(values.find("mapper")==values.end()) return NULL;
			Mapper * am = NULL;
			if(values["mapper"]=="MapperCont") {
				am = new MapperCont(seq,*bsb);
			}
			else if(values["mapper"]=="MapperNone") {
				am = new MapperNone();
			}
			else {
				delete bsb;
				return NULL;
			}
			if(values.find("shape")==values.end()) return NULL;
			wt_coder * wc = NULL;
			if(values["shape"]=="Huffman") {
				wc = NULL;		 // new wt_coder_huff(seq,am);
			}
			else if(values["shape"]=="Plain") {
				wc = new wt_coder_binary(seq,am);
			}
			else {
				delete bsb;
				delete am;
				return NULL;
			}
			SequenceBuilder * fstin = new SequenceBuilderWaveletTree(bsb,am,wc);
			if(values.find("cut")==values.end()) return NULL;
			uint cut = transform(values["cut"]);
			if(values.find("permsamp")==values.end()) return NULL;
			uint permsamp = transform(values["permsamp"]);
			PermutationBuilder * pb = new PermutationBuilderMRRR(permsamp,bsb);
			SequenceBuilder * scb = new SequenceBuilderGMRChunk(bsb,pb);
			SequenceBuilder * sec = new SequenceBuilderGMR(bsb,scb);
			return new SequenceBuilderAlphPart(fstin,sec,cut);
			//return new SequenceBuilderAlphPart(fstin,fstin,cut);
		}
		return NULL;
	}
};
