#include "Chunks_.h"
#include "Chunks.h"

Chunks::Chunks(int w, int h, int d) : w(w), h(h), d(d){
	volume = w*h*d;
	chunks = new Chunk*[volume];

	int index = 0;
	for (int y = 0; y < h; y++){
		for (int z = 0; z < d; z++){
			for (int x = 0; x < w; x++, index++){
				Chunk* chunk = new Chunk(x,y,z);
				chunks[index] = chunk;
			}
		}
	}
}

Chunks::~Chunks(){
	for (int i = 0; i < volume; i++){
		delete chunks[i];
	}
	delete[] chunks;
}