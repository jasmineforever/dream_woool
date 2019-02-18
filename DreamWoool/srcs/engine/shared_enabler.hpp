#pragma once
namespace DW
{
	template<typename  T>
	struct SharedEnabler : public T
	{
		template <typename... Args>
		SharedEnabler(Args &&... args)
			:T(std::forward<Args>(args)...)
		{
		}
	};
};