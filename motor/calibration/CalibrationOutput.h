// Copyright [2017] Ruud Cools
#ifndef MOTOR_CALIBRATION_CALIBRATIONOUTPUT_H_
#define MOTOR_CALIBRATION_CALIBRATIONOUTPUT_H_


// Class represents the output of a calibration
class CalibrationOutput {
  GETSET(bool, m_finished, Finished);
  GETSET(std::vector<std::string>, m_output, Output);
  GETSET(int, m_numberOfChapters, NumberOfChapters);
  
 public:
  CalibrationOutput();
  /// add a string as a new chapter
  void addAsChapter(const std::string& i_string);

  /// add a string to an existing chapter
  void addToChapter(const std::string& i_string,
		   const int& i_chapterNumber);
  
  
};


#endif  // MOTOR_CALIBRATION_CALIBRATIONOUTPUT_H_
