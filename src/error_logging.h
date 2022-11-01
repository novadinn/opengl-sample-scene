#ifndef ERROR_LOGGING
#define ERROR_LOGGING

#include <glad/glad.h>

namespace error_logging {
    void check_opengl_error(const char* stmt, const char* fname, int line) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
	    printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
	    abort();
	}
    }
}

#ifdef DEBUG
#define GL_CHECK(stmt) do { \
	stmt; \
	error_logging::check_opengl_error(#stmt, __FILE__, __LINE__); \
    } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

#endif
