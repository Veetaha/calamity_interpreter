#define _BSD_SOURCE
#define _POSIX_SOURCE

#include <cui.h>
#include <zconf.h>
#include "error.h"
#include <limits>

using namespace std;


namespace Cui {

    Foreground::Foreground(const int & red, const int & green, const int & blue)
        : m_red(static_cast<unsigned char>(red)),
          m_green(static_cast<unsigned char>(green)),
          m_blue(static_cast<unsigned char>(blue)) {}

    Background::Background(const int & red, const int & green, const int & blue)
        : m_red(static_cast<unsigned char>(red)),
          m_green(static_cast<unsigned char>(green)),
          m_blue(static_cast<unsigned char>(blue)) {}

    std::ostream &operator<<(std::ostream &stream, const Cui::Foreground &object) {
        return stream << "\033[38;5;"
                      << (16 + 36 * object.m_red + 6 * object.m_green + object.m_blue)
                      << "m";
    }

    std::wostream &operator<<(std::wostream &stream, const Cui::Foreground &object) {
        return stream << L"\033[38;5;"
                      << (16 + 36 * object.m_red + 6 * object.m_green + object.m_blue)
                      << L"m";
    }


    std::wostream &operator<<(std::wostream &stream, const Cui::Background &object) {
        return stream << L"\033[48;5;"
                      << (16 + 36 * object.m_red + 6 * object.m_green + object.m_blue)
                      << L"m";
    }


    std::ostream &operator<<(std::ostream &stream, const Cui::Background &object) {
        return stream << "\033[48;5;"
                      << (16 + 36 * object.m_red + 6 * object.m_green + object.m_blue)
                      << "m";
    }


    std::ostream & bold(std::ostream & stream) {
        return stream << "\033[1m";
    }

    std::ostream & bold_off(std::ostream & stream) {
        return stream << "\033[22m";
    }

	std::ostream & reset(std::ostream &stream) {
	    return stream << "\033[m";
	}

	std::ostream & reverse(std::ostream &stream){
	    return stream <<"\033[7m";
	}

	std::ostream & reverse_off(std::ostream & stream){
	    return stream <<"\033[27m";
	}

    std::ostream & newline(std::ostream & stream) {
        return stream << '\n';
    }


	std::wostream & bold(std::wostream & stream) {
		return stream << L"\033[1m";
	}

	std::wostream & bold_off(std::wostream & stream) {
		return stream << L"\033[22m";
	}

	std::wostream & reset(std::wostream &stream) {
		return stream << L"\033[m";
	}

	std::wostream & reverse(std::wostream &stream){
		return stream << L"\033[7m";
	}

	std::wostream & reverse_off(std::wostream & stream){
		return stream << L"\033[27m";
	}

    std::wostream & newline(std::wostream & stream) {
        return stream << L'\n';
    }


	std::ostream &  move(const unsigned short & y, const unsigned short & x){
	    return cout << "\033[" << y << ';' << x << 'H'; // Thanks RA for conPos, really appreciate
	}

	void printHLine(const unsigned short & size, const char & token){
	    for (unsigned short i = 0; i < size; ++i){
	        cout << token;
	    }
	}

	void printVLine(const unsigned short & size, const char & token, Point2D<unsigned int> loc){
	    for (unsigned short i = 0; i < size; ++i){
	        cout << token;
	        ++loc.y;
	        Cui::move(loc.y, loc.x);
	    }
	}

	// thanks Ruslan Hadyniak for conGetPos (from C):
	Point2D<unsigned int> cursorPoint2D(void) {
	    Point2D<unsigned int> pos = Point2D<unsigned int>(0, 0);
	    struct termios save, raw;
	    fflush(stdout);
	    tcgetattr(0, &save);
	    cfmakeraw(&raw);
	    tcsetattr(0, TCSANOW, &raw);
	    if (isatty(fileno(stdin))) {
	        char cmd[] = "\033[6n";
	        char buf[8] = {0};
	        write(1, cmd, sizeof(cmd));
	        read(0, buf, sizeof(buf));
	        sscanf(buf, "\033[%u;%u", &(pos.y), &(pos.x));
	    }
	    tcsetattr(0, TCSANOW, &save);
	    fflush(stdout);
	    return pos;
	}

	std::ostream & hideCursor(){
	    return cout << "\033[?25l";
	}
	std::ostream & showCursor(){
	    return cout << "\033[?25h";
	}
    Cin_ext::Cin_ext() {
        tcgetattr(fileno(stdin), &this->vanilla);
    }

    Cin_ext::~Cin_ext() {
        reset();
    }

	// adapted from https://stackoverflow.com/a/42775495/9259330
    void Cin_ext::allchars() {
        struct termios t = vanilla;
        t.c_lflag &= (~ICANON & ~ECHO);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
            perror("Unable to set terminal to single character mode");
        }
    }

    void Cin_ext::reset() {
        if (tcsetattr(fileno(stdin), TCSANOW, &vanilla) < 0)
            perror("Unable to restore terminal mode");
    }

    // adapted from RA conLockInput
    void Cin_ext::noecho() {
        struct termios t = vanilla;
        t.c_lflag &= !(ECHO | ICANON | ECHOE | ECHOK | ECHONL | ICRNL);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &t) < 0){
            perror("Unable to set terminal to no echoing mode");
        }
    }

    void nap(){
        Cin_ext termio = Cin_ext();
        termio.noecho();
        termio.allchars();
        Cui::hideCursor();
        cin >> noskipws;
        char ch;
        cin >> ch;
        showCursor();
        cin >> skipws;
    }

	std::istream & flush(std::istream &stream) {
		stream.clear();
		stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return stream;
	}

	void recover() {
        cout << reset << std::flush;
		system("clear");
	}

    void wait(const char * const & message) {
		cout << reverse << message << reverse_off;
		nap();
	}


}


void foo(){
    std::cout << Cui::fgnd(2,4,5);
}
