#include "RenderObject.h"
#include "GLLibs.h"

#include "glm/gtc/matrix_transform.hpp"

std::string __GetLogs(int iShader)
{
	int len;
	glGetShaderiv(iShader, GL_INFO_LOG_LENGTH, &len);

	if (len > 0) {

		std::vector<char> v(len);
		glGetShaderInfoLog(iShader, len, NULL, v.data());
		return std::string(begin(v), end(v));
	}
	else
		return "No logs";
}

void RenderObject::_Load(std::string* out_log_vs, std::string* out_log_fs) {

	glGenVertexArrays(1, &_iVAO);
	glGenBuffers(1, &_iVBO);

	int iVShader = glCreateShader(GL_VERTEX_SHADER);
	char* szVShaderSrc = GetVShaderSrc();
	glShaderSource(iVShader, 1, &szVShaderSrc, 0);
	glCompileShader(iVShader);

	int iFShader = glCreateShader(GL_FRAGMENT_SHADER);
	char* szFShaderSrc = GetFShaderSrc();
	glShaderSource(iFShader, 1, &szFShaderSrc, 0);
	glCompileShader(iFShader);

	iShaderProgramHandle = glCreateProgram();
	glAttachShader(iShaderProgramHandle, iVShader);
	glAttachShader(iShaderProgramHandle, iFShader);
	glLinkProgram(iShaderProgramHandle);

	if (out_log_vs != nullptr)
		*out_log_vs = __GetLogs(iVShader);

	if (out_log_fs != nullptr)
		*out_log_fs = __GetLogs(iFShader);
}

void RenderObject::DrawPre() {

	glUseProgram(iShaderProgramHandle);
}

void RenderObject::Load(std::string* out_log_vs, std::string* out_log_fs) {

	_Load(out_log_vs, out_log_fs);
}

void RenderObject::Load() {

	_Load(nullptr, nullptr);
}

void RenderObject::Draw() {

	DrawPre();
	glBindVertexArray(_iVAO);
}

void RenderObject::UpdateMatrices(glm::mat4 mat_projection, glm::mat4 mat_model) {

	matProjection = glm::mat4(1.f);
	matProjection *= mat_projection;
	matModel = glm::mat4(1.f);
	matModel *= mat_model;
}
