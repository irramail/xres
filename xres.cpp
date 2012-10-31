//
// xres.cpp
//
#include <iostream>
#include <fstream>
#include <vector>
#include "xlib++/display.hpp"
#include "xlib++/window.hpp"
#include "xlib++/graphics_context.hpp"
#include "xlib++/command_button.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace xlib;
class main_window;
static unsigned int level_Y = 0;
static std::string res;

const std::string FNCONFIG = ".rm";
const std::string FNXSESSIONRC = ".xsessionrc";

bool fexists() {
  bool f;
  std::ifstream ifile;
  ifile.open("/etc/X11/xorg.conf");
  f = ifile.good();
  ifile.close();

  return f;
}

void rmxorg() {
  std::ofstream ofsxorg;
  ofsxorg.open("/tmp/rm_xorg.sh");
  ofsxorg << "#!/bin/sh" << std::endl;
  ofsxorg << "xterm -e 'sudo mv -f /etc/X11/xorg.conf /etc/X11/xorg.conf.old; sudo reboot'"<< std::endl;
  ofsxorg.close();
  pid_t child;

  if (!(child = fork())) {
    execlp("/bin/sh", "/bin/sh", "/tmp/rm_xorg.sh", NULL);
    exit(0);
  }
  //wait(0);
}

void saveConfig() {

  if (res == "")
    res = "1368x768";

  std::string tmp1, tmp2, str1, str2, str3;
  bool f1, f2, f3;
  std::ofstream ofrm, ofxsessionrc;
  std::ifstream ifrm, ifxsessionrc;
  std::vector<std::string> irm, ixsessionrc;

  ifrm.open(FNCONFIG.c_str());
  while(ifrm.good()) {
    std::getline(ifrm, tmp1);
    if (tmp1.length() && (tmp1.find("RES:") == std::string::npos))
      irm.push_back(tmp1);
  }
  ifrm.close();

  ofrm.open(FNCONFIG.c_str());
  
  for (unsigned int i = 0; i < irm.size(); i++) {
    ofrm << irm[i] << std::endl;
  }
  ofrm << "RES:" << res << std::endl;
  ofrm.close();
  
  if (res == "1368x768") {
    str1 = "DISPLAY=:0.0 xrandr --newmode \"1368x768_60.00\"  85.86  1368 1440 1584 1800  768 769 772 795  -HSync +Vsync";
    str2 = "DISPLAY=:0.0 xrandr --addmode VGA1 1368x768_60.00";
    str3 = "DISPLAY=:0.0 xrandr --output VGA1 --mode 1368x768_60.00";
  }

  if (res == "800x600") {
    str1 = "DISPLAY=:0.0 xrandr --newmode \"800x600_60.00\"  38.22  800 832 912 1024  600 601 604 622 -HSync +Vsync";
    str2 = "DISPLAY=:0.0 xrandr --addmode VGA1 800x600_60.00";
    str3 = "DISPLAY=:0.0 xrandr --output VGA1 --mode 800x600_60.00";
  }

  if (res == "1024x768") {
    str1 = "DISPLAY=:0.0 xrandr --newmode \"1024x768_60.00\"  64.11  1024 1080 1184 1344  768 769 772 795  -HSync +Vsync";
    str2 = "DISPLAY=:0.0 xrandr --addmode VGA1 1024x768_60.00";
    str3 = "DISPLAY=:0.0 xrandr --output VGA1 --mode 1024x768_60.00";
  }

  if (res == "1280x1024") {
    str1 = "DISPLAY=:0.0 xrandr --newmode \"1280x1024_60.00\"  108.88  1280 1360 1496 1712  1024 1025 1028 1060  -HSync +Vsync";
    str2 = "DISPLAY=:0.0 xrandr --addmode VGA1 1280x1024_60.00";
    str3 = "DISPLAY=:0.0 xrandr --output VGA1 --mode 1280x1024_60.00";
  }

  if (res == "1600x1200") {
    str1 = "DISPLAY=:0.0 xrandr --newmode \"1600x1200_60.00\"  160.96  1600 1704 1880 2160  1200 1201 1204 1242  -HSync +Vsync";
    str2 = "DISPLAY=:0.0 xrandr --addmode VGA1 1600x1200_60.00";
    str3 = "DISPLAY=:0.0 xrandr --output VGA1 --mode 1600x1200_60.00";
  }
  
  ifxsessionrc.open(FNXSESSIONRC.c_str());
  while(ifxsessionrc.good()) {
    std::getline(ifxsessionrc, tmp2);
    
    if (tmp2.find("DISPLAY=:0.0 xrandr --newmode") != std::string::npos) {
      tmp2 = str1;
      f1 = true;
    }
    if (tmp2.find("DISPLAY=:0.0 xrandr --addmode") != std::string::npos) {
      tmp2 = str2;
      f2 = true;
    }
    if (tmp2.find("DISPLAY=:0.0 xrandr --output") != std::string::npos) {
      tmp2 = str3;
      f3 = true;
    }
  
    if (tmp2.length())
      ixsessionrc.push_back(tmp2);
  }
  ifxsessionrc.close();

  if (!(f1 && f2 && f3)) {
    ixsessionrc.insert(ixsessionrc.begin() + 1, str3); 
    ixsessionrc.insert(ixsessionrc.begin() + 1, str2); 
    ixsessionrc.insert(ixsessionrc.begin() + 1, str1); 
  }

  ofxsessionrc.open(FNXSESSIONRC.c_str());  
  for (unsigned int i = 0; i < ixsessionrc.size(); i++) {
    ofxsessionrc << ixsessionrc[i] << std::endl;
  }
  ofxsessionrc.close();
}

class hello_rd_button : public command_button
{
public:
  hello_rd_button ( main_window& w );
  ~hello_rd_button(){}

  void on_click();

private:
  main_window& m_parent;
};

class main_window : public window
{
 public:
  main_window ( event_dispatcher& e )
    : window ( e )
  {
    m_rd_res1 = new hello_rd_button ( *this );
    m_rd_res2 = new hello_rd_button ( *this );
    m_rd_res3 = new hello_rd_button ( *this );
    m_rd_res4 = new hello_rd_button ( *this );
    m_rd_res5 = new hello_rd_button ( *this );
    m_rd_res6 = new hello_rd_button ( *this );

    m_rd_SAVE = new hello_rd_button ( *this );
    m_rd_EXIT = new hello_rd_button ( *this );

    m_rd_res1->set_name("800x600");
    m_rd_res2->set_name("1024x768");
    m_rd_res3->set_name("1368x768");
    m_rd_res4->set_name("1280x1024");
    m_rd_res5->set_name("1600x1200");
    m_rd_res6->set_name("DEL xorg.conf");
    m_rd_SAVE->set_name("Save");
    m_rd_EXIT->set_name("Exit");

    if (!fexists())
      m_rd_res6->hide();

  }
  ~main_window(){ delete m_rd_res1, m_rd_res2, m_rd_res3, m_rd_res4, m_rd_res5, m_rd_res6, m_rd_SAVE, m_rd_EXIT; }

  void on_hello_click() { }
  void set_name(std::string name) { }

private:

  hello_rd_button *m_rd_res1, *m_rd_res2, *m_rd_res3, *m_rd_res4, *m_rd_res5, *m_rd_res6, *m_rd_SAVE, *m_rd_EXIT;
};


//
// Hello button
//

hello_rd_button::hello_rd_button ( main_window& w )
  : command_button ( w, rectangle(point(20, level_Y*35 + 20),125,30 ), "" ),
    m_parent ( w )
{ set_id(level_Y++); }
void hello_rd_button::on_click() { if (get_id() == 7) exit(0); if (get_id() == 6) saveConfig(); if (get_id() < 5) res = get_name(); if (get_id() == 5) rmxorg(); }

main()
{
      display d("");

      event_dispatcher events ( d );
      main_window w ( events ); // top-level
      events.run();
}

