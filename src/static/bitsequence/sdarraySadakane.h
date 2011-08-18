/* sdarraySadakane.h
 * Copyright (C) 2009, K. Sadakane, all rights reserved.
 *
 * This is an adapatation of the original sdarray implementation
 * by K. Sadakane. Modifications and probable bugs were introduced
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

#ifndef SDARRAY_H
#define SDARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>

#include <libcdsBasics.h>
#include <cppUtils.h>
#include <libcdsSDArray.h>

namespace cds_static
{

    using namespace cds_utils;

    typedef struct
    {
        int n,m;
        int size;
        unsigned char *buf;
        uint *lp;
        uint *sl;
        ushort *ss;
        uint ss_len, sl_len;
        uint *p;
    } selectd2;

    typedef struct
    {
        int n,m,d;
        int size;
        unsigned char *hi;
        uint *low;
        selectd2 *sd0,*sd1;
        uint hi_len, low_len;
        //uint lasti, lasts;
    } selects3;

    int selects3_construct(selects3 *select, const int n, const uint *buf);
    int selects3_select(const selects3 *select, const int i);
    int selects3_rank(const selects3 *select, const int i);
    int selects3_selectnext(const selects3 *select, const int i);

    void make___selecttbl(void);
    void selectd2_save(const selectd2 * s, ofstream & fp);
    void selects3_save(const selects3 * s, ofstream & fp);

    void selectd2_load(selectd2 * s, ifstream & fp);
    void selects3_load(selects3 * s, ifstream & fp);

    void selectd2_free(selectd2 * s);
    void selects3_free(selects3 * s);

}
#endif
