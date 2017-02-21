#ifndef __UTILS_H__
#define __UTILS_H__

#include <shlwapi.h>

namespace utils {
	void RequestBrowser(const char* url) {
		ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
	}
}

#endif // !__UTILS_H__