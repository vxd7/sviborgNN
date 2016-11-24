#pragma once

#include <exception>

//struct MyException : public std::exception {
//	const char *what() const throw() {
//		return "Message here";
//	}
//};

struct InvalidResultArrayDimension : public std::exception {
	const char *what() const throw() {
		return "Invalid result array dimensions";
	}
}InvalidResultArrayDimensionException;
