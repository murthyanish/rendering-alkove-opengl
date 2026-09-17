/*!****************************************************************************
 * \file   Renderer.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * \brief  Renderer implementation to initialize the OpenGL libraries
 * and libraries.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef RENDERER_H
#define RENDERER_H

#pragma once

/* Base class */
#include "Component.h"

/* Used classes */
#include "GameWindow.h"
#include "RenderGraph.h"

/*!****************************************************************************
 * \brief Class Renderer that implements an OpenGL renderer
 * 
 * ## Usage:
 * 
 * Renderer is needed to render objects onto the screen. This needs to be
 * initialized to work with a window, and the draw call must be made to draw
 * objects to the screen.
 * 
 * ## Pre-initialization calls:
 * 
 * - setGameWindow(GameWindow* _gameWindow)
 * 
 * ## Post-initialization calls:
 * 
 * - setIs3D(bool is3D)
 * - setClearColor(float r, float g, float b, float a)
 * 
 * ## General lifecycle of a Renderer:
 * 
 * - Init GameWindow object.
 * - Init Renderer.
 * - Call clear() to empty the window buffer.
 * - Run all your game logic.
 * - Run draw() calls for each of the objects to be rendered.
 * - Call swapBuffers() to render the window buffer to the screen.
 * 
 * ## Notes:
 * 
 * Renderer class is specific to a window. It is possible to have multiple
 * renderers, one for each window you have.
 * 
 *****************************************************************************/
class Renderer {
public:
  struct BlendState {
    bool enabled = false;
    GLenum equation = GL_FUNC_ADD;
    GLenum srcFactor = GL_ONE;
    GLenum destFactor = GL_ONE;

    BlendState(bool _enabled = false) : enabled(_enabled), equation(GL_FUNC_ADD), srcFactor(GL_ONE), destFactor(GL_ONE) {}

    bool operator==(const BlendState& other) const {
      return enabled == other.enabled &&
        equation == other.equation &&
        srcFactor == other.srcFactor &&
        destFactor == other.destFactor;
    }
  };

  struct DepthState {
    bool testEnabled = true;
    bool writeEnabled = true;
    GLenum func = GL_LESS;

    DepthState(bool _enabled = true) : testEnabled(_enabled), writeEnabled(true), func(GL_LESS) {}

    bool operator==(const DepthState& other) const {
      return testEnabled == other.testEnabled &&
        writeEnabled == other.writeEnabled &&
        func == other.func;
    }
  };

  struct Viewport {
    int x;
    int y;
    int width;
    int height;

    Viewport(int _x, int _y, int _width, int _height) :
      x(_x), y(_y), width(_width), height(_height) {};
    
    bool operator==(const Viewport& other) const {
      return x == other.x && y == other.y &&
        width == other.width && height == other.height;
    }
  };

  struct State
  {
    Viewport viewport;
    BlendState blendState;
    DepthState depthState;

    State(const Viewport& vp, const BlendState& bs = BlendState{},
      const DepthState& ds = DepthState{}) :
      viewport(vp), blendState(bs), depthState(ds) {};
  };

  /*!**************************************************************************
   * \brief StateSaver class
   * 
   * ## Usage:
   * 
   * This class is to be used when the state of the Renderer needs to be stored
   * and restored after certain operations such as render passes.
   * 
   * Merely instanciating this class within the render pass's scope will do the
   * job since once the scope is exited, the destructor will be called, which
   * will reset the state of the Renderer to its initial state.
   * 
   ***************************************************************************/
  class StateSaver;

  Renderer() = default;
  ~Renderer() = default;

  /* Component-like functions */
  void initialize();
  void update(); // Dummy
  void shutdown(); // Dummy

  /* Pre-initialization functions */
  Renderer* setGameWindow(GameWindow* _gameWindow);
  Renderer* setClearColor(float r, float g, float b, float a);

  /* Utility functions */
  void clear() const;
  void swapBuffers() const;
  void draw(GLenum mode, GLint count, bool indexed) const;

  /* Advanced Rendering Utility functions */
  Renderer* setViewport(const Viewport& viewport, bool force = false);
  Renderer* resetViewport();
  Renderer* setDepthState(const DepthState& depthState, bool force = false);
  Renderer* setBlendState(const BlendState& blendState, bool force = false);

  std::shared_ptr<RenderGraph> getRenderGraph() { return renderGraph; }

  State getCurrentState() const;
  Renderer* setState(const State& state, bool force = false);

  void addScreenSizeBufferUpdateCallback(
    std::function<void(int, int, int, int)> screenSizeBufferUpdateCallback);

private:
  /** Game Window object */
  GameWindow* gameWindow;
  /** Clear mask for glClear */
  GLbitfield clearMask;
  State state{ Viewport{ 0,0,0,0 }, BlendState{}, DepthState{} };
  std::vector<std::function<void(int, int, int, int)>> screenSizeBufferUpdateCallbacks;

  std::shared_ptr<RenderGraph> renderGraph;

  bool loadGraphicsAPIFunctions() const;
  void setupCallbacks();
  void framebufferSizeCallback(
    GLFWwindow* pWindow, int width, int height);

  void updateScreenSizeBuffers();

};

#endif // RENDERER_H
