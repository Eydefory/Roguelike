#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>

namespace XYZRoguelike
{
    class AudioManager
    {
    public:
        static AudioManager& Instance();

        void PlayMusic(const std::string& path);
        void StopMusic();
        void SetVolume(float volume);
        float GetVolume() const;

        void PlaySound(const std::string& name);
        void SetSoundBuffer(const std::string& name, const sf::SoundBuffer* buffer);

    private:
        AudioManager() = default;

        sf::Music music;
        std::map<std::string, std::unique_ptr<sf::Sound>> sounds;
        float volume = 40.f;
    };
} // namespace XYZRoguelike
