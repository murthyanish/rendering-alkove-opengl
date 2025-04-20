/*!****************************************************************************
 * \file   Node.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * \brief  Node class used for the Scene Graph
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef NODE_H
#define NODE_H

#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "Transform.h"

class Node : public std::enable_shared_from_this<Node> {
protected:
  using ChildrenContainer = std::vector<std::shared_ptr<Node>>;
  using SharedNode = std::shared_ptr<Node>;

  /* Static Next ID counter to ensure every node gets a unique ID. */
  static unsigned int nextID;

  unsigned int id;
  std::string name;
  SharedNode parent;
  unsigned int siblingNumber;
  ChildrenContainer children;

  /* Todo: Find a way to kick these down to the RenderableNode. */
  Transform localTransform;
  Transform worldTransform;
  Matrix4 modelMatrix;
  bool isLocalSpace;

  void updateSiblingNumbers(unsigned int removedIndex);
  static void printNodeRecursive(std::ostream& os, Node* node, int depth);

  friend std::ostream& operator<<(std::ostream& os, Node& node);

public:
  Node(std::string name);
  virtual ~Node() { std::cout << "Node " << name << " destroyed\n"; }//= default;

  virtual void removeNode(SharedNode node);
  virtual void reparent(SharedNode node);
  virtual void update(float deltaTime);
  virtual void updateTransforms();
  virtual void worldToLocalSpace();
  virtual void localToWorldSpace();
  void addChild(SharedNode node);
  SharedNode findNode(unsigned int id);
  SharedNode findNodeFast(unsigned int id);
  SharedNode findNode(const std::string& name);
  SharedNode findNodeFast(const std::string& name);

  unsigned int getID() const { return id; }
  std::string getName() const { return name; }
  SharedNode getParent() const { return parent; }
  const ChildrenContainer& getChildren() const { return children; }

  Matrix4 getTransformMatrix() {
    if (isLocalSpace)
      localToWorldSpace();
    return (worldTransform).getLocalMatrix();
  }

  Transform getTransform() {
    if (isLocalSpace)
      localToWorldSpace();
    return worldTransform;
  }

  Transform getLocalTransform() {
    if (!isLocalSpace)
      worldToLocalSpace();
    return localTransform;
  }

  Transform getWorldTransform() {
    if (isLocalSpace)
      localToWorldSpace();
    return worldTransform;
  }

  Vector3 getLocalPosition();
  Quaternion getLocalRotation();
  Vector3 getLocalScaling();

  Vector3 getWorldPosition();
  Quaternion getWorldRotation();
  Vector3 getWorldScaling();

  SharedNode setLocalTransform(Transform newTransform);
  SharedNode setWorldTransform(Transform newTransform);

  Vector3 getForwardVector();
  Vector3 getRightVector();
  Vector3 getUpVector();

  SharedNode setLocalPosition(const Vector3& position);
  SharedNode setLocalRotation(const Vector3& rotation);
  SharedNode setLocalRotation(const Quaternion& rotation);
  SharedNode setLocalScaling(const Vector3& scaling);

  SharedNode setWorldPosition(const Vector3& position);
  SharedNode setWorldRotation(const Quaternion& rotation);
  SharedNode setWorldRotation(const Vector3& rotation);
  SharedNode setWorldScaling(const Vector3& scaling);

  void deleteNode();
  
};

#endif // NODE_H
