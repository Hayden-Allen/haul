#pragma once

namespace haul
{
	template<typename T>
	class parent : public T
	{
	public:
		typedef T super;
	public:
		HAUL_DCM(parent);
	protected:
		template<typename... ARGS>
		parent(ARGS&&... args) :
			T(std::move(args)...)
		{}
		virtual ~parent() {}
	};
} // namespace haul
