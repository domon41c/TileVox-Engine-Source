#ifndef SHADERS_RENDER_H_
#define SHADERS_RENDER_H_
#include <string>

class Texture{
public:
	unsigned int id;
	int width;
	int height;
	Texture(unsigned int id, int width, int height);
	~Texture();

	void bind();
};

extern Texture* load_texture(std::string filename);

#endif