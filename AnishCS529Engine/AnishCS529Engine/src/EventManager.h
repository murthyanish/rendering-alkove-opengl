/*!****************************************************************************
 * \file   EventManager.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   11-04-2024
 * \brief  Event Manager system that allows components to interact with each 
 * other.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#pragma once

#include <functional>
#include <vector>
#include <queue>

#include "Event.h"
#include "IEventListener.h"
#include "FramerateController.h"


 // Event Manager - Singleton pattern
class EventManager {
public:
  static EventManager& Instance() {
    static EventManager instance;
    return instance;
  }

  struct DelayedEvent {
    using DelayedEventCallback = std::function<void()>;
    DelayedEvent(float _execTime, DelayedEventCallback _callback) :
      execTime(_execTime), callback(std::move(_callback)) {}

    bool operator>(const DelayedEvent& other) const {
      return execTime > other.execTime;
    }

    float execTime;
    DelayedEventCallback callback;
  };

  void update(float deltaTime);

  // Simple version - just store all listeners in a vector
  //void AddListener(EventListener<Event>* listener);
  void AddListener(IEventListener* listener);

  //void RemoveListener(EventListener<Event>* listener);
  void RemoveListener(IEventListener* listener);

  template <typename T>
  void ScheduleEvent(const T& event, float delay) {
    delayedEvents.push(
      {
        FFramerateController::getController()->getTime() + delay,
        [this, event]() {
          BroadcastEvent(event);
        }
      }
    );
  }

  // Broadcast event to interested listeners
  template<typename T>
  void BroadcastEvent(const T& event) {
    for (auto listener : listeners) {
      listener->HandleEvent(event);
    }
  }

private:
  EventManager() = default;
  std::vector<IEventListener*> listeners;
  std::priority_queue<
    DelayedEvent, std::vector<DelayedEvent>, std::greater<>> delayedEvents;
};
