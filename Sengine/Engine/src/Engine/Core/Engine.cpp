#include "Globals.h"
#include "Engine.h"

#include "Engine/Core/Time.h"
#include "Engine/Render/Render.h"

namespace Sengine::Engine
{
	namespace
	{
		EngineContext m_Context;
		std::vector<std::unique_ptr<ILayer>> m_Layers;

		void Create(const EngineSpecification& specification)
		{
			m_Context.Window = std::make_unique<Window>(specification);
			Raylib::InitAudioDevice();

			Time::Init();
			Renderer2D::Init(specification.Render);

			for (auto& layer : m_Layers)
			{
				layer->OnCreate();
			}
		}

		void Tick()
		{
			Time::CalculateDeltaTime();

			m_Context.Window->PollEvents();

			for (auto& layer : m_Layers)
			{
				layer->OnTick(Time::GetDeltaTime());
			}
		}

		void Destroy()
		{
			for (auto& layer : m_Layers)
			{
				layer->OnDestroy();
			}

			m_Layers.clear();
			Raylib::CloseAudioDevice();
			Renderer2D::Destroy();
		}

	}//namespace

	void CreateAndRun(const EngineSpecification& specification, std::vector<std::unique_ptr<ILayer>> layers)
	{
		m_Layers = std::move(layers);

		Create(specification);

		while (m_Context.Window->GetIsWindowRunning())
		{
			Tick();
		}

		Destroy();
	}

	EngineContext& GetContext()
	{
		return m_Context;
	}

	void Quit()
	{
		m_Context.Window->RequestClose();
	}
	//namespace
}//namespace Sengine::Engine