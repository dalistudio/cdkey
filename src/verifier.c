/*
* Copyright (c) 2014 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* ECC SIGN VERIFIER
*/

#include "verifier.h"
#include "base32.h"
#include "hash.h"
#include "sign.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 验证 CDKEY 中的签名
void verifier(unsigned int Key, unsigned char *cdkey)
{
	FILE *fp;
	char CDKEY[30] = { 0 };
	int bits, len;
	big a, b, p, gx, gy, kx, ky;
	big sn, sn0, sn1, sn2;
	big sign, type, data, hash, hash2, hash3;
	epoint *G, *K, *R;
	big rx, ry;
	unsigned char Type, Data;

	miracl *mip;

	memcpy((char*)&CDKEY, cdkey, 29);
	Type = 0; Data = 0;

	// 1 打开曲线，获取参数
	fp = fopen("ecurve.ecs", "rt"); /* 打开曲线参数文件 */
	if (fp == NULL)
	{
		printf("ERROR\n");
		printf("file ecurve.ecs does not exist\n");
		exit(0);
	}
	fscanf(fp, "%d\n", &bits); /* 读取bit数 */

	mip = mirsys(256, 16);  /* 使用十六进制 */
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

	// 2 打开公钥，获取参数
	fp = fopen("public.ecs", "rt"); /* 打开公钥文件 */
	if (fp == NULL)
	{
		printf("ERROR\n");
		printf("file public.ecs does not exist\n");
		exit(0);
	}
	kx = mirvar(0);
	ky = mirvar(0);
	innum(kx, fp); /* 读取Kx */
	innum(ky, fp); /* 读取Ky */
	fclose(fp); /* 关闭公钥文件 */

	// 3 验证CD-KEY
	len = strlen(CDKEY); // 计算 CD-KEY 的长度
	if (len == 0) 	return;
	if (len != 29 || CDKEY[5] != '-' || CDKEY[11] != '-' || CDKEY[17] != '-' || CDKEY[23] != '-') // 验证 CD-KEY 的格式
	{
		printf("ERROR\n");
		printf("CD-KEY Format Errror.");
		exit(0);
	}

	// 4 BASE32 解码
	sn = mirvar(0);
	BASE32_DECODE(sn, CDKEY);

	// 5 分解 sn 值
	sn0 = mirvar(0);
	sn1 = mirvar(0);
	sn2 = mirvar(0);
	sign = mirvar(0);
	data = mirvar(0);
	type = mirvar(0);
	hash = mirvar(0);

	copy(sn, sn0);//sn0 = sn;
	// 获取hash
	sftbit(sn0, -31, sn1); //sn1 = sn0>>31;	
	sftbit(sn1, 31, sn2);  //sn2 = sn1<<31;
	subtract(sn0, sn2, hash);//hash = sn0 - sn2; 

	// 获取 data
	copy(sn1, sn0);//sn0 = sn1;	
	sftbit(sn0, -8, sn1);//sn1 = sn0>>8;	
	sftbit(sn1, 8, sn2); //sn2 = sn1<<8;	
	subtract(sn0, sn2, data);//hash = sn0 - sn2;
	big_to_bytes(1, data, (char*)&Data, TRUE);


	// 获取 type
	copy(sn1, sn0);//sn0 = sn1;	
	sftbit(sn0, -6, sn1);//sn1 = sn0>>6;	
	sftbit(sn1, 6, sn2);//sn2 = sn1<<6;	
	subtract(sn0, sn2, type);//sign = sn0 - sn2; 
	big_to_bytes(1, type, (char*)&Type, TRUE);

	// 获取 sign
	copy(sn1, sn0);//sn0 = sn1;
	sign = sn0;

	// 6 计算 hashS
	hash2 = mirvar(0);
	hash2 = key_sign(Key, Type, Data, hash);

	// 7 计算 rx ry
	ecurve_init(a, b, p, MR_PROJECTIVE); // 椭圆曲线
	G = epoint_init(); /* 初始化曲线上的点 */
	K = epoint_init(); /* 初始化曲线上的点 */
	R = epoint_init(); /* 初始化曲线上的点 */
	if (!epoint_set(gx, gy, 0, G)) /* initialise point of order q */
	{
		printf("ERROR\n");
		printf("point G(x,y) is not on the curve\n");
		exit(0);
	}
	if (!epoint_set(kx, ky, 0, K)) /* initialise point of order q */
	{
		printf("ERROR\n");
		printf("point K(x,y) is not on the curve\n");
		exit(0);
	}

	//R = sign * mul( sign, G, hout, K ); // 计算 R 值
	ecurve_mult2(sign, G, hash2, K, R);
	ecurve_mult(sign, R, R);

	//R.get( rx, ry ); // 获得 rx ry
	rx = mirvar(0);
	ry = mirvar(0);
	epoint_get(R, rx, ry);

	// 8 计算 CDKEY 的散列值
	hash3 = mirvar(0);
	hash3 = key_hash(Key, Type, Data, rx, ry);

	// 9 比较 散列值
	if (mr_compare(hash3, hash) == 0)
	{
		mip->IOBASE = 16;

		printf("OK\n");

		printf("SIGN=0x");
		cotnum(sign, stdout);

		printf("TYPE=0x");
		cotnum(type, stdout);

		printf("DATA=0x");
		cotnum(data, stdout);

		printf("HASH=0x");
		cotnum(hash, stdout);
	}
	else
	{
		printf("ERROR\n");
		printf("CD-KEY Error.\n");
	}

}