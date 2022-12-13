#pragma once

// If the DLL_EXPORTS is not defined, then define the DLL
#ifdef STRINGDLL_EXPORTS
#define STRINGDLL_API __declspec(dllexport) 
#else
#define STRINGDLL_API __declspec(dllimport) 
#endif

#include <stddef.h> // need for size_t

// "C" needed to use this string library inside of Unity
extern "C"
{
	/* Callback definition for GetStringLengthand AttachString functions
	(this is where the text changes and the new length come in) */
	typedef void(*callback)(bool DidTextChange, int NewLength);

	// Library class declaration (needed for the String class to get access to these DLL export functions below
	class String;

	// DLL Exported functions
	/* Side note: the string obj variables are needed to store what input string was written in the DLL to
	the String class */

	STRINGDLL_API String* GetWrittenString(const char* String_, size_t StringLength_);
	STRINGDLL_API void OnDestroyString(String* StringObject_);
	STRINGDLL_API void GetString(String* StringObject_, char* Buffer_, size_t Length_);
	STRINGDLL_API size_t GetStringLength(String* StringObject_);
	STRINGDLL_API void AttachString(String* StringObject_, const char* String_, size_t StringLength_);
}