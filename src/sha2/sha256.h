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

/* https://tools.ietf.org/html/rfc6234 */

#ifndef SHA224
#ifndef SHA256


#define _H00 (0x6a09e667)
#define _H01 (0xbb67ae85)
#define _H02 (0x3c6ef372)
#define _H03 (0xa54ff53a)
#define _H04 (0x510e527f)
#define _H05 (0x9b05688c)
#define _H06 (0x1f83d9ab)
#define _H07 (0x5be0cd19)


#define SHR(n,x) ((x)>>n)
#define ROTR(n,x) ((x)>>n|(x)<<(32-n))

#define CH(x,y,z) (((x)&(y))^(~(x)&(z)))
#define MAJ(x,y,z) (((x)&(y))^((x)&(z))^((y)&(z)))
#define BSIG0(x) (ROTR(2,x)^ROTR(13,x)^ROTR(22,x))
#define BSIG1(x) (ROTR(6,x)^ROTR(11,x)^ROTR(25,x))
#define SSIG0(x) (ROTR(7,x)^ROTR(18,x)^SHR(3,x))
#define SSIG1(x) (ROTR(17,x)^ROTR(19,x)^SHR(10,x))


#define _K \
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, \
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, \
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, \
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, \
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, \
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, \
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, \
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, \
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, \
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, \
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, \
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, \
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, \
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, \
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, \
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2


#define SHA256(a,b,c,d,e,f,g,h,T1,T2,t) \
	for (t = 16; t < 64; t++) \
		W(t) = SSIG1(W(t-2)) + W(t-7) + SSIG0(W(t-15)) + W(t-16); \
	for (t = 0; t < 64; t++) { \
		T1 = h + BSIG1(e) + CH(e,f,g) + K(t) + W(t); \
		T2 = BSIG0(a) + MAJ(a,b,c); \
		h = g; g = f; f = e; \
		e = d + T1; \
		d = c; c = b; b = a; \
		a = T1 + T2; \
	}


#define SHA256_TEST_SUITE \
SHA256TEST("",e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855) \
SHA256TEST("abc",ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad) \
SHA256TEST("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1) \
SHA256TEST("a",ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb) \
SHA256TEST("0123456701234567012345670123456701234567012345670123456701234567",8182cadb21af0e37c06414ece08e19c65bdb22c396d48ba7341012eea9ffdfdd) \
SHA256TEST("012345670123456701234567012345670123456701234567012345670123456789",623ff98de48238aedf780586d3df242d11a09df4101e8be39047e6ef9b9e133c) \
SHA256TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",540363d1071a002997290cd8f4a2bdf3acd0355ffad3b3f25f52aad6ebad936a) \
SHA256TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/",e5b38ac5aa22fee56fe7095e076e74c03ce60d2ea43b818d0f7b4252faf32e5a) \


#endif
#else
#ifndef _SHA224
#define _SHA224


#define _H20 (0xc1059ed8)
#define _H21 (0x367cd507)
#define _H22 (0x3070dd17)
#define _H23 (0xf70e5939)
#define _H24 (0xffc00b31)
#define _H25 (0x68581511)
#define _H26 (0x64f98fa7)
#define _H27 (0xbefa4fa4)


#define SHA224_TEST_SUITE \
SHA224TEST("",d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f) \
SHA224TEST("abc",23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7) \
SHA224TEST("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525) \
SHA224TEST("a",abd37534c7d9a2efb9465de931cd7055ffdb8879563ae98078d6d6d5) \
SHA224TEST("0123456701234567012345670123456701234567012345670123456701234567",1152a558c1dbcd023222e3472f2b9a2bdf8984e5d82153e6b126a201) \
SHA224TEST("012345670123456701234567012345670123456701234567012345670123456789",5794a47f5a2a3d9afdb168986feba2f73024c5d537bb651b114e93d9) \
SHA224TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",cab8aef22f71d54140b4e24ba298c69d74c5cb05cd9eca8c879fdb87) \
SHA224TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/",5b6ec4eeaa426a986ed593edc51b3bc6935dd6bad9427276d63a7af4) \


#endif
#endif

