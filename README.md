[![Stories in Ready](https://badge.waffle.io/dniklaus/arduino-toggle-button.png?label=ready&title=Ready)](https://waffle.io/dniklaus/arduino-toggle-button)

# Toggle Button object model.

* toggles its status on button status change from "not pressed" to "pressed"
* button signal is debounced by a 50ms timer
* button pin is configurable
* status indication pin is configurable
* button logic is configurable
* status change notifications can be emitted through injectable specific adapter implementation
* status can be changed and toggled by client program
* also useful to implement virtual binary status
* also useful when just a a button debouncer is needed

