#include "_Hitbox.h"
bool isDebug=true;
_Hitbox::_Hitbox() {
    // Default 1x1x1 cube
    min = Vector3(-0.5f, -0.5f, -0.5f);
    max = Vector3(0.5f, 0.5f, 0.5f);
}

_Hitbox::_Hitbox(Vector3 vMin, Vector3 vMax) {
    min = vMin;
    max = vMax;
}

void _Hitbox::Draw() {

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 0.0f, 0.0f);
    
    // Draw the 12 edges of the box
    
    // Bottom face (4 lines)
    glBegin(GL_LINE_LOOP);
        glVertex3f(min.x, min.y, min.z);
        glVertex3f(max.x, min.y, min.z);
        glVertex3f(max.x, min.y, max.z);
        glVertex3f(min.x, min.y, max.z);
    glEnd();

    // Top face (4 lines)
    glBegin(GL_LINE_LOOP);
        glVertex3f(min.x, max.y, min.z);
        glVertex3f(max.x, max.y, min.z);
        glVertex3f(max.x, max.y, max.z);
        glVertex3f(min.x, max.y, max.z);
    glEnd();

    // Connecting vertical lines 
    glBegin(GL_LINES);
        glVertex3f(min.x, min.y, min.z);
        glVertex3f(min.x, max.y, min.z);

        glVertex3f(max.x, min.y, min.z);
        glVertex3f(max.x, max.y, min.z);

        glVertex3f(max.x, min.y, max.z);
        glVertex3f(max.x, max.y, max.z);

        glVertex3f(min.x, min.y, max.z);
        glVertex3f(min.x, max.y, max.z);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}