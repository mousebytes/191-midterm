#include "_camera.h"

_camera::_camera()
{
    //ctor
}

_camera::~_camera()
{
    //dtor
}
void _camera::camInit()
{
    eye.x =0; eye.y =0; eye.z =10;
    des.x =0; des.y =0; des.z =0;
    up.x  =0; up.y  =1; up.z  =0;

    // since i'm using delta time now
    // this is how far to move per second
    step =30;

    distance = sqrt(pow(eye.x-des.x,2)+pow(eye.y-des.y,2)+pow(eye.z-des.z,2));

    rotAngle.x =0;
    rotAngle.y =0;
    mouseSensitivity = 0.1f;
}

void _camera::camReset()
{
    eye.x =0; eye.y =0; eye.z =10;
    des.x =0; des.y =0; des.z =0;
    up.x  =0; up.y  =1; up.z  =0;

    step =30;

    distance = sqrt(pow(eye.x-des.x,2)+pow(eye.y-des.y,2)+pow(eye.z-des.z,2));

    rotAngle.x =0;
    rotAngle.y =0;
    mouseSensitivity = 0.1f;
}

void _camera::rotateXY()
{
    eye.x = des.x + distance*cos(rotAngle.y*PI/180.0)*sin(rotAngle.x*PI/180.0);
    eye.y = des.y + distance*sin(rotAngle.y*PI/180.0);
    eye.z = des.z + distance*cos(rotAngle.y*PI/180.0)*cos(rotAngle.x*PI/180.0);
}

void _camera::rotateUP()
{

}

void _camera::camMoveFdBd(int dir)
{
       float moveStep = step * _Time::deltaTime;
       
       eye.z += moveStep*dir; // if forward dir =1 else dir =-1
       des.z += moveStep*dir;
}

void _camera::camMoveLtRt(int dir)
{
    float moveStep = step * _Time::deltaTime;

    eye.x += moveStep*dir;
    des.x += moveStep*dir;
}

void _camera::setUpCamera()
{
    gluLookAt(eye.x,eye.y,eye.z,
              des.x,des.y,des.z,
              up.x, up.y, up.z);
}

void _camera::handleMouse(HWND hWnd, int mouseX, int mouseY, int centerX, int centerY){
    // if the mouse is at the center it's because we just set it
    // in the last frame, ignore this "fake" event
    if (mouseX == centerX && mouseY == centerY) {
        return;
    }

    // calc delta from the center
    float deltaX = (float)(mouseX - centerX);
    float deltaY = (float)(mouseY - centerY);

    // apply to camera rotation
    rotAngle.x += deltaX * mouseSensitivity;
    
    // invert y axis (screen y is down, camera up is up)
    rotAngle.y -= deltaY * mouseSensitivity;

    // clamp vertical rotation
    // we use 89.0 to avoid gimbal lock at 90.0
    if(rotAngle.y > 89.0f) {
        rotAngle.y = 89.0f;
    }
    if(rotAngle.y < -89.0f) {
        rotAngle.y = -89.0f;
    }

    // update the camera's internal state
    rotateXY();

    // reset mouse to center
    // convert the window's center to the screen's coordinate system
    POINT centerPoint = { centerX, centerY };
    ClientToScreen(hWnd, &centerPoint);
    SetCursorPos(centerPoint.x, centerPoint.y);
}