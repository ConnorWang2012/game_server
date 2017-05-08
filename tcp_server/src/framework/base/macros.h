/*******************************************************************************
 @ copyright(C), 2015-2020, ConnorAndHisFriendsCompany.Inc
 @ filename:	  macros.h
 @ author:		  Connor
 @ version:	      1.0.0
 @ date:		  2014-09-12
 @ description:   define common base macros.
 @ others:
 @ history:
 1.date:
 author:
 modification:
 ********************************************************************************/

#ifndef CONNOR_GAME_SRC_BASE_MACROS_H_
#define CONNOR_GAME_SRC_BASE_MACROS_H_

namespace gamer {

#define NS_GAMER_BEGIN					namespace gamer {
#define NS_GAMER_END					}
#define USING_NS_GAMER					using namespace gamer

#define SAFE_DELETE(p)					do { if(nullptr != (p)) { delete (p); (p) = nullptr; } } while(0)

#define CALLBACK_SELECTOR_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CALLBACK_SELECTOR_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_SELECTOR_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_SELECTOR_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)

#if !defined(CONNOR_GAME_DEBUG) || CONNOR_GAME_DEBUG == 0
#define LOG(...)        do {} while (0)
#define LOGERROR(...)   do {} while (0)
#define LOGGREEN(...)   do {} while (0)
#elif CONNOR_GAME_DEBUG == 1
#define LOG(format, ...)        gamer::log::printf(format, ##__VA_ARGS__)
#define LOGERROR(format, ...)   gamer::log::printferr(format, ##__VA_ARGS__)
#define LOGGREEN(format, ...)   gamer::log::printfgreen(format, ##__VA_ARGS__)
#endif

} // namespace gamer

#endif // CONNOR_GAME_SRC_BASE_MACROS_H_
