#include "_scene.h"

_Scene::_Scene()
{
    //ctor
    terrainBlueprint = new _StaticModel();
    terrainInstance = new _StaticModelInstance(terrainBlueprint);
    m_inputs = new _inputs();
    m_camera = new _camera();
    m_playButton = new _Button();
}

_Scene::~_Scene()
{
    //dtor

    delete terrainBlueprint;
    delete terrainInstance;
    delete m_inputs;
    delete m_camera;
    delete m_playButton;
}

void _Scene::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;// keep track of the ratio
    glViewport(0,0,width,height); // adjust my viewport

    glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height= GetSystemMetrics(SM_CYSCREEN);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
}

void _Scene::initGL()
{
    // i know this is redundant since it's in main.cpp
    // but this allows me to access it much easier
    _Time::Init();
    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,0.0f); // black background color
    glClearDepth(2.0f);         //depth test for layers
    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // light properties
    // (x,y,z,w) w=1.0 for positional w=0.0 for directional
    GLfloat light_pos[] = {0.0f,5.0f,2.0f,1.0f};
    GLfloat light_diffuse[] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat light_ambient[] = {0.2f,0.2f,0.2f,1.0f};
    //GLfloat light_specular[] = {0.5f,0.5f,0.5f,1.0f};

    // apply properties to light 0
    glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    //glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);

    // use tex color for lighting
    // makes materials diffuse property track color from tex
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);


    // load terrain model
    terrainBlueprint->LoadModel("models/terrain.obj","models/Terrain_Tex.png");
    terrainInstance->pos = Vector3(0,-1,-5);
    terrainInstance->scale = Vector3(15,15,15);
    // DEBUG change later
    terrainInstance->SetPushable(true);
    terrainInstance->SetRotatable(true);
    // camera initialization
    m_camera->camInit();

    // button initialization
    m_playButton->Init("images/play-btn.png",1,1,0,0,-10,1,1);
}

void _Scene::drawScene()
{
    _Time::Update();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear bits in each itteration
    glLoadIdentity();             // calling identity matrix

    

    m_camera->setUpCamera();
    terrainInstance->Draw();
    m_playButton->Draw();
}


void _Scene::mouseMapping(int x, int y)
{
    GLint viewPort[4];
    GLdouble ModelViewM[16];
    GLdouble projectionM[16];
    GLfloat winX,winY,winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
    glGetIntegerv(GL_VIEWPORT,viewPort);

    winX =(GLfloat)x;
    winY = (GLfloat)y;

    glReadPixels(x,(int)winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    gluUnProject(winX,winY,winZ,ModelViewM,projectionM,viewPort,&msX,&msY,&msZ);
}




int _Scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
        m_inputs->wParam = wParam;
        m_inputs->keyPressed(terrainInstance);
        m_inputs->keyPressed(m_camera);
        break;

        case WM_KEYUP:

        break;

        case WM_LBUTTONDOWN:


             mouseMapping(LOWORD(lParam), HIWORD(lParam));

            break;

        case WM_RBUTTONDOWN:

            break;

         case WM_MBUTTONDOWN:


            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:

            break;

        case WM_MOUSEMOVE:
            m_camera->handleMouse(hWnd, LOWORD(lParam), HIWORD(lParam), width / 2, height / 2);
            break;
        case WM_MOUSEWHEEL:

            break;

        default:
            break;

    }
}
