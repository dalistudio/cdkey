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

big key_hash(unsigned int Key,unsigned char Type,unsigned char Data, big rx, big ry)
{
	unsigned char buf[128], m[132], sha_out[20];
	unsigned long dw;
	int i,j,len,len2;
	sha sh;
	big hash;

	KEY tmp;
	tmp.a=Key;

	m[0] = tmp.b[3];
	m[1] = tmp.b[2];
	m[2] = Type;
	m[3] = Data;

	len=rx->len*4;
	big_to_bytes( len, rx, (char*)buf, FALSE ); // �� rx ת�� ��ֵ
	for ( i=len-1,j=4; i>=0; i--,j++ )  m[j] = buf[i]; // ѭ���� rx д�� m[]
	len2=ry->len*4;
	big_to_bytes( len2, ry, (char*)buf, FALSE ); // �� ry ת�� ��ֵ
	for ( i=len2-1,j=len+4; i>=0; i--,j++ ) m[j] = buf[i]; // ѭ���� ry д�� m[]
		
	shs_init( &sh ); // ��ʼ�� sha1 ɢ��
	for( i=0; i<4+rx->len+ry->len; i++ )
		shs_process( &sh, m[i] ); // �� m[] д�� sha1 ����
	shs_hash( &sh, (char*)sha_out ); // ���� sha1 ɢ��ֵ

	memcpy( &dw, sha_out, 4 ); // ��ǰ4�ֽ�д�� dw
	dw = dw & 0x7FFFFFFF; // ȡ�� 31bit ��ֵ
	hash = mirvar(dw);
	return hash;
}