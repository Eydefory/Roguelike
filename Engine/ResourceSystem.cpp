#include "pch.h"
#include "ResourceSystem.h"
namespace XYZEngine
{
    ResourceSystem* ResourceSystem::Instance()
    {
        static ResourceSystem resourceSystem;
        return &resourceSystem;
    }

    void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
    {
        if (textures.find(name) != textures.end())
            return;

        sf::Texture* newTexture = new sf::Texture();

        if (newTexture->loadFromFile(sourcePath))
        {
            newTexture->setSmooth(isSmooth);
            textures.emplace(name, newTexture);
        }
        else
        {
            delete newTexture;
        }
    }

    const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
    {
        auto texture = textures.find(name);

        if (texture == textures.end())
            return nullptr;

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
            return;

        delete texturePair->second;
        textures.erase(texturePair);
    }

    void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
    {
        if (textureMaps.find(name) != textureMaps.end())
            return;

        if (elementPixelSize.x == 0 || elementPixelSize.y == 0 || totalElements <= 0)
            return;

        sf::Texture textureMap;

        if (!textureMap.loadFromFile(sourcePath))
            return;

        auto textureSize = textureMap.getSize();

        std::vector<sf::Texture*> elements;
        int loadedElements = 0;

        for (unsigned int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
        {
            for (unsigned int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
            {
                if (loadedElements >= totalElements)
                    break;

                sf::Texture* texture = new sf::Texture();

                if (texture->loadFromFile(sourcePath, sf::IntRect(
                    static_cast<int>(x),
                    static_cast<int>(y),
                    static_cast<int>(elementPixelSize.x),
                    static_cast<int>(elementPixelSize.y))))
                {
                    texture->setSmooth(isSmooth);
                    elements.push_back(texture);
                }
                else
                {
                    delete texture;
                }

                ++loadedElements;
            }

            if (loadedElements >= totalElements)
                break;
        }

        textureMaps.emplace(name, std::move(elements));
    }

    const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
    {
        auto textureMap = textureMaps.find(name);

        if (textureMap == textureMaps.end() ||
            elementIndex < 0 ||
            elementIndex >= static_cast<int>(textureMap->second.size()))
            return nullptr;

        return textureMap->second[elementIndex];
    }

    sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
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

        for (sf::Texture* texture : textureMap->second)
            delete texture;

        textureMaps.erase(textureMap);
    }

    void ResourceSystem::LoadSound(const std::string& name, const std::string& sourcePath)
    {
        if (sounds.find(name) != sounds.end())
            return;

        sf::SoundBuffer* buffer = new sf::SoundBuffer();

        if (buffer->loadFromFile(sourcePath))
            sounds.emplace(name, buffer);
        else
            delete buffer;
    }

    const sf::SoundBuffer* ResourceSystem::GetSoundShared(const std::string& name) const
    {
        auto sound = sounds.find(name);

        if (sound == sounds.end())
            return nullptr;

        return sound->second;
    }

    void ResourceSystem::Clear()
    {
        DeleteAllTextures();
        DeleteAllTextureMaps();
        DeleteAllSounds();
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
            for (sf::Texture* texture : map.second)
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
}