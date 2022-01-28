#include "CellGrid.h"
#include "GLLibs.h"
#include "CollisionMap.h"

void CellGrid::Load(
   const CollisionMap::CellMap& cmap) {

	RenderObject::Load();

	std::vector<glm::vec2> vecTranslations;

	for (int x = 0; x < cmap.size(); x++) {

		for (int y = 0; y < cmap[x].size(); y++) {

			glm::vec2 vecTranslation;
			vecTranslation.x = static_cast<float>(x);
			vecTranslation.y = static_cast<float>(y);
			vecTranslations.push_back(vecTranslation);
			iSize++;
		}
	}

	glGenBuffers(1, &_iIVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _iIVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * iSize, &vecTranslations.data()[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	stdvecfVertexData = {

		0.f, 0.f,  0.f, 1.f,  1.f, 1.f,  1.f, 0.f
	};

	glBindVertexArray(_iVAO);

	glBindBuffer(GL_ARRAY_BUFFER, _iVBO);
	glBufferData(GL_ARRAY_BUFFER, stdvecfVertexData.size() * sizeof(float), stdvecfVertexData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, _iIVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(1, 1);
}

void CellGrid::DrawPre() {

	RenderObject::DrawPre();

	static int uniproj = glGetUniformLocation(iShaderProgramHandle, "uniMatProjection");
	glUniformMatrix4fv(uniproj, 1, false, &matProjection[0][0]);

	static int unimodel = glGetUniformLocation(iShaderProgramHandle, "uniMatModel");
	glUniformMatrix4fv(unimodel, 1, false, &matModel[0][0]);
}

void CellGrid::Draw() {

	RenderObject::Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArraysInstanced(GL_QUADS, 0, 4, iSize);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
