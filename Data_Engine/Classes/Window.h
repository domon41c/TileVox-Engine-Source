//Window events

#ifndef WINDOW_WINDOW_H_ 
#define WINDOW_WINDOW_H_ 

class GLFWwindow;

class Window {


public:
	static GLFWwindow* window;

	static int initialize(int window_width, int window_height, const char* title);
	static void terminate();

	static bool isShouldClose();
	static void setShouldClose(bool flag);
	static void swapBuffers();
};

#endif