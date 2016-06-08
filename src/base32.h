/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* BASE32 ENCODE / DECODE
*/

#include "miracl.h"

void BASE32_ENCODE(big sn, char* key); /* 编码 - 将125bit转成CD-KEY字符串 */
void BASE32_DECODE(big sn, char* key); /* 解码 - 将CD-KEY字符串转成125bit */