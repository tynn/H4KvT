/*
 *	This file is part of H4KvT.
 *
 *	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
 *
 *	H4KvT is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, 0xeither version 3 of the License, 0xor
 *	(at your option) any later version.
 *
 *	H4KvT is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with H4KvT. If not, 0xsee <http://www.gnu.org/licenses/>.
 */

/* https://tools.ietf.org/html/rfc6234 */

#ifndef SHA384
#ifndef SHA512


#define _H00 (0x6a09e667f3bcc908)
#define _H01 (0xbb67ae8584caa73b)
#define _H02 (0x3c6ef372fe94f82b)
#define _H03 (0xa54ff53a5f1d36f1)
#define _H04 (0x510e527fade682d1)
#define _H05 (0x9b05688c2b3e6c1f)
#define _H06 (0x1f83d9abfb41bd6b)
#define _H07 (0x5be0cd19137e2179)


#define SHR(n,x) ((x)>>n)
#define ROTR(n,x) ((x)>>n|(x)<<(64-n))

#define CH(x,y,z) (((x)&(y))^(~(x)&(z)))
#define MAJ(x,y,z) (((x)&(y))^((x)&(z))^((y)&(z)))
#define BSIG0(x) (ROTR(28,x)^ROTR(34,x)^ROTR(39,x))
#define BSIG1(x) (ROTR(14,x)^ROTR(18,x)^ROTR(41,x))
#define SSIG0(x) (ROTR(1,x)^ROTR(8,x)^SHR(7,x))
#define SSIG1(x) (ROTR(19,x)^ROTR(61,x)^SHR(6,x))


#define _K \
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, \
	0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, \
	0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, \
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, \
	0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, \
	0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, \
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, \
	0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70, \
	0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, \
	0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, \
	0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, \
	0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, \
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, \
	0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, \
	0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, \
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, \
	0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, \
	0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b, \
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, \
	0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817


#define SHA512(a,b,c,d,e,f,g,h,T1,T2,t) \
	for (t = 16; t < 80; t++) \
		W(t) = SSIG1(W(t-2)) + W(t-7) + SSIG0(W(t-15)) + W(t-16); \
	for (t = 0; t < 80; t++) { \
		T1 = h + BSIG1(e) + CH(e,f,g) + K(t) + W(t); \
		T2 = BSIG0(a) + MAJ(a,b,c); \
		h = g; g = f; f = e; \
		e = d + T1; \
		d = c; c = b; b = a; \
		a = T1 + T2; \
	}


#define SHA512_TEST_SUITE \
SHA512TEST("",cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e) \
SHA512TEST("abc",ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f) \
SHA512TEST("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445) \
SHA512TEST("a",1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75) \
SHA512TEST("0123456701234567012345670123456701234567012345670123456701234567",846e0ef73436438a4acb0ba7078cfe381f10a0f5edebcb985b3790086ef5e7ac5992ac9c23c77761c764bb3b1c25702d06b99955eb197d45b82fb3d124699d78) \
SHA512TEST("012345670123456701234567012345670123456701234567012345670123456789",b5790c39e0e375b6f0c8e0cf5b9eda1c5143c8e05da500720dbdc0da00459ebc5aa30518c80d18736779525d75cb33e9431220e3aefe8ef3f75cd17b522bef86) \
SHA512TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",e26b7daef4366128fc7ae3fb75f31789ae03648c61b91192ac6cffb4924a1ce53c0768fe21daab635f5aebaa7fd112b325fd6a32715926c3d73d1ac31e6431a5) \
SHA512TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/",436617c0f69c3b3db86963af9839669fc42df302b2b513e17d6a19247a83a9196d4e11a68f68a3dbecf6ae8faeb36bc40bdcb49dc68cdeb796549841d0e28f28) \
SHA512TEST("01234567012345670123456701234567012345670123456701234567012345670123456701234567012345670123456701234567012345670123456701234567", 0bd3b2de6fabc60af35a8a29b926f40d963264d800be148e64a23efee737849ad6acec96140a2b1c5f11d2d49f1fc3fb2a2c885e4a850729272884ade9e0fe21) \


#endif
#else
#ifndef _SHA384
#define _SHA384


#define _H30 (0xcbbb9d5dc1059ed8)
#define _H31 (0x629a292a367cd507)
#define _H32 (0x9159015a3070dd17)
#define _H33 (0x152fecd8f70e5939)
#define _H34 (0x67332667ffc00b31)
#define _H35 (0x8eb44a8768581511)
#define _H36 (0xdb0c2e0d64f98fa7)
#define _H37 (0x47b5481dbefa4fa4)


#define SHA384_TEST_SUITE \
SHA384TEST("",38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b) \
SHA384TEST("abc",cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7) \
SHA384TEST("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b) \
SHA384TEST("a",54a59b9f22b0b80880d8427e548b7c23abd873486e1f035dce9cd697e85175033caa88e6d57bc35efae0b5afd3145f31) \
SHA384TEST("0123456701234567012345670123456701234567012345670123456701234567",72f5893331c249312d3c2b7a9709a7b96908b7769179dd9824ed578669fcc1f1c2de02c03b3d35a467aa0b472c1bb3d1) \
SHA384TEST("012345670123456701234567012345670123456701234567012345670123456789",134ded2f19e5301944ff1d3a7bb136add58fda8db7cb6b8cb698fa18aa75c4115cf9983caf87315e2dcb0124715fb1f6) \
SHA384TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",501e72a20c43fc3f2ec502f9518a7d96e8075ff555a9d9c3bc0d88a271916626669c1388b6eaec0a6c3e0690b7c4b5b9) \
SHA384TEST("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/",c1ae6280a9dcb0c0ecf6e9bd3d53e9e6cea7869db4d461a6d1250231c12ba6f55c50cf3b786198ab0db3aab1794a7fba) \


#endif
#endif

