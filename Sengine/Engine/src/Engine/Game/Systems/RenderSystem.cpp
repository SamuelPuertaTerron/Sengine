#include "Globals.h"
#include "RenderSystem.h"
#include "Engine/Game/World.h"
#include "Engine/Game/Components.h"

#include "Engine/Render/Render.h"
#include "Engine/Render/Texture.h"

namespace Sengine
{
	void RenderSystem::OnTick(World& world, float deltaTime)
	{
		Renderer2D::BeginScene(m_Camera);

		auto view = world.GetRegistry().view<const IdentificationComponent, const TransformComponent, const TextureComponent>();
		for (auto [entity, id, transform, texture] : view.each())
		{
			if (!texture.Texture || !id.IsActive)
			{
				continue;
			}

			const Raylib::Vector2 local = GetSpriteLocalSize(texture);
			const float width = local.x * transform.Scale.x;
			const float height = local.y * transform.Scale.y;

			SpriteCommand command;
			command.Texture = texture.Texture.get();
			command.Source = texture.Source;
			command.Dest = Raylib::Rectangle(transform.Position.x, transform.Position.y, width, height);
			command.Origin = Raylib::Vector2(width * 0.5f, height * 0.5f);
			command.Rotation = transform.Rotation;
			command.Tint = texture.Tint;
			command.Layer = texture.Layer;

			Renderer2D::Submit(command);
		}

		Renderer2D::EndScene();
	}
}//namespace Sengine