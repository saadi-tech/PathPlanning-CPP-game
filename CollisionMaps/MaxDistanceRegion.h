#pragma once

#include "RenderObject.h"

#include "glm/mat4x4.hpp"

//class CellMonster;

class MaxDistanceRegion : public RenderObject {

private:

	int iSize{ 0 };
	//CellMonster* mCellMonster{ nullptr };
protected:

	char* GetVShaderSrc() override {

		return (char*)R"====(

				#version 330
				layout (location = 0) in vec2 attrVecPosition;
				uniform vec2 uniVecPosition;
				uniform mat4x4 uniMatProjection, uniMatModel;
				void main() {
					
					gl_Position = uniMatProjection * uniMatModel * vec4(attrVecPosition + uniVecPosition, 0.0, 1.0);
				}
		)====";
	}

	char* GetFShaderSrc() override {

		return (char*)R"====(

				#version 330
				out vec4 outVecColor;
				void main() {
					
					outVecColor = vec4(0.0, 0.0, 1.0, 1.0);
				}
		)====";
	}

	void DrawPre() override;
public:

	void Load(/*CellMonster* cm*/);
	void Draw() override;
	void Set();
};
