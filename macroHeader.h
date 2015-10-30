// Copyright [2014] Ruud Cools
#ifndef MACROHEADER_H_
#define MACROHEADER_H_

// Common header decleration
#include <algorithm>
#include <fstream>
#include <fcntl.h>
#include <glob.h>
#include <iostream>
#include <map>
#include <memory>
#include <tuple>
#include <string>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <cassert>
#ifdef QT
#include <QtCore/QPointF>
#endif
#include <pugixml.hpp>

#define PIN_HIGH 1
#define PIN_LOW 0

#define DEFAULT_STATE PIN_HIGH

#define DELAY_BETWEEN_STEP_MICROSECONDS 250
#define HAS_RPI false

#define PI 3.1415926535897932384626433
#define TOLERANCE 0.0001

#define DEBUG
// #define DEBUG_FILE

// Create getter and setter for properties of a class
#define GETSET(type, varName, property)                                 \
  private:                                                              \
  type varName;                                                         \
public:                                                                 \
const type& get##property() const {                                     \
  return varName;                                                       \
};                                                                      \
                                                                        \
void set##property(const type& val) {                                   \
  varName = val;                                                        \
};

#define GETSETPROTECTED(type, varName, property)      \
 protected:                                           \
  type varName;                                       \
 public:                                              \
  const type& get##property() const {                 \
    return varName;                                   \
  };                                                  \
                                                      \
  void set##property(const type& val) {               \
    varName = val;                                    \
  };


// Create getter and setter for properties of a class
#define GETSET_NO_CONST(type, varName, property)                        \
  private:                                                              \
  type varName;                                                         \
public:                                                                 \
type& get##property()                                                   \
{                                                                       \
  return varName;                                                       \
};                                                                      \
                                                                        \
void set##property(const type& val)                                     \
{                                                                       \
  varName = val;                                                        \
};


// Create getter and setter for Pointers in a class
#define GETSETPOINTER(type, varName, property)                          \
protected:                                                              \
  type* varName;                                                        \
public:                                                                 \
type* get##property() const {                                           \
  return varName;                                                       \
};                                                                      \
                                                                        \
void set##property(type* val) {                                         \
  varName = val;                                                        \
};


#define GET(type, varName, property)                                    \
  private:                                                              \
  type varName;                                                         \
public:                                                                 \
const type& get##property() const {                                     \
  return varName;                                                       \
};


// Create debug messages
#if defined(DEBUG)
#define DEBUG_MSG(message) do {std::cout << message << std::endl;} while (false)

#elif defined(DEBUG_FILE)
#define DEBUG_MSG(message) do {                                         \
    std::ofstream thisStream;                                \
    thisStream.open("Debug.save", std::ios_base::app);                  \
    thisStream << message << std::endl;                                 \
    thisStream.close();                                                 \
  } while (false)
#else
#define DEBUG_MSG(message)
#endif

#define LOG_ERROR(message) do {                                         \
    std::stringstream stream;                                           \
    stream << std::endl << "[ERROR_LOG]" <<                             \
        "(" << __FILE__ << ":" << __LINE__ << "): "                     \
           << message;                                                  \
    std::string what = stream.str();                                    \
    throw std::runtime_error(what);} while (false)

#define LOG_WARNING(message)                                            \
  do {DEBUG_MSG("[WARN](" <<                                            \
                __FILE__ << ":" << __LINE__ << "): " << message);} while (false)

#define LOG_DEBUG(message)                                              \
  do {DEBUG_MSG("[DEBUG] (" <<                                          \
                __FILE__<< ":" << __LINE__ << "): " << message);} while (false)

#define LOG_INFO(message)                                               \
  do {DEBUG_MSG("[INFO] ("<<                                            \
                __FILE__ << ":" << __LINE__ << "): " << message);} while (false)


// Forward declared classes
class PinState;
class StateSequence;
class BaseMotor;
class BaseJoint;
class Robot;
class RobotTreeModel;

enum MovementType {None, Rotational, Translational};

enum GUIMovementMode {ToolMode, AxisMode};

/// conversion of motor direction to joint direction
typedef std::map<std::string, std::string> DirectionConversionMap;

/// type used in trace calculations
typedef double traceType;

#endif  // MACROHEADER_H_
