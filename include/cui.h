#pragma once

#include <point2d.h>
//#include <cstdio>
#include <cwchar>
#include <iostream>
#include <termios.h>



namespace Cui {

    class Foreground { // manipulator
    private:
        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;

    public:  // permitted value range is [0 ... 5]
        Foreground(const int & red, const int & green, const int & blue);


        friend std::ostream  &operator<<(std::ostream &  stream, const Foreground &obj);
        friend std::wostream &operator<<(std::wostream & stream, const Foreground &obj);
    };

    inline Foreground fgnd(const int & red, const int & green, const int & blue)
    { return Foreground(red, green, blue); }

    ///////////////////////////////////////////////////////////////
    class Background { // manipulator
    private: // permitted value range is [0 ... 5]
        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;

    public:
        Background(const int & red, const int & green, const int & blue);

        friend std::ostream  &operator<<(std::ostream  & stream, const Background &obj);
        friend std::wostream &operator<<(std::wostream & stream, const Background &obj);
    };

    inline Background bkgnd(const int & red, const int & green, const int & blue)
    { return Background(red, green, blue); }

    // no arguments ostream manipulators
    std::ostream & bold       (std::ostream & stream);
    std::ostream & bold_off   (std::ostream & stream);
    std::ostream & reset      (std::ostream & stream);
    std::ostream & reverse    (std::ostream & stream);
    std::ostream & reverse_off(std::ostream & stream);
    std::ostream & newline    (std::ostream & stream);

    std::wostream & bold       (std::wostream & stream);
    std::wostream & bold_off   (std::wostream & stream);
    std::wostream & reset      (std::wostream & stream);
    std::wostream & reverse    (std::wostream & stream);
    std::wostream & reverse_off(std::wostream & stream);
    std::wostream & newline    (std::wostream & stream);

    std::istream & flush(std::istream & stream);

    // returns cout (advances current caret location)
    std::ostream & move(const unsigned short & y, const unsigned short & x);

    template <typename TDist>
    inline std::ostream & move(const Point2D<TDist> & point) { return move(point.y, point.x); }

    void printHLine(const unsigned short & size, const char & token);

    void printVLine(const unsigned short & size, const char & token, Point2D<unsigned int> loc);

    Point2D<unsigned int> cursorPoint2D(void);

    std::ostream & hideCursor();
    std::ostream & showCursor();

    void nap(); // wait untill any key was pressed
    void wait(const char * const & message = "Press any key to continue");

    void recover(); // clear console window

    class Cin_ext {
    public:
        Cin_ext();
        ~Cin_ext();
        void allchars(); // read input char per char
        void noecho();
        void reset();

    private:
        struct termios vanilla;
    };


    template <typename T>
    class Reversed {
    private:
        const T & m_item;
    public:
        explicit Reversed(const T & item) : m_item(item) {}

        inline friend std::ostream &operator<<(std::ostream & stream, const Reversed &obj){
            return stream << reverse << obj.m_item << reverse_off;
        }

    };

    template <typename T>
    Reversed<T> reversed(const T & item) {
        return Reversed<T>(item);
    }


    template<typename TEntity>
    void require(std::istream & stream, TEntity &entity) {
        do {
            stream >> entity;
            if (stream.fail()){
                stream >> flush;
                continue;
                //
            } else { return; }

        } while (true);
    }

	template<typename TEntity, typename TFunct>
    void require(std::istream & stream, TEntity &entity, const TFunct &isValid) {
		do {
			stream >> entity;
			if (stream.fail() || !isValid(entity)){
                stream >> flush;
				continue;
				//
			} else { return; }

		} while (true);
	}

}
