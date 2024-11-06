#include "RoamingLogic.h"

namespace {
	Vec2 getSpeed(Vec2& firstPos, Vec2& secondPos, float speed)
	{
		Vec2 dir = secondPos - firstPos;
		dir = dir.normalized();
		dir *= speed;

		return dir;
	}
}

void RoamChar(Entity& object, Vec2& firstPos, Vec2& secondPos)
{
	// True: Object has reached either firstPos or secondPos.
	// False: Object is still moving.
	static bool reachedPos = false;

	// True: Moving from firstPos to secondPos.
	// False: Moving from secondPos to firstPos.
	static bool firstToSecond = true;
	// Enum to check the state of the object
	static ObjectState state = Moving;
	// Timer to wait for a bit before moving again
	static float timer = 0.f;


	// If object is at firstPos, move towards secondPos
	TransformComponent* objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(object.entityID);
	RigidbodyComponent* objectRigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(object.entityID);

	Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	TransformComponent* playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);

	// Make a FSM to handle the movement of the object
	switch (state)
	{
		// Idle state, object is not moving.
	case Idle:
		// If Player is in range, start chasing.
		if (Vec2Dist(objectTransform->position, playerTransform->position) < objectRange) { state = Chasing; }

		// If Object has reached either firstPos or secondPos, relax for a bit.
		if (reachedPos) {
			// Once object waits for a bit, start moving again.
			if (timer >= waitTime) {
				reachedPos = false;
				timer = 0.f;
				state = Moving;
			}
			else {
				timer += DuckEngine::DeltaTime();
			}
		}
		else {
			state = Moving;
		}
		break;

		// Moving state, object is moving between firstPos and secondPos.
	case Moving:
		// If Player is in range, start chasing.
		if (Vec2Dist(objectTransform->position, playerTransform->position) < objectRange) { state = Chasing; }

		if (firstToSecond) {
			// Moving from FirstPos to SecondPos
			if (Vec2Dist(objectTransform->position, secondPos) <= 0.1f) {	// Object has reached secondPos
				reachedPos = true;
				firstToSecond = false;
				state = Idle;
			}

			Vec2 dir = getSpeed(objectTransform->position, secondPos, objectSpeed);
			objectRigidbody->velocity = dir;
		}
		else {
			// Moving from SecondPos to FirstPos
			if (Vec2Dist(objectTransform->position, firstPos) <= 0.1f) {	// Object has reached firstPos
				reachedPos = true;
				firstToSecond = true;
				state = Idle;
			}

			Vec2 dir = getSpeed(objectTransform->position, firstPos, objectSpeed);
			objectRigidbody->velocity = dir;
		}

		break;

		// Chasing state, object is chasing the player.
	case Chasing:
		// If Player is out of range, go back to moving.
		if (Vec2Dist(objectTransform->position, playerTransform->position) > (objectRange*1.5f)) { state = Moving; }

		Vec2 dir = getSpeed(objectTransform->position, playerTransform->position, objectSpeed);
		objectRigidbody->velocity = dir;

		break;
	};

}