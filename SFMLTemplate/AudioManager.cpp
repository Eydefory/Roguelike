#include "AudioManager.h"

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
        music.setVolume(40.f);
        music.play();
    }

    void AudioManager::StopMusic()
    {
        music.stop();
    }

    void AudioManager::SetSoundBuffer(const std::string& name, const sf::SoundBuffer* buffer)
    {
        if (buffer == nullptr)
            return;

        auto sound = std::make_unique<sf::Sound>();
        sound->setBuffer(*buffer);

        sounds[name] = std::move(sound);
    }

    void AudioManager::PlaySound(const std::string& name)
    {
        auto sound = sounds.find(name);

        if (sound != sounds.end())
            sound->second->play();
    }
}