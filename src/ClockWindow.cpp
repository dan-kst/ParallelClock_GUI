#include "ClockWindow.hpp"

namespace GUI {

ClockWindow::ClockWindow() {
  set_title("Lab 9: Parallel Clock");
  set_default_size(Constants::MainWindow::width, Constants::MainWindow::height);
  set_resizable(false);

  SetupUI();
  ConnectSignals();

  // Initial draw
  UpdateLabel();

  // Start the background thread.
  worker_thread_ = std::jthread([this](std::stop_token token) {
    WorkerThread(token);
  });
}

void ClockWindow::SetupUI() {
  set_child(main_box_);

  // 1. Time Label (24h) - Large and Bold
  time_label_.set_markup("<span font='48' weight='bold'>00:00:00</span>");
  time_label_.set_halign(Gtk::Align::CENTER);
  main_box_.append(time_label_);

  // 2. Format Label (12h) - Smaller, gray
  format_label_.set_markup("<span font='18' color='gray'>12:00:00 AM</span>");
  format_label_.set_halign(Gtk::Align::CENTER);
  main_box_.append(format_label_);

  // 3. Buttons
  button_box_.set_halign(Gtk::Align::CENTER);
  button_box_.set_margin_top(Constants::ButtonsBox::margin_t);

  button_box_.append(btn_add_hour_);
  button_box_.append(btn_add_minute_);
  button_box_.append(btn_add_second_);

  main_box_.append(button_box_);

  // Global margins
  main_box_.set_margin_top(Constants::MainBox::margin_vert);
  main_box_.set_margin_bottom(Constants::MainBox::margin_vert);
  main_box_.set_margin_start(Constants::MainBox::margin_hor);
  main_box_.set_margin_end(Constants::MainBox::margin_hor);
}

void ClockWindow::ConnectSignals() {
  // Connect the dispatcher to the UI update method
  dispatcher_.connect(sigc::mem_fun(*this, &ClockWindow::OnTick));

  // Connect buttons
  btn_add_hour_.signal_clicked().connect(
      sigc::mem_fun(*this, &ClockWindow::OnAddHour));
  btn_add_minute_.signal_clicked().connect(
      sigc::mem_fun(*this, &ClockWindow::OnAddMinute));
  btn_add_second_.signal_clicked().connect(
      sigc::mem_fun(*this, &ClockWindow::OnAddSecond));
}

void ClockWindow::OnTick() {
  // This method runs in the Main GUI Thread.
  // It is safe to touch GTK widgets here.
  UpdateLabel();
}

void ClockWindow::UpdateLabel() {
  // Lock mutex to read state safely
  std::lock_guard<std::mutex> lock(model_mutex_);

  // 24H Format
  std::string big_text = std::format("<span font='48' weight='bold'>{}</span>",
                                     time_model_.To24Hour());
  time_label_.set_markup(big_text);

  // 12H Format
  std::string small_text = std::format("<span font='18' color='gray'>{}</span>",
                                       time_model_.To12Hour());
  format_label_.set_markup(small_text);
}

void ClockWindow::OnAddHour() {
  {
    std::lock_guard<std::mutex> lock(model_mutex_);
    time_model_.IncreaseByHour();
  }
  UpdateLabel();
}

void ClockWindow::OnAddMinute() {
  {
    std::lock_guard<std::mutex> lock(model_mutex_);
    time_model_.IncreaseByMinute();
  }
  UpdateLabel();
}

void ClockWindow::OnAddSecond() {
  {
    std::lock_guard<std::mutex> lock(model_mutex_);
    time_model_.IncreaseBySecond();
  }
  UpdateLabel();
}

void ClockWindow::WorkerThread(std::stop_token token) {
  // This runs in a separate thread!
  while (!token.stop_requested()) {
    // Sleep for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (token.stop_requested()) {break;}

    // Modify Model (Protected)
    {
      std::lock_guard<std::mutex> lock(model_mutex_);
      time_model_.IncreaseBySecond();
    }

    // Notify GUI
    dispatcher_.emit();
  }
}

}
