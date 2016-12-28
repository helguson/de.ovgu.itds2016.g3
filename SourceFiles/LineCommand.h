#pragma once

#include <string>
#include <vector>
#include <iostream>

using String = std::string;
template<typename T>
using Vector<T> = std::vector<T>;


void callback() {}
void callback(int, int) {}
void callback(String, int, float) {}


template<int NUMBER_OF_ARGUMENTS>
void print(Vector<String> const & strings) {

	_print<0, NUMBER_OF_ARGUMENTS>(strings);
}

template<int NUMBER_OF_PROCESSED_ARGUMENTS, int NUMBER_OF_ARGUMENTS>
void _print(Vector<String> const & strings) {

	int const currentIndex = NUMBER_OF_PROCESSED_ARGUMENTS;
	std::cout << strings.at(currentIndex) << std::endl;

	_print<NUMBER_OF_PROCESSED_ARGUMENTS +1, NUMBER_OF_ARGUMENTS>(strings);
}

template<int NUMBER_OF_ARGUMENTS>
void _print<NUMBER_OF_ARGUMENTS, NUMBER_OF_ARGUMENTS>(Vector<String> const & strings) {

	int const currentIndex = NUMBER_OF_ARGUMENTS-1;
	std::cout << strings.at(currentIndex) << std::endl;
}

// TODO test
//--------------------------------------------------------------



//template <typename... ArgumentTypeN>
//void process(String... argumentN) {
//	callback(decodeFrom<ArgumentTypeN>(argumentN)...);
//}

template<typename ArgumentType>
ArgumentType decodeFrom(String const & arguments);

template<>
String decodeFrom(String const & argument) {
	return argument;
}

template<>
int decodeFrom(String const & argument) {
	return 1;
}

template<>
double decodeFrom(String const & argument) {
	return 2.0;
}


