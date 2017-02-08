#pragma once

#include <exception>
#include <string>

//struct MyException : public std::exception {
//	const char *what() const throw() {
//		return "Message here";
//	}
//};

struct InvalidResultArrayDimension : public std::exception {
	std::string message_;
	const char *what() const throw() {
		return message_.c_str();
	}
public:
	explicit InvalidResultArrayDimension(const std::string& message) : message_(message) {}
	explicit InvalidResultArrayDimension() : message_("Invalid result array dimensions") {};
}InvalidResultArrayDimensionException;
