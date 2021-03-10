/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_TERMINAL_HXX
#define PKGX_TERMINAL_HXX

// --------------------*  imports  *--------------------
#include <iostream>


// --------------------*  constants (colors)  *--------------------
#define CLEAR_SEQUENCE "\033[2J\033[1;1H"
#define ITALIC "\e[3m"
#define TEXTSTYLE_RESET "\e[0m"

#define COLOR_RESET  "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE  "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN "\x1B[36m"

enum LOG_TYPE {
    DEBUG,
    ERROR,
    SUCCESS,
    INFO
};


// --------------------*  stdout macros  *--------------------
template<typename ...Argument>
static inline void LOG(int type, Argument&& ...args)
{
    switch (type)
    {
        case DEBUG:
            std::cout << COLOR_YELLOW << "[pkgx][DEBUG]: ";
            break;
        case ERROR:
            std::cout << COLOR_RED << "[pkgx][✘]: ";
            break;
        case SUCCESS:
            std::cout << COLOR_GREEN << "[pkgx][✔︎]: ";
            break;
        case INFO:
            std::cout << COLOR_BLUE << "[pkgx][•]: ";
            break;
    }
    (std::cout << ... << args);
    std::cout << COLOR_RESET << "\n";
}


#endif //PKGX_TERMINAL_HXX