/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  event_manager.h
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-01-18
  @ description:  offer function to manager events : add event listener, dispatch
                  event, etc.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_EVENT_MANAGER_H_
#define CONNOR_GAME_SRC_EVENT_MANAGER_H_

#include <map>
#include <string>
#include <vector>

namespace gamer {

class Command;
class Event;
class EventListener;

class EventManager {
  public:
	~EventManager();

	static EventManager* getInstance();

	static void destoryInstance();

	void addEventListener(EventListener* listener);

    void dispatchEvent(Event* event);

    void dispatchEvent(int event_id, void* optional_user_data = nullptr);

    // remove the event listener from the instance of event manager but do not delete it.
	void removeEventListener(EventListener* listener);

    // remove the event listener from the instance of event manager and delete it.
    void removeEventListenerWithCleanup(EventListener* listener);

    // remove the event listener from the instance of event manager but do not delete it.
	void removeEventListener(const std::string& listener_name);

    // remove the event listener from the instance of event manager and delete it.
    void removeEventListenerWithCleanup(const std::string& listener_name);

    // remove all event listeners from the instance of event manager but do not delete any.
	void removeAllEventListeners();

    // remove all event listeners from the instance of event manager and delete all.
    void removeAllEventListenersWithCleanup();

	void setPriority(EventListener* listener, int priority);

    inline void set_enabled(bool enabled) { is_enabled_ = enabled; }

    inline bool is_enabled() const { return is_enabled_; }

private:
    typedef std::map<int, std::vector<EventListener*> >	EventIDListenerMap;

    enum class DirtyFlag {
        NONE                     = 0,
        LISTENER_PRIORITY_CHANGE = 1,
        LISTENER_NUM_CHANGE      = 2,
        ALL                      = 10
    };

	EventManager();
    
    bool init();

    void addEventListener(EventListener* listener, int priority);

	void addEventListenerImpl(EventListener* listener, int priority);

    void removeEventListenerImpl(EventListener* listener, bool cleanup);

    void removeEventListenerImpl(const std::string& listener_name, bool cleanup);

    void removeAllEventListenersImpl(bool cleanup);

	void sortEventListeners(std::vector<EventListener*>* listeners);

	void sortEventListeners(int event_id);

	void dispatchEvent(std::vector<EventListener*> listeners, Event* event);

    // use for command manager.
    void dispatchComand(Command* cmd);

    // use for command manager.
    void dispatchComand(int cmd_id, void* optional_user_data = nullptr);

    // use for command manager.
    void dispatchCommandImpl(std::vector<EventListener*> listeners, Command* cmd);

	bool setPriority(std::vector<EventListener*> listener_list, 
                     EventListener* listener, 
                     int priority);

	void updateEventListeners();

    inline bool is_dispatching() { return 0 < dispatch_count_; }

	inline void set_dirty_flag(DirtyFlag flag) { dirty_flag_ = flag; }

	inline void set_dirty_flag(DirtyFlag flag, int event_id) {
        set_dirty_flag(flag);
        dirty_event_id_ = event_id;
    };

    inline DirtyFlag dirty_flag() const { return dirty_flag_; }

	inline bool is_dirty() const { return DirtyFlag::NONE != dirty_flag_; }

	static EventManager* s_shared_event_manager_;

	EventIDListenerMap id_listener_map_;

	std::vector<EventListener*> listeners_to_add_vec_;

	// Whether the event manager is dispatching event(0 mean not) 
	int dispatch_count_;

	bool is_enabled_;
	DirtyFlag dirty_flag_;
	int dirty_event_id_;
	std::string dirty_event_name_;

    friend class CommandManager;
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_EVENT_MANAGER_H_