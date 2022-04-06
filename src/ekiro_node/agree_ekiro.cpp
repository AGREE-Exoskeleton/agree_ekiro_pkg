#include "agree_ekiro.h"
#include <stdlib.h>

/*******************/
/* ROS Main Thread */
/*******************/

void agree_ekiro_class::ROS_main_thread(){

  interim_ekiro_state = -1;
  interim_ekiro_command = -1;
  while (ros::ok()){

    cout << "Command: " << interim_ekiro_command << endl;
    cout << "Status:  " << interim_ekiro_state   << endl;

    if(interim_ekiro_command==0) ros::shutdown();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
  ros::Rate loop_rate(100);

  // Node Publisher
  ros::Publisher agree_ekiro_publisher = n.advertise<std_msgs::Int64>("agree_ekiro_command",1000);

  while (ros::ok()){

    msg.data = interim_ekiro_command;
    agree_ekiro_publisher.publish(msg);
    loop_rate.sleep();
  }

}

void agree_ekiro_class::ROS_keyboard_thread(){

  //Initialize char
  char c;

  while (ros::ok()){

    // Get character
    c = cin.get();

    if(c != '\n'){
      switch(c){

      case '0': case '1': case '2': case '3':
        interim_ekiro_command = static_cast<int64_t>(atoi(&c));
        ROS_WARN_STREAM("Changed status to: " << interim_ekiro_command);
        break;
      case ' ':
        cout << "Menu" << endl;
        break;
      default:
        std::cout << red_key << "Unrecognized key input!" << color_key <<  std::endl;
        break;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
    else
    {
      std::cout << interim_ekiro_command << " is the state currently active" << std::endl;
    } // else

  } // while

}


void agree_ekiro_class::ROS_subscribe_callback(const std_msgs::Int64 msg)
{
  //Display data.
  ROS_INFO(" Status:[%ld]",msg.data);
  interim_ekiro_state = msg.data;
}
