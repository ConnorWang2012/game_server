/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  command.cc
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-01-18
  @ description:  offer function to store event data.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_EVENT_H_
#define CONNOR_GAME_SRC_EVENT_H_

#include <string>

namespace gamer {

class Event {
  public:
	Event() : Event(0) {};

	Event(int event_id)
		:event_id_(event_id)
		,event_name_("")
		,is_stopped_(false)
		,user_data_(nullptr) {
	}

	~Event() {};

    inline void set_event_id(int event_id) { event_id_ = event_id; }

    inline int event_id() const { return event_id_; }

    inline void set_sub_event_id(int sub_event_id) { sub_event_id_ = sub_event_id; }

    inline int sub_event_id() const { return sub_event_id_; }

    inline void set_event_name(const std::string& event_name) { event_name_ = event_name; }

    inline const std::string& event_name() const { return event_name_; }

    inline void set_user_data(void* user_data) { user_data_ = user_data; }

    inline void* user_data() const { return user_data_; }

    inline bool check_event_id() const { return 0 < event_id_; }

    inline void stop_propagation() { is_stopped_ = true; }

    inline bool is_stopped() const { return is_stopped_; }

 private:
    int event_id_;
    int sub_event_id_;
    std::string	event_name_;
    bool is_stopped_;
    void* user_data_;

    friend class EventManager;
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_EVENT_H_