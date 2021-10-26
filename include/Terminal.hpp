#ifndef Terminal_HPP
#define Terminal_HPP

#include "ITarget.hpp"

#include <ncurses.h>

class Terminal : public ITarget
{
    const int Width, Height;
    uint8_t *Buffer;
    WINDOW *MainWindow;

public:
    Terminal(int frameWidth, int frameHeight);
    virtual ~Terminal();

    virtual uint8_t* GetBuffer() const override;
    virtual int GetWidth() const override;
    virtual int GetHeight() const override;
    virtual void UploadFrame() override;
};

#endif // Terminal_HPP
