/* wt_coder.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * wt_coder definition
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

#ifndef wt_coder_h
#define wt_coder_h

#include <libcdsBasics.h>
#include <fstream>

using namespace std;

namespace cds_static
{

    #define WT_CODER_HUFF_HDR 2
    #define WT_CODER_BINARY_HDR 3

    class wt_coder_binary;
    class wt_coder_huff;

    /** Coder that defines the shape of a wavelet tree
     *
     *  @author Francisco Claude
     */
    class wt_coder
    {
        public:
            wt_coder();
            virtual void use();
            virtual void unuse();
            virtual ~wt_coder() {};
            /** Tells if at level l the symbol is represented by a one or a zero */
            virtual bool is_set(uint symbol, uint l) const=0;
            /** Tells if the path of symbol becomes unique at level l */
            virtual bool done(uint symbol, uint l) const =0;
            /** Returns the size of the coder */
            virtual size_t getSize() const = 0;
            /** Returns the depth of the tree */
            virtual uint depth() const
            {
                return -1;       // Implemented in wt_coder_binary
            }
            /** Saves the coder to a file, returns 0 in case of success */
            virtual void save(ofstream & fp) const = 0;
            /** Loads a coder from a file, returns NULL in case of error */
            static wt_coder * load(ifstream & fp);
        protected:
            uint user_count;
    };

};

#include <wt_coder_huff.h>
#include <wt_coder_binary.h>
#endif
