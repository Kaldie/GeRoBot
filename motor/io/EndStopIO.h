#ifndef MOTOR_IO_ENDSTOPIO_H
#define MOTOR_IO_ENDSTOPIO_H

#include <XMLBuilder.h>
#include <EndStop.h>

class EndStopIO: public XMLBuilder {
  GETSET(std::shared_ptr<EndStop>, m_endStop, EndStop);

 public:
  /// Build the joint based on the given node
  virtual void build();
  /// Constructor
  explicit EndStopIO(const pugi::xml_node&);
  /// Update the node based on the given joint pointer
  bool update(const std::shared_ptr<EndStop>& i_endStop);
  /// Create a new joint node
  static bool createNode(pugi::xml_node*);
 private:
  // No default contructor!!
  EndStopIO();
};

#endif // JointControllerBUilder
