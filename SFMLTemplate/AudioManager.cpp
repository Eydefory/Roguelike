#include "AudioManager.h"

#include <algorithm>

namespace XYZRoguelike
{
    AudioManager& AudioManager::Instance()
    {
        static AudioManager instance;
        return instance;
    }

    void AudioManager::PlayMusic(const std::string& path)
    {
        if (!music.openFromFile(path))
            return;

        music.setLoop(true);
        music.setVolume(volume);
        music.play();
    }

    void AudioManager::StopMusic()
    {
        music.stop();
    }

    void AudioManager::SetVolume(float newVolume)
    {
        volume = std::clamp(newVolume, 0.f, 100.f);
        music.setVolume(volume);
        for (auto& sound : sounds)
        {
            if (sound.second != nullptr)
            {
                sound.second->setVolume(volume);
            }
        }
    }

    float AudioManager::GetVolume() const
    {
        return volume;
    }

    void AudioManager::SetSoundBuffer(const std::string& name, const sf::SoundBuffer* buffer)
    {
        if (buffer == nullptr)
            return;

        auto sound = std::make_unique<sf::Sound>();
        sound->setBuffer(*buffer);
        sound->setVolume(volume);

        sounds[name] = std::move(sound);
    }

    void AudioManager::PlaySound(const std::string& name)
    {
        auto sound = sounds.find(name);

        if (sound != sounds.end())
            sound->second->play();
    }
} // namespace XYZRoguelike
