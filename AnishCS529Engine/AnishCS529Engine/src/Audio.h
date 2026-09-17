/*!****************************************************************************
 * \file   Audio.h
 * \author Lixiao Kuang (li.kuang@digipen.edu)
 * \par    **DigiPen Email**
 *    li.kuang@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   01-26-2025
 * \brief  File to handle audio output
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include "fmod.hpp"
#include "Vector3.h"
#include "EventListener.h"


class AudioEvent : public Event {
public:
  enum Action {
    Start,
    Stop
  };

  AudioEvent(std::string _audioName, Action _action = Action::Start, float _duration = 0.0f) :
    audioName(_audioName), action(_action), duration(_duration) {};
  std::string audioName;
  Action action;
  float duration;
};

class AudioListener : public EventListener<AudioEvent> {
public:
  AudioListener() = default;

  void OnEvent(const AudioEvent& event) override;
};


 /*!****************************************************************************
  * \brief Audio Manager
  *
  * ## Usage:
  *
  * The AudioManager manages all audio functionality using FMOD. It is responsible
  * for initializing the audio system, loading sounds, playing sounds, updating 
  * audio each frame, and properly shutting down the audio system.
  *
  * ## Pre-initialization calls:
  *
  * N/A
  * 
  * (AudioManager is created on first use; be sure to call init() before any
  * other audio operations.)
  *
  * ## General lifecycle of an AudioManager:
  *
  * - Start the AudioManager instance using AudioManager::instance().
  * - Call init() once at startup to initialize the audio system:
  *   - Example: AudioManager::instance().init();
  * - Load all necessary sound assets using loadSound():
  *   - Example: AudioManager::instance().loadSound("pew", "/media/pew.mp3", true, false);
  *     - The third parameter indicates whether the sound is 3D (true) or 2D (false).
  *     - Optionally, a loop flag can be provided on the 4th parameter.
  * - Play sounds using playSound():
  *   - Example: AudioManager::instance().playSound("pew", 0.0f, 0.0f, 0.0f);
  * - Stop a sound using stopSound():
  *   - Example: AudioManager::instance().stopSound("radio");
  * - In the main loop, call update() every frame to process audio updates:
  *   - Example: AudioManager::instance().update();
  *   - You need to call update every frame to play the sounds properly.
  * - Adjust the listener’s position using setListenerPosition() to control 3D audio:
  *   - Example: AudioManager::instance().setListenerPosition(x, y, z);
  * - To change playback speed, call setPlaybackSpeed():
  *   - Example: AudioManager::instance().setPlaybackSpeed(0.5f);
  * - To toggle playback speed, call togglePlaybackSpeed(), when called again reset the speed.
  *   - Example: AudioManager::instance().togglePlaybackSpeed(0.7f);
  * - Before terminating the application, call shutdown() to clean up the audio system:
  *   - Example: AudioManager::instance().shutdown();
  * 
  * ## Defaults:
  *
  * - Maximum channels: 512 (can be adjusted via the init() parameter)
  * - Sounds are non-looping unless the loop flag is set during loadSound()
  *
  * ## Notes:
  *
  * - You need to call update() every frame to ensure proper audio processing.
  * - It is recommended to load all required sounds during initialization to avoid
  *   runtime performance issues.
  *
  *****************************************************************************/
class AudioManager {
public:
    static AudioManager& instance();
    void initialize(int maxChannels = 512);
    void update();
    void shutdown();
    void loadSound(const std::string& name, const std::string& path, bool is3D = false, bool loop = false);
    void playSound(const std::string& name, const Vector3& position = Vector3(), float volume = 1.0f);
    void stopSound(const std::string& name);
    void setListenerPosition(const Vector3& position);
    void setPlaybackSpeed(float speed);
    void togglePlaybackSpeed(float speed = 1);
    void setVolume(const std::string& name, float volume);
    void setMasterVolume(float volume);
    void pauseSound();
    void resumeSound();

    Vector3 getListenerPosition() const { return listenerPosition; }

private:
    AudioManager() = default;
    ~AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    FMOD::System* fmodSystem_ = nullptr;
    std::unordered_map<std::string, FMOD::Sound*> sounds_;

    float currentPlaybackSpeed_ = 1.0f;
    Vector3 listenerPosition = Vector3();

    AudioListener* engineListener;
};

#endif // AUDIO_H
