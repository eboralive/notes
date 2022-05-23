

#include "mainGui.h"




MainGui::MainGui()
{

  SetupMainGui();
  SetupFileChangeGui();

}

MainGui::~MainGui()
{
}


void MainGui::SetupMainGui()
{
  this->set_default_size(800,600);
  this->add(m_fixed);
  this->set_title("notes");
  
  m_stack_sidebar.set_stack(m_stack0);
  m_stack_sidebar.set_visible();
  m_stack_sidebar.set_vexpand(true);
  m_stack_sidebar.set_size_request(150,150); // minimum size quested width, height

  
  std::map<std::string, std::string> dirList = fops.GetFileList();
   
  for (std::map<std::string, std::string>::iterator itr = dirList.begin(); itr != dirList.end(); ++itr) {
        SetupTextview(itr->first, itr->first, itr->second);
  }

  m_stack0.set_hexpand(true);

  SetupButton(&m_button_new , (int) buttonOperation::file_initiate_new);
  SetupButton(&m_button_rename , (int) buttonOperation::file_initiate_rename);
  SetupButton(&m_button_remove , (int) buttonOperation::file_initiate_remove);
  m_grid_buttons.attach(m_button_new, 0, 0);
  m_grid_buttons.attach(m_button_rename, 1, 0);
  m_grid_buttons.attach(m_button_remove, 2, 0);

  m_fixed.put(m_grid,0,0);

  m_fixed.set_size_request(800,600);
  m_grid.set_size_request(800,600);
 
  m_grid.attach(m_stack_sidebar, 0, 0);
  m_grid.attach(m_stack0,1,0);
  m_grid.attach( m_grid_buttons, 1,1);  

  m_stack0.show();
  m_fixed.show();
  m_grid_buttons.show();
  m_grid.show();  
}


void MainGui::SetupFileChangeGui()
{

  m_fixed_modify_file.set_visible(false);
  m_textview_filechange.set_visible(true);
  m_textview_filechange.set_size_request(100,20);

  SetupButton(&m_button_accept , (int) buttonOperation::accept);
  SetupButton(&m_button_cancel , (int) buttonOperation::cancel);

  m_label_file_operation_description.set_visible(true);

  m_fixed_modify_file.put(m_label_file_operation_description, 100,100);
  m_fixed_modify_file.put(m_textview_filechange, 250, 100);
  m_fixed_modify_file.put(m_button_accept, 100, 200);
  m_fixed_modify_file.put(m_button_cancel, 100, 300);

  m_fixed.put(m_fixed_modify_file, 0,0);

}




void MainGui::SetupButton(Gtk::Button* abutton, int button_op) 
{
  abutton->set_border_width(10);
  abutton->set_visible(true);
  
  switch(button_op) {
    case buttonOperation::file_initiate_new :
      abutton->set_label("new");
      break; 
    case buttonOperation::file_initiate_rename :
      abutton->set_label("rename");
      break; 
    case buttonOperation::file_initiate_remove :
      abutton->set_label("remove");
      break; 
    case buttonOperation::accept :
      abutton->set_label("accept");
      break; 
    case buttonOperation::cancel :
      abutton->set_label("cancel");
      break; 
   default : 
      abutton->set_label(std::to_string(button_op));
  }
  
  abutton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MainGui::on_button_clicked), abutton, button_op));

}


void MainGui::SetTextViewText(Gtk::TextView* txtview, std::string msg) 
{
  Glib::RefPtr<Gtk::TextBuffer> buffer = txtview->get_buffer();
  buffer->set_text(msg); 
  
}

Glib::ustring MainGui::GetTextViewText(Gtk::TextView* txtview)
{
  Glib::ustring da_tex = txtview->get_buffer()->get_text();
  return da_tex;
}


void MainGui::SetupTextview(std::string name, std::string title, std::string msg) {

  Gtk::TextView* txtview = new  Gtk::TextView();

  txtview->set_editable(true);
  txtview->set_accepts_tab(true);
  txtview->set_visible(true);
  txtview->set_cursor_visible(true);
  txtview->set_sensitive(true);
  txtview->set_can_focus(true);
  
  SetTextViewText(txtview, msg);
  txtview->show();

  txtview->signal_map().connect(sigc::bind(sigc::mem_fun(*this, &MainGui::on_textview_show), txtview));
  
  txtview->signal_unmap().connect(sigc::bind(sigc::mem_fun(*this, &MainGui::on_textview_hiding), txtview));

/*
  txtview->signal_key_press_event().connect([&](GdkEventKey* event)->bool{
            std::cout<<"pressed: "<<std::hex<<event->keyval<<" "<<std::hex<<event->state<<std::endl; 
            return false; //to propagate the event further
        }, false); //run before other handlers (from txtview)
*/

  m_stack0.add(*txtview, name, title); 

  m_textboxes[txtview] = name;

}





void MainGui::on_textview_show(Gtk::TextView* txtview) 
{
  previous_text = current_text;
  current_text = GetTextViewText( txtview);

}



void MainGui::on_textview_hiding(Gtk::TextView* txtview) 
{
  std::string present_text = GetTextViewText(txtview);
  if (present_text != previous_text) 
  {
    fops.SaveFile(m_textboxes[txtview], present_text);
  }
}



void MainGui::on_button_clicked(Gtk::Button* abutton, int button_op)
{
  m_textview_filechange.set_visible(button_op != buttonOperation::file_initiate_remove);

  switch(button_op) {
    case  buttonOperation::file_initiate_new :
    
      m_label_file_operation_description.set_text("new note name: ");
      acceptHandler = &MainGui::NewFileAccept; //  set function pointer to New file handler

      m_grid.set_visible(false);
      m_fixed_modify_file.set_visible(true);

      break; 
    case buttonOperation::file_initiate_rename :

      m_label_file_operation_description.set_text("rename note to: ");
      acceptHandler = &MainGui::RenameFileAccept; // set function pointer to rename file handler

      m_grid.set_visible(false);
      m_fixed_modify_file.set_visible(true);

      break; 
    case buttonOperation::file_initiate_remove :

      m_label_file_operation_description.set_text("are you sure you want to remove this note?");
      acceptHandler = &MainGui::RemoveFileAccept; // set function pointer to rename file handler

      m_grid.set_visible(false);
      m_fixed_modify_file.set_visible(true);

      break; 
    case buttonOperation::accept :

      (this->*acceptHandler)(); // call function pointer that should point to the correct handler
    
      break; 
    case buttonOperation::cancel :

      m_fixed_modify_file.set_visible(false);
      m_grid.set_visible(true);
      
      break;
   default : 
      std::cout << "not valid button" << std::endl;
  }

}


void MainGui::NewFileAccept()
{

  std::string new_file_name;
  new_file_name = GetTextViewText(&m_textview_filechange);

  if (new_file_name.length() > 0) 
  {
    new_file_name = CheckFileName(new_file_name);
    fops.SaveFile(new_file_name, "");
    SetupTextview(new_file_name, new_file_name, "" );
  }

  SetTextViewText(&m_textview_filechange, "");

  m_fixed_modify_file.set_visible(false);
  m_grid.set_visible(true);

}

void MainGui::RenameFileAccept()
{

  Gtk::TextView* txtview;
  std::string new_file_name, current_file_name;

  new_file_name = GetTextViewText(&m_textview_filechange);

  txtview = (Gtk::TextView*) m_stack0.get_visible_child();
  current_file_name = m_textboxes[txtview];

  if (new_file_name.length() > 0) 
  {
    new_file_name = CheckFileName(new_file_name);
    fops.RenameFile(current_file_name, new_file_name);
   
    m_stack0.remove(*txtview);
    m_stack0.add(*txtview, new_file_name, new_file_name); 

    m_textboxes.erase(txtview);
    m_textboxes[txtview] = new_file_name;

  }

  SetTextViewText(&m_textview_filechange, "");

  m_fixed_modify_file.set_visible(false);
  m_grid.set_visible(true);

}


  std::string MainGui::CheckFileName(std::string input_str)
  {
// todo add check for valid file names
    return input_str;
  }


void MainGui::RemoveFileAccept()
{
  
  Gtk::TextView* txtview;
  std::string current_file_name;

  txtview = (Gtk::TextView*) m_stack0.get_visible_child();
  current_file_name = m_textboxes[txtview];

  fops.RemoveFile(current_file_name);
  m_stack0.remove(*txtview);
  m_textboxes.erase(txtview);

  m_fixed_modify_file.set_visible(false);
  m_grid.set_visible(true);

}