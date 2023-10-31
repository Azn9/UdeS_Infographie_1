#pragma once
#include <any>
#include <functional>
#include <mutex>
#include <ranges>
#include <typeindex>
#include <unordered_map>

#include "Event.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/Singleton.h"
#include "../../Public/Util/Instanceof.h"

namespace PM3D_API
{
class EventSystem : public PM3D::CSingleton<EventSystem>
{
public:
	template <class T, template_extends<Event, T> = 0>
	void publish(T event)
	{
		if (!listeners.contains(typeid(T))) return;

		for (const auto& eventListeners = listeners[typeid(T)]; const auto& val : eventListeners | std::views::values)
		{
			val(event);
		}
	}

	template <class T, template_extends<Event, T> = 0>
	long subscribe(const std::function<void(const T&)>& listener)
	{
		// Pour éviter que deux threads génèrent le même id
		std::lock_guard guard(idMutex);
		long id = nextId++;

		// Car on ne peut pas stocker une std::function<void(const T&)> dans un std::function<void(const Event&)>
		auto wrappedListener = [listener](const Event& event) {
			listener(static_cast<const T&>(event));
		};
		
		if (!listeners.contains(typeid(T)))
		{
			std::unordered_map<long, std::function<void(const Event&)>> map;
			map.insert({id, wrappedListener});
			listeners.insert({ typeid(T), map });
		} else
		{
			listeners[typeid(T)].insert({id, wrappedListener});
		}

		return id;
	}

	template <class T, template_extends<Event, T> = 0>
	void unsubscribe(const long& listenerId)
	{
		if (!listeners.contains(typeid(T))) return;
		
		auto& map = listeners[typeid(T)];
		
		if (!map.contains(listenerId)) return;

		map.erase(listenerId);

		if (map.empty())
		{
			listeners.erase(typeid(T));
		}
	}

	template <class T, template_extends<Event, T> = 0>
	static void Publish(T event)
	{
		GetInstance().publish(event);
	}

	template <class T, template_extends<Event, T> = 0>
	static long Subscribe(const std::function<void(const T&)>& listener)
	{
		return GetInstance().subscribe(listener);
	}

	// Afin de pouvoir utiliser des lambdas
	template <class F>
	static auto Subscribe(F&& listener) -> decltype(Subscribe(std::function(std::forward<F>(listener))))
	{
		return GetInstance().subscribe(std::function(std::forward<F>(listener)));
	}

	template <class T, template_extends<Event, T> = 0>
	static void Unsubscribe(const long& listenerId)
	{
		GetInstance().unsubscribe<T>(listenerId);
	}

private:
	std::mutex idMutex;
	long nextId = 0;
	std::unordered_map<std::type_index, std::unordered_map<long, std::function<void(const Event&)>>> listeners;
};
}
