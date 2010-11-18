#ifndef _BASICS_TREES_H
#define _BASICS_TREES_H

#include <libcdsBasics.h>
using namespace std;

#define MAX(i,j) (((i) > (j)) ? (i) : (j))

namespace cds_utils{	
	typedef unsigned char byte;

	static const unsigned int  MAXINT=0x7fffffff;
	static const unsigned int  FACT_RANK = 20;

	/** Retrieve a given index from array A where every value uses len bits
	 * @param A Array
	 * @param len Length in bits of each field
	 * @param index Position to be retrieved
	 * */
	inline size_t get_field_64(size_t *A, size_t len, size_t index) {
		if(len==0) return 0;
		size_t result;
		long long i=1, j=1;
		i=i*index*len/W, j=j*index*len-W*i;
		if (j+len <= W)
			result = (A[i] << (W-j-len)) >> (W-len);
		else {
			result = A[i] >> j;
			result = result | (A[i+1] << (WW-j-len)) >> (W-len);
		}
		return result;
	}

	inline void set_field_64(size_t *A, size_t len, size_t index, size_t x) {
		if(len==0) return;
		long long i=1, j=1;	  
		i= i*index*len/W, j= j*index*len-i*W;
		size_t mask = ((j+len) < W ? ~0u << (j+len) : 0)
			| ((W-j) < W ? ~0u >> (W-j) : 0);
		A[i] = (A[i] & mask) | x << j;
		if (j+len>W) {
			mask = ((~0u) << (len+j-W));
			A[i+1] = (A[i+1] & mask)| x >> (W-j);
		}
	}

	/*Compute the longest common prefix between the suffix (with max length = lim) that starts in t1 and t2
	 * @param text The text
	 * @param t1 Position in the text where start the first suffix
	 * @param t2 Position in the text where start the second suffix
	 * @param lim Maximum value that can be returned
	 * */
	inline size_t lcp_length(char *text, size_t t1, size_t t2, size_t lim){
		size_t cont=0;
		for(size_t i=0; i<lim;i++){
			if(text[t1+i]==text[t2+i])
				cont++;
			else
				return cont;
		}
		return cont;
	}


	/* returns e[p..p+len-1], assuming len <= W*/
	inline size_t bitread (uint *e, uint p, uint len){ 
		uint answ=0;
		e += p/W; p %= W;
		answ = *e >> p;
		if (len == W){ 
			if (p) 
				answ |= (*(e+1)) << (W-p);
		}
		else{ 
			if (p+len > W) 
				answ |= (*(e+1)) << (W-p);
			answ &= (1<<len)-1;
		}
		return answ;
	}

	/* writes e[p..p+len-1] = s, len <= W*/
	inline void bitwrite (register uint *e, register uint p, register uint len, register uint s){ 
		e += p/W; 
		p %= W;
		if (len == W){ 
			*e |= (*e & ((1<<p)-1)) | (s << p);
			if (!p) 
				return;
			e++;
			*e = (*e & ~((1<<p)-1)) | (s >> (W-p));
		}
		else { 
			if (p+len <= W){ 
				*e = (*e & ~(((1<<len)-1)<<p)) | (s << p);
				return;
			}
			*e = (*e & ((1<<p)-1)) | (s << p);
			e++; len -= W-p;
			*e = (*e & ~((1<<len)-1)) | (s >> (W-p));
		}
	}


	inline size_t loadText(char *filename, char **textt, size_t *length) {
		char *text; 
		ifstream in(filename);
		if(!in.good())
			return 1;
		in.seekg(0,ios_base::end);
		size_t len = in.tellg();
		text = new char[len+1];
		in.seekg(0,ios_base::beg);
		in.read(text,len);
		in.close();
		text[len] = '\0';
		*textt = text;
		*length = len+1;
		 return 0;
	}

};


#endif

