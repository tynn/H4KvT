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

#include <streambuf>
#include <string>

#include <cstdint>

class md5buf : public std::streambuf
{
	public:
		explicit md5buf();

		std::string hexdigest();

	private:
		int_type overflow(int_type);
		int sync();

		void update_md5(uint32_t &, uint32_t &, uint32_t &, uint32_t &);

		char buf[64];
		uint32_t A, B, C, D, dig[4];
		uint64_t len;
};

