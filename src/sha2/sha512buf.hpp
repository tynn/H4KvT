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

#ifndef sha512buf_class
#define sha512buf_class

#include <cstdint>
#include "../hashbuf.hpp"

struct uint128_t {
	uint64_t value[2];
	uint128_t() : uint128_t(0, 0) { }
	uint128_t(const uint128_t &o) : uint128_t(o.value[0], o.value[1]) { }
	uint128_t(const uint64_t &least, const uint64_t &most=0)
	{
		this->value[0] = least;
		this->value[1] = most;
	}
	uint128_t & operator+=(const uint128_t &right)
	{
		value[1] += right.value[1];
		if (value[0] + right.value[0] < value[0])
			value[1] += 1;
		value[0] += right.value[0];
		return *this;
	}
};

typedef hashbuf_t<8, uint64_t, 128, uint128_t> _sha512buf;
class sha512buf : public _sha512buf
{
	public:
		explicit sha512buf();

	protected:
		explicit sha512buf(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

	private:
		int sync();
		void update();

		void update_sha512(uint64_t &, uint64_t &, uint64_t &, uint64_t &, uint64_t &, uint64_t &, uint64_t &, uint64_t &);

		uint64_t H0, H1, H2, H3, H4, H5, H6, H7;
};

#endif

