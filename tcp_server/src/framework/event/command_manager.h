/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  command_manager.h
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-03-03
  @ description:  offer function to manager commands:send cmd,response cmd,etc.
                  it is implemented in terms of event manager.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_COMMAND_MANAGER_H_
#define CONNOR_GAME_SRC_COMMAND_MANAGER_H_

#include <vector>
#include <string>

namespace gamer {

class Command;
class CommandListener;
class EventManager;

class CommandManager {
  public: 
	CommandManager& operator=(const CommandManager&) = delete;

	CommandManager(const CommandManager&) = delete;

    ~CommandManager();

    static CommandManager* instance();

    static void DestoryInstance();

    void AddCmdListener(CommandListener* listener);

    // remove the cmd listener from the instance of cmd manager but do not delete it.
    void RemoveCmdListener(CommandListener* listener);

    // remove the cmd listener from the instance of cmd manager and delete it.
    void RemoveCmdListenerWithCleanup(CommandListener* listener);

    // remove all cmd listeners from the instance of cmd manager but do not delete any.
    void RemoveAllCmdListeners();

    // remove all cmd listeners from the instance of cmd manager and delete all.
    void RemoveAllCmdListenersWithcleanup();

    void SendCmd(Command* cmd);

    void SendCmd(int cmd_id);

    // TODO : bind to lua
    void SendCmd(int cmd_id, void* user_data);

  private:
    CommandManager();

    bool Init();

    void AddCmdListener(CommandListener* listener, int priority);

    void SortCmdListeners(std::vector<CommandListener*>* cmd_listeners);

    void RemoveCmdListenerImpl(CommandListener* listener, bool cleanup);

    void RemoveAllCmdListenersImpl(bool cleanup);

    void RemoveAllEventListeners(bool cleanup);

    EventManager* event_manager_;
    std::vector<CommandListener*>* cmd_listeners_;
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_COMMAND_MANAGER_H_