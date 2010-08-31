/* sdarraySadakane.cpp
 * Copyright (C) 2009, K. Sadakane, all rights reserved.
 *
 * This is an adapatation of the original implementation by
 * K. Sadakane. Modifications and possible bugs were introduced
 * by Francisco Claude.
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

#include <sdarraySadakane.h>

namespace cds_static
{

    #if 0
    typedef unsigned int qword;
    #define logD 4
    #else
    typedef unsigned long long qword;
    #define logD 5
    #endif
    #define PBS (sizeof(uint)*8)
    #define D (1<<logD)
    #define logM 5
    #define M (1<<logM)
    #define logP 8
    #define P (1<<logP)
    #define logLL 16             // size of word
    #define LL (1<<logLL)
    //#define logLLL 7
    #define logLLL 5
    //#define LLL 128
    //#define LLL 32
    #define LLL (1<<logLLL)
    //#define logL 10
    //#define logL (logLL-3)
    #define logL (logLL-1-5)
    #define L (1<<logL)

    int __blog(const int x1) {
        int x=x1;
        int l;
        l = 0;
        while (x>0) {
            x>>=1;
            l++;
        }
        return l;
    }

    int __setbit(uint *B, const int i, const int x) {
        int j,l;
        //printf("%u\n",D);
        j = i / D;
        l = i % D;
        if (x==0) B[j] &= (~(1<<(D-1-l)));
        else if (x==1) B[j] |= (1<<(D-1-l));
        else {
            printf("error __setbit x=%d\n",x);
            exit(1);
        }
        return x;
    }

    int __setbit2(unsigned char *B, const int i, const int x) {
        int j,l;

        j = i / 8;
        l = i % 8;
        if (x==0) B[j] &= (~(1<<(8-1-l)));
        else if (x==1) B[j] |= (1<<(8-1-l));
        else {
            printf("error __setbit2 x=%d\n",x);
            exit(1);
        }
        return x;
    }

    int __setbits(uint *B, const int i, const int d, const int x) {
        int j;

        for (j=0; j<d; j++) {
            __setbit(B,i+j,(x>>(d-j-1))&1);
        }
        return x;
    }

    int __getbit(const uint *B, const int i) {
        int j,l;

        //j = i / D;
        //l = i % D;
        j = i >> logD;
        l = i & (D-1);
        return (B[j] >> (D-1-l)) & 1;
    }

    int __getbit2(const unsigned char *B, const int i) {
        int j,l;

        //j = i / D;
        //l = i % D;
        j = i >> 3;
        l = i & (8-1);
        return (B[j] >> (8-1-l)) & 1;
    }

    #if 1
    uint __getbits(const uint *B, const int i1, const int d) {
        qword x,z;

        int i = i1;
        B += (i >> logD);
        i &= (D-1);
        if (i+d <= 2*D) {
            x = (((qword)B[0]) << D) + B[1];
            x <<= i;
            x >>= (D*2-1-d);
            x >>= 1;
        }
        else {
            x = (((qword)B[0])<<D)+B[1];
            z = (x<<D)+B[2];
            x <<= i;
            x &= (((qword)1L<<D)-1)<<D;
            z <<= i;
            z >>= D;
            x += z;
            x >>= (2*D-d);
        }

        return x;
    }
    #endif

    #if 0
    uint __getbits(uint *B, int i, int d) {
        uint j,x;

        x = 0;
        for (j=0; j<d; j++) {
            x <<= 1;
            x += __getbit(B,i+j);
        }
        return x;
    }
    #endif

    static const unsigned int _popCount[] = {
        0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
    };

    static unsigned int __selecttbl[8*256];
    static int built = 0;

    void make___selecttbl(void) {
        if(built) return;
        built = 1;
        int i,x,r;
        uint buf[1];
        buf[0] = 0;

        for (x = 0; x < 256; x++) {
            __setbits(buf,0,8,x);
            for (r=0; r<8; r++) __selecttbl[(r<<8)+x] = -1;
            r = 0;
            for (i=0; i<8; i++) {
                if (__getbit(buf,i)) {
                    __selecttbl[(r<<8)+x] = i;
                    r++;
                }
            }
        }
    }

    unsigned int __popCount(const uint x) {
        uint r;
        #if 0
        r = x;
        r = r - ((r>>1) & 0x77777777) - ((r>>2) & 0x33333333) - ((r>>3) & 0x11111111);
        r = ((r + (r>>4)) & 0x0f0f0f0f) % 0xff;
        #elif 1
        r = x;
        r = ((r & 0xaaaaaaaa)>>1) + (r & 0x55555555);
        r = ((r & 0xcccccccc)>>2) + (r & 0x33333333);
        //r = ((r & 0xf0f0f0f0)>>4) + (r & 0x0f0f0f0f);
        r = ((r>>4) + r) & 0x0f0f0f0f;
        //r = ((r & 0xff00ff00)>>8) + (r & 0x00ff00ff);
        r = (r>>8) + r;
        //r = ((r & 0xffff0000)>>16) + (r & 0x0000ffff);
        r = ((r>>16) + r) & 63;
        #else
        r = _popCount[x & 0xff];
        x >>= 8;
        r += _popCount[x & 0xff];
        x >>= 8;
        r += _popCount[x & 0xff];
        x >>= 8;
        r += _popCount[x & 0xff];
        #endif
        return r;
    }

    unsigned int __popCount8(const uint x) {
        uint r;
        #if 1
        r = x;
        r = ((r & 0xaa)>>1) + (r & 0x55);
        r = ((r & 0xcc)>>2) + (r & 0x33);
        r = ((r>>4) + r) & 0x0f;
        #else
        r = _popCount[x & 0xff];
        #endif
        return r;
    }

    void selectd2_save(const selectd2 * s, ofstream & fp) {
        assert(fp.good());
        saveValue(fp,s->n);
        saveValue(fp,s->m);
        saveValue(fp,s->size);
        saveValue(fp,s->ss_len);
        saveValue(fp,s->sl_len);
        saveValue(fp,s->buf,(s->n+7)/8+1);
        uint nl = (s->m-1) / L + 1;
        saveValue(fp,s->lp,nl+1);
        saveValue(fp,s->p,nl+1);
        saveValue(fp,s->ss,s->ss_len);
        saveValue(fp,s->sl,s->sl_len);
    }

    void selectd2_load(selectd2 * s, ifstream & fp) {
        s->n = loadValue<int>(fp);
        s->m = loadValue<int>(fp);
        s->size = loadValue<int>(fp);
        s->ss_len = loadValue<uint>(fp);
        s->sl_len = loadValue<uint>(fp);
        s->buf = loadValue<unsigned char>(fp,(s->n+7)/8+1);
        uint nl = (s->m-1) / L + 1;
        s->lp = loadValue<uint>(fp, nl+1);
        s->p = loadValue<uint>(fp, nl+1);
        s->ss = loadValue<ushort>(fp, s->ss_len);
        s->sl = loadValue<uint>(fp, s->sl_len);
    }

    void selectd2_free(selectd2 * s) {
        //delete [] s->buf;
        delete [] s->lp;
        delete [] s->p;
        delete [] s->ss;
        delete [] s->sl;
    }

    int selectd2_construct(selectd2 *select, const int n, const unsigned char *buf) {
        int i,m;
        int nl;
        int p,pp;
        int il,is,ml,ms;
        int r;
        uint *s;

        make___selecttbl();

        if (L/LLL == 0) {
            printf("ERROR: L=%d LLL=%d\n",L,LLL);
            exit(1);
        }

        m = 0;
        for (i=0; i<n; i++) m += __getbit2(buf,i);
        select->n = n;
        select->m = m;
        //printf("n=%d m=%d\n",n,m);

        select->buf = (unsigned char*)buf;

        s = new uint[m];
        m = 0;
        for (i=0; i<n; i++) {
            if (__getbit2(buf,i)) {
                m++;
                s[m-1] = i;
            }
        }

        nl = (m-1) / L + 1;
        select->size = 0;        //ignoring buf, shared with selects3
        select->lp = new uint[nl+1];
        for(int k=0;k<nl+1;k++) select->lp[k]=0;
        select->size += (nl+1)*sizeof(uint);
        select->p = new uint[nl+1];
        for(int k=0;k<nl+1;k++) select->p[k]=0;
        select->size += (nl+1)*sizeof(uint);

        for (r = 0; r < 2; r++) {
            ml = ms = 0;
            for (il = 0; il < nl; il++) {
                pp = s[il*L];
                select->lp[il] = pp;
                i = min((il+1)*L-1,m-1);
                p = s[i];
                //printf("%d ",p-pp);
                if (p - pp >= LL) {
                    if (r == 1) {
                        for (is = 0; is < L; is++) {
                            if (il*L+is >= m) break;
                            select->sl[ml*L+is] = s[il*L+is];
                        }
                    }
                    select->p[il] = -((ml<<logL)+1);
                    ml++;
                }
                else {
                    if (r == 1) {
                        for (is = 0; is < L/LLL; is++) {
                            if (il*L+is*LLL >= m) break;
                            select->ss[ms*(L/LLL)+is] = s[il*L+is*LLL] - pp;
                        }
                    }
                    select->p[il] = ms << (logL-logLLL);
                    ms++;
                }
            }
            if (r == 0) {
                select->sl = new uint[ml*L+1];
                for(int k=0;k<ml*L+1;k++) select->sl[k]=0;
                select->size += sizeof(uint)*(ml*L+1);
                select->sl_len = ml*L+1;
                select->ss = new ushort[ms*(L/LLL)+1];
                for(int k=0;k<ms*(L/LLL)+1;k++) select->ss[k]=0;
                select->ss_len = ms*(L/LLL)+1;
                select->size += sizeof(ushort)*(ms*(L/LLL)+1);
            }
        }
        delete [] s;
        return 0;
    }

    int selectd2_select(const selectd2 *select, const int i1, const int f) {
        int i = i1;
        int p,r;
        int il;
        int rr;
        unsigned char *q;

        if (i == 0) return -1;

        #if 0
        if (i > select->m) {
            printf("ERROR: m=%d i=%d\n",select->m,i);
            exit(1);
        }
        #endif

        i--;

        il = select->p[i>>logL];
        if (il < 0) {
            il = -il-1;
            //p = select->sl[(il<<logL)+(i & (L-1))];
            p = select->sl[il+(i & (L-1))];
        }
        else {
            p = select->lp[i>>logL];
            //p += select->ss[(il<<(logL-logLLL))+(i & (L-1))/LLL];
            p += select->ss[il+((i & (L-1))>>logLLL)];
            r = i - (i & (LLL-1));

            q = &(select->buf[p>>3]);

            if (f == 1) {
                rr = p & (8-1);
                r -= _popCount[*q >> (8-1-rr)];
                //p = p - rr;

                while (1) {
                    rr = _popCount[*q];
                    if (r + rr >= i) break;
                    r += rr;
                    //p += 8;
                    q++;
                }
                p = (q - select->buf) << 3;
                p += __selecttbl[((i-r-1)<<8)+(*q)];
            }
            else {
                rr = p & (8-1);
                r -= _popCount[(*q ^ 0xff) >> (8-1-rr)];
                //p = p - rr;

                while (1) {
                    rr = _popCount[*q ^ 0xff];
                    if (r + rr >= i) break;
                    r += rr;
                    //p += 8;
                    q++;
                }
                p = (q - select->buf) << 3;
                p += __selecttbl[((i-r-1)<<8)+(*q ^ 0xff)];
            }
        }
        return p;
    }

    int selectd2_select2(const selectd2 *select, const int i1, const int f, int *st, int *en) {
        int i = i1;
        int p,r,p2;
        int il;
        int rr;
        unsigned char *q;

        if (i == 0) {
            *st = -1;
            return -1;
        }

        #if 0
        if (i > select->m) {
            printf("ERROR: m=%d i=%d\n",select->m,i);
            exit(1);
        }
        #endif

        i--;

        il = select->p[i>>logL];
        if (il < 0) {
            il = -il-1;
            //p = select->sl[(il<<logL)+(i & (L-1))];
            p = select->sl[il+(i & (L-1))];

            if ((i>>logL) == ((i+1)>>logL)) {
                p2 = select->sl[il+((i+1) & (L-1))];
            }
            else {
                p2 = selectd2_select(select,i+2,f);
            }
        }
        else {
            p = select->lp[i>>logL];
            //p += select->ss[(il<<(logL-logLLL))+(i & (L-1))/LLL];
            p += select->ss[il+((i & (L-1))>>logLLL)];
            r = i - (i & (LLL-1));

            q = &(select->buf[p>>3]);

            if (f == 1) {
                rr = p & (8-1);
                r -= _popCount[*q >> (8-1-rr)];
                //p = p - rr;

                while (1) {
                    rr = _popCount[*q];
                    if (r + rr >= i) break;
                    r += rr;
                    //p += 8;
                    q++;
                }
                p = (q - select->buf) << 3;
                p += __selecttbl[((i-r-1)<<8)+(*q)];

                if ((i>>logL) == ((i+1)>>logL)) {
                    i++;
                    while (1) {
                        rr = _popCount[*q];
                        if (r + rr >= i) break;
                        r += rr;
                        q++;
                    }
                    p2 = (q - select->buf) << 3;
                    p2 += __selecttbl[((i-r-1)<<8)+(*q)];
                }
                else {
                    p2 = selectd2_select(select,i+2,f);
                }

            }
            else {
                rr = p & (8-1);
                r -= _popCount[(*q ^ 0xff) >> (8-1-rr)];
                //p = p - rr;

                while (1) {
                    rr = _popCount[*q ^ 0xff];
                    if (r + rr >= i) break;
                    r += rr;
                    //p += 8;
                    q++;
                }
                p = (q - select->buf) << 3;
                p += __selecttbl[((i-r-1)<<8)+(*q ^ 0xff)];

                if ((i>>logL) == ((i+1)>>logL)) {
                    i++;
                    while (1) {
                        rr = _popCount[*q ^ 0xff];
                        if (r + rr >= i) break;
                        r += rr;
                        q++;
                    }
                    p2 = (q - select->buf) << 3;
                    p2 += __selecttbl[((i-r-1)<<8)+(*q ^ 0xff)];
                }
                else {
                    p2 = selectd2_select(select,i+2,f);
                }
            }
        }
        *st = p;
        *en = p2;
        return p;
    }

    void selects3_save(const selects3 * s, ofstream & fp) {
        saveValue(fp,s->n);
        saveValue(fp,s->m);
        saveValue(fp,s->size);
        saveValue(fp,s->d);
        saveValue(fp,s->hi_len);
        saveValue(fp,s->low_len);
        saveValue(fp,s->hi,s->hi_len);
        saveValue(fp,s->low,s->low_len);
        selectd2_save(s->sd0,fp);
        selectd2_save(s->sd1,fp);
    }

    void selects3_load(selects3 * s, ifstream & fp) {
        s->n = loadValue<int>(fp);
        s->m = loadValue<int>(fp);
        s->size = loadValue<int>(fp);
        s->d = loadValue<int>(fp);
        s->hi_len = loadValue<uint>(fp);
        s->low_len = loadValue<uint>(fp);
        s->hi = loadValue<unsigned char>(fp,s->hi_len);
        s->low = loadValue<uint>(fp,s->low_len);
        s->sd0 = new selectd2;
        selectd2_load(s->sd0,fp);
        s->sd1 = new selectd2;
        selectd2_load(s->sd1,fp);
        delete [] s->sd0->buf;
        delete [] s->sd1->buf;
        s->sd0->buf = s->hi;
        s->sd1->buf = s->hi;
    }

    void selects3_free(selects3 * s) {
        delete [] s->hi;
        delete [] s->low;
        //delete [] s->sd0->buf;
        selectd2_free(s->sd0);
        delete s->sd0;
        selectd2_free(s->sd1);
        delete s->sd1;
    }

    int selects3_construct(selects3 *select, const int n, const uint *buf) {
        int i,m;
        int d,mm;
        uint *low;
        unsigned char *buf2;
        selectd2 *sd0,*sd1;

        m = 0;
        for (i=0; i<n; i++) m += __getbit(buf,i);
        select->n = n;
        select->m = m;

        if (m == 0) return 0;

        mm = m;
        d = 0;
        while (mm < n) {
            mm <<= 1;
            d++;
        }

        select->d = d;

        buf2 = new unsigned char[(2*m+8-1)/8+1];
        for(int k=0;k<(2*m+8-1)/8+1;k++) buf2[k]=0;
        select->hi_len = (2*m+8-1)/8+1;
        low = new uint[(d*m+PBS-1)/PBS+1];
        for(uint k=0;k<(d*m+PBS-1)/PBS+1;k++) low[k]=0;
        select->low_len = (d*m+PBS-1)/PBS+1;

        select->hi = buf2;
        select->low = low;
        select->size = sizeof(unsigned char)*((2*m+8-1)/8+1) + sizeof(uint)*((d*m+PBS-1)/PBS+1);

        for (i=0; i<m*2; i++) __setbit2(buf2,i,0);

        m = 0;
        for (i=0; i<n; i++) {
            if (__getbit(buf,i)) {
                __setbit2(buf2,(i>>d)+m,1);
                __setbits(low,m*d,d,i & ((1<<d)-1));
                m++;
            }
        }

        sd1 = new selectd2;
        sd0 = new selectd2;
        select->size += 2*sizeof(selectd2);

        selectd2_construct(sd1,m*2,buf2);
        select->sd1 = sd1;

        for (i=0; i<m*2; i++) __setbit2(buf2,i,1-__getbit2(buf2,i));
        selectd2_construct(sd0,m*2,buf2);
        select->sd0 = sd0;

        for (i=0; i<m*2; i++) __setbit2(buf2,i,1-__getbit2(buf2,i));
        return 0;
    }

    //selects3 * lasts3=NULL;
    //int lasti=0;
    //int lasts=0;

    int selects3_select(const selects3 *select, const int i) {
        int d,x;

        #if 0
        if (i > select->m) {
            printf("ERROR: m=%d i=%d\n",select->m,i);
            exit(1);
        }
        #endif

        if (i == 0) return -1;

        d = select->d;
        /*if(select->lasti==(uint)i-1) {
            while(!__getbit2(select->sd1->buf,++select->lasts));
        }
        else {
          select->lasts = selectd2_select(select->sd1,i,1);
        }
        select->lasti = i;
        //lasts3 = select; */
        x = selectd2_select(select->sd1,i,1) - (i-1);
        //x = (select->lasts-(i-1)) << d;
        x <<= d;
        x += __getbits(select->low,(i-1)*d,d);
        return x;
    }

    int selects3_selectnext(const selects3 *select, const int i) {
        //return selects3_select(select,selects3_rank(select,i)+1);
        int i2;
        if(i>0) i2=i-1;
        else i2=i;
        int d,x,w,y;
        int r,j;
        int z,ii;
        uint *q;
        d = select->d;
        q = select->low;
        ii = i2>>d;
        y = selectd2_select(select->sd0,ii,0)+1;
        int k2=y-ii;
        x = y - ii;
        int x_orig = x;
        j = i2 - (ii<<d);
        r = y & 7;
        y >>= 3;
        z = select->hi[y];
        while (1) {
            if (((z << r) & 0x80) == 0) {
                if(x!=x_orig) k2++;
                break;
            }
            w = __getbits(q,x*d,d);
            if (w >= j) {
                if (w == j) {
                    if(__getbit2(select->hi,(8*y+r))) k2++;
                    x++;
                    r++;
                }
                break;
            }
            x++;
            r++;
            if(__getbit2(select->hi,(8*y+r))) k2++;
            if (r == 8) {
                r = 0;
                y++;
                z = select->hi[y];
            }
        }
        if(x==select->m)
            return (uint)-1;
        int c=8*y+r;
        int fin=0;
        for(int kk=0;kk<8-r;kk++) {
            if(__getbit2(select->hi,c)) {
                fin=1;
                break;
            }
            c++;
        }
        if(!fin) {
            int pp = c/8;
            while(select->hi[pp]==0) {
                pp++;
                c+=8;
            }
            while(!__getbit2(select->hi,c)) c++;
        }
        c -= (k2);
        return __getbits(q,x*d,d)+((c)<<d);
    }

    int selects3_rank(const selects3 *select, const int i) {
        int d,x,w,y;
        int r,j;
        int z,ii;
        uint *q;

        d = select->d;
        q = select->low;

        ii = i>>d;

        y = selectd2_select(select->sd0,ii,0)+1;
        //  selectd2_select2(select->sd0,ii,0,&y1,&y2);
        //y1++;  y2++;
        //printf("y %d y1 %d  %d\n",y,y1,y2-y1);

        x = y - ii;

        j = i - (ii<<d);

        r = y & 7;
        y >>= 3;
        z = select->hi[y];
        while (1) {
            if (((z << r) & 0x80) == 0) break;
            w = __getbits(q,x*d,d);
            if (w >= j) {
                if (w == j) x++;
                break;
            }
            x++;
            r++;
            if (r == 8) {
                r = 0;
                y++;
                z = select->hi[y];
            }
        }

        return x;
    }
}
