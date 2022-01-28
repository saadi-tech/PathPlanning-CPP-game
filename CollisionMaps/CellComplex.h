#pragma once
#include "RenderObject.h"

class CollisionMap;

class CellComplex : public RenderObject {

private:

	int iSize{ 0 };
protected:

	char* GetVShaderSrc() override {

		return (char*)R"====(

				#version 330
				layout (location = 0) in vec2 attrVecPosition;
				layout (location = 1) in vec2 attrVecOffset;
				uniform mat4x4 uniMatProjection, uniMatModel;
				void main() {
					
					gl_Position = uniMatProjection * uniMatModel * vec4(attrVecPosition + attrVecOffset, 0.0, 1.0);
				}
		)====";
	}

	char* GetFShaderSrc() override {

		return (char*)R"====(

				#version 330
				out vec4 outVecColor;
				void main() {
					
					outVecColor = vec4(0.75, 0.0, 0.0, 1.0);
				}
		)====";
	}

	void DrawPre() override;
public:

	void Load(
	   const CollisionMap& cmap);

	void Draw() override;
};
