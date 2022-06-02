

#ifndef GTKMM_MAINGUI_H
#define GTKMM_MAINGUI_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/fixed.h>
#include <gtkmm/window.h>
#include <gtkmm/stack.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/grid.h>
#include <gtkmm/textview.h>
#include <gtkmm/textbuffer.h>
#include "fileOperations.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <gtkmm/dialog.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>

class MainGui : public Gtk::Window
{

public:
  MainGui();
  virtual ~MainGui();

protected:
  //Signal handlers:
  void on_button_clicked(Gtk::Button*, int);
  void on_textview_hiding(Gtk::TextView* txtview);
  void on_textview_show(Gtk::TextView* txtview);

  void SetupMainGui();
  void SetupButton(Gtk::Button*, int);
  void SetupTextview(std::string name, std::string title, std::string message);
  void SetupFileChangeGui();
  void NewFileAccept();
  void RenameFileAccept();
  void RemoveFileAccept();
  std::string CheckFileName(std::string);
  void (MainGui::*acceptHandler)();
  void SetTextViewText(Gtk::TextView*, std::string);
  Glib::ustring GetTextViewText(Gtk::TextView* );

  

  //Member widgets:
  Gtk::Fixed m_fixed;
  Gtk::Grid m_grid;
  Gtk::Fixed m_fixed_modify_file;

  Gtk::Grid m_grid_buttons;
  Gtk::Button m_button_new;
  Gtk::Button m_button_rename;
  Gtk::Button m_button_remove;
  Gtk::Stack m_stack;
  Gtk::StackSidebar m_stack_sidebar;
  Gtk::ScrolledWindow m_scrolled_window;

  Gtk::Label m_label_file_operation_description;
  Gtk::TextView m_textview_filechange;
  Gtk::Button m_button_accept;
  Gtk::Button m_button_cancel;
  
  fileOperations fops;

  std::map<Gtk::TextView*, std::string> m_textboxes;
  std::string current_text, previous_text;

  enum buttonOperation { file_initiate_new, file_initiate_rename, file_initiate_remove, accept, cancel };
  
};

#endif
