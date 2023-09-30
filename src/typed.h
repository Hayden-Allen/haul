#pragma once

namespace haul
{
	template<typename T>
	class typed
	{
	public:
		using type = T;
	public:
		HAUL_DCM(typed);
	protected:
		typed() {}
		virtual ~typed() {}
	};
}