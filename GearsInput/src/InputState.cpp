#include "InputState.h"

Glib::InputState::InputState(MouseButton button) :
    input{ button }
{}

Glib::InputState::InputState(KeyCode button) :
    input{ button }
{}

Glib::InputState::InputState(GPADKey button) :
    input{ button }
{}
