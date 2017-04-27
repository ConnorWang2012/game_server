/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  event_listener.cc
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-01-18
  @ description:  see the header file.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#include "event_listener.h"

#include <cassert>
#include <stdint.h>

#include "event_define.h"
//#include "lua_bind_helper.h"
#include "framework/base/macros.h"

namespace gamer {

EventListener::EventListener() {
    init();
}

EventListener::~EventListener() {
	init();
}

EventListener* EventListener::create(
    int event_id, 
    const EventCallback& event_callback, 
    const std::string& listener_name, 
    int priority)
{
	EventListener* listener = new EventListener();
	if (listener && listener->init(event_id, 
                                   event_callback, 
                                   listener_name, 
                                   priority))
	{
		return listener;
	}
	SAFE_DELETE(listener);
	return nullptr;
}

EventListener* EventListener::create(int event_id, 
                                     LuaFunction event_callback,
                                     const std::string& listener_name,
                                     int priority)
{
    EventListener* listener = new EventListener();
    if (listener && listener->init(event_id, event_callback, listener_name, priority))
    {
        listener->is_lua_function_ = true;
        return listener;
    }
    SAFE_DELETE(listener);
    return nullptr;
}

void EventListener::executeCallback(Event* event)
{
    if (nullptr != event_callback_)
    {
        event_callback_(event);
    }
}

bool EventListener::checkValidity() const
{
    int event_id = this->event_id();
    if (is_lua_function()) 
    {
        return 0 < event_id && "" != lua_function_id();
    }
    return 0 < event_id && (nullptr != event_callback_ || nullptr != cmd_callback_); 
}

EventListener* EventListener::createCmdListener(int event_id,
                                     const CommandCallback& cmd_callback, 
                                     const std::string& listener_name, 
                                     int priority )
{
    EventListener* listener = new EventListener();
    if (listener && listener->init(event_id, cmd_callback, listener_name, priority))
    {
        return listener;
    }
    SAFE_DELETE(listener);
    return nullptr;
}

void EventListener::init()
{
    target_id_        = 0;
    listener_name_    = "";
    priority_         = UINT32_MAX;
    is_enabled_       = true;
    is_lua_function_  = false;
    lua_function_     = 0;
    lua_function_id_  = "";
    is_registered_    = false;
}

bool EventListener::init(int event_id, 
                         const EventCallback& event_callback, 
                         const std::string& listener_name, 
                         int priority)
{
    assert(nullptr != event_callback);

    set_target_id(event_id);
	event_callback_ = event_callback;
    set_listener_name(listener_name);
    set_priority(priority);

	return true;
}

bool EventListener::init(int event_id, 
                         LuaFunction event_callback, 
                         const std::string& listener_name, 
                         int priority)
{
    assert(0 < event_id);

    set_target_id(event_id);
    set_listener_name(listener_name);
    set_priority(priority);

    return true;
}

bool EventListener::init(int event_id, 
                         const CommandCallback& cmd_callback, 
                         const std::string& listener_name, 
                         int priority)
{
    assert(nullptr != cmd_callback);

    set_target_id(event_id);
    cmd_callback_ = cmd_callback;
    set_listener_name(listener_name);
    set_priority(priority);

    return true;
}

} // namespace gamer