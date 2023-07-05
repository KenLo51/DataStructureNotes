#pragma once

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

// OpenGL Libreries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/FileBrowser/ImGuiFileBrowser.h>

// User Defined Libreries
#include "ShaderClass.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Maze.h"

#include "InputHandler.h"

#include "mainWindow.h"

class MainWindow;

class WaveSimulation;

class MainView {
private:
	void drawImgui();

	// initialize VAO, VBO, EBO, Shader, Texture...
	void initViewObjects();
	void drawView(float deltaTime = 1.0f);
public:
	MainView(MainWindow* mw = NULL);
	void draw(float deltaTime = 1.0f);
	void resize(int width, int height);
	void updateParameter(float deltaTime = 1.0f);

	void updataMaze();
	void updataPath();
private:
public:
	MainWindow* mainWindow;
	bool limitFrameRate;

	imgui_addons::ImGuiFileBrowser file_dialog;
	bool enableEdit;

	Maze* maze;
	glm::ivec2 mazeBegPt, mazeEndPt;
	//camears
	Camera* mainCamera;

	Mesh* mazePlane;
	GLuint mazeTexID;

	Mesh* pathLines;

	// frame buffers
	GLuint fboClickpos;
	Texture* texClickpos;
	GLuint texClickposID;

	//lights
	Camera* directionalLight;
	float directionalLight_theta;
	float directionalLight_phi;

	//shaders
	Shader* mazeShader;
	Shader* pathShader;
	Shader* clickposShader;
};