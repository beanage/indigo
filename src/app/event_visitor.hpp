#ifndef __EVENT_VISITOR_HPP_INCLUDED__
#define __EVENT_VISITOR_HPP_INCLUDED__

namespace indigo
{
class app_terminate_event;
class key_down_event;
class key_up_event;

class event_visitor
{
public:
	virtual ~event_visitor() {}

	virtual bool visit(const app_terminate_event&) { return false; }
	virtual bool visit(const key_down_event&) { return false; }
	virtual bool visit(const key_up_event&) { return false; }

protected:
	event_visitor() = default;
};
}

#endif
