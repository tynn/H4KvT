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

/* https://tools.ietf.org/html/rfc3174 */

#ifndef SHA1


#define _H0 (0x67452301)
#define _H1 (0xEFCDAB89)
#define _H2 (0x98BADCFE)
#define _H3 (0x10325476)
#define _H4 (0xC3D2E1F0)

#define _M(m,v) m##v

#define f0(B,C,D) (((B)&(C))|(~(B)&(D)))
#define f1(B,C,D) ((B)^(C)^(D))
#define f2(B,C,D) (((B)&(C))|((B)&(D))|((C)&(D)))
#define f3(B,C,D) ((B)^(C)^(D))

#define K0 (0x5A827999)
#define K1 (0x6ED9EBA1)
#define K2 (0x8F1BBCDC)
#define K3 (0xCA62C1D6)

#define _LS(v,s) ((v)<<s|(v)>>(32-s))

#define _PROC(A,B,C,D,E,T,t,v) {\
	(T) = _LS(A,5)+_M(f,v)(B,C,D)+E+W(t)+_M(K,v); \
	(E) = (D); (D) = (C); (C) = _LS(B,30); (B) = (A); (A) = (T); \
}

#define SHA1(A,B,C,D,E,T,t) \
	for (t = 16; t < 80; t++) \
		W(t) = _LS((W(t-3))^(W(t-8))^(W(t-14))^(W(t-16)),1);\
	for (t = 0; t < 20; t++) \
		_PROC(A,B,C,D,E,T,t,0) \
	for (; t < 40; t++) \
		_PROC(A,B,C,D,E,T,t,1) \
	for (; t < 60; t++) \
		_PROC(A,B,C,D,E,T,t,2) \
	for (; t < 80; t++) \
		_PROC(A,B,C,D,E,T,t,3) \


#define SHA1_TEST_SUITE \
SHA1TEST("",da39a3ee5e6b4b0d3255bfef95601890afd80709) \
SHA1TEST("abc",a9993e364706816aba3e25717850c26c9cd0d89d) \
SHA1TEST("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",84983e441c3bd26ebaae4aa1f95129e5e54670f1) \
SHA1TEST("a",86f7e437faa5a7fce15d1ddcb9eaeaea377667b8) \
SHA1TEST("0123456701234567012345670123456701234567012345670123456701234567",e0c094e867ef46c350ef54a7f59dd60bed92ae83) \


#endif

