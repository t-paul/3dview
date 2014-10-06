#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

Mesh::Mesh(const std::string fileName) : fileName(fileName)
{
}

Mesh::~Mesh()
{
}

const glm::vec3 &
Mesh::bboxMin()
{
    return min;
}

const glm::vec3 &
Mesh::bboxMax()
{
    return max;
}

void
Mesh::bind()
{
    Assimp::Importer importer;    

    const aiScene* scene = importer.ReadFile(fileName, aiProcess_CalcTangentSpace |
	aiProcess_Triangulate |
	aiProcess_GenNormals |
	aiProcess_JoinIdenticalVertices |
	aiProcess_SortByPType);

    if (!scene) {
	return;
    }

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> bcoords;
    std::vector<unsigned int> indices;
    
    min.x = 1e38;
    min.y = 1e38;
    min.z = 1e38;
    max.x = -1e38;
    max.y = -1e38;
    max.z = -1e38;
    const struct aiNode* nd = scene->mRootNode;
    for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
	const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
	for (unsigned int idx= 0;idx < mesh->mNumVertices;idx++) {
	    float x = mesh->mVertices[idx].x;
	    float y = mesh->mVertices[idx].y;
	    float z = mesh->mVertices[idx].z;
	    vertices.push_back(x);
	    vertices.push_back(y);
	    vertices.push_back(z);
	    vertices.push_back(mesh->mNormals[idx].x);
	    vertices.push_back(mesh->mNormals[idx].y);
	    vertices.push_back(mesh->mNormals[idx].z);
	    
	    if (x < min.x) {
		min.x = x;
	    }
	    if (y < min.y) {
		min.y = y;
	    }
	    if (z < min.z) {
		min.z = z;
	    }
	    if (x > max.x) {
		max.x = x;
	    }
	    if (y > max.y) {
		max.y = y;
	    }
	    if (z > max.z) {
		max.z = z;
	    }
	}
	for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
	    const struct aiFace* face = &mesh->mFaces[t];
	    switch (face->mNumIndices) {
	    case 3:
		break;
	    default:
		std::cout << "ignored" << std::endl;
		continue;
	    }
	    indices.push_back(face->mIndices[0]);
	    indices.push_back(face->mIndices[1]);
	    indices.push_back(face->mIndices[2]);
	}
    }
    
    std::cout << "bounding box: min = "
	    << min.x << ", " << min.y << ", " << min.z
	    << " - max = "
	    << max.x << ", " << max.y << ", " << max.z
	    << std::endl;
//    const struct aiNode* nd = scene->mRootNode;
//    for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
//	const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
//	for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
//	    const struct aiFace* face = &mesh->mFaces[t];
//	    switch (face->mNumIndices) {
//	    case 3:
//		break;
//	    default:
//		std::cout << "ignored" << std::endl;
//		continue;
//	    }
//	    for (unsigned int idx= 0;idx < face->mNumIndices;idx++) {
//		vertices.push_back(mesh->mVertices[face->mIndices[idx]].x);
//		vertices.push_back(mesh->mVertices[face->mIndices[idx]].y);
//		vertices.push_back(mesh->mVertices[face->mIndices[idx]].z);
//		normals.push_back(mesh->mNormals[face->mIndices[idx]].x);
//		normals.push_back(mesh->mNormals[face->mIndices[idx]].y);
//		normals.push_back(mesh->mNormals[face->mIndices[idx]].z);
//		bcoords.push_back(idx == 0 ? 1 : 0);
//		bcoords.push_back(idx == 1 ? 1 : 0);
//		bcoords.push_back(idx == 2 ? 1 : 0);
//	    }
//	}
//    }

    count = vertices.size();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    
//    glGenBuffers(1, &bcoordsBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, bcoordsBuffer);
//    glBufferData(GL_ARRAY_BUFFER, bcoords.size() * sizeof(float), &bcoords[0], GL_STATIC_DRAW);
}

void
Mesh::draw()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glVertexAttribPointer(
	    0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
	    3, // size
	    GL_FLOAT, // type
	    GL_FALSE, // normalized?
	    6 * sizeof(float), // stride
	    (void*) 0 // array buffer offset
	    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
	    1, // attribute 1. No particular reason for 0, but must match the layout in the shader.
	    3, // size
	    GL_FLOAT, // type
	    GL_FALSE, // normalized?
	    6 * sizeof(float), // stride
	    (void*)(3 * sizeof(float)) // array buffer offset
	    );

//    glEnableVertexAttribArray(2);
//    glBindBuffer(GL_ARRAY_BUFFER, bcoordsBuffer);
//    glVertexAttribPointer(
//	    2, // attribute 2. No particular reason for 0, but must match the layout in the shader.
//	    3, // size
//	    GL_FLOAT, // type
//	    GL_FALSE, // normalized?
//	    3 * sizeof(float), // stride
//	    (void*)(0 * sizeof(float)) // array buffer offset
//	    );

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)0);
    
//    glDrawArrays(GL_TRIANGLES, 0, count);
    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}