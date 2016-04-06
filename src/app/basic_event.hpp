#pragma once

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
