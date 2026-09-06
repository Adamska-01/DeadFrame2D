#include "Core/Context/Systems/UI/Abstractions/IUIEventSink.h"
#include "Factories/Products/Context/Systems/UI/RmlEventListener.h"
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/ID.h>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	RmlEventListener::RmlEventListener(IUIEventSink* sink, UIElementID element)
		: sink(sink),
		element(element)
	{
	}

	RmlEventListener::~RmlEventListener()
	{
	}


	void RmlEventListener::ProcessEvent(Rml::Event& event)
	{
		if (sink == nullptr)
			return;

		auto payload = UIEventPayload();
		auto eventType = UIEventType::CLICK;

		switch (event.GetId())
		{
		case Rml::EventId::Click:		eventType = UIEventType::CLICK; break;
		case Rml::EventId::Mouseover:	eventType = UIEventType::POINTER_ENTER; break;
		case Rml::EventId::Mouseout:	eventType = UIEventType::POINTER_EXIT; break;
		case Rml::EventId::Mousedown:	eventType = UIEventType::POINTER_DOWN; break;
		case Rml::EventId::Mouseup:		eventType = UIEventType::POINTER_UP; break;
		case Rml::EventId::Focus:		eventType = UIEventType::FOCUS_GAINED; break;
		case Rml::EventId::Blur:		eventType = UIEventType::FOCUS_LOST; break;

		case Rml::EventId::Submit:
			eventType = UIEventType::SUBMIT;
			payload.value = event.GetParameter<Rml::String>("value", "");
			break;

		case Rml::EventId::Scroll:
		{
			eventType = UIEventType::SCROLL;

			// The event carries no offsets of its own; the element it reached is the thing that moved.
			if (auto* target = event.GetTargetElement())
			{
				payload.offset = Vector2F(target->GetScrollLeft(), target->GetScrollTop());
			}

			break;
		}

		case Rml::EventId::Change:
		{
			eventType = UIEventType::VALUE_CHANGED;
			payload.value = event.GetParameter<Rml::String>("value", "");
			payload.numericValue = event.GetParameter<float>("value", 0.0f);
			payload.index = event.GetParameter<int>("value", 0);

			sink->OnUIEvent(element, eventType, payload);

			// Pressing Enter in a single-line field is reported as a change carrying a line break, not
			// as a submit -- that one belongs to forms, which the engine does not build. Raising both
			// keeps the distinction the component layer actually wants: the value moved, and the player
			// said they were done with it.
			if (event.GetParameter<bool>("linebreak", false))
			{
				sink->OnUIEvent(element, UIEventType::SUBMIT, payload);
			}

			return;
		}

		default:
			return;
		}

		sink->OnUIEvent(element, eventType, payload);
	}
}