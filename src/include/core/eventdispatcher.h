/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * From OTClient OTClient <https://github.com/edubart/otclient>
 * Modded by pregusia
 *
 */


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
