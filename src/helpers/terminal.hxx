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
    ERROR = 0,
    SUCCESS,
    INFO,
    INFO2
};
// ./pkgx -rc ../payload/test/test.pkgx conflicts

// --------------------*  stdout macros  *--------------------
template<typename ...Argument>
static inline void LOG(int type, Argument&& ...args)
{
    switch (type)
    {
        case ERROR:
            std::cout << COLOR_RED << "[pkgx][✘]: ";
            break;
        case SUCCESS:
            std::cout << COLOR_GREEN << "[pkgx][✔︎]: ";
            break;
        case INFO:
            std::cout << COLOR_BLUE << "[pkgx][•]: ";
            break;
        case INFO2:
            std::cout << COLOR_CYAN << "[pkgx][•]: ";
            break;
        default:
            break;
    }
    (std::cout << ... << args);
    std::cout << COLOR_RESET << "\n";
}


// --------------------*  terminal helper funcs  *--------------------
static inline void wipe_prompt() { std::cout << CLEAR_SEQUENCE; }

static inline void print_help()
{
    std::cout << ITALIC << "The extended package (pkgx) utility" << TEXTSTYLE_RESET << "\n";
    std::cout << "Usage:  pkgx <command> <...>" << "\n";
    std::cout << COLOR_MAGENTA << "↳ Building" << COLOR_RESET << "\n";
    std::cout << COLOR_CYAN << "    ↳ -b <src> <dest>" << COLOR_RESET << "    Build pkgx from src and output as dest." << "\n";
    std::cout << COLOR_MAGENTA << "↳ Extraction" << COLOR_RESET << "\n";
    std::cout << COLOR_CYAN << "    ↳ -x <src> <dest>" << COLOR_RESET << "    Extract pkgx from src to dest." << "\n";
    std::cout << COLOR_CYAN << "    ↳ -xc <src> <dest>" << COLOR_RESET << "   Extract control file from the pkgx from src to dest." << "\n";
    std::cout << COLOR_CYAN << "    ↳ -xl <src> <dest>" << COLOR_RESET << "   Extract layout file from the pkgx from src to dest." << "\n";
    std::cout << COLOR_CYAN << "    ↳ -xdr <src> <dest>" << COLOR_RESET << "  Extract data (raw) from the pkgx from src to dest." << "\n";
    std::cout << COLOR_CYAN << "    ↳ -xdf <src> <dest>" << COLOR_RESET << "  Extract data (full) from the pkgx from src to dest." << "\n";
    std::cout << COLOR_MAGENTA << "↳ Information" << COLOR_RESET << "\n";
    std::cout << COLOR_CYAN << "    ↳ -rc <src> <field>" << COLOR_RESET << "  Read field from the control file if existent." << "\n";
    std::cout << COLOR_CYAN << "    ↳ -rl <src> <num>" << COLOR_RESET << "    Read element details from the layout file if existent." << "\n";
    std::cout << "\nThe sauce can be found here:\nhttps://github.com/mass1ve-err0r/pkgx\n";
}


#endif //PKGX_TERMINAL_HXX