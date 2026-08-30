#pragma once
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIEventPayload.h"
#include "Data/Systems/UI/UIEventType.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Receives element events raised inside the UI backend.
	 */
	class DF2D_API IUIEventSink
	{
	public:
		virtual ~IUIEventSink() = default;


		/**
		 * @brief Delivers one element event.
		 *
		 * Implementations must tolerate an element they do not recognise: an event can arrive for an
		 * element whose owning component was destroyed earlier in the same frame.
		 *
		 * @param element: Element the event reached.
		 * @param eventType: What happened.
		 * @param payload: Values carried by the event; which fields are filled depends on eventType.
		 */
		virtual void OnUIEvent(Data::UIElementID element, Data::UIEventType eventType, const Data::UIEventPayload& payload) = 0;
	};
}