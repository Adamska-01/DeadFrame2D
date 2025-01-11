#pragma once


class DispatchableEvent
{
public:
	virtual ~DispatchableEvent() = default;


	template <typename T>
	static std::shared_ptr<T> SafeCast(std::shared_ptr<DispatchableEvent> event);
};


template<typename T>
inline std::shared_ptr<T> DispatchableEvent::SafeCast(std::shared_ptr<DispatchableEvent> event)
{
	auto castedEvent = std::dynamic_pointer_cast<T>(event);

	if (castedEvent == nullptr)
	{
		std::cerr << "Event received is not a valid type of " << typeid(T).name() << std::endl;
	}

	return castedEvent;
}