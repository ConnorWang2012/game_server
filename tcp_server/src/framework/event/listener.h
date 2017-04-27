/*******************************************************************************
  @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
  @ filename:	  listener.h
  @ author:		  Connor
  @ version:	  1.0.0
  @ date:		  2015-03-08
  @ description:  the base class of specific listener, such as event listener.
  @ others:  
  @ history: 
     1.date:
       author:
       modification:
********************************************************************************/

#ifndef CONNOR_GAME_SRC_LISTENER_H_
#define CONNOR_GAME_SRC_LISTENER_H_

#include <string>

namespace gamer {

class Listener {
  public:
    typedef int LuaFunction;

    //Listener();

    virtual ~Listener() {};

    //virtual void execute_callback() = 0;

    virtual bool checkValidity() const = 0 ;

    inline void set_listener_name(const std::string& listener_name) { 
        listener_name_ = listener_name; 
    }

    inline const std::string& listener_name() const { return listener_name_; }

    inline void set_priority(int priority) { priority_ = priority; }

    inline int priority() const { return priority_; }

    inline bool is_lua_function() const { return is_lua_function_; }

    inline void set_lua_function(LuaFunction lua_function) { lua_function_ = lua_function; }

    inline LuaFunction lua_function() const { return lua_function_; }

    inline void set_lua_function_id(const std::string& func_id) { lua_function_id_ = func_id; }

    inline const std::string& lua_function_id() const { return lua_function_id_; }

    inline void set_enabled(bool enabled) { is_enabled_ = enabled; }

    inline bool is_enabled() const { return is_enabled_; }

    inline bool is_registered() const { return is_registered_; }

  protected: 
    // the target id may be a event id, a command id, or other id that the listener listen.
    inline void set_target_id(int target_id) { target_id_ = target_id; }
    
    // the target id may be a event id, a command id, or other id that the listener listen.
    inline int target_id() const { return target_id_; }

    //inline void set_lua_function(LuaFunction func) {
    //    lua_function_ = func;
    //};

    int target_id_;
    std::string listener_name_;
    int priority_;

    bool is_lua_function_;
    LuaFunction lua_function_;
    std::string lua_function_id_;

    bool is_enabled_;
    bool is_registered_;
};

} // namespace gamer

#endif