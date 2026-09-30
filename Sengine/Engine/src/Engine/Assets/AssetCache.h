#pragma once

#include <optional>

namespace Sengine::Assets
{
	template<typename TAsset, typename TTraits>
	class AssetCache
	{
	public:
		AssetCache() = default;
		~AssetCache() = default;

		std::shared_ptr<TAsset> Get(const fs::path& path)
		{
			// Check if the asset is still alive in the cache
			if (auto it = m_Assets.find(path); it != m_Assets.end())
			{
				if (auto existing = it->second.lock())
				{
					return existing;
				}
			}

			std::shared_ptr<TAsset> asset = TTraits::Load(path);
			if (!asset || !TTraits::IsValid(*asset))
			{
				throw std::runtime_error(
					std::format("Failed to load asset at path {}", path.string()));
			}

			m_Assets[path] = asset;
			return asset;
		}

		std::optional<fs::path> FindPath(const TAsset& asset) const
		{
			for (const auto& [path, weak] : m_Assets)
			{
				if (auto locked = weak.lock(); locked.get() == &asset)
				{
					return path;
				}
			}
			return std::nullopt;
		}

		void Clear()
		{
			m_Assets.clear();
		}

		void CollectGarbage()
		{
			for (auto it = m_Assets.begin(); it != m_Assets.end(); )
			{
				it = it->second.expired() ? m_Assets.erase(it) : std::next(it);
			}
		}

	private:
		struct PathHash
		{
			size_t operator()(const fs::path& p) const noexcept
			{
				return fs::hash_value(p);
			}
		};

		std::unordered_map<fs::path, std::weak_ptr<TAsset>, PathHash> m_Assets;
	};
}//namespace Sengine::Assets