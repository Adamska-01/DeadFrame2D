#pragma once


namespace DF2D::Data
{
	/**
	 * @brief The style properties engine components are allowed to set on an element.
	 */
	enum class UIStyleProperty
	{
		// Box and placement
		DISPLAY,

		POSITION,

		LEFT,

		TOP,

		RIGHT,

		BOTTOM,

		WIDTH,

		HEIGHT,

		MIN_WIDTH,

		MIN_HEIGHT,

		MAX_WIDTH,

		MAX_HEIGHT,

		MARGIN_LEFT,

		MARGIN_TOP,

		MARGIN_RIGHT,

		MARGIN_BOTTOM,

		PADDING_LEFT,

		PADDING_TOP,

		PADDING_RIGHT,

		PADDING_BOTTOM,

		OVERFLOW_X,

		OVERFLOW_Y,

		// Flex layout
		FLEX_DIRECTION,

		FLEX_WRAP,

		FLEX_GROW,

		FLEX_SHRINK,

		FLEX_BASIS,

		JUSTIFY_CONTENT,

		ALIGN_ITEMS,

		ROW_GAP,

		COLUMN_GAP,

		// Paint
		BACKGROUND_COLOR,

		DECORATOR,

		IMAGE_COLOR,

		OPACITY,

		// Text
		COLOR,

		FONT_FAMILY,

		FONT_SIZE,

		FONT_WEIGHT,

		FONT_STYLE,

		TEXT_ALIGN,

		TEXT_DECORATION,

		LINE_HEIGHT,

		WHITE_SPACE,

		// Transform
		TRANSFORM,

		TRANSFORM_ORIGIN_X,

		TRANSFORM_ORIGIN_Y,

		// Interaction and navigation
		POINTER_EVENTS,

		NAV_UP,

		NAV_DOWN,

		NAV_LEFT,

		NAV_RIGHT
	};
}