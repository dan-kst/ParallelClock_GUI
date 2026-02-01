#include <gtkmm/application.h>
#include "ClockWindow.hpp"

auto main(int argc, char* argv[]) -> int{
  auto app = Gtk::Application::create("org.rgr.gui_clock");

  return app->make_window_and_run<GUI::ClockWindow>(argc, argv);
}
