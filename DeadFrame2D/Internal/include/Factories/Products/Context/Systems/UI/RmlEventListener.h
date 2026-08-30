#pragma once
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIEventType.h"
#include <RmlUi/Core/EventListener.h>


namespace DF2D::Core
{
	class IUIEventSink;
}


namespace DF2D::Internal
{
	/**
	 * @brief Forwards one element's events to the engine's event sink.
	 *
	 * One listener per element rather than one shared listener: RmlUi reports the element an event
	 * reached, but the engine addresses elements by its own opaque ID, and carrying that ID on the
	 * listener avoids a reverse lookup on every mouse move.
	 */
	class RmlEventListener : public Rml::EventListener
	{
	private:
		Core::IUIEventSink* sink;

		Data::UIElementID element;


	public:
		RmlEventListener(Core::IUIEventSink* sink, Data::UIElementID element);

		~RmlEventListener() override;


		void ProcessEvent(Rml::Event& event) override;
	};
}