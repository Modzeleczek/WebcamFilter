#include "../include/Terminal.hpp"

#include <stdexcept>

Terminal::Terminal(int frameWidth, int frameHeight) :
    Width(frameWidth),
    Height(frameHeight),
    Buffer(new unsigned char[frameWidth * frameHeight]),
    MainWindow(initscr())
{
    if (MainWindow == NULL)
        throw std::runtime_error("Terminal::Terminal; Error initializing ncurses.");
    noecho();
}

Terminal::~Terminal()
{
    delwin(MainWindow);
    endwin();
    delete[] Buffer;
}

uint8_t* Terminal::GetBuffer() const
{
    return Buffer;
}

int Terminal::GetWidth() const
{
    return Width;
}

int Terminal::GetHeight() const
{
    return Height;
}

void Terminal::UploadFrame()
{
    move(0, 0);
    addstr((const char*)Buffer);
    refresh();
}
