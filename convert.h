#ifndef CMDARGCONVERTER_H
#define CMDARGCONVERTER_H

#include <stdexcept>
#include <string>

template<typename FROM, typename TO>
class ArgConvert {
public:
    TO operator()(const FROM& from) { return TO(from); }
};

/*
 * Convertir string a unsigned char
 */
template<>
class ArgConvert<std::string, char> {
public:
    unsigned char operator()(const std::string& from) {
        if (from.length() == 0)
            throw std::runtime_error("Unable to cast command argument from empty string to char");

        return from[0];
    }
};

/*
 * Convertir string a unsigned int
 */
template<>
class ArgConvert<std::string, unsigned int> {
public:
    unsigned int operator()(const std::string& from) { return std::stoul(from); }
};

/*
 * Convertir string a int
 */
template<>
class ArgConvert<std::string, int> {
public:
    int operator()(const std::string& from) { return std::stoi(from); }
};

/*
 * Convertir string a unsigned long int
 */
template<>
class ArgConvert<std::string, unsigned long int> {
public:
    unsigned long int operator()(const std::string& from) { return std::stoul(from); }
};

/*
 * Convertir string a long int
 */
template<>
class ArgConvert<std::string, long int> {
public:
    long int operator()(const std::string& from) { return std::stol(from); }
};

/*
 * Convertir string a unsigned long long int
 */
template<>
class ArgConvert<std::string, unsigned long long int> {
public:
    unsigned long long int operator()(const std::string& from) { return std::stoull(from); }
};

/*
 * Convertir string a long long int
 */
template<>
class ArgConvert<std::string, long long int> {
public:
    long long int operator()(const std::string& from) { return std::stoll(from); }
};

/*
 * Convertir string a float
 */
template<>
class ArgConvert<std::string, float> {
public:
    float operator()(const std::string& from) { return std::stof(from); }
};

/*
 * Convertir string a double
 */
template<>
class ArgConvert<std::string, double> {
public:
    double operator()(const std::string& from) { return std::stod(from); }
};

/*
 * Convertir string a long double
 */
template<>
class ArgConvert<std::string, long double> {
public:
    long double operator()(const std::string& from) { return std::stold(from); }
};

#endif