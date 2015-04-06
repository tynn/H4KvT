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

/* https://tools.ietf.org/html/rfc1321 */

#define _A (0x67452301)
#define _B (0xefcdab89)
#define _C (0x98badcfe)
#define _D (0x10325476)

#define F(x,y,z) (((x)&(y))|(~(x)&(z)))
#define G(x,y,z) (((x)&(z))|((y)&~(z)))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|~(z)))

#define T(i) T##i
#define T1 (0xd76aa478)
#define T2 (0xe8c7b756)
#define T3 (0x242070db)
#define T4 (0xc1bdceee)
#define T5 (0xf57c0faf)
#define T6 (0x4787c62a)
#define T7 (0xa8304613)
#define T8 (0xfd469501)
#define T9 (0x698098d8)
#define T10 (0x8b44f7af)
#define T11 (0xffff5bb1)
#define T12 (0x895cd7be)
#define T13 (0x6b901122)
#define T14 (0xfd987193)
#define T15 (0xa679438e)
#define T16 (0x49b40821)
#define T17 (0xf61e2562)
#define T18 (0xc040b340)
#define T19 (0x265e5a51)
#define T20 (0xe9b6c7aa)
#define T21 (0xd62f105d)
#define T22 (0x2441453)
#define T23 (0xd8a1e681)
#define T24 (0xe7d3fbc8)
#define T25 (0x21e1cde6)
#define T26 (0xc33707d6)
#define T27 (0xf4d50d87)
#define T28 (0x455a14ed)
#define T29 (0xa9e3e905)
#define T30 (0xfcefa3f8)
#define T31 (0x676f02d9)
#define T32 (0x8d2a4c8a)
#define T33 (0xfffa3942)
#define T34 (0x8771f681)
#define T35 (0x6d9d6122)
#define T36 (0xfde5380c)
#define T37 (0xa4beea44)
#define T38 (0x4bdecfa9)
#define T39 (0xf6bb4b60)
#define T40 (0xbebfbc70)
#define T41 (0x289b7ec6)
#define T42 (0xeaa127fa)
#define T43 (0xd4ef3085)
#define T44 (0x4881d05)
#define T45 (0xd9d4d039)
#define T46 (0xe6db99e5)
#define T47 (0x1fa27cf8)
#define T48 (0xc4ac5665)
#define T49 (0xf4292244)
#define T50 (0x432aff97)
#define T51 (0xab9423a7)
#define T52 (0xfc93a039)
#define T53 (0x655b59c3)
#define T54 (0x8f0ccc92)
#define T55 (0xffeff47d)
#define T56 (0x85845dd1)
#define T57 (0x6fa87e4f)
#define T58 (0xfe2ce6e0)
#define T59 (0xa3014314)
#define T60 (0x4e0811a1)
#define T61 (0xf7537e82)
#define T62 (0xbd3af235)
#define T63 (0x2ad7d2bb)
#define T64 (0xeb86d391)

#define _ROT(v,s) ((v)<<s|(v)>>(32-s))

#define _R(a,b,c,d,k,s,i,f) \
	(a) += f(b,c,d) + X(k) + T(i); \
	(a) = _ROT(a,s); \
	(a) += (b);

#define _R1(a,b,c,d,k,s,i) _R(a,b,c,d,k,s,i,F)
#define R1(A,B,C,D) \
	_R1(A,B,C,D,0,7,1) \
	_R1(D,A,B,C,1,12,2) \
	_R1(C,D,A,B,2,17,3) \
	_R1(B,C,D,A,3,22,4) \
	_R1(A,B,C,D,4,7,5) \
	_R1(D,A,B,C,5,12,6) \
	_R1(C,D,A,B,6,17,7) \
	_R1(B,C,D,A,7,22,8) \
	_R1(A,B,C,D,8,7,9) \
	_R1(D,A,B,C,9,12,10) \
	_R1(C,D,A,B,10,17,11) \
	_R1(B,C,D,A,11,22,12) \
	_R1(A,B,C,D,12,7,13) \
	_R1(D,A,B,C,13,12,14) \
	_R1(C,D,A,B,14,17,15) \
	_R1(B,C,D,A,15,22,16)

#define _R2(a,b,c,d,k,s,i) _R(a,b,c,d,k,s,i,G)
#define R2(A,B,C,D) \
	_R2(A,B,C,D,1,5,17) \
	_R2(D,A,B,C,6,9,18) \
	_R2(C,D,A,B,11,14,19) \
	_R2(B,C,D,A,0,20,20) \
	_R2(A,B,C,D,5,5,21) \
	_R2(D,A,B,C,10,9,22) \
	_R2(C,D,A,B,15,14,23) \
	_R2(B,C,D,A,4,20,24) \
	_R2(A,B,C,D,9,5,25) \
	_R2(D,A,B,C,14,9,26) \
	_R2(C,D,A,B,3,14,27) \
	_R2(B,C,D,A,8,20,28) \
	_R2(A,B,C,D,13,5,29) \
	_R2(D,A,B,C,2,9,30) \
	_R2(C,D,A,B,7,14,31) \
	_R2(B,C,D,A,12,20,32)

#define _R3(a,b,c,d,k,s,i) _R(a,b,c,d,k,s,i,H)
#define R3(A,B,C,D) \
	_R3(A,B,C,D,5,4,33) \
	_R3(D,A,B,C,8,11,34) \
	_R3(C,D,A,B,11,16,35) \
	_R3(B,C,D,A,14,23,36) \
	_R3(A,B,C,D,1,4,37) \
	_R3(D,A,B,C,4,11,38) \
	_R3(C,D,A,B,7,16,39) \
	_R3(B,C,D,A,10,23,40) \
	_R3(A,B,C,D,13,4,41) \
	_R3(D,A,B,C,0,11,42) \
	_R3(C,D,A,B,3,16,43) \
	_R3(B,C,D,A,6,23,44) \
	_R3(A,B,C,D,9,4,45) \
	_R3(D,A,B,C,12,11,46) \
	_R3(C,D,A,B,15,16,47) \
	_R3(B,C,D,A,2,23,48)

#define _R4(a,b,c,d,k,s,i) _R(a,b,c,d,k,s,i,I)
#define R4(A,B,C,D) \
	_R4(A,B,C,D,0,6,49) \
	_R4(D,A,B,C,7,10,50) \
	_R4(C,D,A,B,14,15,51) \
	_R4(B,C,D,A,5,21,52) \
	_R4(A,B,C,D,12,6,53) \
	_R4(D,A,B,C,3,10,54) \
	_R4(C,D,A,B,10,15,55) \
	_R4(B,C,D,A,1,21,56) \
	_R4(A,B,C,D,8,6,57) \
	_R4(D,A,B,C,15,10,58) \
	_R4(C,D,A,B,6,15,59) \
	_R4(B,C,D,A,13,21,60) \
	_R4(A,B,C,D,4,6,61) \
	_R4(D,A,B,C,11,10,62) \
	_R4(C,D,A,B,2,15,63) \
	_R4(B,C,D,A,9,21,64)


#define MD5(A,B,C,D) R1(A,B,C,D) R2(A,B,C,D) R3(A,B,C,D) R4(A,B,C,D)


#define MD5_TEST_SUITE \
MD5TEST("",d41d8cd98f00b204e9800998ecf8427e) \
MD5TEST("a",0cc175b9c0f1b6a831c399e269772661) \
MD5TEST("abc",900150983cd24fb0d6963f7d28e17f72) \
MD5TEST("message digest",f96b697d7cb7938d525a2f31aaf161d0) \
MD5TEST("abcdefghijklmnopqrstuvwxyz",c3fcd3d76192e4007dfb496cca67e13b) \
MD5TEST("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",d174ab98d277d9f5a5611c2c9f419d9f) \
MD5TEST("12345678901234567890123456789012345678901234567890123456789012345678901234567890",57edf4a22be3c955ac49da2e2107b67a)

