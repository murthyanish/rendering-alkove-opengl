/*!****************************************************************************
 * \file   Mesh.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-15-2024
 * \brief  Mesh class
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef MESH_H
#define MESH_H

#pragma once

// #ifndef PI
// #define PI 3.14159265358979323846 
// #endif // !PI

#include <cassert>
#include <unordered_map>
#include <vector>

#include "GeometryBuffer.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Transform.h"
#include "assimp/mesh.h"

/** Struct VertexData not in use. */
struct VertexData {
  GeometryBuffer::AttributeType attribute;
  std::vector<float> data;
  int componentsPerVertex;
};

class Mesh {
public:
  using Attributes = 
    std::unordered_map<
      GeometryBuffer::AttributeType,
      std::pair<
        std::vector<float>,
        GLint
      >
    >;

  /* Debug resources */
  enum Type {
    Square,
    Cube,
    Sphere
  };

  Mesh(const std::string& name);
  Mesh(
    const std::string& name,
    Attributes attrData,
    const GLsizei interleavedStride = 0);
  Mesh(
    const std::string& name,
    Attributes attrData,
    const std::vector<unsigned int>& indices,
    const GLsizei interleavedStride = 0);
  virtual ~Mesh() = default;

  void setVertexData(const std::shared_ptr<GeometryBuffer>& geometryBuffer);

  void setAttributeData(
    GeometryBuffer::AttributeType& type,
    const std::vector<float>& data,
    int componentsPerVertex,
    const GLsizei interleavedStride);
  void setIndices(const std::vector<unsigned int>& newIndices);

  std::shared_ptr<GeometryBuffer> getGeometryBuffer() const { 
    return geometryBuffer;
  }

  std::unique_ptr<Mesh> clone() const;

  size_t getVertexCount() const;
  size_t getIndexCount() const;

  bool hasAttribute(GeometryBuffer::AttributeType attr) const;
  const std::string& getName() const;


  void draw(GLenum mode = GL_TRIANGLES);
  void drawEBOTrisWithoutBind();

  /* Mesh factory */
  static void pushquad(std::vector<unsigned int>& Tri, int i, int j, int k, int l);
  static std::shared_ptr<Mesh> loadMesh(const std::string& filename);
  static std::pair<Attributes, std::vector<unsigned int>> createFace(const Transform& tr);

  static std::shared_ptr<Mesh> createSphereMesh(const std::string& name, const int n = 32);
  static std::shared_ptr<Mesh> createMesh(const std::string& name, Type type);
  static std::shared_ptr<Mesh> createSquareMesh(const std::string& name, float scale = 1.0f);
  static std::shared_ptr<Mesh> createCubeMesh(const std::string& name);
  static std::shared_ptr<Mesh> getShapeMesh(Type type);
  static std::shared_ptr<Mesh> createTeapotMesh(const std::string& name, const int n = 12);

private:
  std::shared_ptr<GeometryBuffer> geometryBuffer;
  std::vector<unsigned int> indices;

  static Attributes combineAttributes(const std::vector<Attributes>& attributesList);
  static void combineIndices(unsigned int previousVertices, std::vector<unsigned int>& destIndices, std::vector<unsigned int> newIndices);
  static void processMesh(aiMesh* mesh, Attributes& newMeshData, std::vector<unsigned int>& indices);
  static std::unordered_map<std::string, std::shared_ptr<Mesh>> loadedMeshes;

  /* Mesh factory */
  static std::unordered_map<Type, std::shared_ptr<Mesh>> shapeMeshes;


  void prepareAttributeData(
    GeometryBuffer::ModifiableAttributes& triangleBufferData,
    const Mesh::Attributes& attrData,
    const GLsizei stride = 0);

  std::string name;
};


#endif // MESH_H

