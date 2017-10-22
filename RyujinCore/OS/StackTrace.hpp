#pragma once

#include "../Logger/Logger.hpp"

#if PLATFORM_WINDOWS
#include <DbgHelp.h>
#endif

namespace Ryujin
{
	FORCEINLINE void PrintStackTrace()
	{
#if PLATFORM_WINDOWS
					// Set up the symbol options so that we can gather information from the current
					// executable's PDB files, as well as the Microsoft symbol servers.  We also want
					// to undecorate the symbol names we're returned.  If you want, you can add other
					// symbol servers or paths via a semi-colon separated list in SymInitialized.
		::SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_INCLUDE_32BIT_MODULES | SYMOPT_UNDNAME);
		if (!::SymInitialize(::GetCurrentProcess(), "http://msdl.microsoft.com/download/symbols", TRUE))
			return;

		// Capture up to 25 stack frames from the current call stack.  We're going to
		// skip the first stack frame returned because that's the GetStackWalk function
		// itself, which we don't care about.
		PVOID addrs[25] = { 0 };
		USHORT frames = CaptureStackBackTrace(1, 25, addrs, NULL);
		IMAGEHLP_LINE64 *line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
		RYUJIN_ASSERT(line, "Failed to alloc space for stack trace");
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		Debugf(Info, "====================== STACK TRACE =========================");
		for (USHORT i = 0; i < frames; i++) 
		{
			// Allocate a buffer large enough to hold the symbol information on the stack and get 
			// a pointer to the buffer.  We also have to set the size of the symbol structure itself
			// and the number of bytes reserved for the name.
			ULONG64 buffer[(sizeof(SYMBOL_INFO) + 1024 + sizeof(ULONG64) - 1) / sizeof(ULONG64)] = { 0 };
			SYMBOL_INFO *info = (SYMBOL_INFO *)buffer;
			info->SizeOfStruct = sizeof(SYMBOL_INFO);
			info->MaxNameLen = 1024;

			// Attempt to get information about the symbol and add it to our output parameter.
			DWORD64 displacement = 0;
			if (::SymFromAddr(::GetCurrentProcess(), (DWORD64)addrs[i], &displacement, info))
			{
				DWORD64 address = (DWORD64)(addrs[i]);
				DWORD displacement;
				SymGetLineFromAddr64(::GetCurrentProcess(), address, &displacement, line);
				Debugf(Info, "	0x%x -> %s(%lu)", info->Address, info->Name, line->LineNumber);
			}
		}

		free(line);
		::SymCleanup(::GetCurrentProcess());
#endif
	}
}