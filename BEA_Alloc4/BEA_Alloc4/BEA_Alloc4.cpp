// BEA_Alloc4.cpp : Defines the entry point for the console application.
//
// www.corelan.be
//

#include "stdafx.h"
#include "windows.h"
#include<iostream>


using namespace std;

int main()
{
	HANDLE hChunk, hChunk58, hChunk68, hChunk80;
	HANDLE hDefaultHeap;

	hDefaultHeap = GetProcessHeap();

	printf("Default process heap found at 0x%p\n", hDefaultHeap);
	printf("Press a key to start...\n");
	cin.ignore();

	// try to exhaust freelist for 0x58, 0x80 and 0x68
	// a little bit more allocations, 0x58 may be a popular size :)
	for (int i = 0; i < 14; i++)
	{
		hChunk = HeapAlloc(hDefaultHeap, 0, 0x58);
		printf("Allocated chunk of 0x58 bytes at 0x%p\n", hChunk);
		hChunk = HeapAlloc(hDefaultHeap, 0, 0x80);
		printf("Allocated chunk of 0x80 bytes at 0x%p\n", hChunk);
		hChunk = HeapAlloc(hDefaultHeap, 0, 0x68);
		printf("Allocated chunk of 0x68 bytes at 0x%p\n", hChunk);
	}


	printf("Press return to continue\n");
	cin.ignore();


	// hopefully these 4 chunks will sit next to each other

	hChunk = HeapAlloc(hDefaultHeap, 0, 0x80);
	printf("Allocated start chunk (0x80 bytes) at 0x%p\n", hChunk);

	hChunk68 = HeapAlloc(hDefaultHeap, 0, 0x68);
	printf("Allocated first chunk (0x68 bytes) at 0x%p\n", hChunk68);
	
	hChunk58 = HeapAlloc(hDefaultHeap, 0, 0x58);
	memset(hChunk58, 'A', 0x58);
	printf("Allocated second 'vulnerable' chunk (0x58 bytes) at 0x%p, filled with 'A'\n", hChunk58);

	hChunk = HeapAlloc(hDefaultHeap, 0, 0x80);
	printf("Allocated end chunk (0x80 bytes) at 0x%p\n", hChunk);

	printf("Press return to continue\n");
	cin.ignore();

	// now free the 0x58 byte chunk
	HeapFree(hDefaultHeap, 0, hChunk58);
	printf("Free chunk of 0x58 bytes at 0x%p\n", hChunk58);

	// and free the one before too, so they would merge
	HeapFree(hDefaultHeap, 0, hChunk68);
	printf("Free first chunk of 0x68 bytes at 0x%p\n", hChunk68);

	printf("Press return to continue\n");
	cin.ignore();

	// cause a new allocation
	// with the aim to only replace the first 4 dwords in the 0x58 byte chunk
	// using the last 4 dwords of this allocation
	// we'll use B's in this chunk to show the accuracy.
	hChunk80 = HeapAlloc(hDefaultHeap, 0, 0x80);
	memset(hChunk80, 'B', 0x80);
	printf("Allocated chunk of 0x80 bytes at 0x%p, filled with 'B'\n", hChunk80);

	printf("Done...\n");
	cin.ignore();

	return 0;

}

