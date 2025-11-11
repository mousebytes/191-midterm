#include"_Bullets.h"

//a helper struct to manage bullet's state
// managed by _Bullets

struct _Bullet{
    _StaticModelInstance* instance;
    Vector3 velocity;
    // time in seconds before bullet is destroyed
    float lifetime;

    // creates a new bullet with blueprint, world space pos
    // velocity (dir*speed) and how long the bullet should live
    _Bullet(_StaticModel* blueprint, Vector3 pos, Vector3 vel, float life, float yaw, float pitch){
        instance = new _StaticModelInstance(blueprint);
        instance->pos = pos;
        velocity=vel;
        lifetime=life;

        instance->rotation.x = pitch;
        instance->rotation.y = yaw;
        instance->rotation.z = 0.0f; // No roll

        // DEBUG change scale of bullets here
        instance->scale = Vector3(0.2,0.2,0.2);
    }
    
    ~_Bullet(){
        delete instance;
    }

    // update bullets pos & lifetime, true if expired false otherwise
    bool Update(){
        // apply velocity & no gravity
        instance->pos.x += velocity.x * _Time::deltaTime;
        instance->pos.y += velocity.y * _Time::deltaTime;
        instance->pos.z += velocity.z * _Time::deltaTime;

        // countdown lifetime
        lifetime-=_Time::deltaTime;
        return lifetime<=0.0f;
    }

    // draw bullets model instance
    void Draw(){
        instance->Draw();
    }
};


_Bullets::_Bullets(_StaticModel* bulletBlueprint){
    m_blueprint = bulletBlueprint;
    // units per second
    m_bulletSpeed=150.0f;
    // lifetime in seconds
    m_bulletLifetime=3.0f;
}

_Bullets::~_Bullets(){
    for(_Bullet* bullet : m_activeBullets){
        delete bullet;
    }
    m_activeBullets.clear();
}

void _Bullets::Fire(Vector3 startPos, Vector3 direction){
    // calc vel
    direction.normalize();
    Vector3 velocity = direction*m_bulletSpeed;

    const float RAD_TO_DEG = 180.0f / PI;

    // Calculate Yaw (rotation around the Y-axis)
    // atan2 is the best function for this. It gives the angle in radians
    // on the XZ plane, correctly handling all quadrants.
    float yaw = atan2(direction.x, direction.z) * RAD_TO_DEG;

    // Calculate Pitch (rotation around the X-axis)
    // We use atan2 again, comparing 'y' (height) to the
    // length of the vector on the XZ plane.
    float xzLength = sqrt(direction.x * direction.x + direction.z * direction.z);
    float pitch = atan2(direction.y, xzLength) * RAD_TO_DEG;

    std::cout << "--- FIRING BULLET ---" << std::endl;
    std::cout << "Direction Vector: (x: " << direction.x << ", y: " << direction.y << ", z: " << direction.z << ")" << std::endl;
    std::cout << "Calculated Yaw (Y-rot): " << yaw << " degrees" << std::endl;
    std::cout << "Calculated Pitch (X-rot): " << pitch << " degrees" << std::endl;
    std::cout << "---------------------" << std::endl;

    // create & add the new bullet to our active list
    _Bullet* newBullet = new _Bullet(m_blueprint,startPos,velocity,m_bulletLifetime,yaw,pitch);
    m_activeBullets.push_back(newBullet);
}

void _Bullets::Update(){
    // iterate & update all bullets
    for(auto it = m_activeBullets.begin(); it!=m_activeBullets.end();){
        _Bullet* bullet = *it;

        // check life & update pos
        bool isDead = bullet->Update();

        if(isDead){
            // delete bullet obj & its instance
            delete bullet;
            it=m_activeBullets.erase(it);
        }else{
            // next item
            ++it;
        }
    }
}

void _Bullets::Draw(){
    for(_Bullet* bullet : m_activeBullets){
        bullet->Draw();
    }
}