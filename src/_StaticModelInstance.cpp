#include "_StaticModelInstance.h"

_StaticModelInstance::_StaticModelInstance(_StaticModel* modelAsset) {
    blueprint = modelAsset;
    pos.x = 0.0;
    pos.y = 0.0;
    pos.z = -10.0;
    rotation = Vector3();
    isPushable = false;
}

_StaticModelInstance::~_StaticModelInstance() {
    // dont delete blueprint here bc it's shared by other instances
}

void _StaticModelInstance::Draw() {
    glPushMatrix();
        // apply this instance's transform
        glTranslatef(pos.x, pos.y, pos.z);

        glRotatef(rotation.x, 1.0, 0.0, 0.0);
        glRotatef(rotation.y, 0.0, 1.0, 0.0);
        glRotatef(rotation.z, 0.0, 0.0, 1.0);

        glScalef(scale.x,scale.y,scale.z);


        // Call the blueprint's draw function
        blueprint->Draw();
    glPopMatrix();
}

// can this thing move
void _StaticModelInstance::SetPushable(bool flag){
    isPushable = flag;
}

// can this thing rotate
void _StaticModelInstance::SetRotatable(bool flag){
    isRotatable=flag;
}

// move this object incrementally by a set of (x,y,z) coords
void _StaticModelInstance::Push(float x, float y, float z){
    if(!isPushable) return;
    // move (x,y,z) units per second
    pos.x += x * _Time::deltaTime;
    pos.y += y * _Time::deltaTime;
    pos.z += z * _Time::deltaTime;
}


// rotates the object incrementally by a set of (x,y,z) angles
void _StaticModelInstance::Rotate(float x, float y, float z) {
    if(!isRotatable) return;
    // rotate (x,y,z) units per second 
    rotation.x += x * _Time::deltaTime;
    rotation.y += y * _Time::deltaTime;
    rotation.z += z * _Time::deltaTime;
}

// moves the object's rotation towards a target (x,y,z) smoothly
// \*deprecated*/
//someone else can take this over if they want
void _StaticModelInstance::RotateSmoothly(float x, float y, float z, float interp) {
    // This uses lerp to move the current
    // rotation closer to the target rotation (x,y,z)
    // 'interp' is the interpolation factor (0.1=10%)

    // a = a + (b - a) * t
    rotation.x = rotation.x + (x - rotation.x) * interp;
    rotation.y = rotation.y + (y - rotation.y) * interp;
    rotation.z = rotation.z + (z - rotation.z) * interp;
}