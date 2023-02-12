#ifndef CUST_KEYS
#define CUST_KEYS

#include "ftxui/component/event.hpp"// for Event
#include "ftxui/component/mouse.hpp"// for Mouse
#include "ftxui/component/screen_interactive.hpp"// for ScreenInteractive

enum class KeyInput { arr_left, arr_right, none };

KeyInput getInputFromEvent(ftxui::Event &e)
{
  using namespace ftxui;
  if (e.is_mouse() || e.is_character()) { return KeyInput::none; }// sanity check for quick exit

  std::string event_code_arrLeft = "27 91 68";// tested with example: print_key_press
  std::string event_code_arrRight = "27 91 67";// tested with example: print_key_press
  std::string event_code_arrUp = "27 91 65";// tested with example: print_key_press
  std::string event_code_arrDown = "27 91 66";// tested with example: print_key_press

  std::string out;

  for (auto &it : e.input()) out += " " + std::to_string((unsigned int)it);// combine the event code
  out.erase(0, 1);// remove the first whitespace

  if (out == event_code_arrRight) { return KeyInput::arr_right; }
  if (out == event_code_arrLeft) { return KeyInput::arr_left; }

  return KeyInput::none;// everything else
}

#endif