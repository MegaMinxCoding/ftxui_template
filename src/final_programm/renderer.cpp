// FTXUI DOKU:
// https://arthursonzogni.github.io/FTXUI/


//#include <cmath>// for sin, cos
#include <chrono>// for orperator s and chrono literals
#include <cmath>// for vec2f
#include <cstdlib>// abs
#include <ftxui/dom/elements.hpp>// for canvas, Element, separator, hbox, operator|, border
#include <ftxui/screen/screen.hpp>// for Pixel
#include <iostream>
#include <memory>// for allocator, shared_ptr, __shared_ptr_access
#include <string>// for string, basic_string
#include <utility>// for move
#include <vector>// for vector, __alloc_traits<>::value_type


#include "ftxui/component/captured_mouse.hpp"// for ftxui
#include "ftxui/component/component.hpp"// for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"// for ComponentBase
#include "ftxui/component/event.hpp"// for Event
#include "ftxui/component/mouse.hpp"// for Mouse
#include "ftxui/component/screen_interactive.hpp"// for ScreenInteractive
#include "ftxui/dom/canvas.hpp"// for Canvas
#include "ftxui/screen/color.hpp"// for Color, Color::Red, Color::Blue, Color::Green, ftxui
#include <ftxui/screen/screen.hpp>


// const values to control the game flow
constexpr int canvas_width{ 150 }, canvas_height{ 100 };// most of the ingame structure is relative to the canvas
constexpr float ball_direction_delta{ 1.5f };// ball velocity = ball_direction_delta = the speed of the ball
constexpr int paddle_width{ 14 }, paddle_height{ 2 };
constexpr int block_width{ 8 }, block_height{ 4 };
constexpr int block_distance_x{ block_width + 2 },
  block_distance_y{ block_height + 2 };// the distance between the start of block 1 and start of block 2

constexpr int block_offset_top{ 8 }, block_offset_sides{ 5 },
  block_offset_bottom{ 45 };// the space between the border and the block matrix
constexpr int max_number_blocks_x{ 13 }, max_number_blocks_y{ 8 };// the max number count

// base class for every ui element
class Rectangle
{
public:
  ftxui::Color color;
  int width, height, _x, _y;
  Rectangle(float x, float y)
  {
    pos_x = x;
    pos_y = y;
  }

  int x() const { return std::abs(pos_x); }
  int y() const { return std::abs(pos_y); }
  int top() const { return y(); }
  int bottom() const { return y() + height; }
  int left() const { return x(); }
  int right() const { return x() + width; }
  float mid_x() const { return x() + 0.5f * width; }

  void draw(ftxui::Canvas &canvas);

  float pos_x{ 0.f }, pos_y{ 0.f };
};


// has additionally a direction (to the default position variable)
class Ball : public Rectangle
{
public:
  float direction_y{ -ball_direction_delta }, direction_x{ ball_direction_delta };
  Ball(float x, float y) : Rectangle(x, y)
  {
    color = ftxui::Color::RedLight;
    width = 2;
    height = 2;
  }
  bool updateAndCheckForScoreReset(ftxui::Canvas &canvas, bool canMove)
  {
    // return value is responsible for resetting the score if "true"
    bool hitBottom = false;


    // If it's leaving toward the left, we need to set
    // horizontal direction to a positive value (towards the right).
    if (left() <= 0) direction_x = ball_direction_delta;

    // Otherwise, if it's leaving towards the right, we need to
    // set horizontal velocity to a negative value (towards the left).
    else if (right() + 1 >= canvas_width)
      direction_x = -ball_direction_delta;

    // The same idea can be applied for top/bottom collisions.
    if (top() <= 0)
      direction_y = ball_direction_delta;
    else if (bottom() + 1 >= canvas.height()) {
      direction_y = -ball_direction_delta;
      hitBottom = true;
    }

    // canMove is used to avoid higher refresh rates due to mouse movement events
    if (canMove) move();
    return hitBottom;
  }
  void draw(ftxui::Canvas &canvas)
  {
    // draw it on the canvas (2 blocks is a square 2x2 --> smallest displayabl ftxui unit)
    canvas.DrawBlock(x(), y(), 1, color);
    canvas.DrawBlock(x() + 1, y(), 1, color);
  }

private:
  void move()
  {
    pos_y += direction_y;
    pos_x += direction_x;
  }
};

// is responsive to the mouse input -> x value set in "update(int x)"
class Paddle : public Rectangle
{
public:
  Paddle(int x, int y) : Rectangle(x, y)
  {
    color = ftxui::Color::Aquamarine1;
    width = paddle_width;
    height = paddle_height;
  }
  void draw(ftxui::Canvas &canvas) { canvas.DrawBlockLine(pos_x, pos_y, pos_x + width, pos_y, color); }
  void update(int mouse_x) { pos_x = mouse_x; }
};

// static ui element that only gets drawn when not destroyed
class Block : public Rectangle
{
public:
  bool destroyed{ false };
  Block(int x, int y) : Rectangle(x, y)
  {
    width = block_width;
    height = block_height;
  }
  Block(const Block &b) : Rectangle(b.x(), b.y())
  {

    width = block_width;
    height = block_height;
    destroyed = b.destroyed;
  }
  void destroy() { destroyed = true; }
  void draw(ftxui::Canvas &canvas)
  {
    if (destroyed) return;
    canvas.DrawBlockLine(x(), y(), x() + width, y());
    canvas.DrawBlockLine(x(), y() + 2, x() + width, y() + 2);
  }
};

std::vector<std::vector<Block>> initBlockMatrix()
{
  std::vector<std::vector<Block>> matrix{};// accumulation of rows

  // check if its outside of the canvas or has too many rows
  for (int i_y = 0; (i_y * block_distance_y + block_offset_top) < (canvas_height - block_offset_bottom)
                    && i_y <= max_number_blocks_y;
       i_y++) {

    // init a new row
    std::vector<Block> row{};

    // check if its outside of the canvas or has too many colum-wise elements
    for (int i_x = 0; (i_x * block_distance_x + 2 * block_offset_sides) < canvas_width && i_x <= max_number_blocks_x;
         i_x++) {
      int x = i_x * block_distance_x + block_offset_sides;
      int y = i_y * block_distance_y + block_offset_top;
      row.push_back(Block{ x, y });
    }
    matrix.push_back(row);
  }

  return matrix;
}

// compare two ui elements
template<typename T1, typename T2> bool isIntersecting(T1 const &lhs, T2 const &rhs)
{
  return lhs.right() >= rhs.left() && lhs.left() <= rhs.right() && lhs.bottom() + 1 >= rhs.top()
         && lhs.top() + 1 <= rhs.bottom();
}

// test if pad and ball are colliding and change direction of the ball if nescessary
void testCollision(Paddle const &p, Ball &b)
{
  // If there's no intersection, get out of the function.
  if (!isIntersecting(p, b)) return;

  // Otherwise let's "push" the ball upwards.
  b.direction_y *= -1;
  // And let's direct it dependently on the position where the
  // paddle was hit.
  if (b.mid_x() < p.mid_x())
    b.direction_x = -ball_direction_delta;
  else
    b.direction_x = ball_direction_delta;
}

// test if block and ball are colliding and change direction of the ball if nescessary
bool testCollision(Block &block, Ball &ball)
{
  if (!isIntersecting(block, ball) && !block.destroyed) return false;

  block.destroy();

  int overlapLeft{ ball.right() + 1 - block.left() - 1 };
  int overlapRight{ block.right() + 1 - ball.left() - 1 };
  int overlapTop{ ball.bottom() + 1 - block.top() - 1 };
  int overlapBottom{ block.bottom() + 1 - ball.top() - 1 };

  bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
  bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

  int minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
  int minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

  if (abs(minOverlapX) < abs(minOverlapY))
    ball.direction_x = ballFromLeft ? -ball_direction_delta : ball_direction_delta;
  else
    ball.direction_y = ballFromTop ? -ball_direction_delta : ball_direction_delta;

  // only return true when the block gets destroyed (only once a block)
  return true;
}

int main(int argc, char const *argv[])
{
  using namespace ftxui;

  // tick used to simulate stable "frames per seconds" -> otherwise frequence disruption from mouse movement events
  bool tick = true;

  // score counter at the top
  std::string score = "Your current score: ";
  int destroyedBlocksStreakCounter = 0;

  // copies a new block matrix in the main method
  std::vector<std::vector<Block>> blockMatrix = initBlockMatrix();


  int mouse_x = 0, mouse_y = 0;
  Paddle p{ canvas_width / 2, canvas_height - 10 };
  Ball b{ canvas_width / 2, canvas_height - 30 };

  auto renderer = Renderer([&] {
    Canvas c = Canvas(canvas_width, canvas_height);

    // paddle
    testCollision(p, b);
    p.update(mouse_x);
    p.draw(c);

    // ball
    if (b.updateAndCheckForScoreReset(c, tick)) destroyedBlocksStreakCounter = 0;// ball hit the bottom
    tick = false;
    b.draw(c);

    // iterate all blocks
    for (auto &row : blockMatrix) {
      for (auto &block : row) {
        if (block.destroyed) continue;
        if (testCollision(block, b)) destroyedBlocksStreakCounter++;
        block.draw(c);
      }
    }

    // score
    c.DrawText(0, 0, score + std::to_string((destroyedBlocksStreakCounter * 50)));

    // bottom line in blue - just for optical attractiveness
    c.DrawBlockLine(0, canvas_height - 1, canvas_width, canvas_height - 1, Color::Blue1);

    // wrap the ftxui element with a border
    return canvas(std::move(c)) | border;
  });


  // this is a hook to capture the last mouse event
  // done by decorating the RendererComponent from ftxui
  renderer |= CatchEvent([&](Event e) {
    if (e.is_mouse()) {
      mouse_x = (e.mouse().x - 1) * 2;// recommended translation of captured x
      mouse_y = (e.mouse().y - 1) * 4;// recommended translation of captured y
    }
    return true;// return irrelevant
  });

  // because we define the canvas size, we can set the screen to FitComponent
  auto screen = ScreenInteractive::FitComponent();

  // simulate the stable FPS --> for faster FPS reduce the sleep time
  bool refresh_ui = true;
  std::thread refresh_ui_thread([&] {
    while (refresh_ui) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.05s);
      tick = true;
      screen.PostEvent(Event::Custom);
    }
  });

  // loop is used to display a renderer from ftxui
  screen.Loop(renderer);

  // finish the threading work
  refresh_ui = false;
  refresh_ui_thread.join();
  return EXIT_SUCCESS;
}
