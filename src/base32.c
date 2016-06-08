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

/* 编码 - 将125bit转成CD-KEY字符串 */
void BASE32_ENCODE(big sn, char* key)
{
	char  CDKEY[30] = { 0 };
	int i, j;
	big tmp;
	tmp = mirvar(0);
	copy(sn, tmp); // tmp临时变量，避免修改sn的值。

	for (i = 0, j = 28; i < 25; i++, j--)
	{
		if (j % 6 == 5)	CDKEY[j--] = '-'; // 余5，表示每5个字符加'-'

		CDKEY[j] = base32_tab[remain(tmp, 32)]; // sn = sn mod 32

		subdiv(tmp, 32, tmp); //sn = sn/32;
	}
	memcpy(key, &CDKEY, 30);
}

/* 解码 - 将CD-KEY字符串转成125bit */
void BASE32_DECODE(big sn, char* key)
{
	char  CDKEY[30] = { 0 };
	int i, j, pos;
	char *ptr;
	big tmp;
	tmp = mirvar(0); // 避免sn不为0,造成计算错误。

	memcpy(&CDKEY, key, 30);
	for (i = 0, j = 0; i < 25; i++, j++)
	{
		if (CDKEY[j] == '-')
		{
			j++; // 跳过'-'
		}
		if (strchr(base32_tab, CDKEY[j]) == NULL)
		{
			// 如果CD-KEY中有base32_tab不存在的字符，则错误。
			// 直接设置为0，并返回。
			tmp = mirvar(0);
			break;
		}

		ptr = strchr(base32_tab, CDKEY[j]); // 取出CDKEY[j]首次出现在base32_tab的位置
		pos = ptr - base32_tab; // 将ptr位置，减去base32_tab的首个位置，得到CDKEY[j]对应的值
		premult(tmp, 32, tmp);//sn = sn*32
		incr(tmp, pos, tmp); // sn = sn+pos
	}
	copy(tmp, sn);
}
