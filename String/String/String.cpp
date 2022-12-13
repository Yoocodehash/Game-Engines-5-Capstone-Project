#include "pch.h"
#include "String.h"
#include <string>
#include <string.h>

/* This class is needed to be initialized inside this .cpp file because putting it in the header file will cause
errors due the extern C# functions are similar to these functions inside the String class */
class String
{
private:
	std::string Text;
	callback Callback;

public:
	String();
	String(const std::string&);
	void GetString(char* Buffer_, size_t Length_);
	size_t GetStringLength();
	void AttachString(const std::string&);
	void SetStringCallback(callback Callback_);
};

// Constructor default intialization
String::String()
	: String(std::string("")) 
{
}

// Constructor that will pass in the input string parameter
String::String(const std::string& Text_) 
{
	Text = Text_;
}

// Retreive the string and put in into a given buffer
void String::GetString(char* Buffer_, size_t Length_)
{
	strncpy_s(Buffer_, Length_, Text.c_str(), Text.size());
}

// Retreive the length of the string
size_t String::GetStringLength() 
{
	if (nullptr != Callback) {
		Callback(false, Text.size());
	}
	return Text.size();
}

// Attach the string so that it can be appended (extends the string by appending at the end of the current value)
void String::AttachString(const std::string& String_) 
{
	Text.append(String_);
	if (nullptr != Callback) {
		Callback(String_.size() > 0, Text.size());
	}
}

/* Sets the callback to listen for the GetStringLengthand AttachString functions
This is needed to use the string callback from unity */
void String::SetStringCallback(callback Callback_) 
{
	Callback = Callback_;
}

/* Rest of the functions below are DLL exported functionsand don't require a class declaration in a function
Creates a new instance of the class and returns it */
String* GetWrittenString(const char* String_, size_t StringSize_)
{
	return new String(std::string(String_, StringSize_));
}

// Destroy the created instance and release memory
void OnDestroyString(String* StringObject_) 
{
	delete StringObject_;
}

// Retreive the string of an instance
void GetString(String* StringObject_, char* Buffer_, size_t Size_) 
{
	StringObject_->GetString(Buffer_, Size_);
}

// Retreive the length of the string for an instance
size_t GetStringLength(String* StringObject_) 
{
	return StringObject_->GetStringLength();
}

// Attach the string for an instance
void AttachString(String* StringObject_, const char* Text_, size_t StringSize_) {
	return StringObject_->AttachString(std::string(Text_, StringSize_));
}

// Set callback for listening GetStringLength and AttachString for an instance
void SetStringCallback(String* StringObject_, callback Callback_)
{
	StringObject_->SetStringCallback(Callback_);
}