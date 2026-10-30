#include "pch.h"
#include "PhysicsSystem.h"

#include <algorithm>

namespace XYZEngine
{
    PhysicsSystem* PhysicsSystem::Instance()
    {
        static PhysicsSystem physicsSystem;
        return &physicsSystem;
    }

    float PhysicsSystem::GetFixedDeltaTime() const
    {
        return fixedDeltaTime;
    }

    void PhysicsSystem::Update()
    {
        
        for (auto collider : colliders)
        {
            if (collider != nullptr)
            {
                collider->Update(0.f);
            }
        }

        for (size_t i = 0; i < colliders.size(); ++i)
        {
            ColliderComponent* colliderA = colliders[i];

            if (colliderA == nullptr)
            {
                continue;
            }

            for (size_t j = i + 1; j < colliders.size(); ++j)
            {
                ColliderComponent* colliderB = colliders[j];

                if (colliderB == nullptr)
                {
                    continue;
                }

                sf::FloatRect intersection;

                if (!colliderA->bounds.intersects(
                    colliderB->bounds,
                    intersection))
                {
                    continue;
                }

                
                if (colliderA->isTrigger != colliderB->isTrigger)
                {
                    auto triggerPair = std::find_if(
                        triggersEnteredPair.begin(),
                        triggersEnteredPair.end(),
                        [colliderA, colliderB](const auto& pair)
                        {
                            return
                                (pair.first == colliderA &&
                                    pair.second == colliderB) ||
                                (pair.first == colliderB &&
                                    pair.second == colliderA);
                        }
                    );

                    if (triggerPair == triggersEnteredPair.end())
                    {
                        Trigger trigger(colliderA, colliderB);

                        colliderA->OnTriggerEnter(trigger);
                        colliderB->OnTriggerEnter(trigger);

                        triggersEnteredPair.emplace(colliderA, colliderB);
                    }

                    continue;
                }

                if (colliderA->isTrigger)
                {
                    continue;
                }

                GameObject* objectA = colliderA->GetGameObject();
                GameObject* objectB = colliderB->GetGameObject();

                if (objectA == nullptr || objectB == nullptr)
                {
                    continue;
                }

                auto bodyA =
                    objectA->GetComponent<RigidbodyComponent>();

                auto bodyB =
                    objectB->GetComponent<RigidbodyComponent>();

                bool canMoveA =
                    bodyA != nullptr && !bodyA->GetKinematic();

                bool canMoveB =
                    bodyB != nullptr && !bodyB->GetKinematic();

                if (!canMoveA && !canMoveB)
                {
                    continue;
                }

                ColliderComponent* movingCollider = nullptr;

                if (canMoveA)
                {
                    movingCollider = colliderA;
                }
                else
                {
                    movingCollider = colliderB;
                }

                auto movingTransform =
                    movingCollider->GetGameObject()
                    ->GetComponent<TransformComponent>();

                if (movingTransform == nullptr)
                {
                    continue;
                }

                float overlapX = intersection.width;
                float overlapY = intersection.height;

                float centerAX =
                    colliderA->bounds.left +
                    colliderA->bounds.width * 0.5f;

                float centerAY =
                    colliderA->bounds.top +
                    colliderA->bounds.height * 0.5f;

                float centerBX =
                    colliderB->bounds.left +
                    colliderB->bounds.width * 0.5f;

                float centerBY =
                    colliderB->bounds.top +
                    colliderB->bounds.height * 0.5f;

                
                if (overlapX < overlapY)
                {
                    float direction;

                    if (movingCollider == colliderA)
                    {
                        direction = centerAX < centerBX ? -1.f : 1.f;
                    }
                    else
                    {
                        direction = centerBX < centerAX ? -1.f : 1.f;
                    }

                    movingTransform->MoveBy(
                        direction * overlapX,
                        0.f
                    );
                }
                else
                {
                    float direction;

                    if (movingCollider == colliderA)
                    {
                        direction = centerAY < centerBY ? -1.f : 1.f;
                    }
                    else
                    {
                        direction = centerBY < centerAY ? -1.f : 1.f;
                    }

                    movingTransform->MoveBy(
                        0.f,
                        direction * overlapY
                    );
                }

                
                if (bodyA != nullptr && !bodyA->GetKinematic())
                {
                    auto velocity = bodyA->GetLinearVelocity();

                    if (overlapX < overlapY)
                    {
                        velocity.x = 0.f;
                    }
                    else
                    {
                        velocity.y = 0.f;
                    }

                    bodyA->SetLinearVelocity(velocity);
                }

                if (bodyB != nullptr && !bodyB->GetKinematic())
                {
                    auto velocity = bodyB->GetLinearVelocity();

                    if (overlapX < overlapY)
                    {
                        velocity.x = 0.f;
                    }
                    else
                    {
                        velocity.y = 0.f;
                    }

                    bodyB->SetLinearVelocity(velocity);
                }

                Collision collision(
                    colliderA,
                    colliderB,
                    intersection
                );

                colliderA->OnCollision(collision);
                colliderB->OnCollision(collision);
            }
        }

        
        for (auto it = triggersEnteredPair.begin();
            it != triggersEnteredPair.end();)
        {
            ColliderComponent* first = it->first;
            ColliderComponent* second = it->second;

            if (first == nullptr || second == nullptr ||
                !first->bounds.intersects(second->bounds))
            {
                if (first != nullptr && second != nullptr)
                {
                    Trigger trigger(first, second);

                    first->OnTriggerExit(trigger);
                    second->OnTriggerExit(trigger);
                }

                it = triggersEnteredPair.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void PhysicsSystem::Subscribe(ColliderComponent* collider)
    {
        if (collider == nullptr)
        {
            return;
        }

        if (std::find(
            colliders.begin(),
            colliders.end(),
            collider) == colliders.end())
        {
            colliders.push_back(collider);
        }
    }

    void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
    {
        colliders.erase(
            std::remove(
                colliders.begin(),
                colliders.end(),
                collider
            ),
            colliders.end()
        );

        for (auto it = triggersEnteredPair.begin();
            it != triggersEnteredPair.end();)
        {
            if (it->first == collider || it->second == collider)
            {
                it = triggersEnteredPair.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}