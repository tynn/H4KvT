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

#ifndef sha1buf_class
#define sha1buf_class

#include <cstdint>
#include "../hashbuf.hpp"

class sha1buf : public hashbuf
{
	public:
		explicit sha1buf();
		std::string hexdigest();

	private:
		int_type overflow(int_type);
		int sync();

		void update_sha1(uint32_t &, uint32_t &, uint32_t &, uint32_t &, uint32_t &);

		char buf[64];
		uint32_t H0, H1, H2, H3, H4, dig[5];
		uint64_t len;
};

#endif

