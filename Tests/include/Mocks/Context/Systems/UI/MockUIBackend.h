#pragma once
#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct MockUIBackend : DF2D::Core::IUIBackend
{
	// Configurable results
	DF2D::Data::UIContextID nextContextId{1};

	DF2D::Data::UIElementID nextElementId{100};

	bool failNextContext{false};

	bool failNextElement{false};

	bool failNextFontLoad{false};

	DF2D::Core::RectF elementRect{};


	// Call tracking
	DF2D::Core::IUIEventSink* sink{nullptr};

	int createContextCount{0};

	int destroyContextCount{0};

	int updateContextCount{0};

	int renderContextCount{0};

	int createElementCount{0};

	int destroyElementCount{0};

	std::vector<std::string> loadedFonts;

	std::vector<std::string> loadedFontFamilies;

	std::vector<std::string> loadedStyleSheets;

	std::unordered_set<DF2D::Data::UIContextID> liveContexts;

	std::unordered_set<DF2D::Data::UIElementID> liveElements;

	std::unordered_map<DF2D::Data::UIElementID, std::unordered_map<int, std::string>> properties;


	std::string PropertyOf(DF2D::Data::UIElementID element, DF2D::Data::UIStyleProperty property) const
	{
		auto elementIt = properties.find(element);

		if (elementIt == properties.end())
			return {};

		auto propertyIt = elementIt->second.find(static_cast<int>(property));

		return propertyIt != elementIt->second.end() ? propertyIt->second : std::string();
	}


	void SetEventSink(DF2D::Core::IUIEventSink* newSink) override
	{
		sink = newSink;
	}

	DF2D::Data::UIContextID CreateContext(DF2D::Core::Vector2I) override
	{
		createContextCount++;

		if (failNextContext) return 0;

		auto id = nextContextId++;
		liveContexts.insert(id);

		return id;
	}

	void DestroyContext(DF2D::Data::UIContextID context) override
	{
		destroyContextCount++;
		liveContexts.erase(context);
	}

	void SetContextSize(DF2D::Data::UIContextID, DF2D::Core::Vector2I) override
	{
	}

	void UpdateContext(DF2D::Data::UIContextID) override
	{
		updateContextCount++;
	}

	DF2D::Data::GeometryDrawList RenderContext(DF2D::Data::UIContextID) override
	{
		renderContextCount++;

		return {};
	}

	bool LoadStyleSheet(DF2D::Data::UIContextID, const std::string& path) override
	{
		loadedStyleSheets.push_back(path);

		return true;
	}

	DF2D::Data::UIElementID GetRootElement(DF2D::Data::UIContextID context) const override
	{
		return liveContexts.contains(context) ? context : 0;
	}

	DF2D::Data::UIElementID CreateElement(DF2D::Data::UIContextID, DF2D::Data::UIElementType) override
	{
		createElementCount++;

		if (failNextElement) return 0;

		auto id = nextElementId++;
		liveElements.insert(id);

		return id;
	}

	void DestroyElement(DF2D::Data::UIElementID element) override
	{
		destroyElementCount++;
		liveElements.erase(element);
	}

	void SetElementParent(DF2D::Data::UIElementID, DF2D::Data::UIElementID, int) override
	{
	}

	void SetElementProperty(DF2D::Data::UIElementID element, DF2D::Data::UIStyleProperty property, const std::string& value) override
	{
		properties[element][static_cast<int>(property)] = value;
	}

	void ClearElementProperty(DF2D::Data::UIElementID element, DF2D::Data::UIStyleProperty property) override
	{
		properties[element].erase(static_cast<int>(property));
	}

	void SetElementAttribute(DF2D::Data::UIElementID, DF2D::Data::UIAttribute, const std::string&) override
	{
	}

	void SetElementText(DF2D::Data::UIElementID, const std::string&) override
	{
	}

	void SetElementClass(DF2D::Data::UIElementID, const std::string&, bool) override
	{
	}

	bool HasElementClass(DF2D::Data::UIElementID, const std::string&) const override
	{
		return false;
	}

	bool HasPseudoClass(DF2D::Data::UIElementID, DF2D::Data::UIPseudoClass) const override
	{
		return false;
	}

	void SetElementVisible(DF2D::Data::UIElementID, bool) override
	{
	}

	void FocusElement(DF2D::Data::UIElementID) override
	{
	}

	DF2D::Core::RectF GetElementRect(DF2D::Data::UIElementID) const override
	{
		return elementRect;
	}

	DF2D::Core::Vector2F GetElementContentSize(DF2D::Data::UIElementID) const override
	{
		return DF2D::Core::Vector2F::Zero;
	}

	bool LoadFontFace(const std::string& path, const std::string& family, bool) override
	{
		loadedFonts.push_back(path);
		loadedFontFamilies.push_back(family);

		return !failNextFontLoad;
	}
};
