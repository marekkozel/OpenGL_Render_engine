#include "Model.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const float *vec, int size, GLenum type) : type(type)
{
    vertices = std::make_shared<std::vector<float>>(vec, vec + size);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);

    // Layout: Pos(3) + Normal(3) = 6 floats stride
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));

    glBindVertexArray(0);
}

Model::Model(std::vector<float> *vec, GLenum type) : vertices(std::make_shared<std::vector<float>>(*vec)), type(type)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);

    // Layout: Pos(3) + Normal(3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));

    glBindVertexArray(0);
}

Model::Model(std::vector<float> *vec, GLenum type, int texture) : vertices(std::make_shared<std::vector<float>>(*vec)), type(type)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);

    // Layout: Pos(3) + Normal(3) + UV(2) = 8 floats stride
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(6 * sizeof(float)));

    glBindVertexArray(0);
}

Model::~Model()
{
    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);
    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);
}

void Model::draw() const
{
    if (VAO == 0)
        return;

    glBindVertexArray(VAO);
    if (indices && !indices->empty())
        glDrawElements(type, static_cast<GLsizei>(indices->size()), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(type, 0, static_cast<GLsizei>(vertices->size() / 6));

    glBindVertexArray(0);
}

bool Model::load_from_file(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                                 aiProcess_GenNormals |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_PreTransformVertices |
                                                 aiProcess_GenUVCoords);

    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        std::cerr << "❌ Assimp failed to load model: " << path << "\n"
                  << importer.GetErrorString() << std::endl;
        return false;
    }

    const aiMesh *mesh = scene->mMeshes[0];

    bool hasUVs = mesh->HasTextureCoords(0);
    std::vector<float> data;
    std::vector<unsigned int> inds;

    // Position(3) + normal(3) + uv(2 if present)
    int stride = hasUVs ? 8 : 6;
    data.reserve(mesh->mNumVertices * stride);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        // Position
        aiVector3D pos = mesh->mVertices[i];
        data.push_back(pos.x);
        data.push_back(pos.y);
        data.push_back(pos.z);

        // Normal
        aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[i] : aiVector3D(0, 0, 0);
        data.push_back(normal.x);
        data.push_back(normal.y);
        data.push_back(normal.z);

        // Texture coordinates (UVs)
        if (hasUVs)
        {
            aiVector3D uv = mesh->mTextureCoords[0][i];
            data.push_back(uv.x);
            data.push_back(uv.y);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace &face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            inds.push_back(face.mIndices[j]);
    }

    vertices = std::make_shared<std::vector<float>>(std::move(data));
    indices = std::make_shared<std::vector<unsigned int>>(std::move(inds));

    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO); // cleanup if reloading
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_STATIC_DRAW);

    if (EBO != 0)
        glDeleteBuffers(1, &EBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

    // Attributes
    GLint offset = 0;

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid *)(offset));
    offset += 3 * sizeof(float);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid *)(offset));
    offset += 3 * sizeof(float);

    if (hasUVs)
    {
        // Texture UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid *)(offset));
    }

    glBindVertexArray(0); // Unbind for safety

    std::cout << "✅ Model loaded via Assimp: " << path
              << " (" << mesh->mNumVertices << " vertices, UVs: "
              << (hasUVs ? "yes" : "no") << ")" << std::endl;

    return true;
}