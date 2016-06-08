/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* ECC KEY VERIFIER
*/

#include "base32.h"
#include "hash.h"
#include "sign.h"
#include "verifier.h"

int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		unsigned int Key = 0;

		// 将输入的十六进制字符转换成整数
		Key = strtoul(argv[1], NULL, 16);

		// 验证 CDKEY
		verifier(Key, (unsigned char*)argv[2]);
	}
	else
	{
		printf("ERROR\n");
		printf("");
		printf("usage: keyver.exe secret key\n");
	}

	return 0;
}