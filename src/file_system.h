#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

#include "platform.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define SLASH_CH '\\'
#elif __unix__
#define SLASH_CH '/'
#endif

namespace file_system {
    internal std::string join(std::string path) {
	std::string result;
	for (int i = 0; i < path.size(); ++i) {
	    if (path[i] == '/' || path[i] == '\\') {
		result += SLASH_CH;
		continue;
	    }

	    result += path[i];
	}

	return result;
    }

    internal std::string path_directory(std::string path) {
	return path.substr(0, path.find_last_of(SLASH_CH));
    }
}

#endif
