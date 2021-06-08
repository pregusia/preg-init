#ifndef EVENT_H
#define EVENT_H

#include <base.h>

namespace core {

	class Event : public stdext::object {
		public:
		    Event(const std::function<void()>& callback);
		    virtual ~Event();

		    virtual void execute();
		    void cancel();

		    bool isCanceled() { return m_canceled; }
		    bool isExecuted() { return m_executed; }

		protected:
		    std::function<void()> m_callback;
		    bool m_canceled;
		    bool m_executed;
	};

	typedef stdext::object_ptr<Event> EventPtr;

}	

#endif
