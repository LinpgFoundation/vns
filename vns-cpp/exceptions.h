#ifndef EXCPETIONS_H
#define EXCPETIONS_H

// Define an exception class for script compilation errors
class ScriptCompilerException : public std::exception
{
	std::string _message;
public:
	ScriptCompilerException(const std::string& message) : _message(message)
	{
	}

	const char* what() const throw () override
	{
		return _message.c_str();
	}
};
#endif
