// Copyright [2017] Ruud Cools
#include "./PointCalibrationWidget.h"
#include <regex>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <PointCalibration.h>
#include "../RobotMovementWidget.h"
#include "../../MainWindow.h"

PointCalibrationWidget::PointCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
					       QWidget* i_parent /*= 0*/) 
  : CalibrationWidget(i_calibration, i_parent),
    m_xLineEdit(nullptr),
    m_yLineEdit(nullptr),
    m_addPointButton(nullptr),
    m_removePointButton(nullptr),
    m_registerPointButton(nullptr),
    m_currentPointNumber(0) {
  initialise();
}


void PointCalibrationWidget::initialise() {
  // create robot movement widget
  createAdditionalUIElements();
  updateAddPointButton();

  // if the editing is finished, check if we can make a point from it
  connect(m_xLineEdit, &QLineEdit::editingFinished,
	  this, &PointCalibrationWidget::updateAddPointButton);
  connect(m_yLineEdit, &QLineEdit::editingFinished,
	  this, &PointCalibrationWidget::updateAddPointButton);

  // conenct the add button to the add to list
  connect(m_addPointButton, &QPushButton::clicked,
	  this, &PointCalibrationWidget::addPointToList);
  connect(m_removePointButton, &QPushButton::clicked,
	  this, &PointCalibrationWidget::removePointFromWidget);
  connect(m_registerPointButton, &QPushButton::clicked,
	  this, &PointCalibrationWidget::registerPoint);

  connectModelWithMovementWidget();
}


void PointCalibrationWidget::connectModelWithMovementWidget() {
  MainWindow* mainWindow = dynamic_cast<MainWindow*>(window());
  RobotMovementWidget* robotMovementWidget = findChild<RobotMovementWidget*>();
  if (mainWindow && robotMovementWidget) {
    connect(mainWindow->getModelPointer().get(), &QAbstractItemModel::dataChanged,
	    robotMovementWidget, &RobotMovementWidget::updateFromConfiguration);
    LOG_DEBUG("Yay, connecting the model changed signal " <<
	      "to update from configuration slot");
  } else {
    LOG_DEBUG("Failed to connect the shabba!");
  }  
}


void PointCalibrationWidget::createAdditionalUIElements() {
  // list with all the points 
  m_pointListBox = new QListWidget();
  QHBoxLayout* lowerLayout = new QHBoxLayout();
  QVBoxLayout* inputLayout = new QVBoxLayout();
  // form layout were we add x and y coordinates
  QFormLayout* formLayout = new QFormLayout();
  
  m_xLineEdit = new QLineEdit();
  m_yLineEdit = new QLineEdit();

  // edit boxes to fill in the coordinates
  formLayout->addRow(tr("X: "), m_xLineEdit);
  formLayout->addRow(tr("Y: "), m_yLineEdit);

  // Create a button layout, buttons, add them to the layout and add the layout
  QHBoxLayout* buttonLayout = new QHBoxLayout();

  // buttons to add and remove points
  m_addPointButton = new QPushButton("Add Point");
  m_addPointButton->setStyleSheet("QPushButton:focus { background-color:rgba(0, 153, 0, 75%); }");
  m_removePointButton = new QPushButton("Remove Point");
  m_registerPointButton = new QPushButton("Register point");

  // add the button to the button layout
  buttonLayout->addWidget(m_addPointButton);
  buttonLayout->addWidget(m_removePointButton);
  buttonLayout->addWidget(m_registerPointButton);
  
  // add the layouts to the main layout
  inputLayout->addLayout(formLayout);
  inputLayout->addLayout(buttonLayout);
  inputLayout->addWidget(m_pointListBox);
   lowerLayout->addLayout(inputLayout);
  RobotMovementWidget* robotMovementWidget =
    new RobotMovementWidget(m_calibration->getRobot());
  robotMovementWidget->hideMostFrames(true);
  lowerLayout->addWidget(robotMovementWidget);


  calibrationLayout->addLayout(lowerLayout);
  // set the tab order
  setTabOrder(m_xLineEdit, m_yLineEdit);
  setTabOrder(m_yLineEdit, m_addPointButton);
  setTabOrder(m_addPointButton, m_removePointButton);
  setTabOrder(m_removePointButton, m_registerPointButton);

  executeButton->setText("Calculate");
}


void PointCalibrationWidget::addPointToList() {
  m_pointListBox->addItem(QString("Point(%1,%2)").arg(m_xLineEdit->text(),
						      m_yLineEdit->text()));
  std::shared_ptr<PointCalibration> pointCalibration =
    std::static_pointer_cast<PointCalibration>(m_calibration);
  pointCalibration->addPoint2D(Point2D(m_xLineEdit->text().toDouble(),
				       m_yLineEdit->text().toDouble()));
  m_xLineEdit->setText("");
  m_yLineEdit->setText("");
  updateAddPointButton();
  updatePointIcons();
}


void PointCalibrationWidget::updateAddPointButton() {
  bool conversionWorkedX, conversionWorkedY;
  m_xLineEdit->text().toDouble(&conversionWorkedX);
  m_yLineEdit->text().toDouble(&conversionWorkedY);
  m_addPointButton->setEnabled(conversionWorkedX && conversionWorkedY);
}


void PointCalibrationWidget::removePointFromWidget() {
  LOG_DEBUG("removePointFromWidget");
  auto pointCalibration = std::static_pointer_cast<PointCalibration>(m_calibration);
  Point2D point;
  QListWidgetItem* widgetItem = m_pointListBox->takeItem(m_pointListBox->currentRow());
  getPointFromListItem(widgetItem,&point);
  pointCalibration->removePoint2D(point);
  delete widgetItem;  
}


void PointCalibrationWidget::registerPoint() {
  std::shared_ptr<PointCalibration> calibration =
    std::static_pointer_cast<PointCalibration>(m_calibration);
  Point2D point;
  getPointFromListItem(m_pointListBox->item(m_currentPointNumber), &point);
  calibration->recordPoint(point);
  ++m_currentPointNumber;
  LOG_DEBUG("Current number: " << m_currentPointNumber);
  updatePointIcons();
  m_removePointButton->setEnabled(false);
}


void PointCalibrationWidget::updatePointIcons() {
  QIcon red = QIcon(":/icons/red_circle.png");
  QIcon orange = QIcon(":/icons/orange_circle.png");
  QIcon green = QIcon(":/icons/green_circle.png");
  for (int row = 0; row < m_pointListBox->count(); ++row) {
    QListWidgetItem* item = m_pointListBox->item(row);
    if (row == m_currentPointNumber) {
      item->setIcon(green);
    } else if (row > m_currentPointNumber) {
      item->setIcon(orange);
    } else if (row < m_currentPointNumber) {
      item->setIcon(red);
    }
  }
}


void PointCalibrationWidget::getPointFromListItem(QListWidgetItem* i_item, Point2D* o_point) {
  auto text = i_item->text().toStdString();  
  std::regex doubleRegex("[-+]?([0-9]*\\.[0-9]+|[0-9]+)");
  std::smatch double_match;
  std::vector<double> coordinates;
  LOG_DEBUG(text);
  while (std::regex_search(text, double_match, doubleRegex)) {
    LOG_DEBUG(double_match.str());
    coordinates.push_back(stod(double_match.str()));
    text = double_match.suffix().str();
  }
  if (coordinates.size() == 2) {
    *o_point = Point2D(coordinates[0], coordinates[1]);
  } else {
    LOG_ERROR("Cannot convert the point text to valid point2D!");
  }
}


void PointCalibrationWidget::executeCalibration() {
  LOG_DEBUG("executeCalibration");
  std::shared_ptr<PointCalibration> calibration =
    std::static_pointer_cast<PointCalibration>(m_calibration);
  calibration->execute();
  emit updateCalibrationOutText();
  /// execute the calibration and update the output 
}
