/*!****************************************************************************
 * \file   GeometryBuffer.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * \brief  Geometry Buffer factory class that manages OpenGL buffers and their
 * data.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef GEOMETRY_BUFFER_H
#define GEOMETRY_BUFFER_H

#pragma once

#include <glad/glad.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/*!****************************************************************************
 * \brief Class that sets up the OpenGL buffers
 * 
 *****************************************************************************/
class GeometryBuffer {
public:

  /**
    \brief Attribute types supported by the code

    These are also the positions of each attribute in the GPU.
  */
  enum class AttributeType {
    Position,
    Normal,
    TexCoord,
    Color,
    Tangent,
    Bitangent
  };

  /**
    \brief Info on Attributes to be stored in the buffers
  */
  struct AttributeInfo {
    /** 1D vector of size (Dimension * Num Points) */
    std::vector<float>  data;
    /** Number of dimensions */
    GLint               elementSize;
    /** GL type */
    GLenum              type;
    /** If should normalize */
    GLboolean           normalized;
    /** */
    GLsizei              stride;
  };

  /**
    \brief Modifiable version of GeometryBuffer::Attributes
  */
  using ModifiableAttributes = std::unordered_map<
    AttributeType,
    AttributeInfo
  >;

  /**
    \brief Attribute input data structure
    
    Attributes stores:
    - GeometryBuffer::AttributeType (position or color, etc)
    - GeometryBuffer::AttributeInfo:
      - std::vector<float>  data: 1D vector of size (Dimension * numpoints)
      - GLint               elementSize: Number of dimensions
      - GLenum              type: GL type
      - GLboolean           normalized: If should normalize
  */
  using Attributes = const std::unordered_map<
    AttributeType,
    AttributeInfo
  >;
  
  ~GeometryBuffer();

  /** \brief Deleted copy constructor to prevent copying */
  GeometryBuffer(const GeometryBuffer&) = delete;
  /** \brief Deleted copy assignment to prevent copying */
  GeometryBuffer& operator=(const GeometryBuffer&) = delete;

  // Allow moving
  GeometryBuffer(GeometryBuffer&&) noexcept;
  GeometryBuffer& operator=(GeometryBuffer&&) noexcept;

  static std::shared_ptr<GeometryBuffer> create(
    Attributes& attributeData,
    const std::string& name,
    bool bufferIsInterleaved = true);
  static std::shared_ptr<GeometryBuffer> create(
    Attributes& attributeData,
    const std::vector<unsigned int>& indices,
    const std::string& name,
    bool bufferIsInterleaved = true);

  void updateVertexAttribute(
    const AttributeType& type, 
    const std::vector<float>& data);
  void updateIndices(const std::vector<unsigned int>& indices);

  bool hasAttribute(AttributeType type);

  void bind() const;
  void unbind() const;

  /**
  * \brief Getter for vertexCount
  * 
  * \return \b GLsizei vertexCount.
  */
  GLsizei getVertexCount() const { return vertexCount; }

  /**
  * \brief Getter for indexCount
  * 
  * \return \b GLsizei indexCount.
  */
  GLsizei getIndexCount() const { return indexCount; }

  /**
  * \brief Getter for index data
  *
  * \return \b std::vector<unsigned int> indexData.
  */
  std::vector<unsigned int> getIndexData()  const { return indexData; }

  /**
  * \brief Getter for VAO
  *
  * \return \b GLuint VAO ID.
  */
  GLuint getVAO() const { return vao; }

  /**
  * \brief Getter for VBO
  *
  * \return \b GLuint VBO ID.
  */
  GLuint getVBO() const { return vbo; }

  /**
  * \brief Getter for EBO
  *
  * \return \b GLuint EBO ID.
  */
  GLuint getEBO() const { return ebo; }

private:
  /** VAO ID */
  GLuint vao;
  /** VBO ID */
  GLuint vbo;
  /** EBO ID */
  GLuint ebo;

  /** If data is interleaved*/
  bool isInterleaved;
  
  /** Number of Vertices */
  GLsizei vertexCount;
  /** Number of Indices */
  GLsizei indexCount;

  static GLuint boundVAO;

  /**
   * Offsets for each attribute
   * 
   * Since we are using blocked attributes, this is start of each attribute
   * block
   */
  std::unordered_map<AttributeType, GLsizeiptr> attributeOffsets;

  /* Todo: Convert to interleaved and use the below */
  //std::unordered_map<AttributeType, GLsizei> attributeStrides;

  /** Buffer name */
  std::string name;

  /** Attribute data */
  ModifiableAttributes bufferAttributeData;

  /** Indices */
  std::vector<unsigned int> indexData;

  /** Private constructor to enforce factory */
  GeometryBuffer(std::string name) : 
    vao(0), vbo(0), ebo(0), vertexCount(0), 
    indexCount(0), name(name), isInterleaved(true) {};

  void initializeVertexBuffers(
    Attributes& attributeData);
  void initializeElementBuffers(const std::vector<unsigned int>& indices);

  void initializeBuffers(
    Attributes& attributeData);
  void initializeBuffers(
    Attributes& attributeData, 
    const std::vector<unsigned int>& indices);

  GLenum getGlType(AttributeType);

  void cleanupBuffers();

};

#endif // GEOMETRY_BUFFER_H
