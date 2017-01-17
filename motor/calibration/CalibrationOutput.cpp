// Copyright Ruud Cools [2017]

#include <macroHeader.h>
#include <CalibrationOutput.h>

CalibrationOutput::CalibrationOutput()
  : m_finished(false),
    m_output({""}),
    m_numberOfChapters(0) {
}


void CalibrationOutput::addAsChapter(const std::string& i_string) {
  addToChapter(i_string, m_numberOfChapters);
  ++m_numberOfChapters;
}



void CalibrationOutput::addToChapter(const std::string& i_string,
				     const unsigned int& i_chapterNumber) {
  if (m_numberOfChapters <= i_chapterNumber) {
    m_output.push_back(i_string);
  } else {
    m_output[i_chapterNumber].append(i_string);
  }
}


std::string CalibrationOutput::getChapter(const unsigned int& i_chapterNumber) {
  if (i_chapterNumber >= m_numberOfChapters) {
    return "";
  } else {
    return m_output[i_chapterNumber];
  }
}


std::vector<std::string> CalibrationOutput::getAllChapters() {
  return m_output;
}
