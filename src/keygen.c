/*
* Copyright (c) 2014-2016 Dali Wang, All Rights Reserved.
* Dali Wang <wangdali@qq.com>
* ECC KEY GEN
*/

#include "sign.h"

int main(int argc,char *argv[])
{
	if(argc==4)
	{
		unsigned int Key =0;
		unsigned char Type = 0;
		unsigned char Data = 0;

		Key = strtoul(argv[1],NULL,16);
		Type = strtoul(argv[2],NULL,16);
		Data = strtoul(argv[3],NULL,16);
		keygen(Key,Type,Data);
	}
	else
	{
		printf("ERROR\n");
		printf("usage: keygen.exe secret type data\n");
		printf("");
	}

	return 0;
}