/*!****************************************************************************
 * \file   FirstPersonControllerComponent.h
 * \author Nicholas Shaw (nick.shaw@digipen.edu) (Input handling, Parkour
 *   mechanics, Raycasting)
 * \author Anish Murthy (anish.murthy@digipen.edu) (Reparenting)
 * \author Daoming Wang (daoming.wang@digipen.edu) (Pause Menu)
 * \brief  This is the first person controller componet
 * Responsible for all player actions
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/

#pragma once

#include <cassert>
#include <functional>

#include "Audio.h"
#include "Component.h"
#include "GameObject.h"
#include "Input.h"
#include "PauseMenu.h"
#include "PhysicsBody.h"

/*!****************************************************************************
 * \brief This is the First Person Contorller responsible for player movement,
 * camera mouse movement, Jumping, Wall Running sliding, and Time Abilities
 * 
 * ## Pre - initialization calls :
 *
 * Need to initalizethe Input System, PhysicsBody, Body of the player, and a 
 * pointer to the camera
 * 
 * std::shared_ptr<FirstPersonControllerComponent>
 *      setInputSystem(Input* _inputSystem) { input = _inputSystem; }
 *  std::shared_ptr<FirstPersonControllerComponent>
 *      setPhysicsBody(PhysicsBody* _physicsBody) { physicsBody = _physicsBody; }
 *  std::shared_ptr<FirstPersonControllerComponent>
 *      setBody(GameObject* _body) { body = _body; }
 *  std::shared_ptr<FirstPersonControllerComponent>
 *      setCamera(GameObject* _camera) { camera = _camera; }
 * 
 *****************************************************************************/
class FirstPersonControllerComponent
    : public Component, public std::enable_shared_from_this<FirstPersonControllerComponent> {

public:

    enum PlayerState {
        Free,
        WallRunning,
        Sliding,
        Grounded
    };

    enum Action {
        MoveForward,
        MoveLeft,
        MoveRight,
        MoveBackward,
        Sprint,
        Jump,
        Slide,
        Respawn,
        Debug,
        Creative,
        Freeze,
        Music,
        Pause
    };

    enum Difficulty {
        EASY,
        NORMAL,
        HARD,
        CHEATING
    };

    struct AnchorInfo {
      char direction = '0';
      std::shared_ptr<GameObject> object = nullptr;
      Vector3 normal = Vector3();

      void Reset() {
        this->direction = '0';
        this->object = nullptr;
        this->normal = Vector3();
      }

      bool isLargestFace() const {

          if (!object) return false;

          Vector3 localNormal = object->getLocalTransform().getLocalMatrix().transformDirection(normal);

          // Get the scale of the GameObject
          Vector3 scale = object->getLocalScaling();

          float areaX = scale.y * scale.z;
          float areaY = scale.x * scale.z;
          float areaZ = scale.x * scale.y;

          Vector3 largestFaceNormal;
          if (areaX >= areaY && areaX >= areaZ) {
              largestFaceNormal = Vector3(1, 0, 0);
          }
          else if (areaY >= areaX && areaY >= areaZ) {
              largestFaceNormal = Vector3(0, 1, 0);
          }
          else {
              largestFaceNormal = Vector3(0, 0, 1);
          }

          bool result = std::abs(localNormal.dot(largestFaceNormal)) > 0.99f;
          return result;
      }

    };

    FirstPersonControllerComponent() : playerState(Free),
      anchorInfo(AnchorInfo()),
        input(nullptr), physicsBody(nullptr), body(nullptr), camera(nullptr),
        walkForce(15), maxWalkSpeed(15.0f),
        runForceMultiplier(2.0f), maxRunSpeed(2 * maxWalkSpeed),
        jumpSpeed(40), airDrag(0.1f), anchoredDrag(0.6f),
        mouseXSensitivity(0.5f), mouseYSensitivity(0.5f), pitchLimit(80),
        coyoteTime(0.1f), jumpBufferTime(0.2f), jumpCooldown(0.2f),
        slideForce(50), slideCoolDown(1.4f), slideEffectTime(0.5f),
        slideBufferTime(0.2f), hasSlidSinceAnchored(false),
        wallRunSpeed(30), wallJumpForce(17),
        sceneRoot(nullptr), isCreative(false), isPaused(false), hp(1), maxHP(1), difficulty(NORMAL)
        {}
    ~FirstPersonControllerComponent() = default;

    void initialize();
    void update(float deltaTime);
    void shutdown();

    //Setting up the componenet
    std::shared_ptr<FirstPersonControllerComponent>
        setInputSystem(Input* _inputSystem);
    std::shared_ptr<FirstPersonControllerComponent>
        setPhysicsBody(PhysicsBody* _physicsBody);
    std::shared_ptr<FirstPersonControllerComponent>
        setBody(GameObject* _body);
    std::shared_ptr<FirstPersonControllerComponent>
        setCamera(GameObject* _camera);
    std::shared_ptr<FirstPersonControllerComponent>
      setCameraRotation(Vector3 rotation);
    std::shared_ptr<FirstPersonControllerComponent>
      setSceneRoot(std::shared_ptr<Node> root);

    void setMouseXSensivity(float var);
    void setMouseYSensivity(float var);
    FirstPersonControllerComponent* getSelf();

    std::shared_ptr<FirstPersonControllerComponent>
      setState(PlayerState state);

    //Mapping the Actions to the Keys
    std::shared_ptr<FirstPersonControllerComponent>
        setActionKey(Action _action, Key _key);

    //Setter for difficulty
    std::shared_ptr<FirstPersonControllerComponent> setDifficulty(Difficulty diff);

    //Accessors
    bool getIsGrounded();
    std::shared_ptr<GameObject> getAnchoredSurface();

    bool isCreativeMode() const { return isCreative; }


    //Respawn
    void respawnPlayer(bool silence = false, bool resetRotation = false);
    void setRespawnCheckpoint(Vector3 _checkpoint, Quaternion _rotation);
    Vector3 getRespawnCheckpoint();

    void takeDamage();
    int getHP() const { return hp; }
    int getMaxHP() const { return maxHP; }
    FirstPersonControllerComponent::Difficulty getDifficulty() const { return difficulty; }


private:
    //Utility Functions
    void SwitchState(PlayerState originalState, PlayerState newState);
    void FreeToGrounded();
    void FreeToSliding();
    void FreeToWallRunning();
    void GroundedToFree();
    void GroundedToSliding();
    void WallRunningToFree();
    void WallRunningToGrounded();
    void SlidingToGrounded();
    void SlidingToFree();

    void GroundedJump();
    void SlidingJump();
    void WallrunningJump();

    bool passedCoyoteTime();
    bool JumpBuffered();
    bool CanJump();
    bool SlideBuffered();
    bool CanSlide();
    bool SlidingTimedOut();

    void UpdateAnchorInfo();

    void physicsToAir();
    void physicsToAnchor();


    //PlayerState
    PlayerState playerState;
    Difficulty difficulty;
    AnchorInfo anchorInfo;
    std::shared_ptr<GameObject> anchorSurface;

    std::shared_ptr<GameObject> lastWallRunObject = nullptr;
    float wallRunLockoutTimer = 0;
    const float wallRunLockoutDuration = 0.3f;

    int hp;
    int maxHP;
    float timeSinceDamage = 0.0f;
    const float recoveryDelay = 5.0f;
    float damageCooldown = 0.5f;
    float damageTimer = 0.0f;



    //Respawn
    Vector3 respawnCheckpoint = Vector3(0.0f, 2.0f, 0.0f);
    Quaternion respawnRotation;

    //Sytem Compenet Members
    Input* input;
    PhysicsBody* physicsBody;

    //GameObject Assembly
    GameObject* body;
    GameObject* camera;

    //Action Key Mapping
    std::unordered_map<Action, Key> ActionKey;

    //PlayerMovement Members
    float walkForce;
    float maxWalkSpeed;
    float runForceMultiplier;
    float maxRunSpeed;
    float jumpSpeed;
    float anchoredDrag;
    float airDrag;

    //Player Mouse Members
    float mouseXSensitivity;
    float mouseYSensitivity;
    float pitchLimit;
    float yawRotation = 0.0f;
    float pitchRotation = 0.0;

    //Jumping Members
    float coyoteTime;
    float unanchoredTime = 0.0f;
    float jumpBufferTime;
    float sinceLastJumpPressedTime = 0.0f;
    float jumpCooldown;
    float sinceLastJumpTime = 0.0f;
    //Sliding Members
    float slideForce;
    Vector3 slideVector = Vector3();
    float slideBufferTime;
    float slideCoolDown;
    float slideEffectTime;
    float sinceLastSlidePressedTime = 0.0f;
    float sinceLastSlideTime = 0.0f;
    //WallRunning Members
    float wallRunSpeed;
    float wallJumpForce;
    bool hasSlidSinceAnchored = false;

    std::shared_ptr<Node> sceneRoot;

    //Special States
    bool isCreative = false;
    bool playsMusic = true;
    bool isFrozen = false;
    bool isPaused = false;

    //Debugging
    void debugCheck();
};
