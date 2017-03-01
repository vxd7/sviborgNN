#pragma once

#include <exception>
#include <vector>
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
};

struct MissingConfiguration : public std::exception {
	std::string message_;
	const char *what() const throw() {
		return message_.c_str();
	}

public:
	explicit MissingConfiguration(const std::vector<std::string>& missing) {
		for(size_t i = 0; i < missing.size() - 1; ++i) {
			message_ += missing[i] + ", ";
		}

		message_ += missing[missing.size() - 1] + ".";
	}

	explicit MissingConfiguration() : message_("Invalid configuration") {};
};
