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

#include <ostream>
#include <string>

#include "md5buf.hpp"
#include "md5.h"

static std::string md5sum(const std::string msg)
{
	md5buf md5;
	std::ostream out(&md5);
	out << msg;
	return md5.hexdigest();
}

#define MD5TEST(msg,digit) if (md5sum(msg) != #digit) return 1;

int main()
{
	MD5_TEST_SUITE
	return 0;
}

