/*!****************************************************************************
 * \file   GameWindow.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-05-2024
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "GameWindow.h"


 /* Private functions */

 /*!****************************************************************************
  * \brief Wrapper for resize callback function that is sent to
  * glfwSetFramebufferSizeCallback
  *
  * ## Explanation:
  *
  * The resize callback wrapper and the callback function that comes from the
  * renderer is called automatically when the window undergoes a resize event.
  *
  * This function is used in GameWindow::setResizeCallback(
  *   std::function<void(GLFWwindow*, int, int)> callback).
  *
  * We use this function to ensure that when the resize event occurs, the
  * following happens:
  * - The width and height variables of the window are updated.
  * - The Renderer viewport is modified to use the new width and height.
  *   > The renderer viewport modification is done by using the renderer's
  *     given callback function. This is why the renderer must call
  *     Renderer::setupCallbacks().
  *
  * \param pWindow GLFW window pointer.
  * \param width Width of the window.
  * \param height Height of the window.
  *****************************************************************************/
void GameWindow::resizeCallbackWrapper(
  GLFWwindow* pWindow, int width, int height)
{
  GameWindow* gameWindow =
    static_cast<GameWindow*>(glfwGetWindowUserPointer(pWindow));

  if (gameWindow) {
    gameWindow->width = width;
    gameWindow->height = height;

    if (gameWindow->resizeCallback)
    {
      gameWindow->resizeCallback(pWindow, width, height);
    }
    if (gameWindow->resizeCallback2)
    {
      gameWindow->resizeCallback2(pWindow, width, height);
    }
  }
}

void GameWindow::cursorPosCallbackWrapper(
  GLFWwindow* pWindow, double xpos, double ypos)
{
  GameWindow* gameWindow =
    static_cast<GameWindow*>(glfwGetWindowUserPointer(pWindow));

  if (gameWindow && gameWindow->cursorPosCallback)
  {
    gameWindow->cursorPosCallback(pWindow, xpos, ypos);
  }
}

void GameWindow::mouseButtonCallbackWrapper(
  GLFWwindow* pWindow, int button, int action, int mods)
{
  GameWindow* gameWindow =
    static_cast<GameWindow*>(glfwGetWindowUserPointer(pWindow));

  if (gameWindow && gameWindow->mouseButtonCallback)
  {
    gameWindow->mouseButtonCallback(pWindow, button, action, mods);
  }
}

/*!****************************************************************************
 * \brief Function that calls the provided hint setter function
 *
 * ## Explanation:
 *
 * This is used to setup OpenGL window hints so that the renderer can properly
 * bind to the window.
 *
 * \param hintSetter Hint setter function that is called by this function.
 *****************************************************************************/
void GameWindow::setWindowHints(const std::function<void()>& hintSetter) const
{
  hintSetter();
}

/*!****************************************************************************
 * \brief Function that sets the resize callback to the resize callback wrapper
 *
 * ## Explanation:
 *
 * This is used to register the resize callback wrapper function to the event.
 * Refer to the notes in:
 * GameWindow::resizeCallbackWrapper(GLFWwindow *pWindow, int width,
 *                                   int height)
 *
 * \param callback The callback function sent by the Renderer, which needs to
 * be registered to glfwSetFramebufferSizeCallback.
 *****************************************************************************/
void GameWindow::setResizeCallback(
  std::function<void(GLFWwindow*, int, int)> callback)
{
  resizeCallback = std::move(callback);
  glfwSetFramebufferSizeCallback(pWindow, resizeCallbackWrapper);
  glfwSetWindowUserPointer(pWindow, this);
}

/*!****************************************************************************
 * \brief TEMPORARY SECONDARY RESIZE CALLBACK FOR PAUSE MENU
 * 
 * Note: This has to be fixed later, because this is really messy and not good
 * for the engine in the long run. Refer to the node for this in the .h file
 * under resizeCallback2.
 * 
 * \param callback2
 *****************************************************************************/
void GameWindow::setResizeCallback2(std::function<void(GLFWwindow*, int, int)> callback2)
{
  resizeCallback2 = std::move(callback2);
}

void GameWindow::setCursorPosCallback(
  std::function<void(GLFWwindow*, double, double)> callback)
{
  cursorPosCallback = std::move(callback);
  glfwSetCursorPosCallback(pWindow, cursorPosCallbackWrapper);
}

void GameWindow::setMouseButtonCallback(
  std::function<void(GLFWwindow*, int, int, int)> callback)
{
  mouseButtonCallback = std::move(callback);
  glfwSetMouseButtonCallback(pWindow, mouseButtonCallbackWrapper);
}

/* Public functions */

/*!****************************************************************************
 * \brief Setter for the window title
 *
 * ## Usage:
 *
 * This is used to label and identify windows. Multiple windows must have
 * different titles.
 *
 * \param _title Title of the window.
 * \return \b GameWindow* this
 *****************************************************************************/
GameWindow* GameWindow::setTitle(std::string _title) {
  title = _title;
  return this;
}

/*!****************************************************************************
 * \brief Setter for the width of the window
 *
 * \param _width Width of the window
 * \return \b GameWindow* this
 *****************************************************************************/
GameWindow* GameWindow::setWidth(int _width)
{
  width = _width;
  return this;
}

/*!****************************************************************************
 * \brief Setter for the height of the window
 *
 * \param _height Height of the window.
 * \return \b GameWindow* this
 *****************************************************************************/
GameWindow* GameWindow::setHeight(int _height)
{
  height = _height;
  return this;
}

/*!****************************************************************************
 * \brief Setter for if Vsync should be used
 *
 * ## Usage:
 *
 * THIS FUNCTION MUST ONLY BE CALLED AFTER `Renderer::init()`!
 * Calling this function before initializing the renderer will lead to a
 * `GLFW_NO_CURRENT_CONTEXT` error.
 *
 * This sets up OpenGL to wait for the screen to fully update before swapping
 * the buffers.
 *
 * ## Explanation:
 *
 * Default behaviour for buffer swapping varies by device, so setting this
 * ensures vsync behaviour.
 * If set to false, buffer swapping occurs acyncronouosly as soon as
 * the frame is ready.
 * If set to true, buffer will wait for screen update before swapping.
 *
 * \param flag
 * \return \b GameWindow* this
 *****************************************************************************/
GameWindow* GameWindow::setVsync(bool flag) {
  if (flag)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  return this;
}

/*!****************************************************************************
 * \brief Function to set the window should close signal
 *
 * ## Usage:
 *
 * Use this at the end of game loop to close the window and end the rendering.
 *
 *****************************************************************************/
void GameWindow::setShouldClose() const
{
  glfwSetWindowShouldClose(pWindow, true);
}

/*!****************************************************************************
 * \brief Function to receive the window should close signal
 *
 * Usage:
 *
 * This is typically called in the main loop to check for end of loop or if
 * program shutdown procedures should be run.
 *
 * \return \b true Window is about to close.
 * \return \b false GLFW failure occured when closing the window.
 *****************************************************************************/
bool GameWindow::getShouldClose() const
{
  return glfwWindowShouldClose(pWindow);
}

/*!****************************************************************************
 * \brief Getter for GLFW window object
 *
 * \return \b GLFWwindow* The GLFW window object represented by this class
 * object.
 *****************************************************************************/
GLFWwindow* GameWindow::getNativeWindow() const
{
  return pWindow;
}

/*!****************************************************************************
 * \brief Getter for the width of the window
 *
 * \return \b int Width of the window.
 *****************************************************************************/
int GameWindow::getWidth()
{
  return width;
}

/*!****************************************************************************
 * \brief Getter for the height of the window
 *
 * \return \b int Height of the window.
 *****************************************************************************/
int GameWindow::getHeight()
{
  return height;
}

float GameWindow::getAspectRatio() {
    return (float)width / (float)height;
}

/*!****************************************************************************
 * \brief Initializer for GLFW and the GLFW window
 *
 * ## Required calls before init:
 * - setTitle(std::string _title)
 * - setWidth(int _width)
 * - setHeight(int _height)
 *
 * ## Usage:
 *
 * This must be called at the start of the program to create the game window.
 *
 *****************************************************************************/
void GameWindow::initialize()
{
#ifdef __linux__
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  GLFWmonitor* monitor = nullptr;

  if (isFullscreen) {
    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    width = mode->width;
    height = mode->height;
  }
  else {
    width = windowedWidth;
    height = windowedHeight;
  }

  pWindow = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);

  if (!pWindow)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  if (!isFullscreen) {
    glfwSetWindowPos(pWindow, windowedX, windowedY);
  }
}

GameWindow* GameWindow::setInitialFullscreen(bool flag)
{
  isFullscreen = flag;
  return this;
}

GameWindow* GameWindow::setFullscreen(bool flag)
{
  isFullscreen = flag;

  if (flag) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Save current window attributes
    glfwGetWindowPos(pWindow, &windowedX, &windowedY);
    glfwGetWindowSize(pWindow, &windowedWidth, &windowedHeight);

    width = mode->width;
    height = mode->height;

    // Borderless fullscreen
    glfwSetWindowMonitor(pWindow, monitor, 0, 0, mode->width, mode->height, 0);
  }
  else {
    width = windowedWidth;
    height = windowedHeight;

    // Reposition the window
    glfwSetWindowMonitor(pWindow, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
  }

  return this;
}


/*!****************************************************************************
 * \brief Update function
 *
 * ## Usage:
 *
 * This function must be called at the end/beginning of the game loop to read
 * window events. This includes all mouse, keyboard and other input device
 * signals as long as the window is in focus.
 *
 *****************************************************************************/
void GameWindow::update() {
  //glfwGetFramebufferSize(pWindow, &width, &height);
  glfwPollEvents();
}

/*!****************************************************************************
 * \brief Shutdown function to terminate the window and GLFW
 *
 * ## Usage:
 *
 * This function destroys the game window and terminates OpenGL.
 *
 *****************************************************************************/
void GameWindow::shutdown()
{
  if (pWindow)
  {
    glfwDestroyWindow(pWindow);
    glfwTerminate();
  }
}
