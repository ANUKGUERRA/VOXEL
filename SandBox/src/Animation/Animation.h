#pragma once

#include "../core/config.h"

#define MAX_NUM_BONES_PER_VERTEX 10
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

class AnimatedMesh {
public:
    AnimatedMesh();
    ~AnimatedMesh();
    void setAnimation(int nAnimation);
    int getAnimation();
    void LoadMesh(const string& Filename);
    void Render();
    void GetBoundingBox(vec3& minVertex, vec3& maxVertex);
    unsigned int NumBones() const
    {
        return (unsigned int)m_BoneNameToIndexMap.size();
    }

    void GetBoneTransforms(float deltaTime, vector<mat4>& Transforms);

private:
    #define MAX_NUM_BONES_PER_VERTEX 4
    void Clear();
    void InitFromScene(const aiScene* pScene, const string& Filename);
    void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);
    void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);
    void InitAllMeshes(const aiScene* pScene);
    void InitSingleMesh(unsigned int MeshIndex, const aiMesh* paiMesh);
    void PopulateBuffers();

    struct VertexBoneData {
        unsigned int BoneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
        float Weights[MAX_NUM_BONES_PER_VERTEX] = { 0.0f };

        VertexBoneData()
        {
        }

        void AddBoneData(unsigned int BoneID, float Weight) {
            for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(BoneIDs); i++) {
                if (Weights[i] == 0.0f) {
                    BoneIDs[i] = BoneID;
                    Weights[i] = Weight;
                    return;
                }
            }
            assert(0);
        }
    };
    void LoadMeshBones(unsigned int MeshIndex, const aiMesh* paiMesh);
    void LoadSingleBone(unsigned int MeshIndex, const aiBone* pBone);
    int GetBoneId(const aiBone* pBone);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName);
    void ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const mat4& ParentTransform);

    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        POS_VB = 1,
        TEXCOORD_VB = 2,
        NORMAL_VB = 3,
        BONE_VB = 4,
        NUM_BUFFERS = 5
    };

    struct BasicMeshEntry {
        BasicMeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
    };

    Assimp::Importer Importer;
    const aiScene* pScene = NULL;
    vector<BasicMeshEntry> m_Meshes;

    void CountVertices(const aiScene* pScene);

    GLuint m_VAO = 0;
    GLuint m_Buffers[NUM_BUFFERS] = { 0 };
    vector<vec3> m_Positions;
    vector<vec3> m_Normals;
    vector<vec2> m_TexCoords;
    vector<unsigned int> m_Indices;
    vector<VertexBoneData> m_Bones;
    map<string, GLuint> m_BoneNameToIndexMap;

    float totalTime = 0;

    struct BoneInfo
    {
        mat4 OffsetMatrix;
        mat4 FinalTransformation;

        BoneInfo()
            : OffsetMatrix(1.0f), FinalTransformation(1.0f)
        {}

        BoneInfo(const mat4& Offset)
        {
            OffsetMatrix = Offset;
            FinalTransformation = (mat4(0));
        }
    };

    vector<BoneInfo> m_BoneInfo;
    mat4 m_GlobalInverseTransform;

    mat4 ConvertToGLM(const aiMatrix4x4& assimpMatrix) {
    return mat4(
        assimpMatrix.a1, assimpMatrix.b1, assimpMatrix.c1, assimpMatrix.d1,
        assimpMatrix.a2, assimpMatrix.b2, assimpMatrix.c2, assimpMatrix.d2,
        assimpMatrix.a3, assimpMatrix.b3, assimpMatrix.c3, assimpMatrix.d3,
        assimpMatrix.a4, assimpMatrix.b4, assimpMatrix.c4, assimpMatrix.d4
    );
}
};