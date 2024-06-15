//
#include"hzpch.h"
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

using namespace std;

namespace Opengl {
#define MAX_BONE_INFLUENCE 4

    struct Vertex {//顶点数据
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent切线
        glm::vec3 Tangent;
        // bitangent副切线
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex骨骼索引
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone权重
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Model_Texture {//纹理数据
        unsigned int id;//ID、类型和路径
        string type;
        string path;
    };

    class Mesh {
    public:
        // mesh Data
        vector<Vertex>       vertices;//存储顶点数据的数组
        vector<unsigned int> indices;//顶点索引数据
        vector<Model_Texture>      textures;//存储纹理数据的数组
        unsigned int VAO;//顶点数组

        // constructor
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Model_Texture> textures)//构造函数
        {
            this->vertices = vertices;//this调用该成员函数的对象，Mesh类实例调用Mesh类的构造函数，this指Mesh类实例
            this->indices = indices;//使用this指针，可以访问类中的其他成员变量和成员函数
            this->textures = textures;

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh();
        }

        // render the mesh绘制网格（绑定vao，纹理，glsl，绘制，属于while中）
        void Draw(Shader& shader)
        {
            //不同纹理类型的个数的初始
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)获取纹理编号（uniform sampler2D texture_diffuse1;中的 N）
                //获取glsl中纹理采样的个数和类型
                string number;
                string name = textures[i].type;

                if (name == "texture_diffuse")//当前循环到的纹理，仅有可能是类型中的一个
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to string返回变量本身，之后变量本身再递增（从而为正确的序号）
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to string
                else if (name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to string

                //现在将采样器设置为正确的纹理单位（位置值）
                glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), (name + number).c_str()), i);//name和序号

                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);//绘制前要bind纹理
            }

            // draw mesh绘制网格
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // 一旦配置完成，将所有内容设置回默认值总是好的做法。解除绑定
            glActiveTexture(GL_TEXTURE0);
        }

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh()//设置网格（gen缓冲等，绑定顶点数据和索引数据，设置布局，绑定到VAO中，属于init）
        {
            // create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));//一个顶点的总步长，在当前顶点中的偏移量
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));//normalized参数设置为GL_TRUE，数据会被映射到[-1,1]区间
            // ids
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));//仅处理整数类型的数据，并且不会将整数转换为浮点数，不具有normalized参数
            // weights
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

            glBindVertexArray(0);
        }
    };

}