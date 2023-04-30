#include "GLPtrMgr.h"

GLPtrMgr::GLPtrMgr()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		//std::cerr << "Failed to initialize GLEW." << std::endl;
		return;
	}
}
