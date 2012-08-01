/* suftest3.test.c
   Copyright (C) 2006, Rodrigo Gonzalez, all rights reserved.

   This file contains an implementation of the interface for CSA.
   For more information, see

   K. Sadakane. Compressed text databases with efficient query
	 algorithms based on the compressed suffix array.
	 In Proceedings 11th Annual International Symposium on Algorithms
	 and Computation (ISAAC)}, LNCS v. 1969, pages 410--421, 2000.

   K. Sadakane. Succinct representations of lcp information and
	 improvements in the compressed suffix arrays.
	 In Proceedings 13th Annual ACM-SIAM Symposium on Discrete
	 Algorithms (SODA), 2002.

   K. Sadakane. New text indexing functionalities of the compressed
	 suffix arrays. Journal of Algorithms, 48(2):294--313, 2003.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <stdlib.h>
#include <stdio.h>
#if 1
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif
#include <comparray4.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#ifndef min
#define min(x,y) ((x)<(y)?(x):(y))
#endif

namespace cds_static
{

	#if 1
	typedef struct timeb mytimestruct;
	void mygettime(mytimestruct *t) {
		ftime(t);
	}
	double mylaptime(mytimestruct *before,mytimestruct *after) {
		double t;
		t = after->time - before->time;
		t += (double)(after->millitm - before->millitm)/1000;
		return t;
	}
	#else
	typedef mytimestruct struct rusage;
	void mygettime(mytimestruct *t) {
		getrusage(RUSAGE_SELF,t);
	}
	double mylaptime(mytimestruct *before,mytimestruct *after) {
		double t;
		t = after->ru_utime.tv_sec - before->ru_utime.tv_sec;
		t += (double)(after->ru_utime.tv_usec
			- before->ru_utime.tv_usec)/1000000;
		return t;
	}
	#endif

	/* Three function to variables to manage parameters */
	static bool is_delimeter(char *delimiters, char c) {
		int i=0,len_delimiters=strlen(delimiters);
		bool is=false;
		for (i=0;i<len_delimiters;i++)
			if (c == delimiters[i]) is=true;
		return is;
	}

	static void parse_parameters(char *options, int *num_parameters, char ***parameters, char *delimiters) {
		int i=0,j=0,temp=0,num=0, len_options=strlen(options);
		char *options_temp;
		while  (i<len_options) {
			while ((i<len_options) && is_delimeter(delimiters,options[i])) i++;
			temp=i;
			while ((i<len_options) && !is_delimeter(delimiters,options[i])) i++;
			if (i!=temp) num++;
		}
		(*parameters) = (char **) malloc(num*sizeof(char * ));
		i=0;
		while  (i<len_options) {
			while ((i<len_options) && is_delimeter(delimiters,options[i])) i++;
			temp=i;
			while ((i<len_options) && !is_delimeter(delimiters,options[i])) i++;
			if (i!=temp) {
				(*parameters)[j]=(char *) malloc((i-temp+1)*sizeof(char));
				options_temp = options+temp;
				strncpy((*parameters)[j], options_temp, i-temp);
				((*parameters)[j])[i-temp] = '\0';
				j++;
			}
		}
		*num_parameters = num;
	}

	static void free_parameters(int num_parameters,char ***parameters) {
		int i=0;
		for (i=0; i<num_parameters;i++)
			free((*parameters)[i]);
		free((*parameters));
	}

	/*////////////////////
	//Building the Index//
	////////////////////*/
	extern void suffixsort(int *x, int *p, int n, int k, int l);

	int load_index(char *filename, void **index) {
		char fname1[128]="",fname2[128]="";
		CSA *SA = NULL;
		SA = (CSA *) malloc(sizeof(CSA));
		csa_init(SA);
		initranktables();
		mkdecodetable();
		sprintf(fname1,"%s.psi",filename);
		sprintf(fname2,"%s.idx",filename);
		csa_read(SA,fname1,fname2);
		(*index) = SA;
		return 0;
	}

	int build_index(uchar *text, ulong length, char *build_options, void **index) {
		char delimiters[] = " =;";
		char filename[256]="";
		int j,num_parameters;
		char ** parameters;
		int rankb_w=16,rankb_w2=128;
		int free_text=false;	 /* don't free text by default */
		if (build_options != NULL) {
			parse_parameters(build_options,&num_parameters, &parameters, delimiters);
			for (j=0; j<num_parameters;j++) {
				if ((strcmp(parameters[j], "samplerate") == 0 ) && (j < num_parameters-1) ) {
					rankb_w=atoi(parameters[j+1]);
					j++;
				}
				else if  ((strcmp(parameters[j], "samplepsi") == 0 ) && (j < num_parameters-1) ) {
					rankb_w2=atoi(parameters[j+1]);
					j++;
				}
				else if  ((strcmp(parameters[j], "filename") == 0 ) && (j < num_parameters-1) ) {
					strcpy(filename,parameters[j+1]);
					j++;
				} else if (strcmp(parameters[j], "free_text") == 0 )
				free_text=true;
			}
			free_parameters(num_parameters, &parameters);
		}

		int n=length;
		char fname1[128]="",fname2[128]="";

		/* make the SA */
		int  i,  *x, *p;
		int  k, l;
		p= (int *)malloc((n+1)*sizeof *p);
		x= (int *)malloc((n+1)*sizeof *x);
		if (! p || ! x) {
			return 1;
		}
		for ( i=0; i<n; i++) {
			x[i]=text[i];
		}
		l=0;
		k=UCHAR_MAX+1;

		suffixsort(x, p, n, k, l);
		free(x);
		p[0] = n;
		/* End Make SA */

		/*  0..n-1  n  0 B
			   p[0] K n Bp[1..n]0..n-1B*/
								 /* p[1..n]1..nBp[0]=n+1*/
		for (i=0; i<=n; ++i) p[i]++;

		sprintf(fname1,"%s.psi",filename);
		sprintf(fname2,"%s.idx",filename);
		csa_new(n,p,text,fname1,fname2,rankb_w,rankb_w2);
		free(p);
		if (free_text)
			free(text);
		load_index(filename, index);
		return 0;
	}

	int save_index(void *index, char *filename) {
		/*si lo puedo grabar*/
		return 99;
	}

	int free_index(void *index) {
		CSA *SA=(CSA *) index;
		#ifdef USE_MMAP
		if(SA->mapp!=NULL)
			free(SA->mapp);
		if(SA->mapi!=NULL)
			free(SA->mapi);
		#else
		free(SA->B)
			free(SA->ISA);
		free(SA->R);
		free(SA->SA);
		#endif
		free(SA);
		return 0;
	}
	int index_size(void *index, ulong *size) {
		CSA *SA=(CSA *) index;
		*size=0;
		#ifdef USE_MMAP
		*size+=sizeof(MMAP)*2;
		*size+=(SA->p_size);	 //Size B
		*size+=(SA->i_size);	 //Size ISA+R+SA
		#endif
		*size+=sizeof(CSA);
		return 0;
	}

	int index_size_count(void *index, ulong *size) {
		CSA *SA=(CSA *) index;
		*size=0;
		#ifdef USE_MMAP
		*size+=sizeof(MMAP)*2;
		*size+=(SA->p_size);	 //Size B
		*size+=(SA->i_size);	 //Size ISA+R+SA
		#endif
		*size+=sizeof(CSA);
		return 0;
	}
	int index_size_locate(void *index, ulong *size) {
		CSA *SA=(CSA *) index;
		*size=0;
		#ifdef USE_MMAP
		*size+=sizeof(MMAP)*2;
		*size+=(SA->p_size);	 //Size B
		*size+=(SA->i_size);	 //Size ISA+R+SA
		#endif
		*size+=sizeof(CSA);
		return 0;
	}

	/*////////////////////
	//Querying the Index//
	////////////////////*/
	int count(void *index, uchar *pattern, ulong length, ulong *numocc) {
		int l,r,len;
		CSA *SA=(CSA *) index;
		len = csa_bsearch(pattern,length,SA,&l,&r);
		*numocc = r-l+1;
		return 0;
	}

	ulong locate_extract(void *index) {
		CSA *SA=(CSA *) index;
		ulong largo,*occ,lar,n=SA->n,l,r,lll=0;
		ulong matches,locate;
		ulong random,hh;
		for ( hh=1; hh <= 1000000; hh*=10)
		for (lar=1;lar<=9;lar++) {
			largo=lar*hh;
			occ=NULL;
			random = (ulong) (((float) rand()/ (float) RAND_MAX)*(n-1));
			matches = largo+1;
			locate=0;
			occ = (ulong *) malloc(matches*sizeof(ulong));
			l=random;
			r=min(random+largo,n-3);
			occ = csa_batchlookup2(SA,l,r);
			free(occ);
		}
		return lll;
	}

	int locate(void *index, uchar *pattern, ulong length, ulong **occ, ulong *numocc) {
		//*numocc=locate_extract(index);
		//exit(0);
		int l,r,len;
		CSA *SA=(CSA *) index;
		len = csa_bsearch(pattern,length,SA,&l,&r);
		*numocc = r-l+1;
		(*occ) = csa_batchlookup2(SA,l,r);
		return 0;
	}

	/*///////////////////////
	//Accessing the indexed//
	///////////////////////*/
	int display(void *index, uchar *pattern, ulong length, ulong numc, ulong *numocc, uchar **snippet_text, ulong **snippet_lengths) {
		int l,r;
		int pos;
		ulong *occ, i, j, from, to, len, x;
		uchar *text_aux;
		CSA *SA=(CSA *) index;
		csa_bsearch(pattern,length,SA,&l,&r);
		*numocc = r-l+1;
		occ = csa_batchlookup2(SA,l,r);
		*snippet_lengths = (ulong *) malloc((*numocc)*sizeof(ulong));
		if (!(*snippet_lengths)) return 1;
		*snippet_text = (uchar *) malloc((*numocc)*(length+2*numc)*sizeof(uchar));
		if (!(*snippet_text)) return 1;
		text_aux=*snippet_text;

		for (i=0;i<(*numocc);i++) {
			x=occ[i];
			if (x>numc) from = x-numc;
			else from=0;
			to= ((int)(x+length+numc-1)<(int)(SA->n-1)?(x+length+numc-1):(SA->n-1));
			len =to-from+1;
			pos = csa_inverse(SA,from+1);
			for (j=0; (int)j<(int)len;j++) {
				text_aux[j] = csa_T(SA,pos);
				pos= csa_psi(SA,pos);
			}
			text_aux+=length+2*numc;
			(*snippet_lengths)[i] = len;
		}
		free(occ);
		return 0;
	}

	int extract(void *index, ulong from, ulong to, uchar **snippet, ulong *snippet_length) {
		CSA *SA=(CSA *) index;
		ulong n = SA->n;
		int pos;
		if (to >= n) to=n-1;
		if (from > to) {
			*snippet = NULL;
			*snippet_length=0;
		}
		else {
			ulong j;
			ulong len =to-from+1;
			*snippet = (uchar *) malloc((len)*sizeof(uchar));
			if (!(*snippet)) return 1;
			pos = csa_inverse(SA,from+1);
			for (j=0; j<len;j++) {
				(*snippet)[j]=csa_T(SA,pos);
				pos= csa_psi(SA,pos);
			}
			(*snippet_length)=len;
		}
		return 0;
	}

	int get_length(void *index, ulong *length) {
		(*length)=((CSA *) index)->n;
		return 0;
	}

	/*////////////////
	//Error handling//
	////////////////*/
	char * error_index(int e) {
		char *error = new char[50];
		switch(e) {
			case 0:  printf(error, "No error"); break;
			case 1:  printf(error, "Out of memory"); break;
			case 2:  printf(error, "The text must end with a \\0"); break;
			case 5:  printf(error, "You can't free the text if you don't copy it"); break;
			case 20: printf(error, "Cannot create files"); break;
			case 21: printf(error, "Error writing the index"); break;
			case 22: printf(error, "Error writing the index"); break;
			case 23: printf(error, "Cannot open index; break");
			case 24: printf(error, "Cannot open text; break");
			case 25: printf(error, "Error reading the index"); break;
			case 26: printf(error, "Error reading the index"); break;
			case 27: printf(error, "Error reading the text"); break;
			case 28: printf(error, "Error reading the text"); break;
			case 99: printf(error, "Not implemented"); break;
			default: printf(error, "Unknown error");
		}
		return error;
	}

};
