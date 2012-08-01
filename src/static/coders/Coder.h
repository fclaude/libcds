/*  Coder.h
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
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

#ifndef CODER_H
#define CODER_H

#include <Array.h>
#include <libcdsBasics.h>
namespace cds_static
{

	class Coder
	{
		public:
			virtual size_t encode(uint symb, uint * stream, size_t pos) const = 0;
			virtual size_t decode(uint * symb, uint *stream, size_t pos) const = 0;
			virtual size_t maxLength() const = 0;
			virtual size_t getSize() const = 0;
			virtual void save(ofstream & fp) const = 0;
			static Coder * load(ifstream & fp);
	};

};

#include <HuffmanCoder.h>
#endif
