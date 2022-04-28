#include "agree_ekiro.h"
#include <stdlib.h>


#include <sys/ioctl.h>
#include <termios.h>

/** enum for ekiro control mode */
enum ekiro_state_t{
    disconnected        = 0,
    connected           = 1,
    active_0            = 2,
    active_25           = 3,
    active_50           = 4,
    active_75           = 5,
    active_100          = 6,
};

/** Labels for ekiro control mode */
static map< ekiro_state_t, const char * > ekiro_state_labels = {
    {disconnected,      "DISCONNECTED"},
    {connected,         "CONNECTED"},
    {active_0,          "ACTIVE_0"},
    {active_25,         "ACTIVE_25"},
    {active_50,         "ACTIVE_50"},
    {active_75,         "ACTIVE_75"},
    {active_100,        "ACTIVE_100"},
};

bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

/*******************/
/* ROS Main Thread */
/*******************/

void agree_ekiro_class::ROS_main_thread(){

  interim_ekiro_state = -1;
  interim_ekiro_command = -1;

  // Node Rate
  ros::Rate loop_rate(1);

  while (ros::ok()){

    ROS_WARN_STREAM("Main loop eKiro dummy");
    ROS_INFO_STREAM("Command: " << interim_ekiro_command);
    ROS_INFO_STREAM("Status:  " << interim_ekiro_state);
    loop_rate.sleep();

    if(interim_ekiro_command==ekiro_state_t::disconnected) ros::shutdown();
  }
}

/************************/
/* ROS Publisher Thread */
/************************/

void agree_ekiro_class::ROS_publish_thread(){

  //  Declare a message and setup the publisher for that message
  std_msgs::Int64 msg;
  //  agree_smartbox_pkg::custom_msg msg;

  // Node Handler
  ros::NodeHandle n;

  // Node Rate
  ros::Rate loop_rate(10);

  // Node Publisher
  ros::Publisher agree_ekiro_publisher = n.advertise<std_msgs::Int8>("map_module/command",1000);

  while (ros::ok()){

    interim_counter++;
    msg.data = interim_ekiro_command;
    agree_ekiro_publisher.publish(msg);
    loop_rate.sleep();
  }

}

void agree_ekiro_class::ROS_keyboard_thread(){

  //Initialize char
  char c;

  while (ros::ok()){

    if(kbhit()){
    // Get character
    c = cin.get();

      switch(c){

      case '0': case '1': case '2': case '3': case '4': case '5': case '6':
        interim_ekiro_command = static_cast<int64_t>(atoi(&c));
        ROS_WARN_STREAM("Changed status to: " << ekiro_state_labels[static_cast<ekiro_state_t>(interim_ekiro_command)]);
        break;
      case ' ':
        ROS_WARN_STREAM("Menu");
        break;
      default:
        ROS_ERROR_STREAM("Unrecognized key input!");
        break;
      }

      std::cout << interim_ekiro_command << " is the state currently active" << std::endl;
  }
  } // while

}


void agree_ekiro_class::ROS_subscribe_callback(const agree_map_pkg::msgmap msg)
{
  //Display data.
  ROS_WARN_STREAM("Callback eKiro status");
  ROS_INFO("Status     : [%d]",msg.state);
  ROS_INFO("Temperature: [%f]",(msg.temperature1+msg.temperature2)/2);
  ROS_INFO("Battery    : [%f]",msg.percentage);
  ROS_INFO_STREAM("Connected  : " << static_cast<bool>(msg.present));
  interim_ekiro_state = msg.state;
}
