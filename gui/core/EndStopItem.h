#ifndef GUI_CORE_ENDSTOPITEM_H_
#define GUI_CORE_ENDSTOPITEM_H_

#include "BaseRobotItem.h"
class EndStop;

class EndStopItem : public BaseRobotItem {
  GETSET(std::shared_ptr<EndStop>, m_endStop, EndStop);
 public:
  EndStopItem(BaseRobotItem* i_parrentItem,
	      std::shared_ptr<EndStop> i_endStop);
  virtual QVariant getPropertyData(int row,int column) const;
  virtual bool setPropertyData(int row,int column, const QVariant& value);
  virtual bool construct();

 private:
  //No default constructor!
  EndStopItem();
  static const QList<QString> propertyList;
};



#endif // GUI_CORE_ENDSTOPITEM_H_
