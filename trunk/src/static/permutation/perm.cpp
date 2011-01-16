/* perm.cpp
 * Copyright (C) 2005, Diego Arroyuelo, all rights reserved.
 * Copyright (C) 2009, Francisco Claude
 *
 * Permutation
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

#include <perm.h>

namespace cds_static {

int compare(const void *p1, const void *p2) {
  return  ((auxbwd *)p1)->key - ((auxbwd *)p2)->key;
}


perm createPerm(uint *elems, uint nelems, uint t, BitSequenceBuilder * bmb) {
  perm P;
  uint *b, *baux, nextelem, i, j, bptr,
    aux, antbptr,nbwdptrs, elem,nbits, cyclesize;
  auxbwd *auxbwdptr;
  P = new struct sperm;
  P->elems  = elems;
  P->nelems = nelems;
  P->nbits  = bits(nelems-1);
  nbits = bits(nelems-1);
  P->t = t;
  if (t==1) {
    P->bwdptrs = new uint[uint_len(nelems,nbits)];
    assert(P->bwdptrs!=NULL);
    P->nbwdptrs = nelems;
    for (i=0; i<nelems; i++) {
      uint bg = get_field(elems, nbits, i);
      assert(bg<nelems);
      set_field(P->bwdptrs, nbits, bg, i);
    }
    P->bmap = NULL;
  }
  else {
    b = new uint[uint_len(nelems,1)];
    for(i=0;i<uint_len(nelems,1);i++)
      b[i]=0;
    assert(b!=NULL);    
    baux = new uint[uint_len(nelems,1)];
    for(i=0;i<uint_len(nelems,1);i++)
      baux[i] = 0;
    assert(baux!=NULL);
    nbwdptrs = 0;
    for (i = 0; i < nelems; i++) {
      if (bitget(baux,i) == 0) {
        nextelem = j = bptr = antbptr = i;
        aux = 0;
        bitset(baux, j);
        cyclesize = 0;
        while ((elem=get_field(elems,nbits,j)) != nextelem) {
          j = elem;
          bitset(baux, j);
          aux++;
          if (aux >= t) {
            nbwdptrs++;
            antbptr = bptr;
            bptr    = j;
            aux     = 0;
            bitset(b, j);
          }
          cyclesize++;
        }
        if (cyclesize >= t) {
          nbwdptrs++;
          bitset(b, nextelem);
        }
      }
    }
    auxbwdptr = new auxbwd[nbwdptrs];
    assert(auxbwdptr!=NULL);
    for(i=0;i<uint_len(nelems,1);i++)
      b[i]=0;
    for(i=0;i<uint_len(nelems,1);i++)
      baux[i] = 0;
    nbwdptrs = 0;
    for (i = 0; i < nelems; i++) {
      if (bitget(baux,i) == 0) {
        nextelem = j = bptr = antbptr = i;
        aux = 0;
        bitset(baux, j);
        cyclesize = 0;
        while ((elem=get_field(elems,nbits,j)) != nextelem) {
          j = elem;
          bitset(baux, j);
          aux++;
          if (aux >= t) {
            auxbwdptr[nbwdptrs].key = j;
            auxbwdptr[nbwdptrs++].pointer = bptr;
            antbptr = bptr;
            bptr    = j;
            aux     = 0;
            bitset(b, j);
          }
          cyclesize++;
        }
        if (cyclesize >= t) {
          auxbwdptr[nbwdptrs].key = nextelem;
          auxbwdptr[nbwdptrs++].pointer = bptr;
          bitset(b, nextelem);
        }
      }
    }
    qsort(auxbwdptr, nbwdptrs, sizeof(auxbwd), &compare);
    aux = uint_len(nbwdptrs,P->nbits);
    P->bwdptrs = new uint[aux];
    assert(P->bwdptrs!=NULL);
    for(i=0;i<aux;i++) P->bwdptrs[i] = 0;
    P->nbwdptrs = nbwdptrs;
    for (i = 0; i < nbwdptrs; i++) {
      set_field(P->bwdptrs, nbits, i, auxbwdptr[i].pointer);
      //if(i<5) 
      //  printf(" %d ",get_field(P->bwdptrs,nbits,i));
    }
    //printf("\n");
    P->bmap = bmb->build(b, nelems);
    //delete [] P->bmap;
    delete [] b;
    delete [] (baux);
    delete [] (auxbwdptr);
  }
  return P;
}


void destroyPerm(perm P) {
  delete [] P->elems;
  if (P->bmap) delete P->bmap;
  delete [] P->bwdptrs;
  delete P;
}


// Computes P-1[i]
uint inversePerm(const perm P, uint i) {
  uint j, elem;
  if (P->t==1) {
    j = get_field(P->bwdptrs,P->nbits,i); 
  }
  else {
    j = i;
    while (((elem=get_field(P->elems,P->nbits,j)) != i)&&(!P->bmap->access(j)))
      j = elem;

    if (elem != i) {
      // follows the backward pointer
      j = get_field(P->bwdptrs, P->nbits, P->bmap->rank1(j-1));
      while ((elem = get_field(P->elems,P->nbits,j))!= i)
        j = elem;
    }
  }
  return j;
}


// gets the ith element of a perm P

uint getelemPerm(const perm P, uint i) {
  return get_field(P->elems, P->nbits, i);
}


uint savePerm(const perm P, ofstream & f) {
  uint aux;
  uint v;

  saveValue(f,P->nelems);
  saveValue(f,P->elems,uint_len(P->nelems,P->nbits));

  aux = ((P->nelems+W-1)/W);

  if (P->bmap) {
    v=1;
    saveValue(f,v);
    P->bmap->save(f);
  }
  else {
    v=0;
    saveValue(f,v);
  }

  saveValue(f,P->nbwdptrs);
  aux = uint_len(P->nbwdptrs,P->nbits);
  saveValue(f,P->bwdptrs,aux);
  saveValue(f,P->t);
  return 0;
}


perm loadPerm(ifstream & f) {
  uint aux;
  perm P;
  uint v;

  P = new struct sperm;          //(struct sperm*) malloc(sizeof(struct sperm));

  P->nelems = loadValue<uint>(f);

  P->nbits = bits(P->nelems-1);
  aux = uint_len(P->nelems,P->nbits);
  P->elems = loadValue<uint>(f,aux);

  v = loadValue<uint>(f);

  if (v) {
    P->bmap = BitSequence::load(f);
  }
  else P->bmap = NULL;

  P->nbwdptrs = loadValue<uint>(f);

  aux = uint_len(P->nbwdptrs,P->nbits);
  P->bwdptrs = loadValue<uint>(f,aux);

  P->t = loadValue<uint>(f);

  return P;
}


uint sizeofPerm(perm P) {
  return sizeof(struct sperm) +
    ((uint_len(P->nelems,P->nbits))*sizeof(uint)) +
    ((P->bmap)?(P->bmap->getSize()):0) +
    ((uint_len(P->nbwdptrs,P->nbits))*sizeof(uint));
}

};
