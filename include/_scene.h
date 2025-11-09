#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_light.h>
#include<_model.h>
#include<_inputs.h>
#include<_textureloader.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_sprite.h>
#include<_timer.h>
#include<_3dmodelloader.h>
#include<_camera.h>
#include<_bullets.h>
#include<_collisioncheck.h>
#include"_StaticModelInstance.h"
#include"_inputs.h"
#include"_Button.h"

class _Scene
{
    public:
        _Scene();           //constructor
        virtual ~_Scene();  //Destructor

        int clickCnt =0;

        void reSizeScene(int width, int height);  // resize window
        void initGL();                            // initialize GL graphics
        void drawScene();                         // render scene
        int winMsg(HWND,UINT,WPARAM,LPARAM);      // to get keyboard interrupts and pass it to inputs
        void mouseMapping(int,int);
        double msX,msY,msZ;

        int width, height;  // keep record of the screen size


        _StaticModel *terrainBlueprint;
        _StaticModelInstance *terrainInstance;

        _inputs *m_inputs;
        _camera *m_camera;
        _Button *m_playButton;

        enum SceneState {MainMenu, Help, Playing, Paused};
        SceneState m_sceneState = SceneState::MainMenu;
    protected:

    private:
};

#endif // _SCENE_H
