#ifndef SHADERS_SHADER_H_
#define SHADERS_SHADER_H_
#include <string>

class Shader{
public:
	unsigned int id;
	Shader(unsigned int id);
	~Shader();

	void use();
};

extern Shader* load_shader(std::string vertexFile, std::string fragmentFile);

#endif