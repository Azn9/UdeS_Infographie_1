#pragma once
#if !_HAS_CXX17


#error "This file requires C++17"
#endif

#include <any>
#include <concurrent_vector.h>
#include <functional>
#include <mutex>
#include <ranges>
#include <unordered_map>

#include "Event.h"
#include "PhysicEvent.h"
#include "Core/Public/Util/Singleton.h"
#include "Api/Public/Util/Instanceof.h"

namespace PM3D_API
{
    class EventSystem : public PM3D::CSingleton<EventSystem>
    {
        struct BaseWrapper
        {
            virtual ~BaseWrapper() = default;

            struct id
            {
            private:
                static inline long cur{};
                long val;

            public:
                id() noexcept : val{cur++}
                {
                }

                bool operator==(const id& autre) const noexcept
                {
                    return val == autre.val;
                }

                bool operator<(const id& autre) const noexcept
                {
                    return val < autre.val;
                }

                [[nodiscard]] long get() const noexcept
                {
                    return val;
                }
            };

            [[nodiscard]] virtual const id& getId() const = 0;
            virtual void apply(
                const std::conditional_t<false, const long, std::function<void(const PM3D_API::Event&)>>& function) = 0;
        };

        template <class E, template_extends<Event, E>  = 0>
        class EventWrapper final : public BaseWrapper
        {
        public:
            static inline const id id;
            [[nodiscard]] const BaseWrapper::id& getId() const override { return id; }

            explicit EventWrapper(E e) : event{std::move(e)}
            {
            }

            E& get() { return event; }

            void apply(
                const std::conditional_t<false, const long, std::function<void(const Event&)>>& function) override
            {
                function(event);
            }

        private:
            E event;
        };

    public:
        void processEvents()
        {
            if (eventsToProcess.empty()) return;

            // Swap eventsToProcess with an empty vector
            concurrency::concurrent_vector<std::unique_ptr<BaseWrapper>> events;
            events.swap(eventsToProcess);

            for (const auto& event : events)
            {
                if (!event) continue;

                const auto id = event->getId();
                const auto eventId = id.get();

                if (!listeners.contains(eventId)) continue;

                BaseWrapper* wrapperPtr = event.get();

                for (const auto& eventListeners = listeners[eventId]; const auto& listener : eventListeners |
                     std::views::values)
                {
                    wrapperPtr->apply(listener);
                }
            }
        }

        void processPhysicsEvents()
        {
            if (physicsEventsToProcess.empty()) return;

            // Swap eventsToProcess with an empty vector
            concurrency::concurrent_vector<std::unique_ptr<BaseWrapper>> events;
            events.swap(physicsEventsToProcess);

            for (const auto& event : events)
            {
                if (!event) continue;

                const auto id = event->getId();
                const auto eventId = id.get();

                if (!listeners.contains(eventId)) continue;

                BaseWrapper* wrapperPtr = event.get();

                for (const auto& eventListeners = listeners[eventId]; const auto& listener : eventListeners |
                     std::views::values)
                {
                    wrapperPtr->apply(listener);
                }
            }
        }

        template <class T, template_extends<Event, T>  = 0>
        void publish(T&& event)
        {
            eventsToProcess.push_back(std::make_unique<EventWrapper<T>>(std::forward<T>(event)));
        }

        template <class T, template_extends<PhysicEvent, T>  = 0>
        void publish(T&& event)
        {
            physicsEventsToProcess.push_back(std::make_unique<EventWrapper<T>>(std::forward<T>(event)));
        }

        template <class T, template_extends<Event, T>  = 0>
        long subscribe(const std::function<void(const T&)>& listener)
        {
            long id = generateId();

            // Car on ne peut pas stocker une std::function<void(const T&)> dans un std::function<void(const Event&)>
            auto wrappedListener = [listener](const Event& event)
            {
                listener(static_cast<const T&>(event));
            };

            const auto eventId = EventWrapper<T>::id.get();

            if (!listeners.contains(eventId))
            {
                std::unordered_map<long, std::function<void(const Event&)>> map;
                map.insert({id, wrappedListener});
                listeners.insert({eventId, map});
            }
            else
            {
                listeners[eventId].insert({id, wrappedListener});
            }

            return id;
        }

        template <class T, template_extends<Event, T>  = 0>
        void unsubscribe(const long& listenerId)
        {
            const auto eventId = EventWrapper<T>::id.get();

            if (!listeners.contains(eventId)) return;

            auto& map = listeners[eventId];

            if (!map.contains(listenerId)) return;

            map.erase(listenerId);

            if (map.empty())
            {
                listeners.erase(typeid(T));
            }
        }

        template <class T, template_extends<Event, T>  = 0>
        static void Publish(T&& event)
        {
            GetInstance().publish(std::forward<T>(event));
        }

        template <class T, template_extends<PhysicEvent, T>  = 0>
        static void Publish(T&& event)
        {
            GetInstance().publish(std::forward<T>(event));
        }

        template <class T, template_extends<Event, T>  = 0>
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

        template <class T, template_extends<Event, T>  = 0>
        static void Unsubscribe(const long& listenerId)
        {
            GetInstance().unsubscribe<T>(listenerId);
        }

    private:
        std::mutex idMutex;
        long nextId = 0;
        std::unordered_map<long, std::unordered_map<long, std::function<void(const Event&)>>> listeners;
        concurrency::concurrent_vector<std::unique_ptr<BaseWrapper>> eventsToProcess;
        concurrency::concurrent_vector<std::unique_ptr<BaseWrapper>> physicsEventsToProcess;

        long generateId()
        {
            // Pour éviter que deux threads génèrent le même id
            std::lock_guard guard{idMutex};
            return nextId++;
        }
    };
}
