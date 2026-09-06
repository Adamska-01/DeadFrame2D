#pragma once
#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
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

	bool keyboardFocused{false};

	bool pointerOverElement{false};


	// Call tracking
	DF2D::Core::IUIEventSink* sink{nullptr};


	int advanceCount{0};

	float elapsedTime{0.0f};

	float lastAdvanceDelta{0.0f};

	/** @brief Advance calls seen when UpdateContext last ran, so ordering between them is testable. */
	int advanceCountAtLastUpdateContext{-1};

	int createContextCount{0};

	int destroyContextCount{0};

	int updateContextCount{0};

	int renderContextCount{0};

	int createElementCount{0};

	DF2D::Data::UIElementType lastCreatedType{DF2D::Data::UIElementType::PANEL};

	int destroyElementCount{0};

	std::vector<std::string> loadedFonts;

	std::vector<std::string> loadedFontFamilies;

	std::vector<std::string> loadedStyleSheets;

	int mouseMoveCount{0};

	int mouseButtonCount{0};

	int mouseWheelCount{0};

	int keyCount{0};

	int textInputCount{0};

	DF2D::Core::Vector2F lastPointerPosition{};

	DF2D::Models::MouseButtonCode lastButton{DF2D::Models::MouseButtonCode::UNKNOWN};

	bool lastButtonPressed{false};

	DF2D::Models::KeyboardKeyCode lastKey{DF2D::Models::KeyboardKeyCode::UNKNOWN};

	bool lastKeyPressed{false};

	std::string lastText;

	std::unordered_set<DF2D::Data::UIContextID> liveContexts;

	std::unordered_set<DF2D::Data::UIElementID> liveElements;

	std::unordered_map<DF2D::Data::UIElementID, std::unordered_map<int, std::string>> properties;

	std::unordered_map<DF2D::Data::UIElementID, std::unordered_map<int, std::string>> attributes;

	std::unordered_map<DF2D::Data::UIElementID, std::vector<std::pair<std::string, std::string>>> dropdownOptions;

	std::unordered_map<DF2D::Data::UIElementID, int> dropdownSelections;

	std::unordered_map<DF2D::Data::UIElementID, DF2D::Core::Vector2F> scrollOffsets;

	DF2D::Core::Vector2F scrollSize{};


	std::string AttributeOf(DF2D::Data::UIElementID element, DF2D::Data::UIAttribute attribute) const
	{
		auto elementIt = attributes.find(element);

		if (elementIt == attributes.end())
			return {};

		auto attributeIt = elementIt->second.find(static_cast<int>(attribute));

		return attributeIt != elementIt->second.end() ? attributeIt->second : std::string();
	}

	bool HasAttribute(DF2D::Data::UIElementID element, DF2D::Data::UIAttribute attribute) const
	{
		auto elementIt = attributes.find(element);

		return elementIt != attributes.end() && elementIt->second.contains(static_cast<int>(attribute));
	}


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

	void Advance(float deltaTime) override
	{
		advanceCount++;
		elapsedTime += deltaTime;
		lastAdvanceDelta = deltaTime;
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
		advanceCountAtLastUpdateContext = advanceCount;
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

	DF2D::Data::UIElementID CreateElement(DF2D::Data::UIContextID, DF2D::Data::UIElementType type) override
	{
		createElementCount++;
		lastCreatedType = type;

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

	void SetElementAttribute(DF2D::Data::UIElementID element, DF2D::Data::UIAttribute attribute, const std::string& value) override
	{
		attributes[element][static_cast<int>(attribute)] = value;
	}

	void RemoveElementAttribute(DF2D::Data::UIElementID element, DF2D::Data::UIAttribute attribute) override
	{
		attributes[element].erase(static_cast<int>(attribute));
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

	int AddDropdownOption(DF2D::Data::UIElementID dropdown, const std::string& text, const std::string& value) override
	{
		dropdownOptions[dropdown].push_back({text, value});

		return static_cast<int>(dropdownOptions[dropdown].size()) - 1;
	}

	void ClearDropdownOptions(DF2D::Data::UIElementID dropdown) override
	{
		dropdownOptions[dropdown].clear();
		dropdownSelections[dropdown] = -1;
	}

	void SetDropdownSelection(DF2D::Data::UIElementID dropdown, int index) override
	{
		dropdownSelections[dropdown] = index;
	}

	int GetDropdownSelection(DF2D::Data::UIElementID dropdown) const override
	{
		auto it = dropdownSelections.find(dropdown);

		return it != dropdownSelections.end() ? it->second : -1;
	}

	void SetElementScrollOffset(DF2D::Data::UIElementID element, DF2D::Core::Vector2F offset) override
	{
		scrollOffsets[element] = offset;
	}

	DF2D::Core::Vector2F GetElementScrollOffset(DF2D::Data::UIElementID element) const override
	{
		auto it = scrollOffsets.find(element);

		return it != scrollOffsets.end() ? it->second : DF2D::Core::Vector2F::Zero;
	}

	DF2D::Core::Vector2F GetElementScrollSize(DF2D::Data::UIElementID) const override
	{
		return scrollSize;
	}

	bool LoadFontFace(const std::string& path, const std::string& family, bool) override
	{
		loadedFonts.push_back(path);
		loadedFontFamilies.push_back(family);

		return !failNextFontLoad;
	}

	void ProcessMouseMove(DF2D::Data::UIContextID, DF2D::Core::Vector2F position, DF2D::Data::KeyModifiers) override
	{
		lastPointerPosition = position;
		mouseMoveCount++;

	}

	void ProcessMouseButton(DF2D::Data::UIContextID, DF2D::Models::MouseButtonCode button, bool pressed, DF2D::Data::KeyModifiers) override
	{
		lastButton = button;
		lastButtonPressed = pressed;
		mouseButtonCount++;

	}

	void ProcessMouseWheel(DF2D::Data::UIContextID, DF2D::Core::Vector2F, DF2D::Data::KeyModifiers) override
	{
		mouseWheelCount++;

	}

	void ProcessKey(DF2D::Data::UIContextID, DF2D::Models::KeyboardKeyCode key, bool pressed, DF2D::Data::KeyModifiers) override
	{
		lastKey = key;
		lastKeyPressed = pressed;
		keyCount++;

	}

	void ProcessTextInput(DF2D::Data::UIContextID, const std::string& text) override
	{
		lastText = text;
		textInputCount++;

	}

	bool HasKeyboardFocus(DF2D::Data::UIContextID) const override
	{
		return keyboardFocused;
	}

	bool IsPointerOverElement(DF2D::Data::UIContextID) const override
	{
		return pointerOverElement;
	}
};