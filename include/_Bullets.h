#ifndef _BULLETS_H
#define _BULLETS_H

#include "_common.h"
#include "_StaticModelInstance.h"

// forward declare helper struct
struct _Bullet;

class _Bullets{
    public:
        _Bullets(_StaticModel* bulletBlueprint);
        ~_Bullets();

        // spawns a new bullet at pos
        // and the direction the bullet should travel
        void Fire(Vector3 startPos, Vector3 direction);
        // updates all active bullets (pos,lifetime)
        void Update();

        //draw all active bullets
        void Draw();

    private:
        _StaticModel* m_blueprint; // we don't own this _scene does
        list<_Bullet*> m_activeBullets;

        float m_bulletSpeed;
        float m_bulletLifetime;
};



#endif // _BULLETS_H