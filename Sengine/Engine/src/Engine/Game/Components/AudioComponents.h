#pragma once

namespace Sengine
{
	class AudioClip;

	struct AudioComponent
	{
		std::shared_ptr<AudioClip> Clip;
		float Volume{ 1.0f };
		float Pitch{ 1.0f };
		bool PlayOnCreate{ false };	//Play once when the world starts.
	};

	//Add this to an entity to play its AudioComponent once.
	//The AudioSystem consumes it at the end of the tick.
	struct PlaySoundRequestComponent {};
}//namespace Sengine