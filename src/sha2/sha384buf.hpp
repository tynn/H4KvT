/*
 *	This file is part of H4KvT.
 *
 *	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
 *
 *	H4KvT is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	H4KvT is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with H4KvT. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef sha384buf_class
#define sha384buf_class

#include <cstdint>
#include "sha512buf.hpp"

#define SHA384
#include "sha512.h"

class sha384buf : public sha512buf
{
	public:
		explicit sha384buf()
			: sha512buf(_H30, _H31, _H32, _H33, _H34, _H35, _H36, _H37) { }
		std::string hex() { return sha512buf::hex().substr(0, 96); }
};

#endif

