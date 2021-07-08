#include <iostream>
#include <string>

#include "commands.h"

//============================== FUNCTIONS ==============================
void hello() {
    std::cout << "> Console: Hello World!\n";
}

void say(const std::string& text) {
    std::cout << ">  User: " << text << '\n';
}

void say2(int a, const std::string& text) {
    std::cout << ">  User" << a << ": " << text << '\n';
}

void say3(int a, const std::string& text, int b) {
    std::cout << ">  User" << a << ": " << text << " num: " << b << '\n';
}

void say4(int a, const std::string& b, const std::string& c, int d) {
    std::cout << ">  User" << a << ": " << b << c << " num: " << d << '\n';
}

void sum(float a, float b, float c) {
    std::cout << "> Console: " << a << " + " << b << " + " << c << " = " << a + b + c << '\n';
}

void res(float a, float b) {
    std::cout << "> Console: " << a << " - " << b << " = " << a - b << '\n';
}

bool exitB = true;
void exit() {
    exitB = false;
}
//=================================================================

int main() {
    /*
     * add(arg1, arg2, arg3);
     * 
     * arg1 = Name of how you want to call the command.
     * arg2 = Address of the function you want to call.
     * arg3 = Function type, there are 3 types...
     *           0: For functions that dont have a string as an argument.
     *           1: For functions that only have one argument, and that is of type string.
     *           2: For functions that have more than one argument and at least one of them is of type string.
     */

    add("hello", hello, 0);
    add("say", say, 1);
    add("say2", say2, 2);
    add("say3", say3, 2);
    add("say4", say4, 2);
    add("sum", sum, 0);
    add("res", res, 0);
    add("exit", exit, 0);

    std::string text;
    while (exitB) {
        std::getline(std::cin, text);
        sendCmd(text);
    }

    return 0;
}