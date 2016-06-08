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

// ���� CDKEY ��ǩ����ֵ
big key_sign(unsigned int Key, unsigned char Type, unsigned char Data, big hash)
{
	unsigned char buf[128], s[10], sha_out[20];
	int i;
	sha sh;
	big hash_sign;

	KEY tmp;
	tmp.a = Key; // Կ�׵�ֵ

	hash_sign = mirvar(0);

	s[0] = tmp.b[1]; // ȡԿ�׵ĵ�1���ֽ�
	s[1] = tmp.b[0]; // ȡԿ�׵ĵ�0���ֽ�
	s[2] = Type; // CDKEY �����͵�ֵ
	s[3] = Data; // CDKEY �����ݵ�ֵ
	big_to_bytes(4, hash, (char*)buf, TRUE); // ��ɢ�е�ֵתΪ�ַ�����
	s[4] = buf[3]; // CDKEY ��ɢ��ֵ�õ�3���ֽ�
	s[5] = buf[2]; // CDKEY ��ɢ��ֵ�õ�2���ֽ�
	s[6] = buf[1]; // CDKEY ��ɢ��ֵ�õ�1���ֽ�
	s[7] = buf[0]; // CDKEY ��ɢ��ֵ�õ�0���ֽ�
	s[8] = 0x00; // ����
	s[9] = 0x00; // ����

	// ����ɢ��ֵ������ȡ��ǰ10�ֽ���Ϊǩ��
	shs_init(&sh); // ��ʼ�� sha1 ɢ��
	for (i = 0; i < 10; i++)
		shs_process(&sh, s[i]); // �� s[] д�� sha1 ����
	shs_hash(&sh, (char*)sha_out); // ���� sha1 ɢ��ֵ

	bytes_to_big(10, (char*)&sha_out, hash_sign); // ȡǰ10�ֽ���Ϊǩ��������
	return hash_sign;
}

// ���� CDKEY
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

	Type = Type & 0x3F; // ������6bit��Ϊ����

	// 1 �������ļ�����ȡ����
	fp = fopen("ecurve.ecs", "rt"); /* �����߲����ļ� */
	if (fp == NULL)
	{
		printf("ERROR\n");
		printf("file ecurve.ecs does not exist\n");
		exit(0);
	}
	fscanf(fp, "%d\n", &bits); /* ��ȡbit�� */

	mip = mirsys(bits, 16);  /* ʹ��ʮ������ */
	a = mirvar(0); /* ���� A */
	b = mirvar(0); /* ���� B */
	p = mirvar(0); /* ������ P */
	gx = mirvar(0); /* ���� Gx */
	gy = mirvar(0); /* ���� Gy */

	innum(a, fp); /* ��ȡ���� A */
	innum(b, fp); /* ��ȡ���� B */
	innum(p, fp); /* ��ȡ������ P */
	innum(gx, fp); /* ��ȡ���� Gx */
	innum(gy, fp); /* ��ȡ���� Gy */

	fclose(fp); /* �ر����߲����ļ� */

	// 2 ��˽Կ�ļ�����ȡ����
	fp = fopen("private.ecs", "rt"); /* ��˽Կ�ļ� */
	if (fp == NULL)
	{
		printf("ERROR\n");
		printf("file private.ecs does not exist\n");
		exit(0);
	}

	q = mirvar(0); /* �� Q */
	d = mirvar(0); /* ˽Կ D */

	innum(q, fp); /* ��ȡ��ֵ Q */
	innum(d, fp); /* ˽Կ D ���߽� K */
	fclose(fp); /* �ر�˽Կ�ļ� */

	upperlimit = mirvar(0);
	mip->IOBASE = 16; // ʹ��ʮ������
	cinstr(upperlimit, "100000000000000000000");// ǩ�����ܴ���80bit

	srand((unsigned int)time(NULL));  // ֻ�ڵ�һ������ʱ��ʼ���������
	irand((unsigned int)time(NULL));

	r = mirvar(0); // ����� r
	rx = mirvar(0);
	ry = mirvar(0);

	// ��ѭ��
	while (1)
	{
		ecurve_init(a, b, p, MR_PROJECTIVE); // ��Բ����

		bigrand(q, r); // ���������r

		G = epoint_init(); /* ��ʼ�������ϵĵ� */
		R = epoint_init(); /* ��ʼ�������ϵĵ� */

		if (!epoint_set(gx, gy, 0, G)) /* initialise point of order q */
		{
			printf("ERROR\n");
			printf("point G(x,y) is not on the curve\n");
			exit(0);
		}
		ecurve_mult(r, G, R); // ���������

		epoint_get(R, rx, ry); // ��������

		// 3 ���� CDKEY ��ɢ��ֵ
		hash = mirvar(0);
		hash = key_hash(Key, Type, Data, rx, ry);

		// 4 ���� CDKEY ��ǩ��ֵ
		hash2 = mirvar(0);
		hash2 = key_sign(Key, Type, Data, hash);

		// 5 ǩ��
		kh = mirvar(0);
		kr = mirvar(0);
		z = mirvar(0);
		sr = mirvar(0);
		sign = mirvar(0); // ǩ��

		// kh = ˽Կ*ɢ��(q+1)/2
		incr(q, 1, kh); // kh=q+1
		multiply(kh, d, kh); // kh=kh*d
		multiply(kh, hash2, kh); // kh=kh*hout
		subdiv(kh, 2, kh); // kh=kh/2

		// kr = ( pow( kh, 2 ) + r ) % q; 
		power(kh, 2, z, z); // z=kh^2
		add(z, r, z); // z=z+r
		divide(z, q, q); // z=z%q
		kr = z;

		// ��ƽ������ʧ�ܵļ��ʺܴ�
		if (!sqroot(kr, q, sr))continue;// ���sqrt ʧ�ܣ����¼���

		//sign = ( sr - kh ) % q;
		subtract(sr, kh, sign); // sign=sr-kh
		divide(sign, q, q); // sign=sign%q

		z = mirvar(0);
		while (cmp = mr_compare(sign, z))
		{
			if (cmp < 0)
			{
				add(sign, q, sign); // sign�����Ǹ���: sign=sign+q  
				break;
			}
		}

		if (mr_compare(sign, upperlimit) < 0)break; // ���sign������ķ�Χ��
	}

	// ��� CDKEY 
	// 125bit = [80bit Sign] + [6bit Type] + [8bit Data] + [31bit Hash]
	sn = mirvar(0);

	// ǩ�� 80bit SIGN
	add(sn, sign, sn); // sn = sn + sign

	// ���� 6bit  Type
	sftbit(sn, 6, sn); // sn = sn << 6
	incr(sn, Type, sn); // sn = sn + Type

	// ���� 8bit  Data
	sftbit(sn, 8, sn); // sn = sn << 8
	incr(sn, Data, sn); // sn = sn + Data

	// ɢ�� 31bit HASH
	sftbit(sn, 31, sn); // sn = sn << 31
	add(sn, hash, sn); // sn = sn + hash

	// 6 BASE32����
	BASE32_ENCODE(sn, (char*)&CDKEY);
	printf("%s\n", CDKEY);

	return sn;
}

