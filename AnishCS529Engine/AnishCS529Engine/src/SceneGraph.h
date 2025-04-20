/*!****************************************************************************
 * \file   SceneGraph.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    CS529
 * \date   10-15-2024
 * \brief  Scene Graph to control all nodes in the scene.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#pragma once

#include "Node.h"
#include "Camera.h"
#include "Lights.h"
#include "Shader.h"

class SceneGraph {
private:
  class RootNode : public Node {
  public:
    RootNode() : Node("Root") {
      worldTransform = Transform();
      localTransform = Transform();
    }
    ~RootNode() override = default;

    void removeNode(SharedNode node) override {
      throw std::runtime_error("ERROR::SCENEGRAPH::ROOTNODE::REMOVENODE::FORBIDDEN");
    }

    void reparent(SharedNode node) override {
      throw std::runtime_error("ERROR::SCENEGRAPHvoid::ROOTNODE::REPARENTNODE::FORBIDDEN");
    }

    void worldToLocalSpace() { isLocalSpace = true; }
    void localToWorldSpace() { isLocalSpace = false; }

    void update(float deltaTime) override {
      worldTransform = localTransform;
      /*for (auto& child : children) {
        child->update(deltaTime);
      }*/
    }

    void removeChildren() {
        children.clear();
    }

  };

  std::shared_ptr<RootNode> root;
  std::stack<std::shared_ptr<Node>> nodeUpdateStack;

  void drawNode(
    const std::shared_ptr<Node>& node,
    std::shared_ptr<Shader> shader, uint32_t renderMask);


  Lights lights;
  LightStatus lightStatus;
  std::vector<std::shared_ptr<Camera>> cameraStack;

public:
  SceneGraph() : root(std::make_shared<RootNode>()) {}
  ~SceneGraph() {
    clearLights();
    clearCameras();
  }

  std::shared_ptr<RootNode> getRootNode() const { return root; }

  void addNode(std::shared_ptr<Node> node);
  void removeNode(std::shared_ptr<Node> node);

  std::shared_ptr<Node> findNode(unsigned int id);
  std::shared_ptr<Node> findNodeFast(unsigned int id);

  std::shared_ptr<Node> findNode(const std::string& name);
  std::shared_ptr<Node> findNodeFast(const std::string& name);

  void update(float deltaTime);
  void updateNode(const std::shared_ptr<Node>& node, float deltaTime);
  void draw(std::shared_ptr<Shader> shader, uint32_t renderMask);
  void printSceneTree();

  Vector3 getRootPosition() const { return root->getLocalPosition(); }
  Quaternion getRootRotation() const { return root->getLocalRotation(); }
  Vector3 getRootScaling() const { return root->getLocalScaling(); }

  void setRootPosition(const Vector3& position) { root->setLocalPosition(position); }
  void setRootRotation(const Vector3& rotation) { root->setLocalRotation(rotation); }
  void setRootScaling(const Vector3& scaling) { root->setLocalScaling(scaling); }

  // Lights
  void addDirectionalLight(DirectionalLight light) { lights.sunLight = light; }
  void addAmbientLight(AmbientLight light) { lights.ambientLight = light; }
  void addPointLight(PointLight light) { lights.pointLights.push_back(light); }
  void clearLights() { lights = {}; }
  Lights* getLights() { return &lights; }

  // Cameras
  void addCamera(std::shared_ptr<Camera> camera) { cameraStack.push_back(camera); }
  void clearCameras() { cameraStack.clear(); }
  std::vector<std::shared_ptr<Camera>> getCameras() { return cameraStack; }

  void clear();

};

#endif // SCENE_GRAPH_H
