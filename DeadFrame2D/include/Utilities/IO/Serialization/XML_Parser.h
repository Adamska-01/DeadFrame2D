#pragma once
#include <string_view>


namespace DF2D::Utilities
{
	template <typename T>
	class XML_Parser
	{
	public:
		virtual T Parse(std::string_view source) = 0;
	};
}