/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* BASE32 ENCODE / DECODE
*/

#include "miracl.h"

void BASE32_ENCODE(big sn, char* key); /* ���� - ��125bitת��CD-KEY�ַ��� */
void BASE32_DECODE(big sn, char* key); /* ���� - ��CD-KEY�ַ���ת��125bit */