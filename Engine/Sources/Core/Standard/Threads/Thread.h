#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Result.h"
#include "Platform/Platform.h"

namespace Orion::Engine
{
	/// @brief Enumeration describing type of an error encountered when performing Thread operations.
	enum class ThreadError : UInt8
	{
	};

	/// @brief Result type for Thread operations.
	/// @tparam T Expected type to be stored.
	template <typename T>
	using ThreadResult = Result<T, ThreadError>;

	/// @brief Represents an identifier of a thread.
	using ThreadID = UInt64;

	using ThreadFunction = void (*)(void*);

	/// @brief Represents a single thread of execution. It can be used for running multiple functions concurrently.
	/// @see https://en.wikipedia.org/wiki/Thread_(computing)
	class Thread final
	{
		private:
		Platform::PlatformThreadHandle handle;

		public:
		Thread() noexcept;
		Thread(ThreadFunction function, void* data) noexcept;
		Thread(const Thread&) noexcept = delete;
		Thread(Thread&& other) noexcept;
		~Thread() noexcept;

		Thread& operator=(const Thread&) noexcept = delete;
		Thread& operator=(Thread&& other) noexcept;
	};
}  // namespace Orion::Engine
