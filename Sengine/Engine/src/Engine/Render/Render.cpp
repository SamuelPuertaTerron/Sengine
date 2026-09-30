#include "Globals.h"
#include "Render.h"
#include "Engine/Render/Texture.h" 

namespace Sengine::Renderer2D
{
	namespace
	{
		std::vector<SpriteCommand> m_Commands;
		Raylib::Camera2D m_SceneCamera{};
		std::size_t m_LastSceneCommandCount = 0;

		RenderSettings m_Settings{};
		Raylib::RenderTexture2D m_Canvas{};		
		Raylib::Rectangle m_Viewport{};		
		float m_Scale = 1.0f;

		Raylib::Rectangle m_HostRect{};			
		bool m_bHasHostRect = false;

		bool m_bFrameInProgress = false;
		bool m_bSceneInProgress = false;
		bool m_bPresented = false;

		void UpdateViewport()
		{
			//Fit into the host rect if one is set, otherwise the whole window.
			const Raylib::Rectangle host = m_bHasHostRect
				? m_HostRect
				: Raylib::Rectangle(0.0f, 0.0f,
					static_cast<float>(Raylib::GetScreenWidth()),
					static_cast<float>(Raylib::GetScreenHeight()));

			const float virtualWidth = static_cast<float>(m_Settings.VirtualWidth);
			const float virtualHeight = static_cast<float>(m_Settings.VirtualHeight);

			//Largest scale that fits both dimensions keeps the aspect ratio.
			float scale = std::min(host.width / virtualWidth, host.height / virtualHeight);

			//Integer scaling keeps pixels square and equal-sized. If the host is
			//smaller than the canvas we have to shrink, so fall back to fractional.
			if (m_Settings.Scaling == ScaleMode::Integer && scale >= 1.0f)
			{
				scale = std::floor(scale);
			}

			if (scale <= 0.0f)
			{
				scale = 1.0f;
			}

			m_Scale = scale;

			const float width = virtualWidth * scale;
			const float height = virtualHeight * scale;

			m_Viewport = Raylib::Rectangle(
				std::floor(host.x + (host.width - width) * 0.5f),
				std::floor(host.y + (host.height - height) * 0.5f),
				width,
				height);
		}

		//Makes GetMousePosition() (and therefore raygui and your game code)
		//report positions in virtual pixels while drawing into the canvas.
		void UseVirtualMouse()
		{
			Raylib::SetMouseOffset(static_cast<int>(-m_Viewport.x), static_cast<int>(-m_Viewport.y));
			Raylib::SetMouseScale(1.0f / m_Scale, 1.0f / m_Scale);
		}

		void UseWindowMouse()
		{
			Raylib::SetMouseOffset(0, 0);
			Raylib::SetMouseScale(1.0f, 1.0f);
		}

		void SortCommands()
		{
			std::stable_sort(m_Commands.begin(), m_Commands.end(),
				[](const SpriteCommand& a, const SpriteCommand& b)
				{
					if (a.Layer != b.Layer)
					{
						return a.Layer < b.Layer;
					}
					//Secondary sort by GPU id so identical textures are
					//contiguous and raylib can batch the draw calls.
					return a.Texture->GetRawTexture().id < b.Texture->GetRawTexture().id;
				});
		}

		void Flush()
		{
			Raylib::BeginMode2D(m_SceneCamera);

			for (const SpriteCommand& command : m_Commands)
			{
				const Raylib::Texture& rawTexture = command.Texture->GetRawTexture();

				Raylib::Rectangle source = command.Source;

				//Zero-sized source means "use the whole texture".
				if (source.width == 0.0f && source.height == 0.0f)
				{
					source.width = static_cast<float>(rawTexture.width);
					source.height = static_cast<float>(rawTexture.height);
				}

				Raylib::DrawTexturePro(rawTexture, source, command.Dest,
					command.Origin, command.Rotation, command.Tint);
			}

			Raylib::EndMode2D();
		}
	}//namespace

	void Init(const RenderSettings& settings, std::size_t reserveCommandCount)
	{
		assert(settings.VirtualWidth > 0 && settings.VirtualHeight > 0 && "Virtual resolution must be positive");

		m_Settings = settings;
		m_Commands.reserve(reserveCommandCount);

		//Must run after InitWindow (needs a GL context).
		m_Canvas = Raylib::LoadRenderTexture(m_Settings.VirtualWidth, m_Settings.VirtualHeight);

		//Nearest-neighbour when scaling up: hard pixel edges, no blur.
		Raylib::SetTextureFilter(m_Canvas.texture, Raylib::TEXTURE_FILTER_POINT);

		UpdateViewport();
	}

	void Destroy()
	{
		m_Commands.clear();
		m_Commands.shrink_to_fit();

		if (m_Canvas.id != 0)
		{
			Raylib::UnloadRenderTexture(m_Canvas);
			m_Canvas = {};
		}

		m_bHasHostRect = false;
	}

	void BeginFrame(Raylib::Color clearColor)
	{
		assert(!m_bFrameInProgress && "BeginFrame called twice without EndFrame");
		m_bFrameInProgress = true;
		m_bPresented = false;

		UpdateViewport();
		UseVirtualMouse();

		Raylib::BeginTextureMode(m_Canvas);
		Raylib::ClearBackground(clearColor);
	}

	void Present()
	{
		assert(m_bFrameInProgress && "Present called outside BeginFrame/EndFrame");
		assert(!m_bSceneInProgress && "Present called with a scene still open");
		assert(!m_bPresented && "Present called twice in one frame");
		m_bPresented = true;

		Raylib::EndTextureMode();
		UseWindowMouse();

		Raylib::BeginDrawing();
		Raylib::ClearBackground(m_Settings.LetterboxColor);

		if (m_bHasHostRect)
		{
			return;
		}

		const Raylib::Rectangle source(
			0.0f, 0.0f,
			static_cast<float>(m_Canvas.texture.width),
			-static_cast<float>(m_Canvas.texture.height));

		Raylib::DrawTexturePro(m_Canvas.texture, source, m_Viewport,
			Raylib::Vector2(0.0f, 0.0f), 0.0f, Raylib::Color(255, 255, 255, 255));
	}

	void EndFrame()
	{
		assert(m_bFrameInProgress && "EndFrame called without BeginFrame");
		assert(!m_bSceneInProgress && "EndFrame called with a scene still open");

		if (!m_bPresented)
		{
			Present();
		}

		m_bFrameInProgress = false;
		Raylib::EndDrawing();
	}

	void BeginScene(const Raylib::Camera2D& camera)
	{
		assert(m_bFrameInProgress && "BeginScene called outside BeginFrame/EndFrame");
		assert(!m_bPresented && "BeginScene called after Present; scenes draw into the virtual canvas");
		assert(!m_bSceneInProgress && "BeginScene called twice without EndScene");
		m_bSceneInProgress = true;

		m_SceneCamera = camera;
		m_Commands.clear();
	}

	void Submit(const SpriteCommand& command)
	{
		assert(m_bSceneInProgress && "Submit called outside BeginScene/EndScene");
		assert(command.Texture != nullptr && "Submit called with a null texture");
		assert(command.Texture->IsValid() && "Submit called with an invalid texture");
		m_Commands.push_back(command);
	}

	void EndScene()
	{
		assert(m_bSceneInProgress && "EndScene called without BeginScene");
		m_bSceneInProgress = false;

		SortCommands();
		Flush();

		m_LastSceneCommandCount = m_Commands.size();
		m_Commands.clear();
	}

	void SetHostRect(Raylib::Rectangle windowRect)
	{
		m_HostRect = windowRect;
		m_bHasHostRect = true;

		UpdateViewport();
	}

	void ClearHostRect()
	{
		m_bHasHostRect = false;
		UpdateViewport();
	}

	const Raylib::Texture& GetCanvasTexture()
	{
		return m_Canvas.texture;
	}

	Raylib::Vector2 GetVirtualSize()
	{
		return Raylib::Vector2(
			static_cast<float>(m_Settings.VirtualWidth),
			static_cast<float>(m_Settings.VirtualHeight));
	}

	Raylib::Rectangle GetViewport()
	{
		return m_Viewport;
	}

	float GetScale()
	{
		return m_Scale;
	}

	Raylib::Vector2 WindowToVirtual(Raylib::Vector2 windowPosition)
	{
		return Raylib::Vector2(
			(windowPosition.x - m_Viewport.x) / m_Scale,
			(windowPosition.y - m_Viewport.y) / m_Scale);
	}

	Raylib::Rectangle GetVisibleWorldRect(const Raylib::Camera2D& camera)
	{
		const Raylib::Vector2 topLeft = Raylib::GetScreenToWorld2D(Raylib::Vector2(0.0f, 0.0f), camera);
		const Raylib::Vector2 bottomRight = Raylib::GetScreenToWorld2D(GetVirtualSize(), camera);

		return Raylib::Rectangle(topLeft.x, topLeft.y,
			bottomRight.x - topLeft.x,
			bottomRight.y - topLeft.y);
	}

	std::size_t GetLastSceneCommandCount()
	{
		return m_LastSceneCommandCount;
	}
}//namespace Sengine::Renderer2D