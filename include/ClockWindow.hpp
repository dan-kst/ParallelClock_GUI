#ifndef CLOCK_WINDOW_HPP
#define CLOCK_WINDOW_HPP

#include <glibmm/dispatcher.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <format>
#include <iostream>

#include "TimeModel.hpp"
namespace Constants{
  namespace MainWindow{
    constexpr size_t width = 400;
    constexpr size_t height = 250;
  }
  namespace ButtonsBox{
    constexpr size_t margin_t = 20;
  }
  namespace MainBox{
    constexpr size_t margin_vert = 40;
    constexpr size_t margin_hor = 20;
  }
}

namespace GUI {

class ClockWindow : public Gtk::Window {
public:
  ClockWindow();
  ~ClockWindow() override = default;

private:
  // --- UI Setup ---
  void SetupUI();
  void ConnectSignals();

  // --- Signal Handlers ---
  // Called by Glib::Dispatcher on the MAIN thread to update UI
  void OnTick();

  // Button handlers
  void OnAddHour();
  void OnAddMinute();
  void OnAddSecond();

  // Helper to read model and update labels
  void UpdateLabel();

  // --- Background Worker ---
  // Runs in a separate thread. Ticks the clock every second.
  void WorkerThread(std::stop_token token);

  // --- Widgets ---
  Gtk::Box main_box_{Gtk::Orientation::VERTICAL, 10};

  // Display labels
  Gtk::Label time_label_;
  Gtk::Label format_label_;

  // Controls
  Gtk::Box button_box_{Gtk::Orientation::HORIZONTAL, 5};
  Gtk::Button btn_add_hour_{"Add Hour"};
  Gtk::Button btn_add_minute_{"Add Minute"};
  Gtk::Button btn_add_second_{"Add Second"};

  // --- Data & Concurrency ---
  Core::TimeModel time_model_;

  // Mutex protects access to time_model_ because it is shared
  // between the GUI thread (buttons/drawing) and WorkerThread.
  mutable std::mutex model_mutex_;

  // Dispatcher allows the worker thread to trigger a signal
  // that runs on the main GUI thread.
  Glib::Dispatcher dispatcher_;

  // C++20 thread that supports cancellation (stop_token) and auto-join.
  std::jthread worker_thread_;
};

} // namespace GUI

#endif // CLOCK_WINDOW_HPP
