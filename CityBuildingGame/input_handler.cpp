#include "input_handler.h"

void InputHandler::Keypress(int button, int action)
{
	if (windowFocused)
	{
		if (button == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(Window, true);
		// Keyboard scrolling, maybe to be removed
		if (button == GLFW_KEY_W || button == GLFW_KEY_UP)
			Camera->Scroll(UP, 0.05f);
		if (button == GLFW_KEY_S || button == GLFW_KEY_DOWN)
			Camera->Scroll(DOWN, 0.05f);
		if (button == GLFW_KEY_A || button == GLFW_KEY_LEFT)
			Camera->Scroll(LEFT, 0.05f);
		if (button == GLFW_KEY_D || button == GLFW_KEY_RIGHT)
			Camera->Scroll(RIGHT, 0.05f);
	}
}

void InputHandler::Mouseclick(int button, int action) 
{
	if (windowFocused)
	{
		if (!action == GLFW_PRESS) { return; }

		// Test Code
		glm::vec2 cursor_position = Camera->CursorPositionOnGrid();
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			try {
				if (cursor_position.x >= 0 && cursor_position.y >= 0 && Grid->gridUnits.at((int)cursor_position.y).at((int)cursor_position.x)) {
					Grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->objects.push_back(
						new Fir(glm::vec3(cursor_position.x, cursor_position.y, Grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->averageHeight),
							glm::vec3(1.0f, 1.0f, 1.0f),
							1.0f));
				}
			}
			catch (const std::out_of_range) {
				std::cout << "Cannot insert outside of grid" << std::endl;
			}

		}
	}
}

void InputHandler::Mousewheel(float yOffset)
{
	if (windowFocused)
		Camera->Zoom(yOffset);
}

void InputHandler::WindowFocus(int focused) 
{
	windowFocused = focused;
	if (windowFocused) {

		int width;
		int height;
		int left;
		int top;
		int right;
		int bottom;
		RECT windowEdges;

		glfwGetWindowPos(Window, &left, &top);
		glfwGetWindowSize(Window, &width, &height);
		right = left + width;
		bottom = top + height;

		windowEdges.left = left;
		windowEdges.top = top;
		windowEdges.right = right;
		windowEdges.bottom = bottom;

		Camera->ScreenRatio = (float)width / (float)height;

		ClipCursor(&windowEdges);
	}
}

void InputHandler::MouseScroll()
{
	if (windowFocused) {
		double xpos;
		double ypos;
		int width;
		int height;

		glfwGetCursorPos(Window, &xpos, &ypos);
		glfwGetWindowSize(Window, &width, &height);

		if (xpos == 0)
			Camera->Scroll(LEFT, 0.01f);
		if (ypos == 0)
			Camera->Scroll(UP, 0.01f);
		if (width - (int)xpos <= 1)
			Camera->Scroll(RIGHT, 0.01f);
		if (height - (int)ypos <= 1)
			Camera->Scroll(DOWN, 0.01f);
	}
}