#ifndef VOX_CHUNKSA_H_
#define VOX_CHUNKSA_H_
#include <stdlib.h>

class Chunk;
class voxel;

class Chunks {
public:
	Chunk** chunks;
	size_t volume;
	unsigned int w,h,d;

	Chunks(int w, int h, int d);
	~Chunks();
};

#endif