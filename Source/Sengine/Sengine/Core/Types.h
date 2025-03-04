/**
 *	Filename: Types.h
 *	Date Created: 03/03/25
 *	Purpose: Wrappers around frequently used std types, such as smart pointers. 
 ***/

#pragma once

namespace Sengine
{
	template<typename T>
	using SharedPtr = std::shared_ptr<T>;

	template<typename T, typename... Args>
	SharedPtr<T> CreateSharedPtr(const Args&&... args)
	{
		return std::make_unique<T>(args...);
	}

	template<typename T>
	using UniquePtr = std::unique_ptr<T>;

	template<typename T, typename... Args>
	UniquePtr<T> CreateUniquePtr(const Args&&... args)
	{
		return std::make_unique<T>(args...);
	}


	inline static std::string EmptyString = "NULL";

}

