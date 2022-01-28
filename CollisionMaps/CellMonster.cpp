#include "CellMonster.h"
#include "GLLibs.h"
#include "Monster.h"
#include "Location.h"
#include "CollisionMap.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

float CellMonster::fTickTime = 0.25f;

void CellMonster::Load(Monster* monster) {

	if (monster == nullptr)
		return;

	RenderObject::Load();

	mMonster = monster;
	monster->m_render_object = this;

	stdvecfVertexData = {

		0.f, 0.f,  0.f, 1.f,  1.f, 1.f,  1.f, 0.f
	};

	glBindVertexArray(_iVAO);

	glBindBuffer(GL_ARRAY_BUFFER, _iVBO);
	glBufferData(GL_ARRAY_BUFFER, stdvecfVertexData.size() * sizeof(float), stdvecfVertexData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	UpdatePosition();
}

void CellMonster::DrawPre() {

	RenderObject::DrawPre();

	static int uniproj = glGetUniformLocation(iShaderProgramHandle, "uniMatProjection");
	glUniformMatrix4fv(uniproj, 1, false, &matProjection[0][0]);

	// Does not update here for some reason, while uniMatProjection and uniMatModel do... wtf!?
	// Updates only once, when this method is called from who knows where... OpenGL moment.
	//static int unipos = glGetUniformLocation(iShaderProgramHandle, "uniVecPosition");
	//glUniform2f(unipos, fPosX, fPosY);

	static int unimodel = glGetUniformLocation(iShaderProgramHandle, "uniMatModel");
	glUniformMatrix4fv(unimodel, 1, false, &matModel[0][0]);

	static int univalidcell = glGetUniformLocation(iShaderProgramHandle, "unibValidCell");
	glUniform1i(univalidcell, mMonster->InValidCell());
}

void CellMonster::Draw() {

	if (mMonster == nullptr)
		return;

	RenderObject::Draw();
	glDrawArrays(GL_QUADS, 0, 4);
}

void CellMonster::UpdateMatrices(glm::mat4 mat_projection, glm::mat4 mat_model) {

	RenderObject::UpdateMatrices(mat_projection, mat_model);
}

void CellMonster::UpdatePosition() {

	fPosX = static_cast<float>(mMonster->GetCellX());
	fPosY = static_cast<float>(mMonster->GetCellY());

	// But here, it updates.
	// Lack of glUseProgram in DrawPre is not the issue, since uniProj and uniModel work!!
	glUseProgram(iShaderProgramHandle);
	static int unipos = glGetUniformLocation(iShaderProgramHandle, "uniVecPosition");
	glUniform2f(unipos, fPosX, fPosY);
}
