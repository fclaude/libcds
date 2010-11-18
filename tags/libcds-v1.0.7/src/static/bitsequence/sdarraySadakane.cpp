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
                i = min((il+1)*(int)L-1,m-1);
                p = s[i];
                //printf("%d ",p-pp);
                if (p - pp >= (int)LL) {
                    if (r == 1) {
                        for (is = 0; is < (int)L; is++) {
                            if (il*(int)L+is >= m) break;
                            select->sl[ml*L+is] = s[il*L+is];
                        }
                    }
                    select->p[il] = -((ml<<logL)+1);
                    ml++;
                }
                else {
                    if (r == 1) {
                        for (is = 0; is < (int)(L/LLL); is++) {
                            if ((int)(il*L+is*LLL) >= m) break;
                            select->ss[ms*(L/LLL)+is] = s[il*L+is*LLL] - pp;
                        }
                    }
                    select->p[il] = ms << (logL-logLLL);
                    ms++;
                }
            }
            if (r == 0) {
                select->sl = new uint[ml*L+1];
                for(int k=0;k<ml*(int)L+1;k++) select->sl[k]=0;
                select->size += sizeof(uint)*(ml*L+1);
                select->sl_len = ml*L+1;
                select->ss = new ushort[ms*(L/LLL)+1];
                for(int k=0;k<ms*(int)(L/LLL)+1;k++) select->ss[k]=0;
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
        s->low = loadValue<uint>(fp, s->low_len);
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
