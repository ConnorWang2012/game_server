/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  command_manager.cc
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-03-04
  @ description:  see the header file.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#include "command_listener.h"
#include "command.h"
#include "framework/base/macros.h"

namespace gamer {

CommandListener::CommandListener() : event_listener_(nullptr) {
}

CommandListener::~CommandListener() {
}

CommandListener* CommandListener::create(
    int cmd_id, 
    const CommandCallback& cmd_callback, 
    const std::string& listener_name, 
    int priority) {
    CommandListener* listener = new CommandListener();
    if (listener && listener->init(cmd_id, 
                                   cmd_callback, 
                                   listener_name, 
                                   priority)) {
        return listener;
    }
    SAFE_DELETE(listener);
    return nullptr;
}

CommandListener* CommandListener::create(
    int cmd_id, 
    Listener::LuaFunction cmd_callback, 
    const std::string& listener_name,
    int priority) {
    CommandListener* listener = new CommandListener();
    if (listener && listener->init(cmd_id, 
                                   cmd_callback, 
                                   listener_name, 
                                   priority))
    {
        return listener;
    }
    SAFE_DELETE(listener);
    return nullptr;
}

bool CommandListener::init(int cmd_id, 
                           const CommandCallback& cmd_callback, 
                           const std::string& listener_name, 
                           int priority) {
    event_listener_ = EventListener::createCmdListener(cmd_id, 
                                                       cmd_callback, 
                                                       listener_name, 
                                                       priority);
    return true;
}

bool CommandListener::init(int cmd_id, 
                           Listener::LuaFunction cmd_callback, 
                           const std::string& listener_name,
                           int priority) {
    event_listener_ = EventListener::create(cmd_id, cmd_callback, listener_name, priority);
    return true;
}

} // namespace gamer