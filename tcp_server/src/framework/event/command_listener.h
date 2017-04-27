/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  command_manager.h
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-03-04
  @ description:  offer function to create command listener.it is implemented in terms 
                  of event listener.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_COMMAND_LISTENER_H_
#define CONNOR_GAME_SRC_COMMAND_LISTENER_H_

#include <functional>
#include <string>
#include "event_listener.h"

namespace gamer {

class Command;

class CommandListener {
  public:
    //typedef std::function<void(Event*)> CommandCallback;
    typedef std::function<void(Command*)> CommandCallback;

    virtual ~CommandListener();

    // use for c++ only.
    static CommandListener* create(int cmd_id, 
                                   const CommandCallback& cmd_callback, 
                                   const std::string& listener_name, 
                                   int priority);

    // use for lua only.
    static CommandListener* create(int cmd_id, 
                                   Listener::LuaFunction cmd_callback, 
                                   const std::string& listener_name,
                                   int priority);

    inline int command_id() const { return event_listener_->event_id(); }

    inline const std::string& listener_name() const { return event_listener_->listener_name(); }

    inline int priority() const { return event_listener_->priority(); }

    inline bool is_lua_function() const { return event_listener_->is_lua_function(); }

    inline Listener::LuaFunction lua_function() const { return event_listener_->lua_function(); }

    inline void set_lua_function_id(const std::string& function_id) {
        event_listener_->set_lua_function_id(function_id);
    };

    inline const std::string& lua_function_id() const { 
        return event_listener_->lua_function_id(); 
    };

    inline void set_enabled(bool enabled) { 
        event_listener_->set_enabled(enabled); 
    };

    inline bool is_enabled() const { return event_listener_->is_enabled(); }

    inline bool checkValidity() const { return event_listener_->checkValidity(); }

    inline bool is_registered() const { return event_listener_->is_registered(); }

  private:
    CommandListener();
    
    bool init(int cmd_id, 
              const CommandCallback& command_callback, 
              const std::string& listener_name, 
              int priority);

    bool init(int cmd_id, 
              Listener::LuaFunction command_callback, 
              const std::string& listener_name,
              int priority);

    inline void set_registered(bool registered) { event_listener_->set_registered(registered); }

    EventListener* event_listener_;
    friend class CommandManager;
};

} // namespace gamer 

#endif