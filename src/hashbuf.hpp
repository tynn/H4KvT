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

#ifndef hashbuf_class
#define hashbuf_class

#include <iomanip>
#include <sstream>
#include <streambuf>
#include <string>

class hashbuf : public std::streambuf
{
	public:
		virtual std::string hex() = 0;
};

template <unsigned D, typename W, unsigned B, typename L>
class hashbuf_t : public hashbuf
{
	public:
		explicit hashbuf_t() : len(0) { setp(buf, buf + (B - 1)); }
		std::string hex()
		{
			sync();
			std::stringstream hex;
			hex << std::hex << std::setfill('0');
			for (unsigned i = 0; i < D; i++)
				hex << std::setw(2*sizeof(W)) << val(dig[i]);
			return hex.str();
		}

	private:
		virtual int sync() = 0;
		int_type overflow(int_type ch)
		{
			if (ch != traits_type::eof()) {
				*pptr() = char(ch);
				pbump(1);
				len += B*8;
				update();
				pbump(-B);
			}
			return ch;
		}

		virtual void update() = 0;
		virtual W val(W dig) { return dig; }

	protected:
		char buf[B];
		W dig[D];
		L len;
};

#endif

