#include "ToggleButton.h"
#include "Timer.h"
#include "Arduino.h"

const int ToggleButton::s_defaultKeyPollTime = 50;

//-----------------------------------------------------------------------------

class MyDebounceTimerAdatper : public TimerAdapter
{
private:
  ToggleButton* m_toggleButton;
  bool m_lastWasButtonPressed;
  
public:
  MyDebounceTimerAdatper(ToggleButton* toggleButton)
  : m_toggleButton(toggleButton)
  , m_lastWasButtonPressed(false)
  { }
  
  void timeExpired()
  {
    if (0 != m_toggleButton)
    {
      //Serial.println("timeExpired");
      bool currentIsButtonPressed = m_toggleButton->isButtonPressed();
      
      if (m_lastWasButtonPressed != currentIsButtonPressed)
      {
        m_lastWasButtonPressed = currentIsButtonPressed;
        if (currentIsButtonPressed)
        {
          m_toggleButton->toggle();
          //Serial.println("toggle");
        }
      } 
    }
  }
};

//-----------------------------------------------------------------------------

ToggleButton::ToggleButton(int buttonPin, int indicatorPin)
: m_debounceTimer(new Timer(new MyDebounceTimerAdatper(this), Timer::IS_RECURRING, s_defaultKeyPollTime))
, m_isActive(false)
, m_buttonPin(buttonPin)
, m_indicatorPin(indicatorPin)
{ 
  pinMode(m_buttonPin, INPUT);
  digitalWrite(m_buttonPin, LOW); // pull-down

  pinMode(m_indicatorPin, OUTPUT);
  digitalWrite(m_indicatorPin, m_isActive);
}

ToggleButton::~ToggleButton()
{
  delete m_debounceTimer->adapter();
  delete m_debounceTimer; m_debounceTimer = 0;
}

bool ToggleButton::isActive()
{
  return m_isActive;
}

void ToggleButton::setIsActive(bool isActive)
{
  m_isActive = isActive;
  digitalWrite(m_indicatorPin, m_isActive);
}

void ToggleButton::toggle()
{
  m_isActive = !m_isActive;
  digitalWrite(m_indicatorPin, m_isActive);
}

bool ToggleButton::isButtonPressed()
{
  return (digitalRead(m_buttonPin));
}

