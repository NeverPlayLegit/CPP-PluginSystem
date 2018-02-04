#include <Windows.h>

#include <vector>
#include <iostream>

using namespace std;

class Plugin {
typedef char*(*f_GetName)();
typedef void(*f_DoWork)();

public:
	HINSTANCE handle;

	f_GetName GetName;
	f_DoWork DoWork;

	bool Load(string path) {
		handle = LoadLibraryA(path.c_str());
		
		if (!handle) return false;

		GetName = (f_GetName)GetProcAddress(handle, "GetName");
		if (!GetName) return false;

		DoWork = (f_DoWork)GetProcAddress(handle, "DoWork");
		if (!DoWork) return false;

		return true;
	}

	void Unload() {
		FreeLibrary(handle);
	}
};

string GetPath(bool withoutName = true) {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	if (withoutName) {
		string::size_type pos = string(buffer).find_last_of("\\/");
		return string(buffer).substr(0, pos);
	}
	else return string(buffer);
}

vector<Plugin*> loadedPlugins = vector<Plugin*>();

int main() {
	SetConsoleTitle("PluginSystem");

	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile((GetPath() + "\\plugins\\*.*").c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			string name = data.cFileName;
			if (name.length() > 3) {
				string extension = name.substr(name.length() - 3, 3);
				if (strcmp(extension.c_str(), "dll") == 0) {
					Plugin* plugin = new Plugin();
					if (plugin->Load(GetPath() + "\\plugins\\" + data.cFileName)) {
						cout << "Plugin '" << plugin->GetName() << "' loaded from File '" << name.c_str() << "'." << endl;
						loadedPlugins.push_back(plugin);
					}
					else {
						cerr << "Plugin could not be loaded" << endl;
					}
					
					
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	while (true) {
		for (Plugin* plugin : loadedPlugins) {
			plugin->DoWork();
		}

		Sleep(1000);
	}

	return 0;
}
