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

#include "sha256buf.hpp"
#include "sha256.h"


sha256buf::sha256buf() : sha256buf(_H00, _H01, _H02, _H03, _H04, _H05, _H06, _H07) { }

sha256buf::sha256buf(uint32_t _H0, uint32_t _H1, uint32_t _H2, uint32_t _H3, uint32_t _H4, uint32_t _H5, uint32_t _H6, uint32_t _H7)
	: _sha256buf(), H0(_H0), H1(_H1), H2(_H2), H3(_H3), H4(_H4), H5(_H5), H6(_H6), H7(_H7) { }

void sha256buf::update() { update_sha256(H0, H1, H2, H3, H4, H5, H6, H7); }

int sha256buf::sync()
{
	dig[0] = H0; dig[1] = H1; dig[2] = H2; dig[3] = H3; dig[4] = H4; dig[5] = H5; dig[6] = H6; dig[7] = H7;
	update_sha256(dig[0], dig[1], dig[2], dig[3], dig[4], dig[5], dig[6], dig[7]);
	return 0;
}

#define _BE(X) (((X&0xff)<<24)|((X&0xff00)<<8)|(X>>8&0xff00)|(X>>24&0xff))

static void _update_sha256(uint32_t &H0, uint32_t &H1, uint32_t &H2, uint32_t &H3, uint32_t &H4, uint32_t &H5, uint32_t &H6, uint32_t &H7, uint32_t w[])
{
	uint32_t a(H0), b(H1), c(H2), d(H3), e(H4), f(H5), g(H6), h(H7), T1, T2;
	size_t t;
	uint32_t W[64];

	static const uint32_t K[] = { _K };

	for (t = 0; t < 16; t++)
		W[t] = _BE(w[t]);

	#define K(t) K[t]
	#define W(t) W[t]
	SHA256(a,b,c,d,e,f,g,h,T1,T2,t)
	#undef K
	#undef W

	H0 += a; H1 += b; H2 += c; H3 += d; H4 += e; H5 += f; H6 += g; H7 += h;
}

void sha256buf::update_sha256(uint32_t &H0, uint32_t &H1, uint32_t &H2, uint32_t &H3, uint32_t &H4, uint32_t &H5, uint32_t &H6, uint32_t &H7)
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
			_update_sha256(H0, H1, H2, H3, H4, H5, H6, H7, words);
			words = new uint32_t[16]();
		}

		words[15] = _BE(bits);
		words[14] = _BE(((uint32_t *) &bits)[1]);
	}

	_update_sha256(H0, H1, H2, H3, H4, H5, H6, H7, words);
}

