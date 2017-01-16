// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <JointController.h>
#include "JointCalibrationWidget.h"
#include <Robot.h>
#include <BaseMotor.h>
#include <CalibrationDirector.h>


JointCalibrationWidget::JointCalibrationWidget(const std::shared_ptr<Robot>& i_robot,
                 QWidget* i_parent /*= 0*/) :
  QWidget(i_parent),
  m_robot(i_robot),
  m_jointMap(),
  m_director(std::make_shared<CalibrationDirector>(i_robot)) {
  initialisation();
}


void JointCalibrationWidget::initialisation() {
  setupUi(this);
  // create button group
  m_calibrationButtonGroup = new QButtonGroup(this);
  m_calibrationButtonGroup->setExclusive(false);

  initialiseJointMap();
  populateTypeListbox();
  populateNumberListbox();
  populateButtonGroup();
  updateNumberOfCalibrations();

  // connect the change signal to populate number list box 
  connect(typeComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(populateNumberListbox()));
  connect(typeComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(updateJointInforamtion()));

  // connect the includeCheckbox to the update joint map so it gets updated
  connect(includeCheckBox,SIGNAL(toggled(bool)), this, SLOT(updateJointMap(bool)));
  // connect all the signals which effect the number of calibrations that will have to be done
  connect(includeCheckBox,SIGNAL(toggled(bool)), this, SLOT(updateNumberOfCalibrations()));
  // connect the checkboxes so the director gets updated at change
  connect(m_calibrationButtonGroup,SIGNAL(buttonClicked(int)), this, SLOT(updateCalibrationSelection()));
}


void JointCalibrationWidget::populateButtonGroup() {
  m_calibrationButtonGroup->addButton(endMovementCheckBox);
  m_calibrationButtonGroup->addButton(endPositionCheckBox);
  m_calibrationButtonGroup->addButton(motorCalibrationCheckBox);
  m_calibrationButtonGroup->addButton(pointCalibrationCheckBox);
}


void JointCalibrationWidget::populateTypeListbox() {
  JointController::JointPointerVector jointVector = m_robot->getJointController()->getJointPointerVector();
  std::vector<BaseJoint::MovementType> typeVector;
  for(const auto& joint : jointVector) {
    if (std::find(typeVector.begin(), typeVector.end(), joint->getMovementType()) == typeVector.end()) {
      typeVector.push_back(joint->getMovementType());
    }
  }
  for (const auto& jointType : typeVector) {
    if (jointType == BaseJoint::Rotational) {
      typeComboBox->addItem("Rotational");
    } else if (jointType == BaseJoint::Translational) {
      typeComboBox->addItem("Translational");
    } else {
      LOG_ERROR("Could not resolve joint type");
    }
  }
}


void JointCalibrationWidget::populateNumberListbox() {
  LOG_DEBUG("populateNumberListbox");
  QString currentType = typeComboBox->currentText();
  int numberOfJoints;
  numberComboBox->clear();
  if (currentType.compare("Rotational") == 0) {
    numberOfJoints = m_robot->getJointController()->getNumberOfJoints(BaseJoint::Rotational);
  } else if (currentType.compare("Translational") == 0) {
    numberOfJoints = m_robot->getJointController()->getNumberOfJoints(BaseJoint::Translational);
  } else { /* the first entry is chose, we cannot resolve that!*/
    return;
  }
  for (int i=1; i <= numberOfJoints; ++i) {
    numberComboBox->addItem(QString::number(i));
  }
}


void JointCalibrationWidget::updateJointInforamtion() {
  std::shared_ptr<BaseJoint> joint = resolveJoint();
  if (!joint) {
    movementTypeEdit->setText("");
    positionLineEdit->setText("");
    minRangeEdit->setText("");
    maxRangeEdit->setText("");
    endstopNumberEdit->setText("");
    actuatorTypeEdit->setText("");
    includeCheckBox->setChecked(false);
    return;
  }
  if (joint->getMovementType() == BaseJoint::Rotational) {
    movementTypeEdit->setText("Rotational");
  } else if (joint->getMovementType() == BaseJoint::Translational) {
    movementTypeEdit->setText("Translational");
  }
  positionLineEdit->setText(QString::number(joint->getPosition()));
  minRangeEdit->setText(QString::number(joint->getRange()[0]));
  maxRangeEdit->setText(QString::number(joint->getRange()[1]));
  endstopNumberEdit->setText(QString::number(joint->getEndStops().size()));
  if (joint->getMotor()->getType() == BaseMotor::StepperDriver) {
    actuatorTypeEdit->setText("StepperDriver");
  } else if( joint->getMotor()->getType() == BaseMotor::StepperMotor) {
    actuatorTypeEdit->setText("StepperMotor");
  } else {
    LOG_ERROR("Unknown type!");
  }
  includeCheckBox->setChecked(m_jointMap[joint]);
}


void JointCalibrationWidget::createCalibrations() {
  //TODO: magic()
}


void JointCalibrationWidget::clearCalibrations() {
  //TODO: magic()
}


BaseJoint::JointPointer JointCalibrationWidget::resolveJoint() const {
  QString currentType = typeComboBox->currentText();
  LOG_DEBUG(currentType.toStdString());
  JointController::JointPointerVector jointVector;
  if (currentType.compare("Rotational") == 0) {
    jointVector = m_robot->getJointController()->resolveJoints(BaseJoint::Rotational);
  } else if (currentType.compare("Translational") == 0) {
    jointVector = m_robot->getJointController()->resolveJoints(BaseJoint::Translational);
  } else if (currentType.compare("Type") == 0) {
    return std::shared_ptr<BaseJoint>();
  }
  bool hasWorked;
  LOG_DEBUG(numberComboBox->currentText().toStdString());
  int jointNumber = numberComboBox->currentText().toInt(&hasWorked) - 1;
  std::shared_ptr<BaseJoint> joint;
  if (hasWorked) {
    return jointVector[jointNumber];
  } else {
    return std::shared_ptr<BaseJoint>();
  }
}


void JointCalibrationWidget::initialiseJointMap() {
  for (const auto& element : m_robot->getJointController()->getJointPointerVector()) {
    m_jointMap[element] = true;
  }
  m_director->setJointMap(m_jointMap);
}


void JointCalibrationWidget::updateJointMap(const bool& i_checked) {
  std::shared_ptr<BaseJoint> joint = resolveJoint();
  m_jointMap[joint] = i_checked;
  m_director->updateJointMap(joint, i_checked);
}


void JointCalibrationWidget::updateNumberOfCalibrations() {
  numberOfCalibrationEdit->setText(QString::number(m_director->getNumberOfCalibrations()));
}


void JointCalibrationWidget::updateCalibrationSelection() {
  m_director->setPointCalibration(pointCalibrationCheckBox->isChecked());
  m_director->setMotorCalibration(motorCalibrationCheckBox->isChecked());
  m_director->setEndStopPositionCalibration(endPositionCheckBox->isChecked());
  endMovementCheckBox->setEnabled(endPositionCheckBox->isChecked());
  m_director->setEndStopMovementCalibration
    (endMovementCheckBox->isChecked() && endMovementCheckBox->isEnabled());
  updateNumberOfCalibrations();
}
