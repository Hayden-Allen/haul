#pragma once

namespace haul
{
	template<typename T>
	class parent : public T
	{
	public:
		using super = T;
	public:
		HAUL_DCM(parent);
	protected:
		parent() {}
		virtual ~parent() {}
	};
}