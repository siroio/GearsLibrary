#include "InputState.h"

Glib::InputState::InputState(MouseButton button) :
    type{ InputType::GAMEPAD }, mouse{ button }
{}

Glib::InputState::InputState(KeyCode button) :
    type{ InputType::KEYBOARD }, keyboard{ button }
{}

Glib::InputState::InputState(GPADKey button) :
    type{ InputType::GAMEPAD }, pad{ button }
{}
