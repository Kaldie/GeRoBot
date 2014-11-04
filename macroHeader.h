#ifndef macroHeader_H
#define macroHeader_H

//Common header decleration
#include <algorithm>
#include <fstream>
#include <fcntl.h>
#include <glob.h>
#include <iostream>
#include <map>
#include <memory>
#include <pugixml.hpp>
#include <string>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define PIN_HIGH 1
#define PIN_LOW 0

#define DEFAULT_STATE PIN_HIGH

#define DELAY_BETWEEN_STEP_MICROSECONDS 250
#define HAS_RPI false

#define PI 3.1415926535897932384626433
#define TOLERANCE 0.1

//#define DEBUG

//make #define DEBUG_FILE

//Create getter and setter for properties of a class
#define GETSET(type, varName, property)		\
	private:																\
	type varName;														\
public:																		\
 const type& get##property() const				\
 {																				\
	 return varName;												\
 };																				\
																					\
 void set##property(const type& val)			\
 {																				\
	 varName = val;													\
 };


//Create getter and setter for properties of a class
#define GETSET_NO_CONST(type, varName, property)	\
	private:																				\
	type varName;																		\
public:																						\
 type& get##property()														\
 {																								\
	 return varName;																\
 };																								\
																									\
 void set##property(const type& val)							\
 {																								\
	 varName = val;																	\
 };


//Create getter and setter for Pointers in a class
#define GETSETPOINTER(type, varName, property)	\
	private:																			\
	type* varName;																\
public:																					\
 type* get##property() const										\
 {																							\
	 return varName;															\
 };																							\
																								\
 void set##property(type* val)									\
 {																							\
	 varName = val;																\
 };


#define GET(type, varName, property)						\
	private:																			\
	type varName;																	\
public:																					\
 const type& get##property() const							\
 {																							\
	 return varName;															\
 };


//Create debug messages
#if defined(DEBUG)
#define DEBUG_MSG(message) do {std::cout<<message<<std::endl;} while(false)

#elif defined(DEBUG_FILE)
#define DEBUG_MSG(message) do {				\
  std::ofstream thisStream;					\
  thisStream.open("Debug.save",std::ios_base::app);			\
  thisStream<<message<<std::endl;						\
  thisStream.close();				\
  } while(false)
#else
#define DEBUG_MSG(message)
#endif

#define LOG_ERROR(message) do {						\
    std::stringstream stream;						\
    stream<<std::endl<<"("<<__FILE__<<":"<<__LINE__<<"): "<<message;		\
    throw std::runtime_error(stream.str());} while(false)

#define LOG_WARNING(message) do {DEBUG_MSG("[WARN]("<<__FILE__<<":"<<__LINE__<<"): "<<message);} while(false)

#define LOG_DEBUG(message) do {DEBUG_MSG("[DEBUG] ("<<__FILE__<<":"<<__LINE__<<"): "<<message);} while(false)

#define LOG_INFO(message) do {DEBUG_MSG("[INFO] ("<<__FILE__<<":"<<__LINE__<<"): "<<message);} while(false)


//Forward declared classes
class PinState;
class BaseMotor;
class BaseJoint;
class Robot;

enum MovementType {None,Rotational,Translational}; 
enum TRACE_TYPE{Line,Curve};

typedef std::vector<int> PinVector;
typedef std::map<int,int> PinStateMap;
typedef std::vector<PinState> PinStateSequence; //An array of set pins

typedef std::shared_ptr<BaseJoint> JointPointer;
typedef std::map<std::string,std::string> DirectionConversionMap;

typedef std::vector<PinState> PinStateSequence;
typedef std::vector<JointPointer> JointPointerVector;

typedef std::shared_ptr<Robot> RobotPointer;
#endif // macroHeader
