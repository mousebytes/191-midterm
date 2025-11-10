#include"_ModelInstance.h"



_ModelInstance::_ModelInstance(_AnimatedModel* modelAsset){
    blueprint = modelAsset;

    pos.x=pos.y=0;
    pos.z=-10.0;
    rotation = Vector3();

    currentFrame=0;
    interp=0.0;
    startFrame=0;
    // set default to full animation
    endFrame=blueprint->GetFrameCount()-1;

    // start with no velocity
    velocity = Vector3(0,0,0);
    // start in the air
    isGrounded=false;

}

_ModelInstance::~_ModelInstance(){
    for(_Collider* collider : colliders){
        delete collider;
    }
    colliders.clear();
}

void _ModelInstance::SetAnimation(int start, int end){
    startFrame = start;
    endFrame = end;
    // reset to beginning of new animation
    currentFrame=start;
    interp=0.0f;
}

void _ModelInstance::Update(){
    const float gravity = -9.8f;

    if(!isGrounded){
        velocity.y+=gravity*_Time::deltaTime;
    }

    pos.x+=velocity.x*_Time::deltaTime;
    pos.y+=velocity.y*_Time::deltaTime;
    pos.z+=velocity.z*_Time::deltaTime;

    Animate(startFrame,endFrame,&currentFrame,&interp);
}

void _ModelInstance::Draw(){
    glPushMatrix();

        glTranslatef(pos.x,pos.y,pos.z);
        //DEBUG ROT
        glRotatef(rotation.x,1.0f,0.0f,0.0f);
        glRotatef(rotation.y,0.0f,1.0f,0.0f);
        glRotatef(rotation.z,0.0f,0.0f,1.0f);

        // calc next frame -- handle wraparound
        int nextFrame = currentFrame+1;
        if(nextFrame>endFrame){
            nextFrame=startFrame; // loop back
        }

        blueprint->Draw(
            currentFrame,
            nextFrame,
            interp
        );

        DrawColliders();

    glPopMatrix();
}

void _ModelInstance::Animate(int start, int end, int* frame, float* interp){
    // animation speed (fps)
    float animSpeed = 10.0f;

    // +interp based on deltaTime
    *interp+=animSpeed*_Time::deltaTime;

    // check if interp is done
    if(*interp>=1.0f){
        // reset interp
        *interp = fmod(*interp, 1.0f);

        // next frame
        (*frame)++;

        // loop anim if passed end frame
        if(*frame>end){
            *frame=start;
        }
    }
}

void _ModelInstance::Actions()
{
    // Check the trigger and set the correct animation
    switch(actionTrigger)
    {
        case STAND:
            SetAnimation(0, 1); // "stand" animation frames
            break;
        case RUN:
            SetAnimation(40, 45); // "run" animation frames
            break;
        case ATTACK:
            SetAnimation(46, 53); // "attack" animation frames
            break;
        case JUMP:
            SetAnimation(66, 71); // "jump" animation frames
            break;
        // add other cases like WALKLEFT etc
    }
}

void _ModelInstance::AddCollider(_Collider* collider) {
    colliders.push_back(collider);
}

void _ModelInstance::DrawColliders() {
    if (!isDebug) return;

    // this is called inside Draw(), so the matrix
    // is already transformed by the instance's pos/rot
    for (_Collider* collider : colliders) {
        collider->Draw();
    }
}

bool _ModelInstance::CheckCollision(_StaticModelInstance* other) {
    // checks all of our colliders against all of the other's colliders

    for (_Collider* myModelCol : this->colliders) {
        // create a temporary world space collider for ourselves
        _Collider* myWorldCol = myModelCol->GetWorldSpaceCollider(this->pos, Vector3(1,1,1), this->rotation);

        for (_Collider* otherModelCol : other->colliders) {
            // create a temporary world space collider for them
            _Collider* otherWorldCol = otherModelCol->GetWorldSpaceCollider(other->pos, other->scale, other->rotation);

            // perform the collision check
            if (myWorldCol->CheckCollision(otherWorldCol)) {
                // delete temporary colliders
                delete myWorldCol;
                delete otherWorldCol;
                return true; // found a collision
            }

            delete otherWorldCol; // clean up inner
        }

        delete myWorldCol; // clean up outer
    }

    return false; // no collisions found
}