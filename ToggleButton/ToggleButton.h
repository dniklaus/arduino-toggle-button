#ifndef _ToggleButton_H
#define _ToggleButton_H

class Timer;

//-----------------------------------------------------------------------------

class ToggleButtonAdapter
{
public:
  virtual void notifyStatusChanged(bool isActive) = 0;

protected:
  ToggleButtonAdapter() { }
  virtual ~ToggleButtonAdapter() { }

private:  // forbidden functions
  ToggleButtonAdapter(const ToggleButtonAdapter& src);              // copy constructor
  ToggleButtonAdapter& operator = (const ToggleButtonAdapter& src); // assignment operator
};

//-----------------------------------------------------------------------------

class ToggleButton
{
public:
  /**
   * Constructor.
   * @param buttonPin 
   * @param indicatorPin
   */
  ToggleButton(int buttonPin, int indicatorPin = -1, bool isButtonNegativeLogic = false, ToggleButtonAdapter* adapter = 0);
  virtual ~ToggleButton();
  ToggleButtonAdapter* adapter();
  void attachAdapter(ToggleButtonAdapter* adapter);
  bool isActive();
  void setIsActive(bool isActive);
  void toggle();
  bool isButtonPressed();

  /**
   * Constant for isButtonNegativeLogic parameter of the constructor (@see ToggleButton()), to create a button switching the pin to HIGH.
   */
  static const bool IS_POS_LOGIC;

  /**
   * Constant for isButtonNegativeLogic parameter of the constructor (@see ToggleButton()), to create a button switching the pin to LOW.
   */
  static const bool IS_NEG_LOGIC;

private:
  Timer* m_debounceTimer;
  ToggleButtonAdapter* m_adapter;
  bool m_isButtonNegativeLogic;
  bool m_isActive;
  int m_buttonPin;
  int m_indicatorPin;
  static const int s_defaultKeyPollTime;
  
private:  // forbidden functions
  ToggleButton(const ToggleButton& src);              // copy constructor
  ToggleButton& operator = (const ToggleButton& src); // assignment operator
};

#endif

