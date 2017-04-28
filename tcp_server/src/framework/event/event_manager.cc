/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  event_manager.cc
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

#include "event_manager.h"

#include <cassert>
#include <algorithm>
#include <stdint.h>

#include "command.h"
#include "event.h"
#include "event_listener.h"
//#include "lua_bind_helper.h"
#include "framework/base/macros.h"

namespace {

class DispatchGuard {
 public:
	DispatchGuard(int& count)
        :count_(count) {
		++count_;
	}

	~DispatchGuard() {
		--count_;
	}

  private:
	int& count_;
};

} // namespace

namespace gamer {

EventManager* EventManager::s_event_manager_ = nullptr;

EventManager::EventManager()
    :dispatch_count_(0)
    ,is_enabled_(true)
    ,dirty_flag_(DirtyFlag::DIRTY_FLAG_NONE)
    ,dirty_event_id_(0)
    ,dirty_event_name_("") {
}

EventManager::~EventManager() {
    RemoveAllEventListenersWithCleanup();
}

EventManager* EventManager::instance() {
	if (nullptr == s_event_manager_) {
		s_event_manager_ = new EventManager();
		if( !s_event_manager_->Init() ) {
			SAFE_DELETE(s_event_manager_);
		}
	}
	return s_event_manager_;
}

void EventManager::DestoryInstance() {
	SAFE_DELETE(s_event_manager_);
}

void EventManager::AddEventListener(EventListener* listener) {
    if (nullptr == listener || listener->is_registered())
        return;
    assert(false != listener->check_validity());

    AddEventListener(listener, listener->priority());
}

void EventManager::DispatchEvent(Event* event) {
    assert(nullptr != event);
    assert(false != event->check_event_id());

    if ( !is_enabled_ )
        return;

    DispatchGuard guard(dispatch_count_);

    auto listenerMap = event_listener_.find(event->event_id());
    if (event_listener_.end() != listenerMap) {
        auto listeners = listenerMap->second;
        DispatchEvent(listeners, event);
    }

    UpdateEventListeners();
}

void EventManager::DispatchEvent(int event_id, void* optional_user_data) {
    //assert(EventIDs::INVALID != event_id);
    assert(event_id > 0);

    if ( !is_enabled_ )
        return;

    DispatchGuard guard(dispatch_count_);

    //Event* evn = Event::Create(event_id);
    Event evn(event_id);
    if(nullptr != optional_user_data) {
        //evn->set_user_data(optionalUserData);
        evn.set_user_data(optional_user_data);
    }

    auto listenerMap = event_listener_.find(event_id);
    if (event_listener_.end() != listenerMap) {
        auto listeners = listenerMap->second;
        DispatchEvent(listeners, &evn);
    }

    UpdateEventListeners();
}

void EventManager::RemoveEventListener(EventListener* listener) {
	if (nullptr == listener)
		return;
    RemoveEventListenerImpl(listener, false);
}

void EventManager::RemoveEventListener(const std::string& listener_name) {
    RemoveEventListenerImpl(listener_name, false);
}

void EventManager::RemoveEventListenerWithCleanup(EventListener* listener) {
    if (nullptr == listener)
        return;
    RemoveEventListenerImpl(listener, true);
}

void EventManager::RemoveEventListenerWithCleanup(const std::string& listener_name) {
    RemoveEventListenerImpl(listener_name, true);
}

void EventManager::RemoveAllEventListeners() {
    RemoveAllEventListenersImpl(false);
}

void EventManager::RemoveAllEventListenersWithCleanup() {
    RemoveAllEventListenersImpl(true);
}

void EventManager::SetPriority(EventListener* listener, int priority) {
	if (nullptr == listener)
		return;

	for (auto it = event_listener_.begin(); it != event_listener_.end(); ++it) {
		auto listener_list = it->second;
		if(SetPriority(listener_list, listener, priority)) {
			set_dirty_flag(DirtyFlag::DIRTY_FLAG_LISTENER_PRIORITY_CHANGE, 
                listener->event_id());
			break;
		}
	}

	//if ( !is_dispatching() )
	//{
	//	updateEventListeners();
	//}
}

bool EventManager::Init() {
    return true;
}

void EventManager::AddEventListener(EventListener* listener, int priority) {
    //assert(nullptr != listener);
    //assert(false != listener->checkValidity());

    if ( !is_dispatching() ) {
        AddEventListenerImpl(listener, priority);
    } else {
        listeners_to_add_.push_back(listener);
        set_dirty_flag(DirtyFlag::DIRTY_FLAG_LISTENER_NUM_CHANGE);
    }
}

void EventManager::AddEventListenerImpl(EventListener* listener, 
                                        int priority) {
	EventListenerMap::iterator it = event_listener_.find(
        listener->event_id());
	if(event_listener_.end() != it) {
		std::vector<EventListener*>* vecListener = &it->second;
		vecListener->push_back(listener);        

		SortEventListeners(vecListener);
	} else {
		std::vector<EventListener*> vListener(1, listener);
		// _sListenerMap.insert(std::make_pair(listener->_eventName, vListener));
		event_listener_.insert(EventListenerMap::value_type(
            listener->event_id(), vListener));
	}	

    listener->set_registered(true);
}

void EventManager::RemoveEventListenerImpl(EventListener* listener, bool cleanup) {
    if (is_dispatching()) {
        // TODO:set dirty flag
        return;
    }

    // 1. find listener from id_listener_map_
    for (auto it = event_listener_.begin();it != event_listener_.end();++it) {
        auto iter = std::find(it->second.begin(), it->second.end(), listener);
        if (it->second.end() != iter) {
            if (cleanup) {
                SAFE_DELETE(*iter);
            } else {
                ((EventListener*)*iter)->set_registered(false);
            }
            it->second.erase(iter);
            return;
        }
    }

    // 2. find listener from listeners_to_add_vec_
    if ( !listeners_to_add_.empty() ) {
        std::sort(listeners_to_add_.begin(), listeners_to_add_.end());

        auto it = std::find(listeners_to_add_.begin(), listeners_to_add_.end(), listener);

        if (listeners_to_add_.end() != it){
            if (cleanup) {
                SAFE_DELETE(*it);
            }
            listeners_to_add_.erase(it);
            return;
        }
    }
}

void EventManager::RemoveEventListenerImpl(const std::string& listener_name, 
                                           bool cleanup) {
    if (is_dispatching()) {
        // TODO:set dirty flag
        return;
    }

    // 1. find listener from id_listener_map_
    for (auto it = event_listener_.begin();it != event_listener_.end();++it) {
        auto iter = std::find_if(it->second.begin(), 
								 it->second.end(), 
								 [&](const EventListener* l) {
									 return (l->listener_name() == listener_name);
								 });

        if (it->second.end() != iter) {
            if (cleanup) {
                SAFE_DELETE(*iter);
            } else {
                ((EventListener*)*iter)->set_registered(false);
            }
            it->second.erase(iter);
            return;
        }
    }

    // 2. find listener from listeners_to_add_vec_
    if ( !listeners_to_add_.empty() ) {
        std::sort(listeners_to_add_.begin(), listeners_to_add_.end());
        auto iter = std::find_if(listeners_to_add_.begin(), 
								 listeners_to_add_.end(), 
								 [=](const EventListener* l) {
									 return (l->listener_name() == listener_name);
								 });

        if (listeners_to_add_.end() != iter) {
            if (cleanup) {
                SAFE_DELETE(*iter);
            }
            listeners_to_add_.erase(iter);
            return;
        }
    }
}

void EventManager::RemoveAllEventListenersImpl(bool cleanup) {
    if (is_dispatching()) {
        // TODO:set dirty flag
        return;
    }

    bool delete_ok = true;
    for (auto it = event_listener_.begin(); it != event_listener_.end(); ++it) {
        auto listeners   = it->second;
        bool all_deleted = true;
        if (cleanup) {
            std::for_each (listeners.begin(), 
                           listeners.end(), 
                           [&](EventListener* listener) {
                               if ( !is_dispatching() ) {
                                   SAFE_DELETE(listener);
                               } else {
                                   all_deleted = false;
                               }
                           });
        } else if (!is_dispatching()) {
            std::for_each (listeners.begin(), 
                           listeners.end(), 
                           [&](EventListener* listener) {
                               listener->set_registered(false);
                           });
            listeners.clear();
        }

        if (!all_deleted) {
            // TODO:set dirty flag
            delete_ok = false;
        }        
    }

    if (delete_ok) {
        event_listener_.clear();
    }
}

void EventManager::SortEventListeners(std::vector<EventListener*>* listeners) {
	std::sort(listeners->begin(), 
              listeners->end(), 
              [&](const EventListener* l1, 
              const EventListener* l2) {
		          return l1->priority() < l2->priority();
	          });
}

void EventManager::SortEventListeners(int event_id) {
	auto it = event_listener_.find(event_id);
	if (event_listener_.end() != it) {
		SortEventListeners(&it->second);
	}
}

void EventManager::DispatchEvent(std::vector<EventListener*> listeners, 
                                 Event* event) {
    for (auto& l : listeners) {
        if (l->is_enabled()) {
            if (l->is_lua_function()) {
                //LuaBindHelper::instance()->dispatchEvent(l->lua_function_id(), event);
            } else {
                l->event_callback_(event);
            }
        }
    }
}

void EventManager::DispatchCommand(Command* cmd) {
    assert(nullptr != cmd);
    assert(false != cmd->check_command_id());
    if ( !is_enabled_ )
        return;

    DispatchGuard guard(dispatch_count_);

    auto listenerMap = event_listener_.find(cmd->command_id());
    if (event_listener_.end() != listenerMap) {
        DispatchCommandImpl(listenerMap->second, cmd);
    }

    UpdateEventListeners();
}

void EventManager::DispatchCommand(int cmd_id, void* optional_user_data/*= nullptr*/) {
    assert(cmd_id > 0);

    if ( !is_enabled_ )
        return;

    DispatchGuard guard(dispatch_count_);

    Command cmd(cmd_id);
    if(nullptr != optional_user_data) {
        cmd.set_user_data(optional_user_data);
    }

    auto listenerMap = event_listener_.find(cmd_id);
    if (event_listener_.end() != listenerMap) {
        auto listeners = listenerMap->second;
        DispatchCommandImpl(listeners, &cmd);
    }

    UpdateEventListeners();
}

void EventManager::DispatchCommandImpl(std::vector<EventListener*> listeners, Command* cmd) {
    for (auto& l : listeners) {
        if (l->is_enabled()) {
            if (l->is_lua_function()) {
                //LuaBindHelper::instance()->dispatchEvent(l->lua_function_id(), event);
                // TODO : imply LuaBindHelper::instance()->dispatchCommand(l->lua_function_id(), cmd);
            } else {
                l->cmd_callback_(cmd);
            }
        }
    }
}

bool EventManager::SetPriority(std::vector<EventListener*> listener_list, 
                               EventListener* listener, 
                               int priority) {
	auto found = std::find(listener_list.begin(), listener_list.end(), listener);
	if (listener_list.end() != found) {
		listener->set_priority(priority);
		return true;
	}
	return false;
}

void EventManager::UpdateEventListeners() {
	if ( !is_dirty() )
		return;

	DirtyFlag flag = dirty_flag();
	switch (flag) {
	case DirtyFlag::DIRTY_FLAG_NONE:
		break;
	case DirtyFlag::DIRTY_FLAG_LISTENER_PRIORITY_CHANGE: {
			SortEventListeners(dirty_event_id_);
		}
		break;
	case DirtyFlag::DIRTY_FLAG_LISTENER_NUM_CHANGE: {
			if ( !listeners_to_add_.empty() ) {
				for (auto& listener : listeners_to_add_) {
					AddEventListener(listener);
				}
				listeners_to_add_.clear();
			}
		}
		break;
	case DirtyFlag::DIRTY_FLAG_ALL: {
			SortEventListeners(dirty_event_id_);
		}
		break;
	default:
		break;
	}

	set_dirty_flag(DirtyFlag::DIRTY_FLAG_NONE);
}

} // namespace gamer