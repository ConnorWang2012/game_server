/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  command_manager.cc
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-03-03
  @ description:  see the header file.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#include "command_manager.h"

#include <cassert>
#include <stdint.h>
#include <algorithm>

#include "command.h"
#include "command_listener.h"
#include "event_listener.h"
#include "event_manager.h"
#include "framework/base/common.h"
#include "framework/base/macros.h"

namespace gamer {

CommandManager::CommandManager()
    :event_manager_(nullptr)
    ,cmd_listeners_(nullptr) {
}

CommandManager::~CommandManager() {
    SAFE_DELETE(event_manager_); 
    RemoveAllCmdListenersImpl(true);
    SAFE_DELETE(cmd_listeners_); 
}

CommandManager* CommandManager::instance() {
    static CommandManager* s_cmd_manager = nullptr;
    if (nullptr == s_cmd_manager) {
        s_cmd_manager = new CommandManager();
        if ( !s_cmd_manager->Init() ) {
            SAFE_DELETE(s_cmd_manager); 
        }
    }
    return s_cmd_manager;
}

bool CommandManager::Init() {
    event_manager_ = new EventManager();
    event_manager_->Init();

    cmd_listeners_ = new std::vector<CommandListener*>();

    return true;
}

void CommandManager::DestoryInstance() {
    CommandManager* cmd_manager = instance();
    SAFE_DELETE(cmd_manager);    
}

void CommandManager::AddCmdListener(CommandListener* listener) {
    if (nullptr == listener || listener->is_registered())
        return;

    assert(false != listener->check_validity());

    AddCmdListener(listener, listener->priority());
}

void CommandManager::RemoveCmdListener(CommandListener* listener) {
    if (nullptr == listener)
        return;
    event_manager_->RemoveEventListener(listener->event_listener_);
    RemoveCmdListenerImpl(listener, false);
}

void CommandManager::RemoveCmdListenerWithCleanup(CommandListener* listener) {
    if (nullptr == listener)
        return;
    event_manager_->RemoveEventListenerWithCleanup(listener->event_listener_);
    RemoveCmdListenerImpl(listener, true);
}

void CommandManager::RemoveAllCmdListeners() {
    RemoveAllEventListeners(false);
    RemoveAllCmdListenersImpl(false);
}

void CommandManager::RemoveAllCmdListenersWithcleanup() {
    RemoveAllEventListeners(true);
    RemoveAllCmdListenersImpl(true);
}

void CommandManager::SendCmd(Command* cmd) {
    event_manager_->DispatchCommand(cmd);
}

void CommandManager::SendCmd(int cmd_id) {
    event_manager_->DispatchCommand(cmd_id);
}

void CommandManager::SendCmd(int cmd_id, void* user_data) {
    event_manager_->DispatchCommand(cmd_id, user_data);
}

void CommandManager::AddCmdListener(CommandListener* listener, int priority) {
    cmd_listeners_->push_back(listener);
    event_manager_->AddEventListener(listener->event_listener_, priority);

    SortCmdListeners(cmd_listeners_);
}

void CommandManager::SortCmdListeners(std::vector<CommandListener*>* cmd_listeners) {
    if (cmd_listeners->empty())
        return;

    std::sort(cmd_listeners->begin(), 
              cmd_listeners->end(), 
              [&](const CommandListener* l1, 
                  const CommandListener* l2) {
                   return l1->priority() < l2->priority();
              });
}

void CommandManager::RemoveCmdListenerImpl(CommandListener* listener, bool cleanup) {
    auto it = std::find(cmd_listeners_->begin(), cmd_listeners_->end(), listener);
    if (cmd_listeners_->end() != it) {
        if (cleanup){
            SAFE_DELETE(*it);
        } else {
            ((CommandListener*)*it)->set_registered(false);
        }
        cmd_listeners_->erase(it);
    }
}

void CommandManager::RemoveAllCmdListenersImpl(bool cleanup) {
    if (event_manager_->is_dispatching()) {
        // TODO:set dirty flag
        return;
    }

    if (cleanup) {
        std::for_each (cmd_listeners_->begin(), 
                       cmd_listeners_->end(), 
                       gamer::delete_vector_obj());
    } else {
        std::for_each (cmd_listeners_->begin(), 
                       cmd_listeners_->end(), 
                       [&](CommandListener* cmd_listener) {        
                           cmd_listener->set_registered(false);
                       });
    }

    cmd_listeners_->clear();
}

void CommandManager::RemoveAllEventListeners(bool cleanup) {
    if (cleanup) {
        event_manager_->RemoveAllEventListenersWithCleanup();
    } else {
        event_manager_->RemoveAllEventListeners();
    }
}

} // namespace gamer