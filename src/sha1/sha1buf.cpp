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

#include <sstream>
#include <iomanip>

#include "sha1buf.hpp"
#include "sha1.h"


sha1buf::sha1buf() : H0(_H0), H1(_H1), H2(_H2), H3(_H3), H4(_H4), len(0)
{
	setp(buf, buf + 63);
}

sha1buf::int_type sha1buf::overflow(int_type ch)
{
	if (ch != traits_type::eof()) {
		*pptr() = char(ch);
		pbump(1);
		len += 512;
		update_sha1(H0, H1, H2, H3, H4);
		pbump(-64);
	}
	return ch;
}

int sha1buf::sync()
{
	dig[0] = H0; dig[1] = H1; dig[2] = H2; dig[3] = H3; dig[4] = H4;
	update_sha1(dig[0], dig[1], dig[2], dig[3], dig[4]);
	return 0;
}

#define _BE(X) (((X&0xff)<<24)|((X&0xff00)<<8)|(X>>8&0xff00)|(X>>24&0xff))

static void _update_sha1(uint32_t &H0, uint32_t &H1, uint32_t &H2, uint32_t &H3, uint32_t &H4, uint32_t w[])
{
	uint32_t A(H0), B(H1), C(H2), D(H3), E(H4), T;
	size_t t;
	uint32_t W[80];
	for (t = 0; t < 16; t++)
		W[t] = _BE(w[t]);
	#define W(t) W[t]
	SHA1(A,B,C,D,E,T,t)
	#undef W
	H0 += A; H1 += B; H2 += C; H3 += D; H4 += E;
}

void sha1buf::update_sha1(uint32_t &H0, uint32_t &H1, uint32_t &H2, uint32_t &H3, uint32_t &H4)
{
	uint64_t bits(len);
	uint32_t *words = (uint32_t *) buf;
	char *ptr = pptr(), *end = epptr();

	bits += (ptr - pbase()) * 8;

	std::ptrdiff_t pad = end + 1 - ptr;
	if (pad) {
		*ptr = '\x80';

		if (pad >= 9)
			end -= 8;

		while (ptr++ < end)
			*ptr = '\0';

		if (pad < 9) {
			_update_sha1(H0, H1, H2, H3, H4, words);
			words = new uint32_t[16]();
		}

		words[15] = _BE(bits);
		words[14] = _BE(((uint32_t *) &bits)[1]);
	}

	_update_sha1(H0, H1, H2, H3, H4, words);
}

std::string sha1buf::hexdigest()
{
	sync();
	std::stringstream hex;
	hex << std::hex << std::setfill('0');
	hex << std::setw(8) << dig[0];
	hex << std::setw(8) << dig[1];
	hex << std::setw(8) << dig[2];
	hex << std::setw(8) << dig[3];
	hex << std::setw(8) << dig[4];
	return hex.str();
}


