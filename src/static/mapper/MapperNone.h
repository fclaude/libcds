/* MapperNone.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * MapperNone definition
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

#ifndef _MAPPERNONE_H
#define _MAPPERNONE_H

#include <iostream>

#include <libcdsBasics.h>
#include <Mapper.h>

using namespace std;

namespace cds_static
{

	/** Mapper that doesn't change the value (identity)
	 *
	 *  @author Francisco Claude
	 */
	class MapperNone : public Mapper
	{
		public:
			MapperNone();
			virtual ~MapperNone() {}
			virtual uint map(const uint s) const;
			virtual uint unmap(const uint s) const;
			virtual size_t getSize() const;
			virtual void save(ofstream & out) const;
			static MapperNone * load(ifstream & input);
	};
};
#endif							 /* __MAPPER_NONE_H */
