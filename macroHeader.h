#ifndef macroHeader_H
#define macroHeader_H

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PIN_HIGH 1
#define PIN_LOW 0

#define DEFAULT_STATE PIN_HIGH

#define DELAY_BETWEEN_STEP_MICROSECONDS 250
#define HAS_RPI false

#define PI 3.1415926535897932384626433
#define TOLERANCE 0.1

//Create getter and setter for properties of a class
#define GETSET(type, varName, property)		\
private:					\
 type varName;					\
public:						\
 const type& get##property() const		\
 {						\
     return varName;				\
 }						\
 						\
 void set##property(const type& val)		\
 {						\
     varName = val;				\
 }


//Create getter and setter for properties of a class
#define GETSET_NO_CONST(type, varName, property)	\
private:						\
 type varName;						\
public:							\
 type& get##property()					\
 {							\
     return varName;					\
 }							\
 							\
 void set##property(const type& val)			\
 {							\
     varName = val;					\
 }


//Create getter and setter for Pointers in a class
#define GETSETPOINTER(type, varName, property)		\
private:						\
 type* varName;						\
public:							\
 type* get##property() const				\
 {							\
     return varName;					\
 }							\
 							\
 void set##property(type* val)				\
 {							\
     varName = val;					\
 }


#define GET(type, varName, property)		\
private:					\
 type varName;					\
public:						\
 const type& get##property() const		\
 {						\
     return varName;				\
 }


//Create debug messages

//#define NDEBUG
#ifdef NDEBUG
#define DEBUG_MSG(message)
#else
#define DEBUG_MSG(message) std::cerr << message << std::endl
#endif

#define LOG_ERROR(message) do {DEBUG_MSG("[ERROR] ("<<__FILE__<<":"<<__LINE__<<"): "<<message); throw 100;} while(false)

#define LOG_WARNING(message) do {DEBUG_MSG("[WARN]("<<__FILE__<<":"<<__LINE__<<"): "<<message);} while(false)

#define LOG_DEBUG(message) do {DEBUG_MSG("[DEBUG] ("<<__FILE__<<":"<<__LINE__<<"): "<<message);} while(false)

#define LOG_INFO(message) do {DEBUG_MSG("[INFO] ("<<__FILE__<<":"<<__LINE__<<"): "<<message);} while(false)

#endif // macro
