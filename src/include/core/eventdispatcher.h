#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <base.h>
#include "clock.h"
#include "scheduledevent.h"

#include <queue>

namespace core {

	class EventDispatcher {
		public:
			EventDispatcher();

			void shutdown();
			void poll(bool useSleep=true);

			void pushEventSynchronized(const std::function<void()>& func);

			EventPtr addEvent(const std::function<void()>& callback, bool pushFront = false);
			ScheduledEventPtr scheduleEvent(const std::function<void()>& callback, int delay);
			ScheduledEventPtr cycleEvent(const std::function<void()>& callback, int delay);

		private:
			std::list<EventPtr> m_eventList;
			int m_pollEventsSize;
			int32_t m_pollsWithoutExecuting;
			stdext::boolean<false> m_disabled;
			std::priority_queue<ScheduledEventPtr, std::vector<ScheduledEventPtr>, lessScheduledEvent> m_scheduledEventList;

			std::mutex m_syncEventsMutex;
			std::vector<std::function<void(void)>> m_syncEvents;
			volatile int32_t m_syncEventNum;
	};

}

extern core::EventDispatcher g_dispatcher;

#endif
