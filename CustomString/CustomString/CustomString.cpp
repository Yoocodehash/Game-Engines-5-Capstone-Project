#include "pch.h"
#include "CustomString.h"
#include <string>
#include <string.h>

class CustomString
{
private:
	std::string text;
	TEXT_CALLBACK textCallback;

public:
	CustomString();
	CustomString(const std::string&);
	void GetString(char* buffer, size_t length);
	size_t GetLength();
	void Append(const std::string&);
	void SetCallback(TEXT_CALLBACK callback);
};

CustomString::CustomString() : CustomString(std::string("")) 
{

}

CustomString::CustomString(const std::string& inputText) 
{
	text = inputText;
}

void CustomString::GetString(char* buffer, size_t length) 
{
	strncpy_s(buffer, length, text.c_str(), text.size());
}

size_t CustomString::GetLength() 
{
	if (nullptr != textCallback) 
	{
		textCallback(false, text.size());
	}
	return text.size();
}

void CustomString::Append(const std::string& str)
{
	text.append(str);
	if (nullptr != textCallback) {
		textCallback(str.size() > 0, text.size());
	}
}

void CustomString::SetCallback(TEXT_CALLBACK callback)
{
	textCallback = callback;
}

CustomString* GetCustomString(const char* str, size_t strLength)
{
	return new CustomString(std::string(str, strLength));
}

void DestroyString(CustomString* targetObj)
{
	delete targetObj;
}

void GetString(CustomString* targetObj, char* buf, size_t length)
{
	targetObj->GetString(buf, length);
}

size_t GetLength(CustomString* targetObj)
{
	return targetObj->GetLength();
}

void Append(CustomString* targetObj, const char* text, size_t textLength)
{
	return targetObj->Append(std::string(text, textLength));
}

void SetCallback(CustomString* targetObj, TEXT_CALLBACK callback)
{
	targetObj->SetCallback(callback);
}