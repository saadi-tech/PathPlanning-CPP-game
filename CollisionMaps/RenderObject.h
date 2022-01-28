#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "glm/mat4x4.hpp"

class RenderObject
{
private:
	void _Load(std::string* out_log_vs, std::string* out_log_fs);
protected:

	glm::mat4 matProjection{ glm::mat4(1.f) };
	glm::mat4x4 matModel{ glm::mat4x4(1.f) };

	int iShaderProgramHandle{ 0 };

	std::vector<float> stdvecfVertexData;

	virtual char* GetVShaderSrc() { return (char*)""; }
	virtual char* GetFShaderSrc() { return (char*)""; }

	virtual void DrawPre();

	unsigned int _iIVBO{ 0 };

	unsigned int _iVAO{ 0 };
	unsigned int _iVBO{ 0 };
public:

	virtual void Load(std::string* out_log_vs, std::string* out_log_fs);
	virtual void Load();
	virtual void Draw();
	virtual void UpdateMatrices(glm::mat4 mat_projection, glm::mat4 mat_model);
};
