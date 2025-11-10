#include"_AnimatedModel.h"

_AnimatedModel::_AnimatedModel(){
    modelTexID=0;
}

_AnimatedModel::~_AnimatedModel(){
    FreeModel();
}

int _AnimatedModel::GetFrameCount(){
    return m_Frames.size();
}

bool _AnimatedModel::LoadAnimation(const char* baseName, int frameCount, char* texpath){
    for(int i =0;i<frameCount;++i){
        // build filename (models/sktaer/push_02.obj)
        std::stringstream ss;
        ss << baseName << "_";
        if(i<10) ss << "0"; // add leading 0
        ss << i << ".obj";

        std::string filename = ss.str();

        // create ObjModel & load
        ObjModel* newFrame = new ObjModel();
        newFrame->init(filename);

        if(newFrame->vertices.empty()){
            std::cerr << "Error: could not load: " << filename << '\n';
            delete newFrame;
            return false;
        }
        // add loaded frame to the flipbook
        m_Frames.push_back(newFrame);
    }

    // initialize m_ScratchFrame to match the structure of our frames
    // so we don't reallocate memory every frame
    if (!m_Frames.empty()) {
        ObjModel* firstFrame = m_Frames[0];
        m_ScratchFrame.faces = firstFrame->faces;
        m_ScratchFrame.tex_coords = firstFrame->tex_coords;
        m_ScratchFrame.normals.resize(firstFrame->normals.size()); // preallocate normal vector
        m_ScratchFrame.vertices.resize(firstFrame->vertices.size()); // preaallocate vertex vector
    }

    // load tex
    // assume all frames share a texture
    // ex ("models/skater/push.jpg")
    std::cout << "Loading texture: " << texpath << '\n';
    myTex.loadTexture(texpath);
    modelTexID = myTex.textID;

    return true;
}

void _AnimatedModel::FreeModel(){
    // delete all ObjModel we've created
    for(ObjModel* frame : m_Frames){
        delete frame;
    }
    // clean vec
    m_Frames.clear();

    // clean tex
    glDeleteTextures(1,&modelTexID);
}

void _AnimatedModel::Draw(int frameA, int frameB, float interp){
    if(m_Frames.empty() || frameA >= m_Frames.size() || frameB >= m_Frames.size()){
        return;
    }

    //get two pages of the flipbook we're gonna blend
    ObjModel* modelA = m_Frames[frameA];
    ObjModel* modelB = m_Frames[frameB];

    // check that they match
    // vert count & face data has to be the same
    if(modelA->vertices.size() != modelB->vertices.size() || 
       modelA->normals.size() != modelB->normals.size())
    {
        std::cerr<<"Error: Animation frames do not match\n";
        return;
    }

    // calc the final vertex positions and normals,
    // and store them in m_ScratchFrame.
    
    for (size_t i = 0; i < modelA->vertices.size(); ++i) {
        // Get vert pos from both frames
        Vector3& posA = modelA->vertices[i];
        Vector3& posB = modelB->vertices[i];

        // lerp pos
        m_ScratchFrame.vertices[i].x = posA.x + (posB.x - posA.x) * interp;
        m_ScratchFrame.vertices[i].y = posA.y + (posB.y - posA.y) * interp;
        m_ScratchFrame.vertices[i].z = posA.z + (posB.z - posA.z) * interp;
    }
    
    // Also lerp normals for smooth lighting (as suggested)
    for (size_t i = 0; i < modelA->normals.size(); ++i) {
        Vector3 &normA = modelA->normals[i];
        Vector3& normB = modelB->normals[i];

        m_ScratchFrame.normals[i].x = normA.x + (normB.x - normA.x) * interp;
        m_ScratchFrame.normals[i].y = normA.y + (normB.y - normA.y) * interp;
        m_ScratchFrame.normals[i].z = normA.z + (normB.z - normA.z) * interp;
    }


    // make sure to bind tex
    glBindTexture(GL_TEXTURE_2D, modelTexID);

    // m_ScratchFrame contains the final interpolated geometry
    // (and its .faces and .tex_coords were already copied)
    m_ScratchFrame.Draw();
}