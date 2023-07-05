#include "InputHandler.h"


InputHandler::InputHandler(GLFWwindow* window, Camera* camera) {
	InputHandler::window = window;
	InputHandler::camera = camera;
	firstClick = false;
	moveSpeed = 1.0f;

	prevState.F1 = GLFW_RELEASE;

	showGui = true;
}


void InputHandler::SetWindow(GLFWwindow* window) {
	InputHandler::window = window;
}
void InputHandler::SetCamera(Camera* camera) {
	InputHandler::camera = camera;
}

void InputHandler::Process(float deltaTime) {
}