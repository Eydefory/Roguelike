
#include "pch.h"
#include "GameWorld.h"
#include "Loger.h"

#include <cassert>
#include <stdexcept>

namespace XYZEngine
{
    GameWorld* GameWorld::Instance()
    {
        static GameWorld world;
        return &world;
    }

    void GameWorld::Update(float deltaTime)
    {
        assert(deltaTime >= 0.f);

        for (auto* gameObject : gameObjects)
        {
            if (gameObject == nullptr)
            {
                LOG_WARN("Null GameObject found during Update");
                continue;
            }

            if (std::find(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(),
                          gameObject) != markedToDestroyGameObjects.end())
            {
                continue;
            }

            try
            {
                if (paused)
                {
                    gameObject->UpdateOverlay(deltaTime);
                }
                else
                {
                    gameObject->Update(deltaTime);
                }
            }
            catch (const std::exception& exception)
            {
                LOG_ERROR(std::string("GameObject Update exception: ") + exception.what());
            }
            catch (...)
            {
                LOG_ERROR("Unknown GameObject Update exception");
            }
        }

        std::vector<std::function<void()>> actions;
        actions.swap(postUpdateActions);
        for (auto& action : actions)
        {
            if (!action)
            {
                continue;
            }

            try
            {
                action();
            }
            catch (const std::exception& exception)
            {
                LOG_ERROR(std::string("Post-update action exception: ") + exception.what());
            }
            catch (...)
            {
                LOG_ERROR("Unknown post-update action exception");
            }
        }
    }

    void GameWorld::FixedUpdate(float deltaTime)
    {
        if (paused)
        {
            return;
        }

        assert(deltaTime >= 0.f);

        try
        {
            fixedCounter += deltaTime;

            float fixedDeltaTime = PhysicsSystem::Instance()->GetFixedDeltaTime();

            if (fixedDeltaTime <= 0.f)
            {
                LOG_ERROR("Invalid physics fixed delta time");
                return;
            }

            while (fixedCounter >= fixedDeltaTime)
            {
                fixedCounter -= fixedDeltaTime;

                try
                {
                    PhysicsSystem::Instance()->Update();
                }
                catch (const std::exception& exception)
                {
                    LOG_ERROR(std::string("Physics exception: ") + exception.what());
                }
                catch (...)
                {
                    LOG_ERROR("Unknown Physics exception");
                }
            }
        }
        catch (const std::exception& exception)
        {
            LOG_ERROR(std::string("FixedUpdate exception: ") + exception.what());
        }
        catch (...)
        {
            LOG_ERROR("Unknown FixedUpdate exception");
        }
    }

    void GameWorld::Render()
    {
        for (auto* gameObject : gameObjects)
        {
            if (gameObject == nullptr)
            {
                LOG_WARN("Null GameObject found during Render");
                continue;
            }

            if (std::find(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(),
                          gameObject) != markedToDestroyGameObjects.end())
            {
                continue;
            }

            try
            {
                gameObject->Render();
            }
            catch (const std::exception& exception)
            {
                LOG_ERROR(std::string("Render exception: ") + exception.what());
            }
            catch (...)
            {
                LOG_ERROR("Unknown Render exception");
            }
        }

        for (auto* gameObject : gameObjects)
        {
            if (gameObject == nullptr ||
                std::find(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(),
                          gameObject) != markedToDestroyGameObjects.end())
            {
                continue;
            }

            try
            {
                gameObject->RenderOverlay();
            }
            catch (const std::exception& exception)
            {
                LOG_ERROR(std::string("Overlay render exception: ") + exception.what());
            }
            catch (...)
            {
                LOG_ERROR("Unknown overlay render exception");
            }
        }
    }

    void GameWorld::LateUpdate()
    {
        for (int i = static_cast<int>(markedToDestroyGameObjects.size()) - 1; i >= 0; --i)
        {
            DestroyGameObjectImmediate(markedToDestroyGameObjects[i]);
        }
    }

    GameObject* GameWorld::CreateGameObject()
    {
        auto* newGameObject = new GameObject();

        gameObjects.push_back(newGameObject);

        LOG_INFO("GameObject created");

        return newGameObject;
    }

    GameObject* GameWorld::CreateGameObject(std::string name)
    {
        if (name.empty())
            LOG_WARN("GameObject created with empty name");

        auto* newGameObject = new GameObject(name);

        gameObjects.push_back(newGameObject);

        LOG_INFO("GameObject created: " + name);

        return newGameObject;
    }

    void GameWorld::MoveGameObjectToEnd(GameObject* gameObject)
    {
        const auto object = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (gameObject == nullptr || object == gameObjects.end())
        {
            LOG_WARN("MoveGameObjectToEnd received an unregistered GameObject");
            return;
        }

        gameObjects.erase(object);
        gameObjects.push_back(gameObject);
    }

    void GameWorld::DestroyGameObject(GameObject* gameObject)
    {
        if (gameObject == nullptr)
        {
            LOG_WARN("DestroyGameObject received null pointer");
            return;
        }

        if (std::find(gameObjects.begin(), gameObjects.end(), gameObject) == gameObjects.end())
        {
            LOG_WARN("GameObject is not registered in GameWorld");
            return;
        }

        if (std::find(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(),
                      gameObject) != markedToDestroyGameObjects.end())
        {
            LOG_WARN("GameObject already marked for destruction");
            return;
        }

        markedToDestroyGameObjects.push_back(gameObject);

        LOG_INFO("GameObject marked for destruction");
    }

    void GameWorld::Clear()
    {
        ClearExcept(nullptr);
        paused = false;
    }

    void GameWorld::ClearExcept(GameObject* preservedRoot)
    {
        ClearExcept(preservedRoot, nullptr);
    }

    void GameWorld::ClearExcept(GameObject* firstPreservedRoot, GameObject* secondPreservedRoot)
    {
        LOG_INFO(firstPreservedRoot == nullptr && secondPreservedRoot == nullptr
                     ? "Clearing GameWorld"
                     : "Clearing GameWorld except preserved roots");

        if (std::find(gameObjects.begin(), gameObjects.end(), firstPreservedRoot) ==
            gameObjects.end())
        {
            firstPreservedRoot = nullptr;
        }
        if (std::find(gameObjects.begin(), gameObjects.end(), secondPreservedRoot) ==
            gameObjects.end())
        {
            secondPreservedRoot = nullptr;
        }

        for (int i = static_cast<int>(gameObjects.size()) - 1; i >= 0; --i)
        {
            auto* gameObject = gameObjects[i];

            if (gameObject == nullptr || gameObject == firstPreservedRoot ||
                gameObject == secondPreservedRoot)
                continue;

            auto* transform = gameObject->GetComponent<TransformComponent>();

            if (transform == nullptr)
            {
                LOG_WARN("GameObject has no TransformComponent");
                DestroyGameObjectImmediate(gameObject);
                continue;
            }

            if (transform->GetParent() == nullptr)
                DestroyGameObjectImmediate(gameObject);
        }

        markedToDestroyGameObjects.erase(
            std::remove_if(
                markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(),
                [firstPreservedRoot, secondPreservedRoot](GameObject* object)
                { return object == firstPreservedRoot || object == secondPreservedRoot; }),
            markedToDestroyGameObjects.end());

        fixedCounter = 0.f;
        postUpdateActions.clear();

        LOG_INFO("GameWorld cleared");
    }

    void GameWorld::ScheduleAfterUpdate(std::function<void()> action)
    {
        if (action)
        {
            postUpdateActions.push_back(std::move(action));
        }
    }

    void GameWorld::SetPaused(bool shouldPause)
    {
        paused = shouldPause;
    }

    bool GameWorld::IsPaused() const
    {
        return paused;
    }

    void GameWorld::Print() const
    {
        for (auto* gameObject : gameObjects)
        {
            if (gameObject == nullptr)
                continue;

            auto* transform = gameObject->GetComponent<TransformComponent>();

            if (transform == nullptr)
                continue;

            if (transform->GetParent() == nullptr)
                gameObject->Print();
        }
    }

    void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
    {
        if (gameObject == nullptr)
            return;

        auto* transform = gameObject->GetComponent<TransformComponent>();

        if (transform == nullptr)
        {
            LOG_WARN("Destroying GameObject without Transform");

            gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject),
                              gameObjects.end());

            markedToDestroyGameObjects.erase(std::remove(markedToDestroyGameObjects.begin(),
                                                         markedToDestroyGameObjects.end(),
                                                         gameObject),
                                             markedToDestroyGameObjects.end());

            delete gameObject;
            return;
        }

        auto parent = transform->GetParent();

        if (parent != nullptr)
            parent->GetGameObject()->RemoveChild(gameObject);

        auto transforms = gameObject->GetComponentsInChildren<TransformComponent>();

        for (auto* childTransform : transforms)
        {
            if (childTransform == nullptr)
                continue;

            auto* objectToDelete = childTransform->GetGameObject();

            gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), objectToDelete),
                              gameObjects.end());

            markedToDestroyGameObjects.erase(std::remove(markedToDestroyGameObjects.begin(),
                                                         markedToDestroyGameObjects.end(),
                                                         objectToDelete),
                                             markedToDestroyGameObjects.end());

            delete objectToDelete;
        }

        LOG_INFO("GameObject destroyed");
    }
} // namespace XYZEngine
