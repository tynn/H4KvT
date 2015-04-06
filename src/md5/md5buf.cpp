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

#include "md5buf.hpp"
#include "md5.h"


md5buf::md5buf() : A(_A), B(_B), C(_C), D(_D), len(0)
{
	setp(buf, buf + 63);
}

md5buf::int_type md5buf::overflow(int_type ch)
{
	if (ch != traits_type::eof()) {
		*pptr() = char(ch);
		pbump(1);
		len += 512;
		update_md5(A, B, C, D);
		pbump(-64);
	}
	return ch;
}

int md5buf::sync()
{
	dig[0] = A; dig[1] = B; dig[2] = C; dig[3] = D;
	update_md5(dig[0], dig[1], dig[2], dig[3]);
	return 0;
}

static void _update_md5(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D, uint32_t x[])
{
	uint32_t a(A), b(B), c(C), d(D);
	#define X(k) x[k]
	MD5(a,b,c,d)
	#undef X
	A += a; B += b; C += c; D += d;
}

void md5buf::update_md5(uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D)
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
			_update_md5(A, B, C, D, words);
			words = new uint32_t[16]();
		}

		words[14] = bits & 0xffffffff;
		words[15] = bits>>32 & 0xffffffff;
	}

	_update_md5(A, B, C, D, words);
}

#define _BE(X) (((X&0xff)<<24)|((X&0xff00)<<8)|(X>>8&0xff00)|(X>>24&0xff))
std::string md5buf::hexdigest()
{
	sync();
	std::stringstream hex;
	hex << std::hex << std::setfill('0');
	hex << std::setw(8) << _BE(dig[0]);
	hex << std::setw(8) << _BE(dig[1]);
	hex << std::setw(8) << _BE(dig[2]);
	hex << std::setw(8) << _BE(dig[3]);
	return hex.str();
}


