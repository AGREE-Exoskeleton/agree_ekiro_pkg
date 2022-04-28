#ifndef INTERFACE_H
#define INTERFACE_H

#include "ros/ros.h"
#include <iostream>
#include <thread>
#include <chrono>


//Include message types
#include "std_msgs/Int8.h"
#include "std_msgs/Int64.h"
#include "agree_map_pkg/msgmap.h"

//Include threads
#include <boost/thread/thread.hpp>

using namespace std;

// Text Color Identifiers
const string boldred_key = "\033[1;31m";
const string red_key = "\033[31m";
const string boldpurple_key = "\033[1;35m";
const string yellow_key = "\033[33m";
const string blue_key = "\033[36m";
const string green_key = "\033[32m";
const string color_key = "\033[0m";

class agree_ekiro_class
{
public:
  agree_ekiro_class()
  {
    boost_ROS_publish_thread    = boost::thread(&agree_ekiro_class::ROS_publish_thread, this);
    boost_ROS_keyboard_thread   = boost::thread(&agree_ekiro_class::ROS_keyboard_thread, this);
    boost_ROS_main_thread       = boost::thread(&agree_ekiro_class::ROS_main_thread, this);
    std::cout << "ROS interface objects instantiated" << std::endl;

  }

  ~agree_ekiro_class()
  {
    std::cout << "ROS interface threads joining" << std::endl;
    boost_ROS_publish_thread.join();
    boost_ROS_keyboard_thread.join();
    boost_ROS_main_thread.join();
  }

  int64_t interim_ekiro_state;
  int64_t interim_ekiro_command;

  int64_t interim_counter = 0;

  void ROS_subscribe_callback(const agree_map_pkg::msgmap msg);

private:

  boost::thread boost_ROS_publish_thread;
  boost::thread boost_ROS_keyboard_thread;
  boost::thread boost_ROS_main_thread;

  void ROS_publish_thread();
  void ROS_keyboard_thread();
  void ROS_main_thread();

};


#endif // INTERFACE_H
