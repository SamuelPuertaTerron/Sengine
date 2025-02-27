/**
 *	Filename: Editor.h
 *	Date Created: 21/02/25
 *	Purpose: The main sengine editor functionality
 *	Last Modified: N/A
 *	Changes:	   N/A
 ***/
#pragma once

namespace SengineEditor
{
	using namespace Sengine;

	class Editor : public ISengineApp
	{
	public:
		Editor() = default;
		virtual ~Editor() override = default;

		[[nodiscard]] virtual WindowDescription& GetWindowDescription() override;
		[[nodiscard]] virtual bool OnEarlyInit() override;
		[[nodiscard]] virtual bool OnInit() override;
		virtual void OnTick() override;
		virtual void OnDestroy() override;
		virtual void OnLateDestroy() override;

	private:
		WindowDescription m_WindowDescription;
	};
}
