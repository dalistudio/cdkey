/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* ECC SIGN
*/

#include "base32.h"
#include "hash.h"
#include "sign.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef union {
	unsigned int a;
	unsigned char b[4];
}KEY;

// 生成 CDKEY 中签名的值
big key_sign(unsigned int Key, unsigned char Type, unsigned char Data, big hash)
{
	unsigned char buf[128], s[10], sha_out[20];
	int i;
	sha sh;
	big hash_sign;

	KEY tmp;
	tmp.a = Key; // 钥匙的值

	hash_sign = mirvar(0);

	s[0] = tmp.b[1]; // 取钥匙的第1个字节
	s[1] = tmp.b[0]; // 取钥匙的第0个字节
	s[2] = Type; // CDKEY 中类型的值
	s[3] = Data; // CDKEY 中数据的值
	big_to_bytes(4, hash, (char*)buf, TRUE); // 将散列的值转为字符数组
	s[4] = buf[3]; // CDKEY 中散列值得第3个字节
	s[5] = buf[2]; // CDKEY 中散列值得第2个字节
	s[6] = buf[1]; // CDKEY 中散列值得第1个字节
	s[7] = buf[0]; // CDKEY 中散列值得第0个字节
	s[8] = 0x00; // 结束
	s[9] = 0x00; // 结束

	// 生成散列值，从中取出前10字节作为签名
	shs_init(&sh); // 初始化 sha1 散列
	for (i = 0; i < 10; i++)
		shs_process(&sh, s[i]); // 将 s[] 写入 sha1 处理
	shs_hash(&sh, (char*)sha_out); // 计算 sha1 散列值

	bytes_to_big(10, (char*)&sha_out, hash_sign); // 取前10字节作为签名的数据
	return hash_sign;
}

// 生成 CDKEY
big keygen(unsigned int Key, unsigned char Type, unsigned char Data)
{
	char  CDKEY[30] = { 0 };
	FILE *fp;
	int bits, cmp;
	big a, b, p, gx, gy;
	big q, d;
	big r, rx, ry;
	epoint  *G, *R;
	big hash, hash2;
	big kh, kr, z, sr, upperlimit, sign, sn;
	miracl *mip;

	Type = Type & 0x3F; // 保留后6bit作为类型

	// 1 打开曲线文件，获取参数
	fp = fopen("ecurve.ecs", "rt"); /* 打开曲线参数文件 */
	if (fp == NULL)
	{
		printf("ERROR\n");
		printf("file ecurve.ecs does not exist\n");
		exit(0);
	}
	fscanf(fp, "%d\n", &bits); /* 读取bit数 */

	mip = mirsys(bits, 16);  /* 使用十六进制 */
	a = mirvar(0); /* 参数 A */
	b = mirvar(0); /* 参数 B */
	p = mirvar(0); /* 有限域 P */
	gx = mirvar(0); /* 基点 Gx */
	gy = mirvar(0); /* 基点 Gy */

	innum(a, fp); /* 读取参数 A */
	innum(b, fp); /* 读取参数 B */
	innum(p, fp); /* 读取有限域 P */
	innum(gx, fp); /* 读取基点 Gx */
	innum(gy, fp); /* 读取基点 Gy */

	fclose(fp); /* 关闭曲线参数文件 */

	// 2 打开私钥文件，获取参数
	fp = fopen("private.ecs", "rt"); /* 打开私钥文件 */
	if (fp == NULL)
	{
		printf("ERROR\n");
		printf("file private.ecs does not exist\n");
		exit(0);
	}

	q = mirvar(0); /* 阶 Q */
	d = mirvar(0); /* 私钥 D */

	innum(q, fp); /* 读取阶值 Q */
	innum(d, fp); /* 私钥 D 或者叫 K */
	fclose(fp); /* 关闭私钥文件 */

	upperlimit = mirvar(0);
	mip->IOBASE = 16; // 使用十六进制
	cinstr(upperlimit, "100000000000000000000");// 签名不能大于80bit

	srand((unsigned int)time(NULL));  // 只在第一次运行时初始化随机函数
	irand((unsigned int)time(NULL));

	r = mirvar(0); // 随机数 r
	rx = mirvar(0);
	ry = mirvar(0);

	// 死循环
	while (1)
	{
		ecurve_init(a, b, p, MR_PROJECTIVE); // 椭圆曲线

		bigrand(q, r); // 生成随机数r

		G = epoint_init(); /* 初始化曲线上的点 */
		R = epoint_init(); /* 初始化曲线上的点 */

		if (!epoint_set(gx, gy, 0, G)) /* initialise point of order q */
		{
			printf("ERROR\n");
			printf("point G(x,y) is not on the curve\n");
			exit(0);
		}
		ecurve_mult(r, G, R); // 计算随机点

		epoint_get(R, rx, ry); // 获得随机点

		// 3 计算 CDKEY 的散列值
		hash = mirvar(0);
		hash = key_hash(Key, Type, Data, rx, ry);

		// 4 计算 CDKEY 的签名值
		hash2 = mirvar(0);
		hash2 = key_sign(Key, Type, Data, hash);

		// 5 签名
		kh = mirvar(0);
		kr = mirvar(0);
		z = mirvar(0);
		sr = mirvar(0);
		sign = mirvar(0); // 签名

		// kh = 私钥*散列(q+1)/2
		incr(q, 1, kh); // kh=q+1
		multiply(kh, d, kh); // kh=kh*d
		multiply(kh, hash2, kh); // kh=kh*hout
		subdiv(kh, 2, kh); // kh=kh/2

		// kr = ( pow( kh, 2 ) + r ) % q; 
		power(kh, 2, z, z); // z=kh^2
		add(z, r, z); // z=z+r
		divide(z, q, q); // z=z%q
		kr = z;

		// 求平方根，失败的几率很大
		if (!sqroot(kr, q, sr))continue;// 如果sqrt 失败，重新计算

		//sign = ( sr - kh ) % q;
		subtract(sr, kh, sign); // sign=sr-kh
		divide(sign, q, q); // sign=sign%q

		z = mirvar(0);
		while (cmp = mr_compare(sign, z))
		{
			if (cmp < 0)
			{
				add(sign, q, sign); // sign不会是负数: sign=sign+q  
				break;
			}
		}

		if (mr_compare(sign, upperlimit) < 0)break; // 如果sign在允许的范围内
	}

	// 组合 CDKEY 
	// 125bit = [80bit Sign] + [6bit Type] + [8bit Data] + [31bit Hash]
	sn = mirvar(0);

	// 签名 80bit SIGN
	add(sn, sign, sn); // sn = sn + sign

	// 类型 6bit  Type
	sftbit(sn, 6, sn); // sn = sn << 6
	incr(sn, Type, sn); // sn = sn + Type

	// 数据 8bit  Data
	sftbit(sn, 8, sn); // sn = sn << 8
	incr(sn, Data, sn); // sn = sn + Data

	// 散列 31bit HASH
	sftbit(sn, 31, sn); // sn = sn << 31
	add(sn, hash, sn); // sn = sn + hash

	// 6 BASE32编码
	BASE32_ENCODE(sn, (char*)&CDKEY);
	printf("%s\n", CDKEY);

	return sn;
}

