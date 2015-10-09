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

const bool ToggleButton::IS_POS_LOGIC = false;
const bool ToggleButton::IS_NEG_LOGIC = true;

ToggleButton::ToggleButton(int buttonPin, int indicatorPin, bool isButtonNegativeLogic, ToggleButtonAdapter* adapter)
: m_debounceTimer(new Timer(new MyDebounceTimerAdatper(this), Timer::IS_RECURRING, s_defaultKeyPollTime))
, m_adapter(adapter)
, m_isButtonNegativeLogic(isButtonNegativeLogic)
, m_isActive(false)
, m_buttonPin(buttonPin)
, m_indicatorPin(indicatorPin)
{ 
  pinMode(m_buttonPin, INPUT);
  digitalWrite(m_buttonPin, m_isButtonNegativeLogic ? HIGH : LOW); // pull

  pinMode(m_indicatorPin, OUTPUT);
  digitalWrite(m_indicatorPin, m_isActive);
}

ToggleButton::~ToggleButton()
{
  delete m_debounceTimer->adapter();
  delete m_debounceTimer; m_debounceTimer = 0;
}

ToggleButtonAdapter* ToggleButton::adapter()
{
  return m_adapter;
}

void ToggleButton::attachAdapter(ToggleButtonAdapter* adapter)
{
  m_adapter = adapter;
}

bool ToggleButton::isActive()
{
  return m_isActive;
}

void ToggleButton::setIsActive(bool isActive)
{
  bool changed = (isActive != m_isActive);
  m_isActive = isActive;
  digitalWrite(m_indicatorPin, m_isActive);
  if ((0 != m_adapter) && (changed))
  {
    m_adapter->notifyStatusChanged(m_isActive);
  }
}

void ToggleButton::toggle()
{
  m_isActive = !m_isActive;
  digitalWrite(m_indicatorPin, m_isActive);
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChanged(m_isActive);
  }
}

bool ToggleButton::isButtonPressed()
{
  bool pressed = digitalRead(m_buttonPin);
  return (m_isButtonNegativeLogic ? !pressed : pressed);
}

