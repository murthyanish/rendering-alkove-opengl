/*!****************************************************************************
 * \file   EventListener.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   11-04-2024
 * \brief  Event listener object that contains logic to trigger an event
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#pragma once
#include "Event.h"
#include "IEventListener.h"

// Forward declaration - There's a "circular reference with EventManager"
class EventManager;

template<typename T>
class EventListener : public IEventListener {
public:
  // Default constructor
  EventListener() = default;

  // Virtual destructor - We know we will extend this class
  virtual ~EventListener();

  // Register this listener with the event manager
  void RegisterListener();

  // Pure virtual function to be implemented by derived classes
  virtual void OnEvent(const T& event) = 0;

  // Implementation of IEventListener interface
  void HandleEvent(const Event& event) override;
};

#include "EventListener.inl"
