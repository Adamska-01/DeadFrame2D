#pragma once
#include "Core/Math/Color.h"
#include <string>


namespace DF2D::Utilities::StyleValues
{
	namespace Internal
	{
		/** @brief std::to_string's trailing zeros, trimmed -- shared by every plain-number formatter below. */
		inline std::string TrimTrailingZeros(float value)
		{
			auto text = std::to_string(value);

			auto lastDigit = text.find_last_not_of('0');

			if (lastDigit != std::string::npos && text[lastDigit] == '.')
			{
				lastDigit--;
			}

			text.erase(lastDigit + 1);

			return text;
		}
	}

	/** @brief Formats a color as the "#rrggbbaa" literal style syntax expects. */
	inline std::string ToColor(const Core::Color& color)
	{
		constexpr char digits[] = "0123456789abcdef";

		auto literal = std::string("#");

		for (auto channel : { color.r, color.g, color.b, color.a })
		{
			literal += digits[(channel >> 4) & 0x0F];
			literal += digits[channel & 0x0F];
		}

		return literal;
	}

	/** @brief Formats a true, device-pixel length -- immune to a canvas's UI scale factor. */
	inline std::string ToPixels(float value)
	{
		return Internal::TrimTrailingZeros(value) + "px";
	}

	/**
	 * @brief Formats a device-independent-pixel length -- scales with a canvas's UI scale factor
	 * (Canvas::SetUIScaleMode / Context::SetDensityIndependentPixelRatio)
	 */
	inline std::string ToDp(float value)
	{
		return Internal::TrimTrailingZeros(value) + "dp";
	}

	/** @brief Formats a plain number, for the unitless properties such as the flex factors. */
	inline std::string ToNumber(float value)
	{
		return Internal::TrimTrailingZeros(value);
	}

	/** @brief Formats a 0-1 ratio as a percentage length. */
	inline std::string ToPercent(float normalized)
	{
		return std::to_string(normalized * 100.0f) + "%";
	}
}