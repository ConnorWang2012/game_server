/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  event_listener.h
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-01-18
  @ description:  offer function to create event listener, etc.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_EVENT_LISTENER_H_
#define CONNOR_GAME_SRC_EVENT_LISTENER_H_

#include <functional>
#include <string>
//#include "command_listener.h"
#include "listener.h"

namespace gamer {

class Command;
class Event;

class EventListener : public Listener {
  public:
    typedef std::function<void(Event*)> EventCallback;

	~EventListener();

    // use for c++ only.
	static EventListener* Create(int event_id, 
                                 const EventCallback& event_callback, 
                                 const std::string& listener_name, 
                                 int priority);

    // use for lua only.
    static EventListener* Create(int event_id, 
                                 LuaFunction event_callback, 
                                 const std::string& listener_name,
                                 int priority);

    void ExecuteCallback(Event* event);

    bool check_validity [[override]] () const;

	inline int event_id() const { return target_id(); };

    inline void set_event_callback(const EventCallback& event_callback) { 
        event_callback_ = event_callback; 
    };

	inline const EventCallback& event_callback() { return event_callback_; };
  
    inline bool is_registered() const { return is_registered_; };

  private:
    // use for command listener
    typedef std::function<void(Command*)> CommandCallback;

    EventListener();

    // use for c++ only. use for command listener.
    static EventListener* CreateCmdListener(int event_id, 
											const CommandCallback& cmd_callback,
											const std::string& listener_name,
											int priority);

    // use for both c++ and lua.
    void Init();

    // use for c++ only.
    bool Init(int event_id, 
              const EventCallback& event_callback, 
              const std::string& listener_name, 
              int priority);

    // use for lua only.
    bool Init(int event_id, 
              LuaFunction event_callback, 
              const std::string& listener_name, 
              int priority);

    // use for c++ only.use for command listener.
    bool Init(int event_id, 
              const CommandCallback& cmd_callback, 
              const std::string& listener_name, 
              int priority);

    inline void set_registered(bool registered) { is_registered_ = registered; };

    EventCallback event_callback_;
    CommandCallback cmd_callback_;

    friend class CommandListener;
    friend class EventManager;
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_EVENT_LISTENER_H_