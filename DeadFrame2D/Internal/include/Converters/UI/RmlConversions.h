#pragma once
#include "Data/Systems/UI/CursorType.h"
#include "Data/Systems/UI/UIAttribute.h"
#include "Data/Systems/UI/UIElementType.h"
#include "Data/Systems/UI/UIPseudoClass.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include <RmlUi/Core/Types.h>
#include <string>


namespace DF2D::Internal::RmlConversions
{
	/**
	 * @brief The markup tag an engine element type is built from.
	 */
	inline const char* ToElementTag(Data::UIElementType type)
	{
		switch (type)
		{
		case Data::UIElementType::IMAGE:			return "img";
		case Data::UIElementType::CHECKBOX:			return "input";
		case Data::UIElementType::RANGE:			return "input";
		case Data::UIElementType::TEXT_INPUT:		return "input";
		case Data::UIElementType::TEXT_AREA:		return "textarea";
		case Data::UIElementType::DROPDOWN:			return "select";
		case Data::UIElementType::DROPDOWN_OPTION:	return "option";
		case Data::UIElementType::BUTTON:			return "button";
		default:								return "div";
		}
	}

	/**
	 * @brief The input kind an element type needs, or null when the tag already says it.
	 *
	 * Several controls share one tag and are told apart by an attribute, so creating them takes both.
	 */
	inline const char* ToElementInputType(Data::UIElementType type)
	{
		switch (type)
		{
		case Data::UIElementType::CHECKBOX:		return "checkbox";
		case Data::UIElementType::RANGE:		return "range";
		case Data::UIElementType::TEXT_INPUT:	return "text";
		default:								return nullptr;
		}
	}

	/** @brief The style property name an engine property maps to. */
	inline const char* ToPropertyName(Data::UIStyleProperty property)
	{
		switch (property)
		{
		case Data::UIStyleProperty::DISPLAY:			return "display";
		case Data::UIStyleProperty::POSITION:			return "position";
		case Data::UIStyleProperty::LEFT:				return "left";
		case Data::UIStyleProperty::TOP:				return "top";
		case Data::UIStyleProperty::RIGHT:				return "right";
		case Data::UIStyleProperty::BOTTOM:				return "bottom";
		case Data::UIStyleProperty::WIDTH:				return "width";
		case Data::UIStyleProperty::HEIGHT:				return "height";
		case Data::UIStyleProperty::MIN_WIDTH:			return "min-width";
		case Data::UIStyleProperty::MIN_HEIGHT:			return "min-height";
		case Data::UIStyleProperty::MAX_WIDTH:			return "max-width";
		case Data::UIStyleProperty::MAX_HEIGHT:			return "max-height";
		case Data::UIStyleProperty::MARGIN_LEFT:		return "margin-left";
		case Data::UIStyleProperty::MARGIN_TOP:			return "margin-top";
		case Data::UIStyleProperty::MARGIN_RIGHT:		return "margin-right";
		case Data::UIStyleProperty::MARGIN_BOTTOM:		return "margin-bottom";
		case Data::UIStyleProperty::PADDING_LEFT:		return "padding-left";
		case Data::UIStyleProperty::PADDING_TOP:		return "padding-top";
		case Data::UIStyleProperty::PADDING_RIGHT:		return "padding-right";
		case Data::UIStyleProperty::PADDING_BOTTOM:		return "padding-bottom";
		case Data::UIStyleProperty::OVERFLOW_X:			return "overflow-x";
		case Data::UIStyleProperty::OVERFLOW_Y:			return "overflow-y";
		case Data::UIStyleProperty::FLEX_DIRECTION:		return "flex-direction";
		case Data::UIStyleProperty::FLEX_WRAP:			return "flex-wrap";
		case Data::UIStyleProperty::FLEX_GROW:			return "flex-grow";
		case Data::UIStyleProperty::FLEX_SHRINK:		return "flex-shrink";
		case Data::UIStyleProperty::FLEX_BASIS:			return "flex-basis";
		case Data::UIStyleProperty::JUSTIFY_CONTENT:	return "justify-content";
		case Data::UIStyleProperty::ALIGN_ITEMS:		return "align-items";
		case Data::UIStyleProperty::ROW_GAP:			return "row-gap";
		case Data::UIStyleProperty::COLUMN_GAP:			return "column-gap";
		case Data::UIStyleProperty::BACKGROUND_COLOR:	return "background-color";
		case Data::UIStyleProperty::DECORATOR:			return "decorator";
		case Data::UIStyleProperty::IMAGE_COLOR:		return "image-color";
		case Data::UIStyleProperty::OPACITY:			return "opacity";
		case Data::UIStyleProperty::COLOR:				return "color";
		case Data::UIStyleProperty::FONT_FAMILY:		return "font-family";
		case Data::UIStyleProperty::FONT_SIZE:			return "font-size";
		case Data::UIStyleProperty::FONT_WEIGHT:		return "font-weight";
		case Data::UIStyleProperty::FONT_STYLE:			return "font-style";
		case Data::UIStyleProperty::TEXT_ALIGN:			return "text-align";
		case Data::UIStyleProperty::TEXT_DECORATION:	return "text-decoration";
		case Data::UIStyleProperty::LINE_HEIGHT:		return "line-height";
		case Data::UIStyleProperty::WHITE_SPACE:		return "white-space";
		case Data::UIStyleProperty::TRANSFORM:			return "transform";
		case Data::UIStyleProperty::TRANSFORM_ORIGIN_X:	return "transform-origin-x";
		case Data::UIStyleProperty::TRANSFORM_ORIGIN_Y:	return "transform-origin-y";
		case Data::UIStyleProperty::POINTER_EVENTS:		return "pointer-events";
		case Data::UIStyleProperty::NAV_UP:				return "nav-up";
		case Data::UIStyleProperty::NAV_DOWN:			return "nav-down";
		case Data::UIStyleProperty::NAV_LEFT:			return "nav-left";
		case Data::UIStyleProperty::NAV_RIGHT:			return "nav-right";
		default:										return "";
		}
	}

	inline const char* ToAttributeName(Data::UIAttribute attribute)
	{
		switch (attribute)
		{
		case Data::UIAttribute::SOURCE:		return "src";
		case Data::UIAttribute::VALUE:		return "value";
		case Data::UIAttribute::DISABLED:	return "disabled";
		default:							return "id";
		}
	}

	inline const char* ToPseudoClassName(Data::UIPseudoClass pseudoClass)
	{
		switch (pseudoClass)
		{
		case Data::UIPseudoClass::ACTIVE:	return "active";
		case Data::UIPseudoClass::FOCUS:	return "focus";
		case Data::UIPseudoClass::DISABLED:	return "disabled";
		case Data::UIPseudoClass::CHECKED:	return "checked";
		default:							return "hover";
		}
	}

	/** @brief Maps a cursor name reported by the styling layer onto the engine's cursor vocabulary. */
	inline Data::CursorType ToCursorType(const std::string& cursorName)
	{
		if (cursorName == "pointer" || cursorName == "hand")	return Data::CursorType::HAND;
		if (cursorName == "text")								return Data::CursorType::TEXT;
		if (cursorName == "move")								return Data::CursorType::MOVE;
		if (cursorName == "cross" || cursorName == "ew-resize")	return Data::CursorType::RESIZE_HORIZONTAL;
		if (cursorName == "ns-resize")							return Data::CursorType::RESIZE_VERTICAL;
		if (cursorName == "unavailable")						return Data::CursorType::UNAVAILABLE;

		return Data::CursorType::ARROW;
	}
}
