#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/gtc/type_ptr.hpp>
class Shader2 {
public:
	// the program ID
	unsigned int ID;

	Shader2(const char* vertex_file_path, const char* fragment_file_path);
	// use/activate the shader
	void use();


	void setMatrix4fv(const std::string& name, float matrix[]) const;
	void setMatrix4fv(const std::string& name, glm::mat4& matrix) const;
private:
};


#endif
