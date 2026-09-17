/*!****************************************************************************
 * \file   RaycastManager.h
 * \author Nicholas Shaw (nick_shaw@me.com)
 * \par    **DigiPen Email**
 *    nick.shaw@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   03-01-2025
 * \brief  This is a singleton Class that needs it's SceneGraph to be set 
 * before any calls can be made. 
 * The Raycast() function will perform a raycasting test
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef RAYCAST_MANAGER
#define RAYCAST_MANAGER

#include <cfloat>
#pragma once

#include "Ray.h"
#include "RaycastHit.h"
#include "SceneGraph.h"
#include "Node.h"

class RaycastManager
{

public:
	static RaycastManager& Instance() {
		static RaycastManager instance;
		return instance;
	}
	RaycastManager(const RaycastManager&) = delete;
	RaycastManager& operator=(const RaycastManager&) = delete;

	void setSceneGraph(const SceneGraph* _sceneGraph) {
		this->sceneGraph = _sceneGraph;
	}

	bool Raycast(const Ray& ray, RaycastHit& hit, float maxDistance = FLT_MAX, const std::vector<GameObject::Tag> tagToCollide = {}, const std::vector<GameObject::Tag> tagToIgnore = {});

private:
	RaycastManager() = default;

	const SceneGraph* sceneGraph = nullptr;

	bool processNode(const std::shared_ptr<Node>& node,
					 const Ray& ray,
					 RaycastHit& hit,
					 float& closeHitDistance,
					 const std::vector<GameObject::Tag> tagToCollide,
					 const std::vector<GameObject::Tag> tagToIgnore);

};


#endif // !RAYCAST_MANAGER
