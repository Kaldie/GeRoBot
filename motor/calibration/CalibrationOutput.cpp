// Copyright Ruud Cools [2017]

#include <macroHeader.h>
#include <CalibrationOutput.h>

CalibrationOutput::CalibrationOutput()
  : m_finished(false),
    m_output({""}),
    m_numberOfChapters(0) {
}


void CalibrationOutput::addAsChapter(const std::string& i_string) {
  addToChapter(i_string, m_numberOfChapters+1);
}



void CalibrationOutput::addToChapter(const std::string& i_string,
				     const int& i_chapterNumber) {
  if (m_numberOfChapters <= i_chapterNumber) {
    m_output.push_back(i_string);
  } else {
    m_output[i_chapterNumber].append(i_string);
  }
}
