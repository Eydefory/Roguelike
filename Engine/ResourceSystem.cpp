#include "pch.h"
#include "ResourceSystem.h"
#include "Loger.h"

#include <cassert>

namespace XYZEngine
{
    ResourceSystem* ResourceSystem::Instance()
    {
        static ResourceSystem resourceSystem;
        return &resourceSystem;
    }

    void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
    {
        assert(!name.empty());

        if (textures.find(name) != textures.end())
        {
            LOG_WARN("Texture already loaded: " + name);
            return;
        }

        auto* newTexture = new sf::Texture();

        if (newTexture->loadFromFile(sourcePath))
        {
            newTexture->setSmooth(isSmooth);
            textures.emplace(name, newTexture);

            LOG_INFO("Texture loaded: " + name);
        }
        else
        {
            delete newTexture;

            LOG_ERROR("Texture loading failed: " + sourcePath);
        }
    }

    const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
    {
        auto texture = textures.find(name);

        if (texture == textures.end())
        {
            LOG_WARN("Texture not found: " + name);
            return nullptr;
        }

        return texture->second;
    }

    sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
    {
        const sf::Texture* texture = GetTextureShared(name);

        if (texture == nullptr)
            return nullptr;

        return new sf::Texture(*texture);
    }

    void ResourceSystem::DeleteSharedTexture(const std::string& name)
    {
        auto texturePair = textures.find(name);

        if (texturePair == textures.end())
        {
            LOG_WARN("Texture not found for deletion: " + name);
            return;
        }

        delete texturePair->second;
        textures.erase(texturePair);

        LOG_INFO("Texture deleted: " + name);
    }

    void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath,
                                        sf::Vector2u elementPixelSize, int totalElements,
                                        bool isSmooth)
    {
        assert(!name.empty());

        if (textureMaps.find(name) != textureMaps.end())
        {
            LOG_WARN("Texture map already loaded: " + name);
            return;
        }

        if (elementPixelSize.x == 0 || elementPixelSize.y == 0 || totalElements <= 0)
        {
            LOG_ERROR("Invalid texture map parameters");
            return;
        }

        sf::Texture textureMap;

        if (!textureMap.loadFromFile(sourcePath))
        {
            LOG_ERROR("Texture map loading failed: " + sourcePath);
            return;
        }

        auto textureSize = textureMap.getSize();

        if (textureSize.x < elementPixelSize.x || textureSize.y < elementPixelSize.y)
        {
            LOG_ERROR("Texture map dimensions are invalid");
            return;
        }

        std::vector<sf::Texture*> elements;
        int loadedElements = 0;

        for (unsigned int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
        {
            for (unsigned int x = 0; x <= textureSize.x - elementPixelSize.x;
                 x += elementPixelSize.x)
            {
                if (loadedElements >= totalElements)
                    break;

                auto* texture = new sf::Texture();

                if (texture->loadFromFile(sourcePath,
                                          sf::IntRect(static_cast<int>(x), static_cast<int>(y),
                                                      static_cast<int>(elementPixelSize.x),
                                                      static_cast<int>(elementPixelSize.y))))
                {
                    texture->setSmooth(isSmooth);
                    elements.push_back(texture);
                }
                else
                {
                    delete texture;
                    LOG_WARN("Texture map element loading failed");
                }

                ++loadedElements;
            }

            if (loadedElements >= totalElements)
                break;
        }

        textureMaps.emplace(name, std::move(elements));

        LOG_INFO("Texture map loaded: " + name);
    }

    const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name,
                                                                  int elementIndex) const
    {
        auto textureMap = textureMaps.find(name);

        if (textureMap == textureMaps.end() || elementIndex < 0 ||
            elementIndex >= static_cast<int>(textureMap->second.size()))
        {
            LOG_WARN("Texture map element not found");
            return nullptr;
        }

        return textureMap->second[elementIndex];
    }

    sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name,
                                                          int elementIndex) const
    {
        const sf::Texture* texture = GetTextureMapElementShared(name, elementIndex);

        if (texture == nullptr)
            return nullptr;

        return new sf::Texture(*texture);
    }

    int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
    {
        auto textureMap = textureMaps.find(name);

        if (textureMap == textureMaps.end())
            return 0;

        return static_cast<int>(textureMap->second.size());
    }

    void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
    {
        auto textureMap = textureMaps.find(name);

        if (textureMap == textureMaps.end())
            return;

        for (auto* texture : textureMap->second)
            delete texture;

        textureMaps.erase(textureMap);

        LOG_INFO("Texture map deleted: " + name);
    }

    void ResourceSystem::LoadSound(const std::string& name, const std::string& sourcePath)
    {
        assert(!name.empty());

        if (sounds.find(name) != sounds.end())
        {
            LOG_WARN("Sound already loaded: " + name);
            return;
        }

        auto* buffer = new sf::SoundBuffer();

        if (buffer->loadFromFile(sourcePath))
        {
            sounds.emplace(name, buffer);
            LOG_INFO("Sound loaded: " + name);
        }
        else
        {
            delete buffer;
            LOG_ERROR("Sound loading failed: " + sourcePath);
        }
    }

    const sf::SoundBuffer* ResourceSystem::GetSoundShared(const std::string& name) const
    {
        auto sound = sounds.find(name);

        if (sound == sounds.end())
        {
            LOG_WARN("Sound not found: " + name);
            return nullptr;
        }

        return sound->second;
    }

    void ResourceSystem::Clear()
    {
        LOG_INFO("Clearing resources");

        DeleteAllTextures();
        DeleteAllTextureMaps();
        DeleteAllSounds();

        LOG_INFO("Resources cleared");
    }

    void ResourceSystem::DeleteAllTextures()
    {
        for (auto& texture : textures)
            delete texture.second;

        textures.clear();
    }

    void ResourceSystem::DeleteAllTextureMaps()
    {
        for (auto& map : textureMaps)
        {
            for (auto* texture : map.second)
                delete texture;
        }

        textureMaps.clear();
    }

    void ResourceSystem::DeleteAllSounds()
    {
        for (auto& sound : sounds)
            delete sound.second;

        sounds.clear();
    }
} // namespace XYZEngine