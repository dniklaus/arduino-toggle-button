#include "ToggleButton.h"
#include "Timer.h"
#include "Arduino.h"

const bool ToggleButton::IS_POS_LOGIC = false;
const bool ToggleButton::IS_NEG_LOGIC = true;
const int  ToggleButton::BTN_NC       = -1;
const int  ToggleButton::IND_NC       = -1;

const int  ToggleButton::s_defaultKeyPollTime = 50;

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
      bool currentIsButtonPressed = m_toggleButton->isButtonPressed();
      
      if (m_lastWasButtonPressed != currentIsButtonPressed)
      {
        m_lastWasButtonPressed = currentIsButtonPressed;
        if (currentIsButtonPressed)
        {
          m_toggleButton->toggle();
        }
      } 
    }
  }
};

//-----------------------------------------------------------------------------

ToggleButton::ToggleButton(int buttonPin, int indicatorPin, bool isButtonNegativeLogic, ToggleButtonAdapter* adapter)
: m_debounceTimer(new Timer(new MyDebounceTimerAdatper(this), Timer::IS_RECURRING, s_defaultKeyPollTime))
, m_adapter(adapter)
, m_isButtonNegativeLogic(isButtonNegativeLogic)
, m_isActive(false)
, m_buttonPin(buttonPin)
, m_indicatorPin(indicatorPin)
{ 
  if (0 <= m_buttonPin)
  {
    pinMode(m_buttonPin, INPUT);
    digitalWrite(m_buttonPin, m_isButtonNegativeLogic ? HIGH : LOW); // pull
  }
  if (0 <= m_indicatorPin)
  {
    pinMode(m_indicatorPin, OUTPUT);
    digitalWrite(m_indicatorPin, m_isActive);
  }
  if (0 != m_adapter)
  {
    m_adapter->assignToggleButton(this);
  }
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
  if (0 <= m_indicatorPin)
  {
    digitalWrite(m_indicatorPin, m_isActive);
  }
  if ((0 != m_adapter) && (changed))
  {
    m_adapter->notifyStatusChanged(m_isActive);
  }
}

void ToggleButton::toggle()
{
  m_isActive = !m_isActive;
  if (0 <= m_indicatorPin)
  {
    digitalWrite(m_indicatorPin, m_isActive);
  }
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChanged(m_isActive);
  }
}

bool ToggleButton::isButtonPressed()
{
  bool pressed = false;
  if (0 <= m_buttonPin)
  {
    pressed = digitalRead(m_buttonPin);
    pressed = (m_isButtonNegativeLogic ? !pressed : pressed);
  }
  return pressed;
}

