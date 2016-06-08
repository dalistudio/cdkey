/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* ECC SGIN HASH
*/

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union {
	unsigned int a;
	unsigned char b[4];
}KEY;

// 计算 CDKEY 中 hash 散列的值
big key_hash(unsigned int Key, unsigned char Type, unsigned char Data, big rx, big ry)
{
	unsigned char buf[128], m[132], sha_out[20];
	unsigned long dw;
	int i, j, len, len2;
	sha sh;
	big hash;

	KEY tmp;
	tmp.a = Key; // 钥匙的值

	m[0] = tmp.b[3]; // 取钥匙的第3个字节
	m[1] = tmp.b[2]; // 取钥匙的第2个字节
	m[2] = Type; // CDKEY 中类型的值
	m[3] = Data; // CDKEY 中数据的值

	// 循环将 RX 的值放入 m[] 数组中，作为用于生成散列的内容。
	len = rx->len * 4; // 获得RX的长度
	big_to_bytes(len, rx, (char*)buf, FALSE); // 将 rx 转成 数值
	for (i = len - 1, j = 4; i >= 0; i--, j++)  m[j] = buf[i]; // 循环将 rx 写入 m[]

	// 循环将 RY 的值放入 m[] 数组中，作为用于生成散列的内容。
	len2 = ry->len * 4; // 获得RY的长度
	big_to_bytes(len2, ry, (char*)buf, FALSE); // 将 ry 转成 数值
	for (i = len2 - 1, j = len + 4; i >= 0; i--, j++) m[j] = buf[i]; // 循环将 ry 写入 m[]

	// 开始散列
	shs_init(&sh); // 初始化 sha1 散列
	for (i = 0; i < (4 + rx->len + ry->len); i++)
		shs_process(&sh, m[i]); // 将 m[] 写入 sha1 处理
	shs_hash(&sh, (char*)sha_out); // 计算 sha1 散列值

	memcpy(&dw, sha_out, 4); // 将前4字节写入 dw
	dw = dw & 0x7FFFFFFF; // 取后 31bit 数值
	hash = mirvar(dw);
	return hash; // 返回31bit散列的值
}