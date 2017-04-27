/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  command.h
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-03-04
  @ description:  offer function to store command data.it is implemented in terms 
                  of a event pointer.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_COMMAND_H_
#define CONNOR_GAME_SRC_COMMAND_H_

#include "framework/event/event.h"

namespace gamer {

class Command {
  public:
    Command() = delete;

    Command(int cmd_id);
      
	~Command();

    inline void set_command_id(int cmd_id) { event_.set_event_id(cmd_id); }

    inline int command_id() const { return event_.event_id(); }

    inline void set_sub_command_id(int cmd_id) { event_.set_sub_event_id(cmd_id); }

    inline int sub_command_id() const { return event_.sub_event_id(); }

    inline void set_command_name(const std::string& name) { event_.set_event_name(name); }

    inline const std::string& command_name() const { return event_.event_name(); }

    inline void set_user_data(void* user_data) { event_.set_user_data(user_data); }

    inline void* user_data() const { return  event_.user_data(); }

    inline bool check_command_id() const { return event_.check_event_id(); }

    inline void stop_propagation() { event_.stop_propagation(); }

    inline bool is_stopped() const { return event_.is_stopped(); }

private:
    bool init(int cmd_id);

    Event event_;

    friend class EventManager;
}; // namespace gamer

}

#endif // CONNOR_GAME_SRC_EVENT_H_