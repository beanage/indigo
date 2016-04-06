#pragma once

#include "event_visitor.hpp"

namespace indigo
{
class event_visitor;

class basic_event
{
public:
	virtual ~basic_event() {}

	virtual bool accept(event_visitor& v) const = 0;
};

template <typename SubClass>
class basic_event_t : public basic_event
{
public:
	virtual ~basic_event_t() {}

	virtual bool accept(event_visitor& v) const override
	{
		v.visit(static_cast<const SubClass&>(*this));
	}
};
}
