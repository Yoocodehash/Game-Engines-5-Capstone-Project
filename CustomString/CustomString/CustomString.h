#pragma once

#ifdef CUSTOMSTRING_EXPORTS
#define CUSTOMSTRING_API __declspec(dllexport) 
#else
#define CUSTOMSTRING_API __declspec(dllimport) 
#endif

#include <stddef.h>

extern "C" 
{

		typedef void(*TEXT_CALLBACK)(bool textChanged, int newLength);

		class CustomString;

		CUSTOMSTRING_API CustomString* GetCustomString(const char* str, size_t strLength);

		CUSTOMSTRING_API void DestroyString(CustomString* targetObj);

		CUSTOMSTRING_API void GetString(CustomString* targetObj, char* buffer, size_t length);

		CUSTOMSTRING_API size_t GetLength(CustomString* targetObj);

		CUSTOMSTRING_API void Append(CustomString* targetObj, const char* str, size_t strLength);

		CUSTOMSTRING_API void SetCallback(CustomString* targetObj, TEXT_CALLBACK callback);
}