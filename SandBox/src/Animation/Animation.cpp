#include "Animation.h"
AnimatedMesh::AnimatedMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(NUM_BUFFERS, m_Buffers);
}
//Animations:
//0,1,2 Atack
//3 death
//4,5,6 get hit
//7 idle
//8 Jump
//9,11 run
//10 walk?
int animation = 11;

AnimatedMesh::~AnimatedMesh()
{
    Clear();
}

void AnimatedMesh::setAnimation(int nAnimation) 
{
    animation = nAnimation;
}

int AnimatedMesh::getAnimation()
{
    return animation;
}

void AnimatedMesh::Clear()
{
    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


void AnimatedMesh::LoadMesh(const string& Filename)
{
    Clear();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    pScene = Importer.ReadFile(Filename.c_str(), 
    aiProcess_Triangulate | 
    aiProcess_GenSmoothNormals | 
    aiProcess_FlipUVs | 
    aiProcess_CalcTangentSpace);

    if (pScene) {
        m_GlobalInverseTransform = inverse(ConvertToGLM(pScene->mRootNode->mTransformation));
        InitFromScene(pScene, Filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }

    glBindVertexArray(0);
}

void AnimatedMesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
    m_Meshes.resize(pScene->mNumMeshes);

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    CountVerticesAndIndices(pScene, NumVertices, NumIndices);

    ReserveSpace(NumVertices, NumIndices);

    InitAllMeshes(pScene);

    PopulateBuffers();
}


void AnimatedMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Meshes[i].BaseVertex = NumVertices;
        m_Meshes[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Meshes[i].NumIndices;
    }
}


void AnimatedMesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
{
    m_Positions.reserve(NumVertices);
    m_Normals.reserve(NumVertices);
    m_TexCoords.reserve(NumVertices);
    m_Indices.reserve(NumIndices);
    m_Bones.resize(NumVertices);
}


void AnimatedMesh::InitAllMeshes(const aiScene* pScene)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitSingleMesh(i, paiMesh);
    }
}


void AnimatedMesh::InitSingleMesh(unsigned int MeshIndex, const aiMesh* paiMesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {

        const aiVector3D& pPos = paiMesh->mVertices[i];
        m_Positions.push_back(vec3(pPos.x, pPos.y, pPos.z));

        if (paiMesh->mNormals) {
            const aiVector3D& pNormal = paiMesh->mNormals[i];
            m_Normals.push_back(vec3(pNormal.x, pNormal.y, pNormal.z));
        }
        else {
            aiVector3D Normal(0.0f, 1.0f, 0.0f);
            m_Normals.push_back(vec3(Normal.x, Normal.y, Normal.z));
        }
        const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;
        m_TexCoords.push_back(vec2(pTexCoord.x, pTexCoord.y));
    }

    LoadMeshBones(MeshIndex, paiMesh);

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        m_Indices.push_back(Face.mIndices[0]);
        m_Indices.push_back(Face.mIndices[1]);
        m_Indices.push_back(Face.mIndices[2]);
    }

    //for(glm::vec2 var : m_TexCoords)
    //{
    //    std::cout << std::string("textcoord x: ") + std::to_string(var.x) << std::endl;
    //    std::cout << std::string("textcoord y: ") + std::to_string(var.y) << std::endl;
    //    std::cout << "________" << std::endl;
    //}
}


void AnimatedMesh::LoadMeshBones(unsigned int MeshIndex, const aiMesh* pMesh)
{
    for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
        LoadSingleBone(MeshIndex, pMesh->mBones[i]);
    }
}


void AnimatedMesh::LoadSingleBone(unsigned int MeshIndex, const aiBone* pBone)
{
    int BoneId = GetBoneId(pBone);

    if (BoneId == m_BoneInfo.size()) {
        BoneInfo bi(ConvertToGLM(pBone->mOffsetMatrix));
        m_BoneInfo.push_back(bi);
    }

    for (unsigned int i = 0; i < pBone->mNumWeights; i++) {
        const aiVertexWeight& vw = pBone->mWeights[i];
        unsigned int GlobalVertexID = m_Meshes[MeshIndex].BaseVertex + pBone->mWeights[i].mVertexId;
        m_Bones[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
    }
}


int AnimatedMesh::GetBoneId(const aiBone* pBone)
{
    int BoneIndex = 0;
    string BoneName(pBone->mName.C_Str());

    if (m_BoneNameToIndexMap.find(BoneName) == m_BoneNameToIndexMap.end()) {
        BoneIndex = (int)m_BoneNameToIndexMap.size();
        m_BoneNameToIndexMap[BoneName] = BoneIndex;
    }
    else {
        BoneIndex = m_BoneNameToIndexMap[BoneName];
    }

    return BoneIndex;
}
void AnimatedMesh::PopulateBuffers()
{
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Bones[0]) * m_Bones.size(), &m_Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindVertexArray(0);
}

void AnimatedMesh::Render()
{
    glBindVertexArray(m_VAO);

    for (const auto& mesh : m_Meshes) {
        glDrawElements(GL_TRIANGLES, mesh.NumIndices, GL_UNSIGNED_INT, (void*)(mesh.BaseIndex * sizeof(GLuint)));
    }

    glBindVertexArray(0);
}

void AnimatedMesh::GetBoneTransforms(float deltaTime, vector<mat4>& Transforms)
{
    totalTime += deltaTime;
    float TicksPerSecond = (float)(pScene->mAnimations[animation]->mTicksPerSecond != 0 ? pScene->mAnimations[animation]->mTicksPerSecond : 25.0f);
    float TimeInTicks = totalTime * TicksPerSecond;
    float AnimationTimeTicks = fmod(TimeInTicks, (float)pScene->mAnimations[animation]->mDuration);
    Transforms.resize(m_BoneInfo.size());

    mat4 Identity = mat4(1.0f);

    ReadNodeHierarchy(AnimationTimeTicks,pScene->mRootNode, Identity);

    for (unsigned int i = 0; i < m_BoneInfo.size(); i++) {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}

unsigned int  AnimatedMesh::FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}


void AnimatedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    unsigned int PositionIndex = FindPosition(AnimationTimeTicks, pNodeAnim);
    unsigned int NextPositionIndex = PositionIndex + 1;
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
    float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


unsigned int AnimatedMesh::FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}


void AnimatedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    unsigned int RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
    unsigned int NextRotationIndex = RotationIndex + 1;
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
    float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out.Normalize();
}


unsigned int AnimatedMesh::FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}


void AnimatedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
    unsigned int NextScalingIndex = ScalingIndex + 1;
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
    float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void AnimatedMesh::ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const mat4& ParentTransform)
{
    string NodeName(pNode->mName.data);
    const aiAnimation* pAnimation = pScene->mAnimations[animation];

    mat4 NodeTransformation(ConvertToGLM(pNode->mTransformation));

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
        mat4 ScalingM = scale(mat4(1.0f), vec3(Scaling.x, Scaling.y, Scaling.z));

        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
        mat4 RotationM = mat4_cast(quat(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z));

        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
        mat4 TranslationM = translate(mat4(1.0f), vec3(Translation.x, Translation.y, Translation.z));

        NodeTransformation = TranslationM * RotationM * ScalingM;
    }
    mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
        unsigned int BoneIndex = m_BoneNameToIndexMap[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].OffsetMatrix;
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHierarchy(AnimationTimeTicks, pNode->mChildren[i], GlobalTransformation);
    }

}

const aiNodeAnim* AnimatedMesh::FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName)
{
    for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}

void AnimatedMesh::GetBoundingBox(vec3& minVertex, vec3& maxVertex)
{
    if (m_Positions.empty()) {
        minVertex = maxVertex = vec3(0.0f);
        return;
    }

    minVertex = m_Positions[0];
    maxVertex = m_Positions[0];

    for (const auto& position : m_Positions) {
        minVertex.x = std::min(minVertex.x, position.x);
        minVertex.y = std::min(minVertex.y, position.y);
        minVertex.z = std::min(minVertex.z, position.z);

        maxVertex.x = std::max(maxVertex.x, position.x);
        maxVertex.y = std::max(maxVertex.y, position.y);
        maxVertex.z = std::max(maxVertex.z, position.z);
    }
}