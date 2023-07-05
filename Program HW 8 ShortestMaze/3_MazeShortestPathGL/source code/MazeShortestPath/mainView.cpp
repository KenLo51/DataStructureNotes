#include "mainView.h"

MainView::MainView(MainWindow* mw) {
	MainView::mainWindow = mw;
	initViewObjects();
	directionalLight_theta = 29.0f;
	directionalLight_phi = 67.0f;
	limitFrameRate = true;
	enableEdit = true;
}

void MainView::drawImgui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// User Manual Halper
	ImGui::Begin("User Manual");
	ImGui::Text("left click        : Set a barrier in the maze.");
	ImGui::Text("right click       : Clear a barrier in the maze.");
	ImGui::Text("alt + left click  : Set the end point.");
	ImGui::Text("alt + right click : Set the starting point.");
	ImGui::End();

	// References
	//ImGui::Begin("References");
	//ImGui::Text("   OpenGL Water Tutorial. https://youtu.be/HusvGeEDU_U");
	//ImGui::Text("   FLTK Project Water Surface. https://medium.com/maochinn/fltk-project-water-surface-a811c9cfc3b");
	//ImGui::Text("Textures:");
	//ImGui::Text("   Floor Tiles 09. https://polyhaven.com/a/floor_tiles_09");
	//ImGui::End();

	// Light source parameters
	ImGui::Begin("Maze");
	int mazeSize[2] = { maze->w()-2, maze->h()-2};
	ImGui::SliderInt2("Size", mazeSize, 2, 20);
	if (mazeSize[0] != maze->w()-2 || mazeSize[1] != maze->h()-2) {
		maze->resize(mazeSize[0], mazeSize[1]);
		mazeEndPt = glm::ivec2(maze->w() - 2, maze->h() - 2);
		updataMaze();
		updataPath();
	}

	bool open = false, save = false;
	open = ImGui::Button("Open File"); ImGui::SameLine();
	save = ImGui::Button("Save File");
	if (open) 
		ImGui::OpenPopup("Open File");
	if (save)
		ImGui::OpenPopup("Save File");
	enableEdit = ! (ImGui::IsPopupOpen("Open File") || ImGui::IsPopupOpen("Save File"));
	if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".txt")){
		std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
		std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file
		maze->load(file_dialog.selected_path.c_str());

		updataMaze();
		updataPath();
	}
	if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".txt")){
		std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
		std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file
		std::cout << file_dialog.ext << std::endl;              // Access ext separately (For SAVE mode)

		std::string filename = file_dialog.selected_path + file_dialog.ext;
		maze->save(filename.c_str());
	}
	ImGui::End();


	ImGui::Render();
}

// initialize VAO, VBO, EBO, Shader, Texture...
void MainView::initViewObjects() {
	maze = new Maze("maze.txt");
	mazeBegPt = glm::ivec2(1, 1);
	mazeEndPt = glm::ivec2(maze->w() - 2, maze->h() - 2);

	// init shader ///////////////////////////////////////////////////////////////////////////////////
	MainView::mazeShader = new Shader ("mazeImg.vert", NULL, NULL, NULL, "mazeImg.frag");
	//MainView::pathShader = new Shader("path.vert", NULL, NULL, NULL, "path.frag");
	MainView::pathShader = new Shader("path.vert", NULL, NULL, "path.geom", "path.frag");

	MainView::clickposShader = new Shader("click.vert", NULL, NULL, NULL, "click.frag");

	// init camera ///////////////////////////////////////////////////////////////////////////////////
	MainView::mainCamera = new Camera(mainWindow->w(), mainWindow->h(), glm::vec3(0.0f, 5.0f, -5.0f), glm::vec3(0.0f, -1.0f, 1.0f));
	MainView::mainCamera->perspectiveInit(glm::radians(120.0f), 0.1f, 1000.0f);
	MainView::mainCamera->updateMatrix();

	// init opengl ///////////////////////////////////////////////////////////////////////////////////
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// init models ///////////////////////////////////////////////////////////////////////////////////
	glm::mat4 modelTransform;
	// Maze plane
	mazePlane = new Mesh();
	basicMesh_Plane(mazePlane);

	// create a texture
	glGenTextures(1, &mazeTexID);
	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mazeTexID);
	// setting up texture
	//		Texture Wrapping
	//			2d or 3d texture; s and t (and r)axis; repeat, mirror repeat, edge or border mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//		border mode setting
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//		Texture Filtering 
	//			2d or 3d texture; scaling up and scaling down; nearest or linear;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//			Mipmaps. use low resolution textures when objects are far away.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mazeTexID);
	unsigned int mazeWidth = maze->w();
	unsigned int mazeHeight = maze->h();
	mazeWidth = mazeWidth + ((mazeWidth & 0x03) > 0) * (4 - (mazeWidth & 0x03));
	char* mazeData = new char[mazeWidth * mazeHeight];
	for (unsigned int i = 0; i < mazeHeight; i++)
		memcpy(mazeData + mazeWidth * i, maze->data() + maze->w() * i, maze->w());


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, maze->w(), maze->h(), 0, GL_RED, GL_UNSIGNED_BYTE, mazeData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// path line
	pathLines = NULL;
	updataPath();


	// click position frame buffer
	glGenFramebuffers(1, &fboClickpos);
	glGenTextures(1, &texClickposID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texClickposID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainView::mainWindow->w(), MainView::mainWindow->h(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboClickpos);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texClickposID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	texClickpos = new Texture(texClickposID);
}

void MainView::resize(int width, int height) {
	MainView::mainCamera->setSize(width, height);
	glm::ivec2 screenSize(width, height);
	MainView::mazeShader->setUniform("screenSize", screenSize);

	//		click position frame buffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texClickposID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainView::mainWindow->w(), MainView::mainWindow->h(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//		attach texture as FBO's buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboClickpos);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texClickposID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0, 0, width, height);
}

void MainView::drawView(float deltaTime) {

	// update objects ////////////////////////////////////////////
	glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
	glm::vec3 viewPos = mainCamera->position_vec3;
	glm::mat4 modelTransform = glm::mat4(1.0f);

	// draw objects //////////////////////////////////////////////
	glDisable(GL_BLEND);
	// clear frame
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Draw maze
	MainView::mazeShader->Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mazeTexID);
	MainView::mazeShader->setUniform("image", 0);
	MainView::mazeShader->setUniform("begPoint", mazeBegPt);
	MainView::mazeShader->setUniform("endPoint", mazeEndPt);
	glm::ivec2 screenSize(mainWindow->w(), mainWindow->h());
	MainView::mazeShader->setUniform("screenSize", screenSize);
	mazePlane->Draw();

	// Draw path
	MainView::pathShader->Activate();
	MainView::pathShader->setUniform("t", (float)fmod(glfwGetTime(), 1.0f));
	glm::mat4 pathTransform(1.0f);
	pathTransform = glm::scale(glm::vec3(2.0f / maze->w(), 2.0f / maze->h(), 1)) * pathTransform;
	pathTransform = glm::translate(glm::vec3(-1, 1, 1)) * pathTransform;
	glm::vec2 scaleScreen = glm::vec2((float)mainWindow->w() / (float)mainWindow->h(), (float)maze->w() / (float)maze->h());
	if (scaleScreen.x > scaleScreen.y) {
		pathTransform = glm::scale(glm::vec3(1.0f * (scaleScreen.y / scaleScreen.x), 1.0f, 1)) * pathTransform;
	}
	else {
		pathTransform = glm::scale(glm::vec3(1.0f, 1.0f * (scaleScreen.x / scaleScreen.y), 1)) * pathTransform;
	}
	MainView::pathShader->setUniform("model", pathTransform);
	glLineWidth(5);
	glPointSize(10);
	pathLines->DrawArray(GL_LINES);
}

void MainView::draw(float deltaTime) {
	// draw imgui
	if(mainWindow->inputHandler->showGui)
		drawImgui();

	// draw view
	drawView(deltaTime);
}

void MainView::updateParameter(float deltaTime) {
	
	if (enableEdit && (
		glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
		glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)) {
		double mouseX, mouseY;
		glfwGetCursorPos(mainWindow->window, &mouseX, &mouseY);
		mouseX = mouseX;
		mouseY = (float)mainWindow->h() - mouseY;

		glBindBuffer(GL_FRAMEBUFFER, fboClickpos);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		MainView::clickposShader->Activate();
		glm::ivec2 screenSize(mainWindow->w(), mainWindow->h());
		MainView::clickposShader->setUniform("screenSize", screenSize);
		mazePlane->Draw();

		float data[4];
		glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
		glReadPixels(mouseX, mouseY, 1, 1, GL_RGBA, GL_FLOAT, data);

		glBindBuffer(GL_FRAMEBUFFER, 0);

		//printf("  %7.2lf-%7.2lf-%7.2lf\n", data[0], data[1], data[2]);
		
		unsigned int blockIdx_x = (int)(data[0]* maze->w());
		unsigned int blockIdx_y = (int)(data[1] * maze->h());
		unsigned int blockIdx = blockIdx_x + blockIdx_y * maze->w();
		if (blockIdx >= maze->w() * maze->h()) return;
		char& block = maze->data()[blockIdx];
		//printf("  %7.2lf-%7.2lf-%7.2lf  : %d\n", data[0], data[1], data[2], block);

		if (glfwGetKey(mainWindow->window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
			if (glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
				mazeBegPt = glm::ivec2(blockIdx_x, blockIdx_y);
				updataPath();
			}
			if (glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				mazeEndPt = glm::ivec2(blockIdx_x, blockIdx_y);
				updataPath();
			}

		}
		else{
			if (block == Maze::BARRIER &&
				glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS &&
				(blockIdx % maze->w() != 0) &&
				(blockIdx % maze->w() != maze->w() - 1) &&
				(blockIdx / maze->w() != 0) &&
				(blockIdx / maze->w() != maze->h() - 1)) {
				block = Maze::ROAD;
				glBindTexture(GL_TEXTURE_2D, mazeTexID);
				glTexSubImage2D(GL_TEXTURE_2D, 0, blockIdx_x, blockIdx_y, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &block);
				glBindTexture(GL_TEXTURE_2D, 0);
				updataPath();
			}
			if (block == Maze::ROAD &&
				glfwGetMouseButton(mainWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				block = Maze::BARRIER;
				glBindTexture(GL_TEXTURE_2D, mazeTexID);
				glTexSubImage2D(GL_TEXTURE_2D, 0, blockIdx_x, blockIdx_y, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &block);
				glBindTexture(GL_TEXTURE_2D, 0);
				updataPath();
			}
		}
	}
}

void MainView::updataMaze() {
	glDeleteTextures(1, &mazeTexID);
	// create a texture
	glGenTextures(1, &mazeTexID);
	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mazeTexID);
	// setting up texture
	//		Texture Wrapping
	//			2d or 3d texture; s and t (and r)axis; repeat, mirror repeat, edge or border mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//		border mode setting
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//		Texture Filtering 
	//			2d or 3d texture; scaling up and scaling down; nearest or linear;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//			Mipmaps. use low resolution textures when objects are far away.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mazeTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, maze->w(), maze->h(), 0, GL_RED, GL_UNSIGNED_BYTE, maze->data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void MainView::updataPath() {
	std::vector<glm::ivec2> pathInt = maze->Solve(mazeBegPt, mazeEndPt);
	std::vector<Vertex> pathLine;
	if (pathInt.size() > 0) {
		pathLine.resize(pathInt.size() * 2 + 2);
		for (unsigned int i = 0; i < pathInt.size() - 1; i++) {
			pathLine[i * 2 + 0].position = glm::vec3(pathInt[i + 0].x + 0.5, -pathInt[i + 0].y - 0.5, -0.2f);
			pathLine[i * 2 + 1].position = glm::vec3(pathInt[i + 1].x + 0.5, -pathInt[i + 1].y - 0.5, -0.2f);
		}
		pathLine[pathLine.size() - 2].position = glm::vec3(pathInt[pathInt.size() - 1].x + 0.5, -pathInt[pathInt.size() - 1].y - 0.5, -0.2f);
		pathLine[pathLine.size() - 1].position = glm::vec3(mazeBegPt.x + 0.5, -mazeBegPt.y - 0.5, -0.2f);
	}
	if(pathLines)	pathLines->Load(pathLine);
	else pathLines = new Mesh(pathLine);
}