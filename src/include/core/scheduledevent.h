#ifndef SCHEDULEDEVENT_H
#define SCHEDULEDEVENT_H

#include "event.h"
#include "clock.h"

namespace core {

	class ScheduledEvent : public Event {
		public:
			ScheduledEvent(const std::function<void()>& callback, int delay, int maxCycles);
			void execute();
			bool nextCycle();

			int ticks() { return m_ticks; }
			int remainingTicks() { return m_ticks - g_clock.millis(); }
			int delay() { return m_delay; }
			int cyclesExecuted() { return m_cyclesExecuted; }
			int maxCycles() { return m_maxCycles; }

		private:
			ticks_t m_ticks;
			int m_delay;
			int m_maxCycles;
			int m_cyclesExecuted;
	};

	typedef stdext::object_ptr<ScheduledEvent> ScheduledEventPtr;

	struct lessScheduledEvent : std::binary_function<ScheduledEventPtr, ScheduledEventPtr&, bool> {
		bool operator()(const ScheduledEventPtr& a, const ScheduledEventPtr& b) {
			return  b->ticks() < a->ticks();
		}
	};
	
}


#endif
