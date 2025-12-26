#pragma once
#include "Utilities/Delegates/Listeners/ListenerID.h"
#include <typeinfo>


namespace DeadFrame2D::Utilities
{
	template<typename... Args>
	struct IListener
	{
		template<typename...>
		friend class MulticastDelegate;


	private:
		void SetID(ListenerID newID);


	protected:
		ListenerID id = 0;


	public:
		virtual ~IListener() = default;


		virtual void Invoke(const Args& ...args) = 0;

		virtual bool IsExpired() const = 0;

		virtual bool MatchesIdentity(const void* identity, const std::type_info& type) const = 0;


		// MatchesIdentity wrapper 
		template<typename T>
		bool Matches(const T& identity) const;


		ListenerID GetID() const;
	};
}


namespace DeadFrame2D::Utilities
{
	template<typename ...Args>
	inline void IListener<Args...>::SetID(ListenerID newID)
	{
		id = newID;
	}


	template<typename ...Args>
	inline ListenerID IListener<Args...>::GetID() const
	{
		return id;
	}

	template<typename ...Args>
	template<typename T>
	inline bool IListener<Args...>::Matches(const T& identity) const
	{
		return MatchesIdentity(&identity, typeid(T));
	}
}