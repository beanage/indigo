#ifndef __BASIC_EVENT_HPP_INCLUDED__
#define __BASIC_EVENT_HPP_INCLUDED__

namespace indigo
{
class event_visitor;

class basic_event
{
public:
	virtual ~basic_event() {}

	virtual bool accept(event_visitor& v) const = 0;
};
}

#endif
