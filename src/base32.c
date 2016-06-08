/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* BASE32 ENCODE / DECODE
*/

#include "base32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char base32_tab[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

/* ���� - ��125bitת��CD-KEY�ַ��� */
void BASE32_ENCODE(big sn, char* key)
{
	char  CDKEY[30] = { 0 };
	int i, j;
	big tmp;
	tmp = mirvar(0);
	copy(sn, tmp); // tmp��ʱ�����������޸�sn��ֵ��

	for (i = 0, j = 28; i < 25; i++, j--)
	{
		if (j % 6 == 5)	CDKEY[j--] = '-'; // ��5����ʾÿ5���ַ���'-'

		CDKEY[j] = base32_tab[remain(tmp, 32)]; // sn = sn mod 32

		subdiv(tmp, 32, tmp); //sn = sn/32;
	}
	memcpy(key, &CDKEY, 30);
}

/* ���� - ��CD-KEY�ַ���ת��125bit */
void BASE32_DECODE(big sn, char* key)
{
	char  CDKEY[30] = { 0 };
	int i, j, pos;
	char *ptr;
	big tmp;
	tmp = mirvar(0); // ����sn��Ϊ0,��ɼ������

	memcpy(&CDKEY, key, 30);
	for (i = 0, j = 0; i < 25; i++, j++)
	{
		if (CDKEY[j] == '-')
		{
			j++; // ����'-'
		}
		if (strchr(base32_tab, CDKEY[j]) == NULL)
		{
			// ���CD-KEY����base32_tab�����ڵ��ַ��������
			// ֱ������Ϊ0�������ء�
			tmp = mirvar(0);
			break;
		}

		ptr = strchr(base32_tab, CDKEY[j]); // ȡ��CDKEY[j]�״γ�����base32_tab��λ��
		pos = ptr - base32_tab; // ��ptrλ�ã���ȥbase32_tab���׸�λ�ã��õ�CDKEY[j]��Ӧ��ֵ
		premult(tmp, 32, tmp);//sn = sn*32
		incr(tmp, pos, tmp); // sn = sn+pos
	}
	copy(tmp, sn);
}
