#pragma once
#include <Math/Vector2.h>


enum class UIAnchor
{
	TOP_LEFT,
	
	TOP_CENTER,
	
	TOP_RIGHT,
	
	CENTER_LEFT,
	
	CENTER,
	
	CENTER_RIGHT,
	
	BOTTOM_LEFT,
	
	BOTTOM_CENTER,
	
	BOTTOM_RIGHT
};


inline Vector2F GetAnchorFromPreset(UIAnchor preset)
{
	switch (preset)
	{
		case UIAnchor::TOP_LEFT:
			return { 0.0f, 0.0f };
	
		case UIAnchor::TOP_CENTER:
			return { 0.5f, 0.0f };
	
		case UIAnchor::TOP_RIGHT:
			return { 1.0f, 0.0f };
	
		case UIAnchor::CENTER_LEFT:
			return { 0.0f, 0.5f };

		case UIAnchor::CENTER:
			return { 0.5f, 0.5f };
	
		case UIAnchor::CENTER_RIGHT:
			return { 1.0f, 0.5f };
	
		case UIAnchor::BOTTOM_LEFT:
			return { 0.0f, 1.0f };
	
		case UIAnchor::BOTTOM_CENTER:
			return { 0.5f, 1.0f };
		
		case UIAnchor::BOTTOM_RIGHT:
			return { 1.0f, 1.0f };
		
		default:
			return { 0.5f, 0.5f };
	}
}