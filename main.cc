// File: main.cc

#include "mainGui.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "");

  MainGui mainGui;

  //Shows the window and returns when it is closed.
  return app->run(mainGui);
}

