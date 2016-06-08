/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* ECC KEY GENERATOR
*/

#include "sign.h"

int main(int argc, char *argv[])
{
	if (argc == 4)
	{
		unsigned int Key = 0;
		unsigned char Type = 0;
		unsigned char Data = 0;

		// 将输入的十六进制字符转成整数形式
		Key = strtoul(argv[1], NULL, 16);
		Type = strtoul(argv[2], NULL, 16);
		Data = strtoul(argv[3], NULL, 16);

		// 生成 CDKEY
		keygen(Key, Type, Data);
	}
	else
	{
		printf("ERROR\n");
		printf("usage: keygen.exe secret type data\n");
		printf("");
	}

	return 0;
}