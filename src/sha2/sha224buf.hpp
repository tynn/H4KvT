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

#ifndef sha224buf_class
#define sha224buf_class

#include <cstdint>
#include "sha256buf.hpp"

#define SHA224
#include "sha256.h"

class sha224buf : public sha256buf
{
	public:
		explicit sha224buf()
			: sha256buf(_H20, _H21, _H22, _H23, _H24, _H25, _H26, _H27) { }
		std::string hex() { return sha256buf::hex().substr(0, 56); }
};

#endif

