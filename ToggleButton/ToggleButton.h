#ifndef _ToggleButton_H
#define _ToggleButton_H

class Timer;

class ToggleButton
{
public:
  /**
   * Constructor.
   * @param buttonPin 
   * @param indicatorPin
   */
  ToggleButton(int buttonPin, int indicatorPin);
  virtual ~ToggleButton();
  bool isActive();
  void setIsActive(bool isActive);
  void toggle();
  bool isButtonPressed();

private:
  Timer* m_debounceTimer;
  bool m_isActive;
  int m_buttonPin;
  int m_indicatorPin;
  static const int s_defaultKeyPollTime;
  
private:  // forbidden functions
  ToggleButton(const ToggleButton& src);              // copy constructor
  ToggleButton& operator = (const ToggleButton& src); // assignment operator
};

#endif

