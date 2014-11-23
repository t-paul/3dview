#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

glm::vec4 positions[8] = {
    glm::vec4( 0.5, -0.5,  0.5, 1.0),
    glm::vec4( 0.5, -0.5, -0.5, 1.0),
    glm::vec4(-0.5, -0.5, -0.5, 1.0),
    glm::vec4(-0.5, -0.5,  0.5, 1.0),
    glm::vec4( 0.5,  0.5,  0.5, 1.0),
    glm::vec4( 0.5,  0.5, -0.5, 1.0),
    glm::vec4(-0.5,  0.5, -0.5, 1.0),
    glm::vec4(-0.5,  0.5,  0.5, 1.0),
};

void push_back(std::vector<float> &v, glm::vec4 vec) {
    v.push_back(vec.x);
    v.push_back(vec.y);
    v.push_back(vec.z);
    v.push_back(vec.w);
}

void quad(std::vector<float> &vertices, std::vector<float> &normals, int a, int b, int c, int d)
{
    glm::vec3 const pa = glm::vec3(positions[a]);
    glm::vec3 const pb = glm::vec3(positions[b]);
    glm::vec3 const pc = glm::vec3(positions[c]);
    glm::vec3 const n = glm::normalize(glm::cross(pc - pa, pb - pa));
    push_back(normals, glm::vec4(n, 0.0)); push_back(vertices, positions[a]);
    push_back(normals, glm::vec4(n, 0.0)); push_back(vertices, positions[c]);
    push_back(normals, glm::vec4(n, 0.0)); push_back(vertices, positions[b]);
    push_back(normals, glm::vec4(n, 0.0)); push_back(vertices, positions[a]);
    push_back(normals, glm::vec4(n, 0.0)); push_back(vertices, positions[d]);
    push_back(normals, glm::vec4(n, 0.0)); push_back(vertices, positions[c]);
}

Mesh::Mesh(const std::string fileName, bool generateVertexNormals) : fileName(fileName), generateVertexNormals(generateVertexNormals)
{
    vertexBuffer = -1;
    normalBuffer = -1;
    textureBuffer = -1;
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

    unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
    
    if (generateVertexNormals) {
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
	importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 30.0f);
	// ignore normals in file and generate per vertex normals
	flags |= aiProcess_RemoveComponent | aiProcess_GenSmoothNormals;
    } else {
	// generate face normals if not already present in the file
	flags |= aiProcess_GenNormals;
    }
    
    const aiScene* scene = importer.ReadFile(fileName, flags);
    
    if (!scene) {
	return;
    }

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> textureCoordinates;
    
    min.x = 1e38;
    min.y = 1e38;
    min.z = 1e38;
    max.x = -1e38;
    max.y = -1e38;
    max.z = -1e38;

    const struct aiNode* nd = scene->mRootNode;
    for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
	const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
	
	std::cout << "vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces << std::endl;
	
	for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
	    const struct aiFace* face = &mesh->mFaces[t];
	    switch (face->mNumIndices) {
	    case 3:
		break;
	    default:
		std::cout << "ignored" << std::endl;
		continue;
	    }
	    for (unsigned int idx= 0;idx < face->mNumIndices;idx++) {
		float x = mesh->mVertices[face->mIndices[idx]].x;
		float y = mesh->mVertices[face->mIndices[idx]].y;
		float z = mesh->mVertices[face->mIndices[idx]].z;
		float nx = mesh->mNormals[face->mIndices[idx]].x;
		float ny = mesh->mNormals[face->mIndices[idx]].y;
		float nz = mesh->mNormals[face->mIndices[idx]].z;

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
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		vertices.push_back(1.0f);
		normals.push_back(nx);
		normals.push_back(ny);
		normals.push_back(nz);
		normals.push_back(0.0f);
		textureCoordinates.push_back(x / 8);
		textureCoordinates.push_back(z / 8);
	    }
	}
    }

    std::cout << "bounding box: min = "
	    << min.x << ", " << min.y << ", " << min.z
	    << " - max = "
	    << max.x << ", " << max.y << ", " << max.z
	    << std::endl;
 
//    vertices.clear();
//    normals.clear();
//    quad(vertices, normals, 0, 1, 2, 3);
//    quad(vertices, normals, 0, 4, 5, 1);
//    quad(vertices, normals, 1, 5, 6, 2);
//    quad(vertices, normals, 2, 6, 7, 3);
//    quad(vertices, normals, 3, 7, 4, 0);
//    quad(vertices, normals, 4, 7, 6, 5);
    
    count = vertices.size();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(float), &textureCoordinates[0], GL_STATIC_DRAW);
}

glm::mat4
Mesh::matrix() const
{
    glm::vec3 mid = -0.5f * (max + min);
    
    glm::vec3 diff = max - min;
    float length = std::max(diff.x, std::max(diff.y, diff.z));
    float factor = 50.0f / length;

    glm::mat4 model;
    model = glm::scale(model, glm::vec3(factor, factor, factor));
    model = glm::translate(model, glm::vec3(mid.x, mid.z, -mid.y));
    model = glm::rotate(model, (float)(-M_PI / 2.0f), glm::vec3(1.0, 0.0, 0.0));

    return model;
}

void
Mesh::draw(GLuint vertex, GLuint normal, GLuint texture) const
{
    glEnableVertexAttribArray(vertex);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glVertexAttribPointer(
	    vertex, // attribute 0. No particular reason for 0, but must match the layout in the shader.
	    4, // size
	    GL_FLOAT, // type
	    GL_FALSE, // normalized?
	    0, // stride
	    (void*) 0 // array buffer offset
	    );

    glEnableVertexAttribArray(normal);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
	    normal, // attribute 1. No particular reason for 0, but must match the layout in the shader.
	    4, // size
	    GL_FLOAT, // type
	    GL_FALSE, // normalized?
	    0, // stride
	    (void*)0 // array buffer offset
	    );

    glEnableVertexAttribArray(texture);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glVertexAttribPointer(
	    texture, // attribute 1. No particular reason for 0, but must match the layout in the shader.
	    2, // size
	    GL_FLOAT, // type
	    GL_FALSE, // normalized?
	    0, // stride
	    (void*)0 // array buffer offset
	    );

    glDrawArrays(GL_TRIANGLES, 0, count);
    
    glDisableVertexAttribArray(texture);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(vertex);
}