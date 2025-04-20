/*!****************************************************************************
 * \file   SceneGraph.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-15-2024
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "SceneGraph.h"
#include "GameObject.h"


void SceneGraph::addNode(std::shared_ptr<Node> node) {
  if (!node) 
    throw std::invalid_argument(
      "ERROR::SCENEGRAPH::ADDNODE::INVALID::NULL");

  root->addChild(node);
}

void SceneGraph::removeNode(std::shared_ptr<Node> node) {
  if (!node) 
    throw std::invalid_argument(
      "ERROR::SCENEGRAPH::REMOVENODE::INVALID::NULL");
  if (node == root)
    throw std::invalid_argument(
      "ERROR::SCENEGRAPH::REMOVENODE::INVALID::ROOT");
  root->removeNode(node);
}

std::shared_ptr<Node> SceneGraph::findNode(unsigned int id) {
  return root->findNode(id);
}

std::shared_ptr<Node> SceneGraph::findNodeFast(unsigned int id) {
  return root->findNode(id);
}

std::shared_ptr<Node> SceneGraph::findNode(const std::string& name) {
  return root->findNode(name);
}

std::shared_ptr<Node> SceneGraph::findNodeFast(const std::string& name) {
  return root->findNode(name);
}

void SceneGraph::update(float deltaTime) {
  updateNode(root, deltaTime);

  for (const auto& camera : cameraStack)
    camera->update();
}

void SceneGraph::updateNode(const std::shared_ptr<Node>& node, float deltaTime) {
  nodeUpdateStack.push(node);
  std::shared_ptr<Node> currNode;
  while (nodeUpdateStack.size() > 0) {
    currNode = nodeUpdateStack.top();
    nodeUpdateStack.pop();
    currNode->update(deltaTime);
    for (auto child : currNode->getChildren()) {
      nodeUpdateStack.push(child);
    }
  }
  
}

void SceneGraph::drawNode(
  const std::shared_ptr<Node>& node,
  std::shared_ptr<Shader> shader, uint32_t renderMask) {
  std::stack<std::shared_ptr<Node>> nodeDrawStack;
  nodeDrawStack.push(node);
  std::shared_ptr<Node> currNode;
  while ( nodeDrawStack.size() > 0 ) {
    currNode = nodeDrawStack.top();
    nodeDrawStack.pop();
    
    auto gameObjectNode = std::dynamic_pointer_cast<GameObject>( currNode );
    if (gameObjectNode) {
      gameObjectNode->draw(shader, renderMask);

      if (gameObjectNode->isEnabled()) {
        for (auto child : currNode->getChildren()) {
          nodeDrawStack.push(child);
        }
      }
    }
    else {
      for (auto child : currNode->getChildren()) {
        nodeDrawStack.push(child);
      }
    }
  }
}

void SceneGraph::draw(std::shared_ptr<Shader> shader, uint32_t renderMask) {
  drawNode(root, shader, renderMask);
}

void SceneGraph::printSceneTree() {
  std::cout << *root << std::endl;
}

void SceneGraph::clear()
{
    for (auto child : root->getChildren()) {
        nodeUpdateStack.push(child);
    }
    std::shared_ptr<Node> currNode;
    while (nodeUpdateStack.size() > 0) {
        currNode = nodeUpdateStack.top();
        nodeUpdateStack.pop();
        for (auto child : currNode->getChildren()) {
            nodeUpdateStack.push(child);
        }
        currNode->deleteNode();
    }

    root->removeChildren();

    clearCameras();
    clearLights();

}