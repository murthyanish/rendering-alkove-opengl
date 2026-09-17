#pragma once
#include "Event.h"

class IEventListener {
public:
  virtual ~IEventListener() = default;
  virtual void HandleEvent(const Event& event) = 0;
};
