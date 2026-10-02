#include "Globals.h"
#include "AudioSystem.h"

#include "Engine/Game/World.h"
#include "Engine/Game/Components.h"
#include "Engine/Game/Audio/AudioClip.h"

namespace Sengine
{
	void AudioSystem::OnCreate(World& world)
	{
		auto& registry = world.GetRegistry();
		auto view = registry.view<const AudioComponent>();
		for (auto [entity, audio] : view.each())
		{
			if (audio.PlayOnCreate)
			{
				registry.emplace_or_replace<PlaySoundRequestComponent>(entity);
			}
		}
	}

	void AudioSystem::OnTick(World& world, float deltaTime)
	{
		auto& registry = world.GetRegistry();

		auto view = registry.view<const IdentificationComponent, const AudioComponent, const PlaySoundRequestComponent>();
		for (auto [entity, id, audio] : view.each())
		{
			if (!id.IsActive || !audio.Clip)
			{
				continue;
			}

			audio.Clip->SetVolume(audio.Volume);
			audio.Clip->SetPitch(audio.Pitch);
			audio.Clip->Play();
		}

		//Every request is handled exactly once, then thrown away.
		registry.clear<PlaySoundRequestComponent>();
	}

	void AudioSystem::OnDestroy(World& world)
	{
		//Don't let sounds keep playing into the next world.
		auto view = world.GetRegistry().view<const AudioComponent>();
		for (auto [entity, audio] : view.each())
		{
			if (audio.Clip)
			{
				audio.Clip->Stop();
			}
		}
	}
}//namespace Sengine