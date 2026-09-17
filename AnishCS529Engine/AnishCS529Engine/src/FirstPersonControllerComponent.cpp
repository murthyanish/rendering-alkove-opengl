/*!****************************************************************************
 * \file   FirstPersonControllerComponent.cpp
 * \author Nicholas Shaw (nick.shaw@digipen.edu) (Level Management)
 * \author Anish Murthy (anish.murthy@digipen.edu) (Reparenting functions)
 * \author Daoming Wang (daoming.wang@digipen.edu) (Controller and Pause menu)
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "FirstPersonControllerComponent.h"
#include "Ray.h"
#include "RaycastHit.h"
#include "RaycastManager.h"
#include "RigidBody.h"
#include "LevelManager.h"

void FirstPersonControllerComponent::initialize() {
	//Checks to make sure the values have been initialized
	assert(input && "Input system is null!");
	assert(physicsBody && "Physics body is null!");
	assert(body && "Body (GameObject) is null!");
	assert(camera && "Camera is null!");

}

#pragma region State Switching Function

void FirstPersonControllerComponent::SwitchState(PlayerState originalState, PlayerState newState)
{
	switch (originalState)
	{
	case FirstPersonControllerComponent::Free:
		switch (newState)
		{
		case FirstPersonControllerComponent::WallRunning:
			FreeToWallRunning();
			return;
			break;
		case FirstPersonControllerComponent::Sliding:
			FreeToSliding();
			return;
			break;
		case FirstPersonControllerComponent::Grounded:
			FreeToGrounded();
			return;
			break;
		default:
			break;
		}
		break;
	case FirstPersonControllerComponent::WallRunning:
		switch (newState)
		{
		case FirstPersonControllerComponent::Free:
			WallRunningToFree();
			return;
			break;
		case FirstPersonControllerComponent::Grounded:
			WallRunningToGrounded();
			return;
			break;
		default:
			break;
		}
		break;
	case FirstPersonControllerComponent::Sliding:
		switch (newState)
		{
		case FirstPersonControllerComponent::Free:
			SlidingToFree();
			return;
			break;
		case FirstPersonControllerComponent::Grounded:
			SlidingToGrounded();
			return;
			break;
		default:
			break;
		}
		break;
	case FirstPersonControllerComponent::Grounded:
		switch (newState)
		{
		case FirstPersonControllerComponent::Free:
			GroundedToFree();
			return;
			break;
		case FirstPersonControllerComponent::Sliding:
			GroundedToSliding();
			return;
			break;
		case FirstPersonControllerComponent::Grounded:
			return;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	assert("Invalid state transition triggered for FPC" && false);
}

inline void FirstPersonControllerComponent::FreeToGrounded() {
	unanchoredTime = 0.0f;
	hasSlidSinceAnchored = false;
	physicsBody->setDrag(anchoredDrag);
	physicsToAnchor();
	playerState = Grounded;
}

inline void FirstPersonControllerComponent::FreeToSliding() {
	hasSlidSinceAnchored = true;
	sinceLastSlideTime = 0.0f;
	playerState = Sliding;

	physicsBody->setDrag(anchoredDrag);
}

inline void FirstPersonControllerComponent::FreeToWallRunning() {
	unanchoredTime = 0.0f;
	hasSlidSinceAnchored = false;
	physicsBody->setDrag(anchoredDrag);

	physicsToAnchor();
	playerState = WallRunning;

	RigidBody* const rb = static_cast<RigidBody*>(physicsBody);
	rb->usingGravity(false);
}

inline void FirstPersonControllerComponent::GroundedToFree() {
	physicsBody->setDrag(airDrag);
	physicsToAir();
	playerState = Free;
}

inline void FirstPersonControllerComponent::GroundedToSliding() {
	sinceLastSlideTime = 0.0f;
	physicsToAir();
	playerState = Sliding;
}

inline void FirstPersonControllerComponent::WallRunningToFree() {
	physicsBody->setDrag(airDrag);

	physicsToAir();
	playerState = Free;
	RigidBody* const rb = static_cast<RigidBody*>(physicsBody);
	rb->usingGravity(true);

	lastWallRunObject = anchorInfo.object;
	wallRunLockoutTimer = wallRunLockoutDuration;
}

inline void FirstPersonControllerComponent::WallRunningToGrounded() {
	physicsToAnchor();
	playerState = Grounded;
	RigidBody* const rb = static_cast<RigidBody*>(physicsBody);
	rb->usingGravity(true);
}

inline void FirstPersonControllerComponent::SlidingToGrounded() {
	unanchoredTime = 0.0f;
	hasSlidSinceAnchored = false;
	physicsToAnchor();
	playerState = Grounded;
	physicsBody->setDrag(anchoredDrag);
}

inline void FirstPersonControllerComponent::SlidingToFree() {
	physicsToAir();
	playerState = Free;
	physicsBody->setDrag(airDrag);
}

#pragma endregion

#pragma region Jumping

void FirstPersonControllerComponent::GroundedJump()
{

	//-----Handle Jumping-----//
#pragma region Jumping
	//Apply Jump
	//Reset Cooldown Timer
	sinceLastJumpTime = 0.0f;

	//Apply Jump Force
	const Vector3 currentVelocity = physicsBody->getVelocity();
	/* Todo: Consider adding forward momentum? */
	const Vector3 newVelocity = Vector3(currentVelocity.x, jumpSpeed, currentVelocity.z);
	physicsBody->setVelocity(newVelocity);

	AudioManager::instance().playSound("jump", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());

	// Prevent multiple jumps until grounded again
	sinceLastJumpPressedTime = jumpBufferTime + 1.0f;
#pragma endregion
}

void FirstPersonControllerComponent::SlidingJump() {
	sinceLastJumpTime = 0.0f;
	sinceLastJumpPressedTime = jumpBufferTime + 1.0f;

	const float slideJumpMultiplier = 1.5f;

	Vector3 currentVelocity = physicsBody->getVelocity();
	Vector3 newVelocity = Vector3(currentVelocity.x / 2, jumpSpeed * slideJumpMultiplier, currentVelocity.z / 2);
	physicsBody->setVelocity(newVelocity);

	AudioManager::instance().playSound("jump", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
}

void FirstPersonControllerComponent::WallrunningJump() {
	sinceLastJumpTime = 0.0f;
	sinceLastJumpPressedTime = jumpBufferTime + 1.0f;
	lastWallRunObject = anchorInfo.object;
	wallRunLockoutTimer = wallRunLockoutDuration; 

	const float wallJumpMultiplier = 2.0f;

	Vector3 currentVelocity = physicsBody->getVelocity();
	Vector3 wallJumpVelocity = anchorInfo.normal * wallJumpForce * wallJumpMultiplier + Vector3(0.0f, wallJumpForce * 2 * wallJumpMultiplier, 0.0f);
	Vector3 combinedVelocity = (currentVelocity + wallJumpVelocity) / 2.0f;
	physicsBody->setVelocity(combinedVelocity);

	AudioManager::instance().playSound("jump", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
}

inline bool FirstPersonControllerComponent::passedCoyoteTime() {
	return (unanchoredTime > coyoteTime);
}

inline bool FirstPersonControllerComponent::JumpBuffered() {
	return (sinceLastJumpPressedTime < jumpBufferTime);
}

inline bool FirstPersonControllerComponent::CanJump() {
	return (sinceLastJumpTime > jumpCooldown);
}

#pragma endregion

#pragma region Sliding

inline bool FirstPersonControllerComponent::SlideBuffered()
{
	return (sinceLastSlidePressedTime < slideBufferTime);
}

inline bool FirstPersonControllerComponent::CanSlide() {
	return !hasSlidSinceAnchored && (sinceLastSlideTime > slideCoolDown);
}

bool FirstPersonControllerComponent::SlidingTimedOut() {
	return (sinceLastSlideTime > slideEffectTime);
}

#pragma endregion

#pragma region Physics Anchoring

void FirstPersonControllerComponent::UpdateAnchorInfo()
{

#pragma region Grounded
	const Transform bodyWorldTransform = body->getWorldTransform();
	const Vector3 currentPos = bodyWorldTransform.getPosition();
	RaycastHit hitGround;
	/* Todo: This currently assumes z is down, fix for rotated objects.
	* Consider using getFarthestExtent()?
	*/
	const bool isGrounded = RaycastManager::Instance().Raycast(
		Ray(currentPos, Vector3(0.0f, -1.0f, 0.0f)),
		hitGround, (bodyWorldTransform.getScaling().z) * 2 + 0.25
	);
	const bool isGroundedRB = isGrounded && (hitGround.object->findComponent<RigidBody>() != nullptr);
	if (isGroundedRB) {
		anchorInfo.object = hitGround.object;
		anchorInfo.direction = 'd';
		anchorInfo.normal = hitGround.normal;

		//If the ground is a checkpoint
		if (hitGround.object->getTag() == GameObject::CHECKPOINT) {
			static std::shared_ptr<GameObject> currentCheckpoint = nullptr;
			if (currentCheckpoint != hitGround.object) {
				currentCheckpoint = hitGround.object;
				Vector3 newRespawCheckpoint = hitGround.object->getLocalPosition() + Vector3(0.0f, 2.0f, 0.0f);
				Quaternion newRespawnRotation = hitGround.object->getLocalRotation();
				setRespawnCheckpoint(newRespawCheckpoint, newRespawnRotation);
			}
			else {
				//std::cout << "Checkpoint already set to this object." << std::endl;
			}
		}

		return;
	}
#pragma endregion

	const Vector3 forwardVector = body->getForwardVector();
	const Vector3 rightVector = body->getRightVector();
	const float rayDist = parent->getWorldTransform().getScaling().x * 3;
	RaycastHit leftWallHit, rightWallHit;

#pragma region RightWallAndBoth
	if (anchorInfo.direction == 'r' || anchorInfo.direction == '0' || anchorInfo.direction == 'd') {
		const Ray rightRay = Ray(currentPos, rightVector);
		const Ray right45Ray = Ray(currentPos, (rightVector + forwardVector).normalized());
    const bool isRightWall = RaycastManager::Instance().Raycast(
                                  rightRay, rightWallHit, rayDist,
                                  {GameObject::RUNNABLE_WALL}) ||
                              RaycastManager::Instance().Raycast(
                                  right45Ray, rightWallHit, rayDist,
                                  {GameObject::RUNNABLE_WALL});
    const bool isRightWallRB =
        isRightWall &&
        (rightWallHit.object->findComponent<RigidBody>() !=
          nullptr);
    if (isRightWallRB) {
      anchorInfo.direction = 'r';
      anchorInfo.object = rightWallHit.object;
      anchorInfo.normal = rightWallHit.normal;
      return;
    } else {
      const Ray leftRay = Ray(currentPos, -rightVector);
      const Ray left45Ray = Ray(
          currentPos, (-rightVector + forwardVector).normalized());
      const bool isLeftWall = RaycastManager::Instance().Raycast(
                                  leftRay, leftWallHit, rayDist,
                                  {GameObject::RUNNABLE_WALL}) ||
                              RaycastManager::Instance().Raycast(
                                  left45Ray, leftWallHit, rayDist,
                                  {GameObject::RUNNABLE_WALL});
      const bool isLeftWallRB =
          isLeftWall &&
          (leftWallHit.object->findComponent<RigidBody>() !=
            nullptr);
      if (isLeftWallRB) {
        anchorInfo.direction = 'l';
        anchorInfo.object = leftWallHit.object;
        anchorInfo.normal = leftWallHit.normal;
        return;
      } else {
        anchorInfo.Reset();
        return;
      }
    }
  }
	else if (anchorInfo.direction == 'l') {
		const Ray leftRay = Ray(currentPos, -rightVector);
		const Ray left45Ray = Ray(currentPos, (-rightVector + forwardVector).normalized());
		const bool isLeftWall = RaycastManager::Instance().Raycast(leftRay, leftWallHit, rayDist, { GameObject::RUNNABLE_WALL }) ||
			RaycastManager::Instance().Raycast(left45Ray, leftWallHit, rayDist, { GameObject::RUNNABLE_WALL });
		const bool isLeftWallRB = isLeftWall && (leftWallHit.object->findComponent<RigidBody>() != nullptr);
		if (isLeftWallRB) {
			anchorInfo.object = leftWallHit.object;
			anchorInfo.normal = leftWallHit.normal;
			return;
		}
		else {
			const Ray rightRay = Ray(currentPos, rightVector);
			const Ray right45Ray = Ray(currentPos, (rightVector + forwardVector).normalized());
			const bool isRightWall = RaycastManager::Instance().Raycast(rightRay, rightWallHit, rayDist, { GameObject::RUNNABLE_WALL }) ||
				RaycastManager::Instance().Raycast(right45Ray, rightWallHit, rayDist, { GameObject::RUNNABLE_WALL });
			const bool isRightWallRB = isRightWall && (rightWallHit.object->findComponent<RigidBody>() != nullptr);
			if (isRightWallRB) {
				anchorInfo.direction = 'r';
				anchorInfo.object = rightWallHit.object;
				anchorInfo.normal = rightWallHit.normal;
				return;
			}
			else {
				anchorInfo.Reset();
				return;
			}
		}
	}


	anchorInfo.Reset();
	return;
#pragma endregion

}

void FirstPersonControllerComponent::physicsToAir() {
	Transform currentWorld = parent->getWorldTransform();
	std::shared_ptr<GameObject> parentGameObject = std::dynamic_pointer_cast<GameObject>(parent->getParent());
	if (parentGameObject) {
		std::shared_ptr<PhysicsBody> anchorPhysics = parentGameObject->findComponent<PhysicsBody>();
		if (anchorPhysics) {
			physicsBody->setVelocity(anchorPhysics->getVelocity() + physicsBody->getVelocity());
			physicsBody->setAcceleration(anchorPhysics->getAcceleration() + physicsBody->getAcceleration());
			physicsBody->setForce(anchorPhysics->getAcceleration() + physicsBody->getAcceleration());
			physicsBody->setRotationalVelocity(anchorPhysics->getRotationalVelocity() + physicsBody->getRotationalVelocity());
			physicsBody->setRotationalAcceleration(anchorPhysics->getRotationalAcceleration() + physicsBody->getRotationalAcceleration());
			physicsBody->setRotationalForce(anchorPhysics->getRotationalAcceleration() + physicsBody->getRotationalAcceleration());
		}
	}
	parent->reparent(sceneRoot);
	parent->setWorldTransform(currentWorld);
}

void FirstPersonControllerComponent::physicsToAnchor() {
	Transform currentWorld = parent->getWorldTransform();
	std::shared_ptr<GameObject> anchorGameObject = std::dynamic_pointer_cast<GameObject>(anchorInfo.object);
	if (anchorGameObject) {
		std::shared_ptr<PhysicsBody> anchorPhysics =
			anchorInfo.object->findComponent<PhysicsBody>();
		if (anchorPhysics) {
			physicsBody->setVelocity(physicsBody->getVelocity() - anchorPhysics->getVelocity());
			physicsBody->setAcceleration(physicsBody->getAcceleration() - anchorPhysics->getAcceleration());
			physicsBody->setForce(physicsBody->getAcceleration() - anchorPhysics->getAcceleration());
			physicsBody->setRotationalVelocity(physicsBody->getRotationalVelocity() - anchorPhysics->getRotationalVelocity());
			physicsBody->setRotationalAcceleration(physicsBody->getRotationalAcceleration() - anchorPhysics->getRotationalAcceleration());
			physicsBody->setRotationalForce(physicsBody->getRotationalAcceleration() - anchorPhysics->getRotationalAcceleration());
		}
		parent->reparent(anchorInfo.object);
	}
	else {
		parent->reparent(sceneRoot);
	}
	parent->setWorldTransform(currentWorld);
}

#pragma endregion

void FirstPersonControllerComponent::update(float deltaTime)
{

	debugCheck();

	//-----Input-----//
#pragma region Input
	bool isMovingForward = input->isKeyHeld(ActionKey[MoveForward]);
	bool isMovingBackward = input->isKeyHeld(ActionKey[MoveBackward]);
	bool isMovingLeft = input->isKeyHeld(ActionKey[MoveLeft]);
	bool isMovingRight = input->isKeyHeld(ActionKey[MoveRight]);
	bool isSprinting = input->isKeyHeld(ActionKey[Sprint]);
	float forwardMotion = input->isKeyHeld(ActionKey[MoveForward]) - input->isKeyHeld(ActionKey[MoveBackward]);
	float lateralMotion = input->isKeyHeld(ActionKey[MoveRight]) - input->isKeyHeld(ActionKey[MoveLeft]);
	bool isJumping = input->isKeyPressed(ActionKey[Jump]);
	bool isSliding = input->isKeyPressed(ActionKey[Slide]);
	bool isRespawning = input->isKeyPressed(ActionKey[Respawn]);
	bool creative = input->isKeyPressed(ActionKey[Creative]);
	bool music = input->isKeyPressed(ActionKey[Music]);
	bool freezePressed = input->isKeyPressed(ActionKey[Freeze]);
	float upMotion = input->isKeyHeld(ActionKey[Jump]) - input->isKeyHeld(ActionKey[Slide]);
	bool pause = input->isKeyPressed(ActionKey[Pause]);

	//Mouse
	float mouseXDelta = 0.0f;
	float mouseYDelta = 0.0f;
#pragma endregion

	//Creative mode
	if (isCreative) {
		isSliding = false;
		isJumping = false;
		isCreative = !creative;
		if (!isCreative) body->findComponent<RigidBody>()->usingGravity(true);
	}
	else {
		upMotion = 0;
		isCreative = creative;
		if (isCreative) body->findComponent<RigidBody>()->usingGravity(false);
	}

	//music on/off
	if (playsMusic) {
		playsMusic = !music;
		if (!playsMusic) AudioManager::instance().stopSound("music");
	}
	else {
		playsMusic = music;
		if (playsMusic)  AudioManager::instance().playSound("music", PauseMenu::Instance().getMusicVolume());
	}

	//back to start lobby
	if (PauseMenu::Instance().isStart()) {
		LevelManager::Instance().resetToMenu();
		PauseMenu::Instance().setStart(false);
	}

	//Pause Menu
	bool inConsistent = false;
	if (isPaused != PauseMenu::Instance().gameIsPaused() && isPaused) {
		isPaused = PauseMenu::Instance().gameIsPaused();
		inConsistent = true;
	}
	
	if (inConsistent) {
		isFrozen = false;
	}


	if (isPaused) {
		isPaused = !pause;
	}
	else {
		isPaused = pause;
		//Frozen Mode
		if (freezePressed) {
			isFrozen = !isFrozen;
			input->controlMouse(!isFrozen);
		}
	}
	PauseMenu::Instance().setState(isPaused);

	if (wallRunLockoutTimer > 0) {
		wallRunLockoutTimer -= deltaTime;
		if (wallRunLockoutTimer < 0) wallRunLockoutTimer = 0;
	}


	//Player HP system

	if (damageTimer < damageCooldown)
		damageTimer += deltaTime;

	if (timeSinceDamage < recoveryDelay) {
		timeSinceDamage += deltaTime;
	}
	else {
		if (hp < maxHP) {
			hp = maxHP;
			std::cout << "HP recovered to full";
		}
	}

	//Frozen Mode
	if (isFrozen) {
		return;
	}

	//-----Handling Camera Movement-----//
#pragma region Camera
	//Get Mouse State Data
	const MouseState mouseState = input->getMouseState();
	if (input->isKeyDown(KEY_M)) {
		if (mouseState.deltaX != 0) {
			mouseXDelta = static_cast<float>(mouseState.deltaX) * mouseXSensitivity;
			//Rotate Body
			const Quaternion currentBodyRotation = body->getLocalRotation();
			const Quaternion mouseRotation = Quaternion::axisAngleToQuaternion(Vector3(0.0f, 1.0f, 0.0f), (-mouseXDelta * 3.14159265f / 180.0f));
			body->setLocalRotation(currentBodyRotation * mouseRotation);
		}
		if (mouseState.deltaY != 0) {
			mouseYDelta = static_cast<float>(mouseState.deltaY) * mouseYSensitivity;
			//Rotate Camera
			const Vector3 currentEuler = camera->getLocalRotation().toEuler();
			float newPitch = currentEuler.x + (-mouseYDelta * 3.14159265f / 180.0f);
			newPitch = std::clamp(newPitch, -pitchLimit * (3.14159265f / 180.0f), pitchLimit * (3.14159265f / 180.0f)); // Convert degrees to radians
			const Quaternion newCameraRotation = Quaternion::fromEuler(Vector3(newPitch, currentEuler.y, currentEuler.z));
			camera->setLocalRotation(newCameraRotation);
		}
	}
	
	//Reset Mouse Delta
	input->resetMouseDelta();
#pragma endregion

	if (isJumping) sinceLastJumpPressedTime = 0.0f;
	if (isSliding) sinceLastSlidePressedTime = 0.0f;

	//----Some Cached Variables----//
	const Vector3 forwardVector = body->getForwardVector();
	const Vector3 upVector = body->getUpVector();
	const Vector3 rightVector = body->getRightVector();
	const Transform bodyWorldTransform = body->getWorldTransform();
	const Vector3 currentPos = bodyWorldTransform.getPosition();
	const float rayDist = parent->getWorldTransform().getScaling().x * 2.5f;
	RigidBody* const rb = static_cast<RigidBody*>(physicsBody);

	UpdateAnchorInfo();

	//Check if the player is Respawning
	if (isRespawning && difficulty != HARD) {
		respawnPlayer(true,false);
	}

	//------------------------------STATES------------------------------//
	//-----State Switching-----//
#pragma region StateSwitching
	if (playerState == Free) {
		if (sinceLastJumpTime > jumpCooldown && anchorInfo.direction == 'd')
			SwitchState(Free, Grounded);
		else if (SlideBuffered() && CanSlide())
		{
			if (!forwardMotion && !lateralMotion) {
				slideVector = forwardVector * slideForce;
			}
			const Vector3 forwardMotionVector = forwardVector * forwardMotion;
			//Lateral
			const Vector3 lateralMotionVector = rightVector * lateralMotion;
			//Combine Forward and Lateral Movement and Apply Force
			const Vector3 combinedMotionVector = forwardMotionVector + lateralMotionVector;
			slideVector = combinedMotionVector * slideForce;
			if (slideVector.magnitude() == 0) {
				slideVector = forwardVector * slideForce;
			}
			physicsBody->setVelocity(slideVector);
			AudioManager::instance().playSound("slide", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
			SwitchState(Free, Sliding);
		}
		else if ((anchorInfo.direction == 'l' || anchorInfo.direction == 'r')
		 && isMovingForward
		 && (wallRunLockoutTimer <= 0 || anchorInfo.object != lastWallRunObject)) {
			if (anchorInfo.isLargestFace()) {
				const float maxAngle = 45.0f;
				const float maxDotThreshold = std::cos(maxAngle * (3.14159265f / 180.0f));
				float dotProduct = std::abs(forwardVector.dot(anchorInfo.normal));
				if (dotProduct > -maxDotThreshold && dotProduct < maxDotThreshold) {
					SwitchState(Free, WallRunning);
				}
			}
		}
	}

	else if (playerState == Grounded) {
		/* If coyote time, preserve state */
		if (passedCoyoteTime()) {
			SwitchState(Grounded, Free);
		}
		else if (JumpBuffered())
		{
			SwitchState(Grounded, Free);
			GroundedJump();
		}
		else if (SlideBuffered() && CanSlide())
		{
			if (!forwardMotion && !lateralMotion) {
				slideVector = forwardVector * slideForce;
			}
			const Vector3 forwardMotionVector = forwardVector * forwardMotion;
			//Lateral
			const Vector3 lateralMotionVector = rightVector * lateralMotion;
			//Combine Forward and Lateral Movement and Apply Force
			const Vector3 combinedMotionVector = forwardMotionVector + lateralMotionVector;
			slideVector = combinedMotionVector * slideForce;
			if (slideVector.magnitude() == 0) {
				slideVector = forwardVector * slideForce;
			}
			physicsBody->setVelocity(slideVector);
			AudioManager::instance().playSound("slide", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
			SwitchState(Grounded, Sliding);
		}
		else if (anchorInfo.direction != 'd' && anchorInfo.direction != '0')
		{
			SwitchState(Grounded, Free);
			if ((anchorInfo.direction == 'l' || anchorInfo.direction == 'r') && isMovingForward)
				SwitchState(Free, WallRunning);
		}
	}

	else if (playerState == WallRunning) {
		if (passedCoyoteTime()) {
			SwitchState(WallRunning, Free);
		}
		else if (anchorInfo.direction == 'd')
			SwitchState(WallRunning, Grounded);
		else if (!isMovingForward || anchorInfo.direction == '0')
			SwitchState(WallRunning, Free);
		else if (isJumping) {
			SwitchState(WallRunning, Free);
			WallrunningJump();
		}
	}

	else if (playerState == Sliding) {
		if (SlidingTimedOut())
			SwitchState(Sliding, (anchorInfo.direction == 'd') ? Grounded : Free);
		else if (isJumping)
		{
			SwitchState(Sliding, Free);
			SlidingJump();
		}
	}

#pragma endregion

	//-----State Processing-----//
#pragma region StateProcessing

	//-----First Person Ground Movement-----//
	if (playerState == Grounded) {

		const float movementForce = (isSprinting ? runForceMultiplier : 1.0f) * walkForce * 100.0f;
		const float maxMovementSpeed = isSprinting ? maxRunSpeed : maxWalkSpeed;
		//Forward
		const Vector3 forwardMotionVector = forwardVector * forwardMotion;
		//Lateral
		const Vector3 lateralMotionVector = rightVector * lateralMotion;
		const Vector3 verticalMotionVector = upVector * upMotion;
		//Combine Forward and Lateral Movement and Apply Force
		if (upMotion == 0 && isCreative && physicsBody->getVelocity().y != 0) {
			physicsBody->applyForce(Vector3(0.0f, -100 * physicsBody->getVelocity().y, 0.0f));
		}
		const Vector3 combinedMotionVector = forwardMotionVector + lateralMotionVector + verticalMotionVector;
		if (combinedMotionVector.magnitude() > 0.0f) {
			const Vector3 movementVector = combinedMotionVector.normalized() * movementForce;
			physicsBody->applyForce(movementVector);
			if (isSprinting) {
				AudioManager::instance().playSound("run", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
			}
			else {
				AudioManager::instance().playSound("walk", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
			}
		}
	}


	//-----First Person Air Movement-----//
	else if (playerState == Free) {
		//---Applying Movement Force---//
		const float movementForce = (isSprinting ? runForceMultiplier : 1.0f) * walkForce * 50.0f;
		const float maxMovementSpeed = isSprinting ? maxRunSpeed : maxWalkSpeed;
		//Forward
		const Vector3 forwardMotionVector = forwardVector * forwardMotion;
		//Lateral
		const Vector3 lateralMotionVector = rightVector * lateralMotion;
		const Vector3 verticalMotionVector = upVector * upMotion;
		if (upMotion == 0 && isCreative && physicsBody->getVelocity().y != 0) {
			physicsBody->applyForce(Vector3(0.0f, -100 * physicsBody->getVelocity().y, 0.0f));
		}
		//Combine Forward and Lateral Movement and Apply Force
		const Vector3 combinedMotionVector = forwardMotionVector + lateralMotionVector + verticalMotionVector;
		if (combinedMotionVector.magnitude() > 0.0f) {
			const Vector3 movementVector = combinedMotionVector.normalized() * movementForce;
			physicsBody->applyForce(movementVector);
		}
	}
	//-----Handle Sliding-----//
	else if (playerState == Sliding) {
		//Apply a Slide Force
		//Forward
		/*
		// Force based
		slideVector = combinedMotionVector * slideForce * 10.0f;
		physicsBody->applyForce(slideVector);
		*/

		Vector3 driftVector = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 driftDirection = Vector3(-slideVector.z, 0.0f, slideVector.x).normalized();

		if (isMovingLeft) {
			driftVector = driftDirection * -0.2f;
		}
		if (isMovingRight) {
			driftVector = driftDirection * 0.2f;
		}

		Vector3 newSlideVector = slideVector + driftVector;
		newSlideVector = newSlideVector.normalized() * slideVector.magnitude();

		//Continue to Maintain Velocity
		physicsBody->setVelocity(newSlideVector);
	}

	//-----Handle Wallrunning-----//
	else if (playerState == WallRunning) {
		/* Todo: verify that this code works regardless of wall angle. */
		/* Todo: consider wallrun max time */

		//Get the direction to move along the wall in
		Vector3 wallRunDirection = Vector3(0, 1, 0).cross(anchorInfo.normal);
		if (wallRunDirection.dot(forwardVector) < 0.0f) {
			wallRunDirection = -wallRunDirection;
		}

		// Apply movement along the wall
		physicsBody->setVelocity(wallRunDirection * wallRunSpeed);
		AudioManager::instance().playSound("run", Vector3(body->getWorldPosition()), PauseMenu::Instance().getSFXVolume());
		const float wallWidth = (anchorInfo.normal * anchorInfo.object->getWorldScaling()).magnitude();
		const float playerWidth = 1.415;
		const float wallOffset = wallWidth / 2 + playerWidth / 2;

		// Maintain floating offset from the wall
		float distanceToWall = anchorInfo.normal.dot(currentPos - anchorInfo.object->getWorldPosition());
		Vector3 correctedPosition = currentPos - anchorInfo.normal * (distanceToWall - wallOffset);
		body->setWorldPosition(correctedPosition);

	} //End Wallrunning State

#pragma endregion


	//-----Timers-----//
#pragma region Timers

	if ((anchorInfo.direction != 'd') && playerState != WallRunning)
		unanchoredTime += deltaTime;

	sinceLastJumpTime += deltaTime;
	sinceLastJumpPressedTime += deltaTime;
	sinceLastSlideTime += deltaTime;
	sinceLastSlidePressedTime += deltaTime;

#pragma endregion
}

void FirstPersonControllerComponent::shutdown() {

}

std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setInputSystem(Input* _inputSystem) {
	input = _inputSystem;
	return shared_from_this();
}

void FirstPersonControllerComponent::setMouseXSensivity(float var) {
	mouseXSensitivity = var;
	return;
}

void FirstPersonControllerComponent::setMouseYSensivity(float var) {
	mouseYSensitivity = var;
	return;
}

FirstPersonControllerComponent* FirstPersonControllerComponent::getSelf() {
	return this;
}

std::shared_ptr<FirstPersonControllerComponent> FirstPersonControllerComponent::setState(PlayerState state) {
	SwitchState(playerState, state);
	return shared_from_this();
}

std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setPhysicsBody(PhysicsBody* _physicsBody) {
	physicsBody = _physicsBody;
	return shared_from_this();
}

std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setBody(GameObject* _body) {
	body = _body;
	return shared_from_this();
}

std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setCamera(GameObject* _camera) {
	camera = _camera;
	return shared_from_this();
}

/*!****************************************************************************
 * \brief Set Camera rotation to a specific angle
 *
 * This function can be used to specifically set the camera to look at a
 * specific X and Y axis rotation. Note, this is disabled for Z axis.
 *
 * \param rotation Pitch and Yaw to set the camera to, as specified by the
 * first two elements of the Vector3.
 * \return \b std::shared_ptr<FirstPersonControllerComponent> Self.
 *****************************************************************************/
std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setCameraRotation(Vector3 rotation) {
	body->setLocalRotation(
		body->getLocalRotation() *
		Quaternion::fromEuler(Vector3(0.0f, rotation.y, 0.0f)));

	Quaternion currentCameraRoation = camera->getLocalRotation();
	Vector3 currentEuler = currentCameraRoation.toEuler();
	float newPitch = currentEuler.x + rotation.x;
	newPitch = std::clamp(
		newPitch,
		-pitchLimit * (3.14159265f / 180.0f),
		pitchLimit * (3.14159265f / 180.0f)); // Convert degrees to radians
	Quaternion newCameraRotation = Quaternion::fromEuler(
		Vector3(newPitch, currentEuler.y, currentEuler.z));
	camera->setLocalRotation(newCameraRotation);
	return shared_from_this();
}

std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setSceneRoot(std::shared_ptr<Node> root) {
	sceneRoot = root;
	return shared_from_this();
}

std::shared_ptr<FirstPersonControllerComponent>
FirstPersonControllerComponent::setActionKey(Action _action, Key _key) {
	ActionKey[_action] = _key;
	return shared_from_this();
}

bool FirstPersonControllerComponent::getIsGrounded() {
	return (anchorInfo.direction == 'd');
}

std::shared_ptr<GameObject> FirstPersonControllerComponent::getAnchoredSurface() {
	return anchorInfo.object;
}

#pragma region Respawn

void FirstPersonControllerComponent::respawnPlayer(bool silence, bool resetRotation)
{

	//Set the player state to Free
	if (playerState != Free) {
		setState(Free);
	}

	//Reset Velocity
	physicsBody->setVelocity(Vector3());

	//Set Position
	body->setLocalPosition(respawnCheckpoint);

	if (!silence) {
		AudioManager::instance().playSound("hurt", body->getWorldPosition(), PauseMenu::Instance().getSFXVolume());
	}

	hp = maxHP;

	//Set Rotation
	if (resetRotation) {
		body->setLocalRotation(respawnRotation);
	}

	body->updateTransforms();
	camera->updateTransforms();

}

void FirstPersonControllerComponent::setRespawnCheckpoint(Vector3 _checkpoint, Quaternion _rotation)
{
	respawnCheckpoint = _checkpoint;
	respawnRotation = _rotation;
}

Vector3 FirstPersonControllerComponent::getRespawnCheckpoint()
{
	return respawnCheckpoint;
}

#pragma endregion


void FirstPersonControllerComponent::takeDamage() {
	if (damageTimer < damageCooldown)
		return;

	hp--;

	// reset the damage timer
	damageTimer = 0;
	timeSinceDamage = 0.0f;




	// Respawn if no hp
	if (hp <= 0) {
		if (difficulty == HARD) {
			AudioManager::instance().playSound("hurt", body->getWorldPosition(), PauseMenu::Instance().getSFXVolume());
			LevelManager::Instance().resetToMenu();
			return;
		}
		else {
			respawnPlayer();

		}
	}
	else {
		AudioManager::instance().playSound("hurt", body->getWorldPosition(), PauseMenu::Instance().getSFXVolume());
	}
	std::cerr << "Current HP: " << hp << "\n";
}

void FirstPersonControllerComponent::debugCheck()
{
	if (input->isKeyPressed(ActionKey[Debug])) {

		std::cout << "Here" << std::endl;

	}

}

std::shared_ptr<FirstPersonControllerComponent> FirstPersonControllerComponent::setDifficulty(Difficulty diff) {
	difficulty = diff;
	switch (diff) {
	case EASY:
		maxHP = 3;
		break;
	case NORMAL:
	case HARD:
		maxHP = 1;
		break;
	case CHEATING:
		maxHP = 10;
		break;
	}
	hp = maxHP;
	return shared_from_this();
}
