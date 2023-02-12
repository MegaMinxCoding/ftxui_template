//#include <cmath>// for sin, cos
#include <ftxui/dom/elements.hpp>// for canvas, Element, separator, hbox, operator|, border
#include <ftxui/screen/screen.hpp>// for Pixel
#include <memory>// for allocator, shared_ptr, __shared_ptr_access
#include <string>// for string, basic_string
#include <utility>// for move
#include <vector>// for vector, __alloc_traits<>::value_type

#include "ftxui/component/component.hpp"// for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"// for ComponentBase
#include "ftxui/component/event.hpp"// for Event
#include "ftxui/component/mouse.hpp"// for Mouse
#include "ftxui/component/screen_interactive.hpp"// for ScreenInteractive
#include "ftxui/dom/canvas.hpp"// for Canvas
#include "ftxui/screen/color.hpp"// for Color, Color::Red, Color::Blue, Color::Green, ftxui


int main(int argc, char const *argv[])
{

  using namespace ftxui;
  int counter = 0,
  int mouse_x = 0, mouse_y = 0;
  auto renderer = Renderer([&] {
    Canvas c = Canvas(100, 100);
    auto x_str = std::to_string(mouse_x);
    auto y_str = std::to_string(mouse_y);
    c.DrawText(2, 4, x_str + " , " + y_str);
    c.DrawBlock();

    // c.DrawBlockCircleFilled(mouse_x, mouse_y, 30);
    // std::cout << mouse_x;
    return canvas(std::move(c));
  });

  auto tab = Container::Tab(
    {
      renderer,
    },
    0);
  // This capture the last mouse position.
  auto tab_with_mouse = CatchEvent(renderer, [&](Event e) {
    std::cout << ;
    if (e.is_mouse()) {
      mouse_x = (e.mouse().x - 1) * 2;
      mouse_y = (e.mouse().y - 1) * 4;
    }
    return false;
  });

  auto component = Container::Horizontal({ tab_with_mouse });


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

  auto screen = ScreenInteractive::FitComponent();

  screen.Loop(tab_with_mouse);
  return EXIT_SUCCESS;
}
