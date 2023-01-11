#ifndef SHADER_VOX_GEN_H_
#define SHADER_VOX_GEN_H_
#include <stdlib.h>



class Mesh;
class Chunk;

class VoxelRenderer {
	float* buffer;
	size_t capacity;
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(Chunk* chunk, const Chunk** chunks);
};

#endif