#pragma once

#include "GLLibs.h"
#include "CellGrid.h"
#include "CellComplex.h"
#include "CellMonster.h"

#define STARTING_WIDTH 800
#define STARTING_HEIGHT 600
// Note: Window is resizable.

// Return values after exiting the main window loop.
#define WINDOW_FINISHED		0	// User exited the window.
#define WINDOW_INIT_FAIL	1
#define WINDOW_GLFW_FAIL	2
#define WINDOW_GLEW_FAIL	3

class Window
{
public:
	typedef unsigned char EXIT_REASON;

	EXIT_REASON Begin();
private:

	GLFWwindow* w{ nullptr };

	CellGrid mCellGrid;
	CellComplex mCellComplex;

	std::vector<CellMonster> mCellMonsters;

	void StartLoop();
	void OnUpdate(double dTime);
	void OnRender();
	void OnResize(int wi, int he);
	void OnMouseClick(int iBtn, int iAction, int iMods);
	void OnMouseMove(GLFWwindow* w, float x, float y);
	void OnScroll(double xoffset, double yoffset);
	void KeyListener();
};
