#pragma once

#include "RenderObject.h"

#include "glm/mat4x4.hpp"

class Monster;

class CellMonster : public RenderObject {

private:

	float fPosX{ 0.f }, fPosY{ 0.f };
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
				uniform int unibValidCell;
				out vec4 outVecColor;
				void main() {
					
					if (unibValidCell == 1)
						outVecColor = vec4(0.0, 1.0, 1.0, 1.0);
					else
						outVecColor = vec4(1.0, 0.0, 1.0, 1.0);
				}
		)====";
	}

	void DrawPre() override;
public:

	Monster* mMonster{ nullptr };

	void Load(Monster* monster);
	void Draw() override;
	void UpdatePosition();
	void UpdateMatrices(glm::mat4 mat_projection, glm::mat4 mat_model) override;

	static float fTickTime;
};
