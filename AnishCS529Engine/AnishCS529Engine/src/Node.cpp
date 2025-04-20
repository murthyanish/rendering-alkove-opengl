/*!****************************************************************************
 * \file   Node.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"

#include "Node.h"
#include "GameObject.h"

/* Initializing static Next ID counter. */
unsigned int Node::nextID = 0;

/*!****************************************************************************
 * \brief Node Constructor
 * 
 * \param name Name of the node.
 *****************************************************************************/
Node::Node(std::string name) : 
  id(nextID++), name(name), parent(nullptr), siblingNumber(0), isLocalSpace(true),
  localTransform(Transform()), worldTransform(Transform()) {}

/*!****************************************************************************
 * \brief Add a passed node as a child to the current node
 * 
 * \param child Node to be added to children.
 *****************************************************************************/
void Node::addChild(SharedNode child) {
  children.push_back(child);
  child->siblingNumber = static_cast<unsigned int>(children.size()) - 1;
  child->parent = shared_from_this();
}

/*!****************************************************************************
 * \brief Find a node by passing the node id to be found
 * 
 * \param id ID of the node to be found.
 * \return \b Node::SharedNode pointer to the found node.
 *****************************************************************************/
Node::SharedNode Node::findNode(unsigned int id) {
  if (this->getID() == id) {
    return shared_from_this();
  }
  for (auto& child : children) {
    auto found = child->findNode(id);
    if (found) return found;
  }
  return nullptr;
}

/*!****************************************************************************
 * \brief Find a node by passing the node id to be found, but fast
 *
 * \param id ID of the node to be found.
 * \return \b Node::SharedNode pointer to the found node.
 *****************************************************************************/
Node::SharedNode Node::findNodeFast(unsigned int id) {
  std::stack<Node*> nodeStack;
  nodeStack.push(this);

  while (!nodeStack.empty()) {
    Node* currentNode = nodeStack.top();
    nodeStack.pop();

    if (currentNode->getID() == id) return currentNode->shared_from_this();

    for (
      auto it = currentNode->children.rbegin(); 
      it != currentNode->children.rend(); 
      it++) {
      nodeStack.push(it->get());
    }

  }

  return nullptr;
}

/*!****************************************************************************
 * \brief Find a node by passing the node id to be found
 * 
 * \param id ID of the node to be found.
 * \return \b Node::SharedNode pointer to the found node.
 *****************************************************************************/
Node::SharedNode Node::findNode(const std::string& name) {
  if (this->name == name) {
    return shared_from_this();
  }
  for (auto& child : children) {
    auto found = child->findNode(name);
    if (found) return found;
  }
  return nullptr;
}

/*!****************************************************************************
 * \brief Find a node by passing the node id to be found, but fast
 *
 * \param id ID of the node to be found.
 * \return \b Node::SharedNode pointer to the found node.
 *****************************************************************************/
Node::SharedNode Node::findNodeFast(const std::string& name) {
  std::stack<Node*> nodeStack;
  nodeStack.push(this);

  while (!nodeStack.empty()) {
    Node* currentNode = nodeStack.top();
    nodeStack.pop();

    if (currentNode->name == name) return currentNode->shared_from_this();

    for (
      auto it = currentNode->children.rbegin(); 
      it != currentNode->children.rend(); 
      it++) {
      nodeStack.push(it->get());
    }

  }

  return nullptr;
}

/*!****************************************************************************
 * \brief Update sibling numbers of the children when one child is removed
 * 
 * Will change the index of all proceeding children to the approriate index.
 * 
 * \param removedIndex index of the child that was removed.
 *****************************************************************************/
void Node::updateSiblingNumbers(unsigned int removedIndex) {
  for (unsigned int i = removedIndex; i < children.size(); i++) {
    children[i]->siblingNumber = i;
  }
}

/*!****************************************************************************
 * \brief Remove a node from the tree
 * 
 * Note: removeNode cannot be called on root of tree.
 * This will delete the node and reparent all children to the parent of the
 * node being deleted.
 * 
 * \param node
 *****************************************************************************/
void Node::removeNode(SharedNode node) {
  auto foundNode = findNode(node->getID());
  // no node found, nothing to do
  if (!foundNode)
    return;

  // node found
  // 0. node will always have a parent (root is overrided)
  SharedNode parent = foundNode->getParent();

  // 1. erase the found node from parent's children vector and update
  // sibling numbers
  unsigned int removedIndex = foundNode->siblingNumber;
  // still have a reference shared in foundNode variable
  parent->children.erase(parent->children.begin() + removedIndex);
  parent->updateSiblingNumbers(removedIndex);

  // 2. foundNode has children, and now that we reset the children of parent,
  // let's just push back the new children
  for (auto& child : foundNode->getChildren()) parent->addChild(child);

  // 3. clear foundNode
  foundNode->children.clear();
  foundNode->parent = nullptr;
  foundNode->siblingNumber = 0;

  // foundNode will be automatically destroyed here since all references are
  // deleted.
}

/*!****************************************************************************
 * \brief Reparent a node to a new node
 * 
 * \param dstNode
 *****************************************************************************/
void Node::reparent(SharedNode dstNode) {
  //auto thisNode = shared_from_this(); // sasafe usage of 'this'

  SharedNode parent = getParent();
  // 1. erase the found node from parent's children vector and update sibling numbers
  unsigned int removedIndex = siblingNumber;
  parent->children.erase(parent->children.begin() + removedIndex); 
  // still have a reference shared in foundNode variable
  parent->updateSiblingNumbers(removedIndex);

  // 2. attach this to dstNode
  dstNode->addChild(shared_from_this());
  updateTransforms();
}

/*!****************************************************************************
 * \brief Main update function
 * 
 * Todo: Add Transform object instead of local modelMatrix and implement world
 * transforms modifying local transfroms.
 * 
 * \param deltaTime
 *****************************************************************************/
void Node::update(float deltaTime) {
  /* Todo: add other update logic here? */
  if (!parent) throw std::runtime_error("ERROR::NODE::UPDATE::NOPARENT");

  updateTransforms();

  if (GameObject* obj = dynamic_cast<GameObject*>(this)) {
    obj->updateComponents(deltaTime);
  }

  //// Update all children
  //for (auto& child : children) {
  //  child->update(deltaTime);
  //}
}

void Node::updateTransforms()
{
  if (isLocalSpace) {
    worldTransform = parent->getWorldTransform() * localTransform;
  }
  else {
    Transform parentWorldTransform = parent->getWorldTransform();
    localTransform.setPosition(parentWorldTransform.getInverseLocalMatrix() * worldTransform.getPosition());
    localTransform.setRotation(worldTransform.getRotation() * parentWorldTransform.getRotation().inverse());
    localTransform.setScaling(worldTransform.getScaling() * parentWorldTransform.getScaling().reciprocal());
    isLocalSpace = true;
  }
}

void Node::worldToLocalSpace() {
  Transform parentWorldTransform = parent->getWorldTransform();
  localTransform.setPosition(parentWorldTransform.getInverseLocalMatrix() * worldTransform.getPosition());
  localTransform.setRotation(worldTransform.getRotation() * parentWorldTransform.getRotation().inverse());
  localTransform.setScaling(worldTransform.getScaling() * parentWorldTransform.getScaling().reciprocal());
  isLocalSpace = true;
}

void Node::localToWorldSpace() {
  worldTransform = (parent->getWorldTransform() * localTransform);
}

Vector3 Node::getLocalPosition() {
  if (!isLocalSpace)
    worldToLocalSpace();
  return localTransform.getPosition();
}
Quaternion Node::getLocalRotation() {
  if (!isLocalSpace)
    worldToLocalSpace();
  return localTransform.getRotation();
}
Vector3 Node::getLocalScaling() {
  if (!isLocalSpace)
    worldToLocalSpace();
  return localTransform.getScaling();
}

Vector3 Node::getWorldPosition() {
  if (isLocalSpace)
    localToWorldSpace();
  return worldTransform.getPosition();
}
Quaternion Node::getWorldRotation() {
  if (isLocalSpace)
    localToWorldSpace();
  return worldTransform.getRotation();
}
Vector3 Node::getWorldScaling() {
  if (isLocalSpace)
    localToWorldSpace();
  return worldTransform.getScaling();
}

std::shared_ptr<Node> Node::setLocalTransform(Transform newTransform) {
  if (!isLocalSpace)
    worldToLocalSpace();
  localTransform = newTransform; return shared_from_this();
}

std::shared_ptr<Node> Node::setWorldTransform(Transform newTransform) {
  if (isLocalSpace)
    localToWorldSpace();
  isLocalSpace = false;
  worldTransform = newTransform;
  return shared_from_this();
}

Vector3 Node::getForwardVector()
{
    Vector3 forward = getTransform().getRotation() * Vector3(0.0f, 0.0f, -1.0f);
    return forward;
}

Vector3 Node::getRightVector()
{
    Vector3 right = getTransform().getRotation() * Vector3(1.0f, 0.0f, 0.0f);
    return right;
}

Vector3 Node::getUpVector()
{
    Vector3 up = getTransform().getRotation() * Vector3(0.0f, 1.0f, 0.0f);
    return up;
}

std::shared_ptr<Node> Node::setLocalPosition(const Vector3& position) {
  if (!isLocalSpace)
    worldToLocalSpace();
  localTransform.setPosition(position);
  return shared_from_this();
}
std::shared_ptr<Node> Node::setLocalRotation(const Quaternion& rotation) {
  if (!isLocalSpace)
    worldToLocalSpace();
  localTransform.setRotation(rotation);
  return shared_from_this();
}
std::shared_ptr<Node> Node::setLocalRotation(const Vector3& rotation) {
  if (!isLocalSpace)
    worldToLocalSpace();
  localTransform.setRotation(rotation);
  return shared_from_this();
}
std::shared_ptr<Node> Node::setLocalScaling(const Vector3& scaling) {
  if (!isLocalSpace)
    worldToLocalSpace();
  localTransform.setScaling(scaling);
  return shared_from_this();
}

std::shared_ptr<Node> Node::setWorldPosition(const Vector3& position) {
  if (isLocalSpace)
    localToWorldSpace();
  isLocalSpace = false;
  worldTransform.setPosition(position);
  return shared_from_this();
}
std::shared_ptr<Node> Node::setWorldRotation(const Quaternion& rotation) {
  if (isLocalSpace)
    localToWorldSpace();
  isLocalSpace = false;
  worldTransform.setRotation(rotation);
  return shared_from_this();
}
std::shared_ptr<Node> Node::setWorldRotation(const Vector3& rotation) {
  if (isLocalSpace)
    localToWorldSpace();
  isLocalSpace = false;
  worldTransform.setRotation(rotation);
  return shared_from_this();
}
std::shared_ptr<Node> Node::setWorldScaling(const Vector3& scaling) {
  if (isLocalSpace)
    localToWorldSpace();
  isLocalSpace = false;
  worldTransform.setScaling(scaling);
  return shared_from_this();
}

/*!****************************************************************************
 * \brief Debugging printing
 * 
 * \param os Stream to output to.
 * \param node Node to start with.
 * \param depth Depth to which nodes are printed.
 *****************************************************************************/
void Node::printNodeRecursive(std::ostream& os, Node* node, int depth) {
  if (!node) return;

  // Print indentation
  for (int i = 0; i < depth; ++i) {
    os << "  ";
  }

  // Print current node
  os << "+-" << node->getName() << std::endl;

  // Print children
  for (const auto& child : node->getChildren()) {
    printNodeRecursive(os, child.get(), depth + 1);
  }
}

void Node::deleteNode()
{
    children.clear();
    parent = nullptr;
    siblingNumber = 0;
}

/*!****************************************************************************
 * \brief Debugging printing
 * 
 * \param os Stream to output to.
 * \param node Node to start with.
 * \return \b std::ostream& The output stream.
 *****************************************************************************/
std::ostream& operator<<(std::ostream& os, Node& node) {
  Node::printNodeRecursive(os, &node, 0);
  return os;
}
