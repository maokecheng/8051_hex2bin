// 8051_hex2bin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc , char *argv[])
{
	if (argc != 3)
	{
		printf("Usage:hex2bin input.hex output.bin\r\n");
		return 0;
	}

	//start process file
	FILE* fhex = NULL, * fbin = NULL;
	char szline[2014];
	int nbinlen = 0;
	unsigned char* pbinbuf = (unsigned char*)malloc(65 * 1024);
	memset(pbinbuf, 0x00, 65 * 1024);

	fhex = fopen(argv[1], "r");
	if (fhex == NULL) {
		printf("open hex failed\r\n");
		return 0;
	}
	unsigned short smaxcount = 0;
	//start process hex
	while (!feof(fhex))
	{
		fgets(szline, 1023, fhex);
		if (szline[0] != ':')
		{
			printf("%s\r\n", szline);
			printf("file format error\r\n");
			return 0;
		}
		printf("%s\r\n", szline);
		char cbuflen,sztmp[5],csig;
		unsigned short saddress;
		sztmp[0] = szline[1]; sztmp[1] = szline[2]; sztmp[2] = 0x00;
		cbuflen = strtol(sztmp, NULL, 16);
		sztmp[0] = szline[3]; sztmp[1] = szline[4]; sztmp[2] = szline[5]; sztmp[3] = szline[6]; sztmp[4] = 0x00;
		saddress = strtol(sztmp, NULL, 16);
		sztmp[0] = szline[7]; sztmp[1] = szline[8]; sztmp[2] = 0x00;
		csig = strtol(sztmp, NULL, 16);
		printf("%x,%x,%x\r\n", cbuflen,saddress , csig);
		if (csig != 0)
		{
			if (csig == 1)	break;
			else
			{
				printf("unknow bus!\r\n");
				break;
			}
		}
		if (smaxcount < saddress + cbuflen)
			smaxcount = saddress + cbuflen;
		for (int i=0;i<cbuflen;i++)
		{
			sztmp[0] = szline[9 + i*2]; sztmp[1] = szline[10 + i*2]; sztmp[2] = 0x00;
			pbinbuf[saddress++] = (unsigned char)strtol(sztmp, NULL, 16);
		}
	}
	
	printf("%X", smaxcount);
	fclose(fhex);
	//write
	fbin = fopen(argv[2], "wb");
	fwrite(pbinbuf, 1, smaxcount, fbin);
	fclose(fbin);

	free(pbinbuf);
    return 0;
}