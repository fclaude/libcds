/*  timing.h
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<stack>
#include<cassert>
#include<cmath>
#include<ctime>

using namespace std;

namespace cds_utils
{
	/** Resets the system timer (yes, we only have one)
	 */
	void start_timing();

	/** Obtains the number of milliseconds since the last timer start
	 */
	double get_timing();

};
