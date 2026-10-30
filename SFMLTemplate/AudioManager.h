#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

namespace XYZRoguelike
{
    class AudioManager
    {
    public:
        static AudioManager& Instance();

        void PlayMusic(const std::string& path);
        void StopMusic();

        void PlaySound(const std::string& name);
        void SetSoundBuffer(const std::string& name, const sf::SoundBuffer* buffer);

    private:
        AudioManager() = default;

        sf::Music music;
        std::map<std::string, std::unique_ptr<sf::Sound>> sounds;
    };
}