#include <Windows.h>

#include <iostream>

using namespace std;

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved) {
	return true;
}

extern "C" __declspec(dllexport) char* GetName() {
	return "TestPlugin v1.0";
}

extern "C" __declspec(dllexport) void DoWork() {
	cout << "Hello from Testplugin!" << endl;
}
