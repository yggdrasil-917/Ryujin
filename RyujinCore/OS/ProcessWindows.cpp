#include "Process.hpp"
#include "../Containers/String.hpp"

namespace Ryujin
{
	void Process::RunProcess(const char* pathToExecutable, const char* arguments, bool bCaptureOutput, bool bWait /* = true */)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		char* args = new char[strlen(arguments)+1];
		strcpy(args, arguments);

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
				pathToExecutable,
				args, NULL, NULL, FALSE,
				NULL,
				NULL, NULL,
				&si,
				&pi)
			)
		{
			delete args;
			return;
		}
		delete args;
	}
}