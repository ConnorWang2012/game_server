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

class EventManager;
class Command;
class CommandListener;

class CommandManager {
  public:    
    ~CommandManager();

    static CommandManager* getInstance();

    static void destoryInstance();

    void addCmdListener(CommandListener* listener);

    // remove the cmd listener from the instance of cmd manager but do not delete it.
    void removeCmdListener(CommandListener* listener);

    // remove the cmd listener from the instance of cmd manager and delete it.
    void removeCmdListenerWithCleanup(CommandListener* listener);

    // remove all cmd listeners from the instance of cmd manager but do not delete any.
    void removeAllCmdListeners();

    // remove all cmd listeners from the instance of cmd manager and delete all.
    void removeAllCmdListenersWithcleanup();

    void sendCmd(Command* cmd);

    void sendCmd(int cmd_id);

    // TODO : bind to lua
    void sendCmd(int cmd_id, void* user_data);

  private:
    CommandManager();

    bool init();

    void addCmdListener(CommandListener* listener, int priority);

    void sortCmdListeners(std::vector<CommandListener*>* cmd_listeners);

    void removeCmdListenerImpl(CommandListener* listener, bool cleanup);

    void removeAllCmdListenersImpl(bool cleanup);

    void removeAllEventListeners(bool cleanup);

    EventManager* event_manager_;
    std::vector<CommandListener*>* cmd_listeners_vec_;
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_COMMAND_MANAGER_H_