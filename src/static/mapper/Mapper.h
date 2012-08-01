/* Mapper.h
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 *
 * Mapper definition
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

#ifndef _MAPPER_H
#define _MAPPER_H

#include <libcdsBasics.h>
#include <iostream>

using namespace std;

namespace cds_static
{

	#define MAPPER_NONE_HDR 2
	#define MAPPER_CONT_HDR 3
	#define MAPPER_REV_HDR 4

	/** Base class for alphabet mappers
	 *
	 *  @author Francisco Claude
	 */
	class Mapper
	{
		public:
			Mapper();
			virtual ~Mapper() {}
			/** Maps the symbol */
			virtual uint map(uint s) const=0;
			/** Unmaps the symbol */
			virtual uint unmap(uint s) const=0;
			/** Returns the size of the mapper */
			virtual size_t getSize()  const=0;
			/** Saves the mapper to a file */
			virtual void save(ofstream & out) const=0;
			/** Loads the mapper from a file */
			static Mapper * load(ifstream & input);
			/** Reference counter incrementor */
			virtual void use();
			/** Reference counter decrementor */
			virtual void unuse();

		protected:
			/** Nr of references */
			int userCount;
	};
};

#include <MapperNone.h>
#include <MapperCont.h>
#include <MapperRev.h>
#endif							 /* _MAPPER_H */
