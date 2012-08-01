/* mapper_cont.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * mapper_cont definition
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

#ifndef _MAPPERCONT_H
#define _MAPPERCONT_H

#include <libcdsBasics.h>
#include <iostream>

#include <Array.h>
#include <Mapper.h>
#include <BitSequence.h>
#include <BitSequenceBuilder.h>

using namespace std;
using namespace cds_utils;

namespace cds_static
{

	/** Mapper that makes the values in the set contiguous
	 *
	 *  @author Francisco Claude
	 */
	class MapperCont : public Mapper
	{
		public:
			MapperCont(const Array & seq, const BitSequenceBuilder & bsb);
			MapperCont(const uint * A, const size_t len, const BitSequenceBuilder & bsb);

			virtual ~MapperCont();

			virtual uint map(uint s) const;
			virtual uint unmap(uint s) const;
			virtual size_t getSize() const;

			virtual void save(ofstream & out) const;
			static MapperCont * load(ifstream & input);

		protected:
			MapperCont();
			BitSequence * m;
	};

};
#endif							 /* _MAPPERCONT_H */
