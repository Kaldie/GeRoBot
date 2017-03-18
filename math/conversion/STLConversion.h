// Copyright Ruud Cools [2017]

#ifndef MATH_CONVERSION_STLCONVERSION_H_
#define MATH_CONVERSION_STLCONVERSION_H_

#include <BaseConversion.h>


class STLConversion : public BaseConversion {

  GETSET(std::string, m_fileName, FileName);
  
 public:
  /// Constructor without file name  
  STLConversion();

  /// Constructor with file name
  STLConversion(std::string i_fileName);

 private:
  /// Checks if the file exists and loads it
  virtual preConversion() override;

  /// no post processing necessary
  virtual postConversion() override {};

  void loadFile();

  
};


#endif  /// MATH_CONVERSION_STLCONVERSION_H_
