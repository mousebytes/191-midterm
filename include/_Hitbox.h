#ifndef _HITBOX_H
#define _HITBOX_H

#include "_common.h" 

class _Hitbox {
public:
    // The two corners that define the box
    Vector3 min;
    Vector3 max;

    _Hitbox();
    _Hitbox(Vector3 vMin, Vector3 vMax);
    
    // The function that draws the box as lines
    void Draw();

    

};

#endif // _HITBOX_H