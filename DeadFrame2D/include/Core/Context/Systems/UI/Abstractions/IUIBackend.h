#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/GeometryDrawList.h"
#include "Data/Systems/UI/UIAttribute.h"
#include "Data/Systems/UI/UIContextID.h"
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIElementType.h"
#include "Data/Systems/UI/UIPseudoClass.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	class IUIEventSink;


	/**
	 * @brief The boundary between the engine's UI component model and whatever library implements it.
	 */
	class DF2D_API IUIBackend
	{
	public:
		virtual ~IUIBackend() = default;


		/**
		 * @brief Sets where element events raised inside the backend are delivered.
		 *
		 * The sink is not owned and must outlive the backend. Passing null silently drops events.
		 *
		 * @param sink: Receiver for element events, or null to stop reporting them.
		 */
		virtual void SetEventSink(IUIEventSink* sink) = 0;


		/**
		 * @brief Creates an independent UI context with its own element tree and its own styling.
		 *
		 * Contexts share nothing, so one canvas cannot restyle or reach into another.
		 *
		 * @param size: Surface size in pixels that percentage lengths resolve against.
		 * @return The new context, or 0 on failure.
		 */
		virtual Data::UIContextID CreateContext(Vector2I size) = 0;

		/**
		 * @brief Destroys a context and every element inside it.
		 *
		 * Element handles belonging to the context become invalid; the backend is responsible for
		 * dropping them, so a component that outlives its canvas cannot dereference freed memory.
		 *
		 * @param context: Context to destroy. Unknown contexts are ignored.
		 */
		virtual void DestroyContext(Data::UIContextID context) = 0;

		/**
		 * @brief Resizes the surface the context lays out against.
		 *
		 * Called when the render target changes size, so percentage-anchored elements re-resolve.
		 *
		 * @param size: New surface size in pixels.
		 */
		virtual void SetContextSize(Data::UIContextID context, Vector2I size) = 0;

		/**
		 * @brief Advances animations and resolves layout for the context.
		 *
		 * Must run after components have finished writing properties for the frame and before anything
		 * reads a rectangle back, otherwise reads return the previous frame's layout.
		 */
		virtual void UpdateContext(Data::UIContextID context) = 0;

		/**
		 * @brief Renders the context into an ordered draw list.
		 *
		 * Nothing is drawn here and the render backend is not touched: the engine's pipeline decides
		 * when pixels are produced, so this only captures what to draw. The command order is
		 * significant and carries scissor state, so the list must be replayed whole and in order.
		 *
		 * @return The commands for this context, empty if the context is unknown or drew nothing.
		 */
		virtual Data::GeometryDrawList RenderContext(Data::UIContextID context) = 0;

		/**
		 * @brief Applies a stylesheet to the context, restyling everything already in it.
		 *
		 * Sheets accumulate rather than replace, so a game theme layers over the engine defaults and
		 * later sheets win on equal specificity. Inline properties still beat both.
		 *
		 * @param path: Stylesheet path, already resolved by the engine's path handling.
		 * @return Whether the sheet was loaded and applied.
		 */
		virtual bool LoadStyleSheet(Data::UIContextID context, const std::string& path) = 0;

		/**
		 * @brief The context's root element, which every other element ultimately hangs from.
		 *
		 * This is the element a Canvas component adopts as its own, rather than creating one.
		 *
		 * @return The root element, or 0 if the context is unknown.
		 */
		virtual Data::UIElementID GetRootElement(Data::UIContextID context) const = 0;


		/**
		 * @brief Creates a detached element belonging to a context.
		 *
		 * The element takes no part in layout and is not visible until SetElementParent attaches it.
		 * Until then the backend owns it, so an element that is created and never parented is still
		 * released by DestroyElement.
		 *
		 * @param type: What kind of element to build; the backend picks the concrete representation.
		 * @return The new element, or 0 on failure.
		 */
		virtual Data::UIElementID CreateElement(Data::UIContextID context, Data::UIElementType type) = 0;

		/**
		 * @brief Destroys an element and everything under it.
		 *
		 * @param element: Element to destroy. Unknown or already-destroyed elements are ignored.
		 */
		virtual void DestroyElement(Data::UIElementID element) = 0;

		/**
		 * @brief Moves an element under a new parent.
		 *
		 * Also handles the first attachment of a freshly created element, which is where ownership
		 * passes from the backend to the parent.
		 *
		 * @param siblingIndex: Position among the parent's children, or a negative value to append.
		 *                      Order matters: it decides both layout order and draw order.
		 */
		virtual void SetElementParent(Data::UIElementID element, Data::UIElementID parent, int siblingIndex) = 0;

		/**
		 * @brief Sets an inline style property, which outranks every stylesheet rule.
		 *
		 * This is how components express themselves. Writing the same value repeatedly is wasteful
		 * rather than wrong: each write dirties layout, so callers should only write on change.
		 *
		 * @param value: Value in the styling language's syntax, such as "50%", "12px" or "#ff0000ff".
		 */
		virtual void SetElementProperty(Data::UIElementID element, Data::UIStyleProperty property, const std::string& value) = 0;

		/** @brief Drops a previously set inline property so the stylesheets decide the value again. */
		virtual void ClearElementProperty(Data::UIElementID element, Data::UIStyleProperty property) = 0;

		/**
		 * @brief Sets a non-style attribute, such as an image source or an input's value.
		 *
		 * Separate from properties because attributes describe what an element *is*, not how it looks,
		 * and are not part of the styling cascade.
		 */
		virtual void SetElementAttribute(Data::UIElementID element, Data::UIAttribute attribute, const std::string& value) = 0;

		/**
		 * @brief Replaces the element's text content.
		 *
		 * Implementations must treat the text as data and escape it: game strings reach this from
		 * player input and save files, and must never be able to inject markup into the tree.
		 */
		virtual void SetElementText(Data::UIElementID element, const std::string& text) = 0;

		/**
		 * @brief Adds or removes a style class on the element.
		 *
		 * Classes are the seam that lets a game stylesheet target engine-built elements without the
		 * engine knowing anything about the game's theme.
		 *
		 * @param enabled: True to add the class, false to remove it.
		 */
		virtual void SetElementClass(Data::UIElementID element, const std::string& className, bool enabled) = 0;

		/** @brief Whether the element currently carries the given style class. */
		virtual bool HasElementClass(Data::UIElementID element, const std::string& className) const = 0;

		/**
		 * @brief Whether the element is in the given interaction state.
		 *
		 * The backend owns hit testing and focus, so this is how a component reads back whether it is
		 * hovered, pressed or focused instead of tracking the pointer itself.
		 */
		virtual bool HasPseudoClass(Data::UIElementID element, Data::UIPseudoClass pseudoClass) const = 0;

		/**
		 * @brief Shows or hides the element.
		 *
		 * A hidden element leaves layout entirely rather than merely becoming invisible, which is what
		 * deactivating its GameObject implies.
		 */
		virtual void SetElementVisible(Data::UIElementID element, bool visible) = 0;

		/** @brief Gives the element keyboard focus, taking it from whatever held it before. */
		virtual void FocusElement(Data::UIElementID element) = 0;

		/**
		 * @brief The element's resolved border box, in context space.
		 *
		 * Only meaningful after UpdateContext has run for the frame; empty before the first layout.
		 *
		 * @return The resolved rectangle, or an empty rectangle if the element is unknown.
		 */
		virtual RectF GetElementRect(Data::UIElementID element) const = 0;

		/**
		 * @brief The size the element's content wants, independent of the box it was given.
		 *
		 * Used for content-driven sizing, such as growing a label to fit its text.
		 */
		virtual Vector2F GetElementContentSize(Data::UIElementID element) const = 0;


		/**
		 * @brief Registers a font file with the shared font database.
		 *
		 * Size, weight and style are per-element style properties rather than part of loading, so one
		 * face serves every size in the game and changing size costs nothing but a restyle.
		 *
		 * @param path: Font file path, already resolved by the engine's path handling.
		 * @param family: Family name to register the face under; empty keeps the name inside the file.
		 *                Naming it explicitly lets stylesheets refer to a stable family regardless of
		 *                which font file the game actually ships.
		 * @param fallbackFace: Whether the face may supply glyphs missing from other faces.
		 * @return Whether the face was loaded.
		 */
		virtual bool LoadFontFace(const std::string& path, const std::string& family, bool fallbackFace) = 0;
	};
}