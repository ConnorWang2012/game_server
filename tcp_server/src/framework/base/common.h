/*******************************************************************************
 @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
 @ filename:	  common.h
 @ author:		  Connor
 @ version:	      1.0.0
 @ date:		  2014-09-12
 @ description:   define common and across platform functions.
 @ others:
 @ history:
 1.date:
 author:
 modification:
 ********************************************************************************/

#ifndef CONNOR_GAME_SRC_FRAMEWORK_COMMON_H_
#define CONNOR_GAME_SRC_FRAMEWORK_COMMON_H_

#include "macros.h"

namespace gamer {

// use for deleting std::vector object in std::for_each.
struct delete_vector_obj {
    template <typename T>
    void operator() (const T* ptr) const {
        SAFE_DELETE(ptr);
    }
};

// use for deleting std::map object in std::for_each.
struct delete_map_obj {
    template <typename T1, typename T2>
    void operator() (const T1, const T2* ptr) const {
        SAFE_DELETE(ptr);
    }
};

} // namespace gamer

#endif // CONNOR_GAME_SRC_FRAMEWORK_COMMON_H_