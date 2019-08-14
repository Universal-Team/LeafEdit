#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <3ds.h>
#include <memory>

class SCREEN
{
public:
    virtual ~SCREEN() {}
    virtual void Logic(u32 hDown, u32 hHeld, touchPosition touch) = 0;
    virtual void Draw() const = 0;
private:
};

#endif