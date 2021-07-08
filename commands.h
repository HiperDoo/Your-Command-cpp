#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <type_traits>
#include <tuple>
#include <map>

#include "convert.h"

//============================== GLOBAL VARIABLES ==============================
struct Funcs {
    std::function<void(const std::vector<std::string>&)> A;
    std::size_t B; //Number of args of the function
    int C; //Type of function
};
std::map<std::string, struct Funcs> commandMap;
bool tupleB = true;
//==============================================================================


//================================ APPLY TUPPLE ================================
template <std::size_t N>
struct Apply_Help;

template<typename Func, typename... Args>
void apply(const Func& func, const std::tuple<Args...>& args) {
    Apply_Help<std::tuple_size<std::tuple<Func, Args...>>::value - 1 >::apply(func, args);
}

template <std::size_t N>
struct Apply_Help {
    template <typename Func, typename Tuple, typename... Args>
    static void apply(const Func& func, const Tuple& tuple, const Args&... args) {
        Apply_Help<N - 1>::apply(func, tuple, std::get<N - 1>(tuple), args...);
    }
};

template <>
struct Apply_Help<0> {
    template <typename Func, typename Tuple, typename... Args>
    static void apply(const Func& func, const Tuple& tuple, const Args&... args) {
        (void)tuple;

        func(args...);
    }
};
//==============================================================================


//============================== CONVERT TO TUPLE ==============================
bool isNumber(std::string& str) {
    int char_pos = 0;

    //Saltamos la posicion si hay un signo de + o -
    if (str[char_pos] == '+' || str[char_pos] == '-') {
        ++char_pos;
        if (str[char_pos] == '+' || str[char_pos] == '-') { //En caso de que se repita el signo
            return false;
        }
    }

    int n_nm, n_pt;
    for (n_nm = 0, n_pt = 0; std::isdigit(str[char_pos]) || str[char_pos] == '.'; ++char_pos) {
        str[char_pos] == '.' ? ++n_pt : ++n_nm;
    }
    if (n_pt > 1 || n_nm < 1) //Verificar que no haya mas de un punto
        return false;

    return char_pos == str.size();
}

template<typename T, typename U, typename... Args>
std::tuple<T, U, Args...> a2t(const std::vector<std::string>& a, int pos);
template<typename T>
std::tuple<T> a2t(const std::vector<std::string>& a, int pos);

template<typename... Ts>
void order(const std::function<void(Ts...)>& func, const std::vector<std::string>& cmdArgs) {
    std::tuple<typename std::remove_reference<Ts>::type...> args =
        a2t<typename std::remove_reference<Ts>::type...>(cmdArgs, 0);

    if (tupleB) {
        apply(func, args);
    } else {
        tupleB = true;
        std::cout << "ERROR::Se ingreso un parametro de tipo no numerico!\n";
    }
}

template<>
void order<>(const std::function<void()>& func, const std::vector<std::string>& cmdArgs) {
    (void)cmdArgs;

    func();
}

template<typename T, typename U, typename... Args>
std::tuple<T, U, Args...> a2t(const std::vector<std::string>& a, int pos) {
    std::string temp = a[pos];
    pos++;
    if (std::is_same_v<T, unsigned int> || std::is_same_v<T, int> || std::is_same_v<T, unsigned long int> ||
        std::is_same_v<T, long int> || std::is_same_v<T, unsigned long long int> || std::is_same_v<T, long long int> ||
        std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>) {
        if (isNumber(temp)) {
            return std::tuple_cat(
                std::make_tuple(ArgConvert<std::string, T>{}(temp)),
                a2t<U, Args...>(a, pos)
            );
        } else {
            tupleB = false;
            return std::tuple_cat(
                std::make_tuple(ArgConvert<std::string, T>{}("0")),
                a2t<U, Args...>(a, pos)
            );
        }
    }
    return std::tuple_cat(
        std::make_tuple(ArgConvert<std::string, T>{}(temp)),
        a2t<U, Args...>(a, pos)
    );
}

template<typename T>
std::tuple<T> a2t(const std::vector<std::string>& a, int pos) {
    std::string temp = a[pos];
    if (std::is_same_v<T, unsigned int> || std::is_same_v<T, int> || std::is_same_v<T, unsigned long int> ||
        std::is_same_v<T, long int> || std::is_same_v<T, unsigned long long int> || std::is_same_v<T, long long int> ||
        std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>) {
        if (isNumber(temp)) {
            return std::make_tuple(ArgConvert<std::string, T>{}(temp));
        } else {
            tupleB = false;
            return std::make_tuple(ArgConvert<std::string, T>{}("0"));
        }
    } else {
        return std::make_tuple(ArgConvert<std::string, T>{}(temp));
    }
}
//===========================================================================


//============================== ADD FUNCTIONS ==============================
template <typename T, typename... Args>
constexpr size_t getCount(T(*f)(Args...)) {
    return sizeof...(Args);
}

template<typename Func, typename FuncParser>
void add_next(const std::string& cmdName, const Func& func, const FuncParser& funcParser, int args) {
    commandMap[cmdName] = {
        std::bind(funcParser, func, std::placeholders::_1),
        getCount(func),
        args
    };
}

template<typename... FuncArgs>
void add(const std::string& cmdName, void(*func)(FuncArgs...), int args) {
    add_next(cmdName, func, order<FuncArgs...>, args);
}
//=======================================================================

void case0(const std::string& cmdName, const std::string& text, std::vector<std::string>& vecS, std::string& temp) {
    std::size_t args_pos = std::min(cmdName.size() + 1, text.length());

    if (commandMap[cmdName].B == 0) {
        commandMap[cmdName].A(vecS);
        return;
    }

    int j = 0;

    while (j < text.length()) {
        if (text[args_pos] == ' ') {
            std::cout << "ERROR\n";
            return;
        }
        j = args_pos;
        while (!std::isspace(text[j])) {
            if (j < text.length()) {
                j++;
            } else {
                break;
            }
        }
        temp = text.substr(args_pos, j - args_pos);
        vecS.push_back(temp);
        j++;
        args_pos = j;
    }

    if (vecS.size() != commandMap[cmdName].B) {
        std::cout << "ERROR::Se ingresaron " << vecS.size() << " argumentos de " << commandMap[cmdName].B << "!\n";
        return;
    }

    commandMap[cmdName].A(vecS);
}

void case1(const std::string& cmdName, const std::string& text, std::vector<std::string>& vecS) {
    std::size_t args_pos = std::min(cmdName.size() + 1, text.length());
    std::size_t args_end = text.find_last_not_of(" \t\r\n");
    std::size_t args_len = std::max(args_pos, args_end) - args_pos + 1;

    vecS.push_back((cmdName.size() > 0) ? text.substr(args_pos, args_len) : "");

    commandMap[cmdName].A(vecS);
}

void case2(const std::string& cmdName, const std::string& text, std::vector<std::string>& vecS, std::string& temp) {
    std::size_t args_pos = std::min(cmdName.size() + 1, text.length());
    int j = 0;

    while (j < text.length()) {
        if (text[args_pos] == ' ') {
            std::cout << "ERROR\n";
            return;
        }
        j = args_pos;
        if (text[args_pos] == '[') {
            while (text[j] != ']') {
                if (j < text.length()) {
                    j++;
                } else {
                    std::cout << "ERROR::La cadena de texto debe de terminar con un ']'!\n";
                    return;
                }
            }
            j++;
            temp = text.substr(args_pos, j - args_pos);
            vecS.push_back(temp);
        } else {
            while (j < text.length() && !std::isspace(text[j])) {
                j++;
            }
            temp = text.substr(args_pos, j - args_pos);
            vecS.push_back(temp);
        }
        j++;
        args_pos = j;
    }

    if (vecS.size() != commandMap[cmdName].B) {
        std::cout << "ERROR::Se ingresaron " << vecS.size() << " argumentos de " << commandMap[cmdName].B << "!\n";
        return;
    }

    commandMap[cmdName].A(vecS);
}


//============================== CMD ==============================
void sendCmd(std::string& text) {
    std::string cmdName, temp;
    std::istringstream stream(text);
    std::vector<std::string> vecS;
    stream >> cmdName;

    if (!commandMap.count(cmdName) > 0 || text.length() > 32) {
        std::cout << "ERROR::El comando ingresado no existe o es demaciado largo!\n";
        return;
    }

    switch (commandMap[cmdName].C) {
        case 0:
            case0(cmdName, text, vecS, temp);
            break;

        case 1:
            case1(cmdName, text, vecS);
            break;

        case 2:
            case2(cmdName, text, vecS, temp);
            break;
    }
}
//=================================================================