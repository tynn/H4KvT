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

#include "sha1buf.hpp"
#include "sha1.h"

static std::string sha1sum(const std::string msg)
{
	sha1buf sha1;
	std::ostream out(&sha1);
	out << msg;
	return sha1.hexdigest();
}

#define SHA1TEST(msg,digit) if (sha1sum(msg) != #digit) return 1; \

int main()
{
	SHA1_TEST_SUITE
	return 0;
}

