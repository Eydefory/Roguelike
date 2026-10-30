#pragma once

#include "EngineAPI.h"
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace XYZEngine
{
    class XYZENGINE_API ResourceSystem
    {
    public:
        static ResourceSystem* Instance();

        void LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth = true);
        const sf::Texture* GetTextureShared(const std::string& name) const;
        sf::Texture* GetTextureCopy(const std::string& name) const;
        void DeleteSharedTexture(const std::string& name);

        void LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth = true);
        const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
        sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
        int GetTextureMapElementsCount(const std::string& name) const;
        void DeleteSharedTextureMap(const std::string& name);

        void LoadSound(const std::string& name, const std::string& sourcePath);
        const sf::SoundBuffer* GetSoundShared(const std::string& name) const;

        void Clear();

    private:
        std::map<std::string, sf::Texture*> textures;
        std::map<std::string, std::vector<sf::Texture*>> textureMaps;
        std::map<std::string, sf::SoundBuffer*> sounds;

        ResourceSystem() {}
        ~ResourceSystem() {}

        ResourceSystem(const ResourceSystem&) = delete;
        ResourceSystem& operator=(const ResourceSystem&) = delete;

        void DeleteAllTextures();
        void DeleteAllTextureMaps();
        void DeleteAllSounds();
    };
}