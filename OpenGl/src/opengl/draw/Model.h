#include"hzpch.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "opengl/renderer/Shader.h"
#include"opengl/renderer/Texture.h"

using namespace std;

namespace Opengl {

    class Model
    {
    public:
        // model data 
        vector<Model_Texture> textures_loaded;//纹理数组	// 存储目前已加载的所有纹理，优化以确保纹理不会被加载多次。
        vector<Mesh>    meshes; //网格数组
        string directory; //目录
        bool gammaCorrection;//伽玛校正

        // constructor, expects a filepath to a 3D model.构造函数，需要一个 3D 模型的文件路径。
        Model(string const& path, bool gamma = false) : gammaCorrection(gamma)//gamma默认否
        {
            loadModel(path);
        }

        // draws the model, and thus all its meshes//绘制模型及其所有网格
        void Draw(const std::shared_ptr<Shader>& shader)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(shader);
        }

    private:
        // 从文件中加载支持 ASSIMP 扩展名的模型，并将生成的网格存储在网格向量中。
        void loadModel(string const& path)
        {
            // read file via ASSIMP通过 ASSIMP 读取文件
            Assimp::Importer importer;//用于读取3D模型文件//后期处理（三角化、生成平滑法线、翻转UV坐标和计算切线空间）//返回一个指向场景的指针scene，
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            // check for errors检查 场景 / AI_SCENE_FLAGS_INCOMPLETE标记（数据是否完整）根节点 是否为空
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }
            // retrieve the directory path of the filepath//找到最后一个'/'字符的位置，然后使用substr()函数从字符串的开头截取到该位置
            directory = path.substr(0, path.find_last_of('/'));

            // process ASSIMPs root node recursively递归处理 ASSIMP 的根节点
            processNode(scene->mRootNode, scene);
        }

        // 以递归方式处理节点。处理节点上的每个网格，并在其子节点（如果有）上重复这一过程。
        void processNode(aiNode* node, const aiScene* scene)
        {
            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            // after weve processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }

        }
        //处理网格
        Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            // data to fill3个数组为了传入mesh构造
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Model_Texture> textures;

            // walk through each of the meshs vertices遍历每个网格顶点
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;//mesh类中的顶点类型
                glm::vec3 vector; // 我们声明了一个占位符向量，因为 assimp 使用自己的向量类，不能直接转换为 glm 的 vec3 类，所以我们先将数据传输到这个占位符 glm::vec3。
                // positions位置
                vector.x = mesh->mVertices[i].x;//从网格获取当前顶点的位置数据
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;//传入顶点
                // normals法线
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;//从网格获取当前顶点的法线数据
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }
                // texture coordinates纹理坐标（纹理，切线，副切线）
                if (mesh->mTextureCoords[0]) // 网格是否包含纹理坐标？
                {
                    glm::vec2 vec;
                    // 一个顶点最多可以包含 8 个不同的纹理坐标。因此，我们假设不会 
                    // 使用一个顶点可以有多个纹理坐标的模型，因此我们总是取第一组（0）。
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                    // tangent切线
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                    // bitangent副切线
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);//添加到顶点数组
            }
            // now wak through each of the meshs faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // 检索面的所有索引并将其存储在索引向量中
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // 我们假定着色器中采样器名称的约定俗成。每个漫反射纹理应命名为
            // 每个漫反射纹理都应命名为 "texture_diffuseN"，其中 N 是一个从 1 到 MAX_SAMPLER_NUMBER 的顺序号。
            // 其他纹理也是如此，如下表所示：
            // diffuse: texture_diffuseN
            // specular：texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            vector<Model_Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            vector<Model_Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Model_Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Model_Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            // 返回根据提取的网格数据创建的网格对象
            return Mesh(vertices, indices, textures);//传入到mesh构造中
        }

        // 检查给定类型的所有材质纹理，如果尚未加载，则加载这些纹理。  
        // 所需的信息将作为纹理结构返回。
        vector<Model_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)//材质，纹理类型，纹理名称
        {
            vector<Model_Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)//循环材质的每个纹理
            {
                aiString str;
                mat->GetTexture(type, i, &str);//获取纹理，并将str赋值为路径
                // 检查之前是否加载过纹理，如果是，则继续下一次迭代： 跳过加载新纹理
                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++)//当前数组中的数量（已经添加过的纹理）
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)//比较，==0表示相等
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // 如果已经加载了具有相同文件路径的纹理，则继续加载下一个。优化
                        break;
                    }
                }
                if (!skip)//没有加载过纹理
                {
                    Model_Texture texture;
                    texture.id = Texture::TextureFromFile(str.C_Str(), this->directory);//加载纹理，返回id
                    texture.type = typeName;//传入名字
                    texture.path = str.C_Str();//传入路径str.C_Str()
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // 将其存储为整个模型的纹理加载，以确保我们不会不必要地加载重复的纹理。
                }
            }
            return textures;
        }
    };


}