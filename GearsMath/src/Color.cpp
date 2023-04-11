#include "Color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r{ r }, g{ g }, b{ b }, a{ a }
{}

Color::Color(uint8_t rgba) : rgba{ rgba }
{}
