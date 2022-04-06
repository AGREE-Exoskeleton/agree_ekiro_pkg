#include "ros/ros.h"
#include "agree_ekiro.h"

int main(int argc, char **argv)
{

  //Initialize ROS
  ros::init(argc, argv, "agree_ekiro_node");

  // Create node handler
  ros::NodeHandle n;

  //Create Class Object
  agree_ekiro_class agree_ekiro;

  //Start Spinner - Use 4 threads
  ros::MultiThreadedSpinner spinner(0);

  // Create subscriber
  ros::Subscriber agree_ekiro_subscriber = n.subscribe("agree_ekiro_status", 1000, &agree_ekiro_class::ROS_subscribe_callback, &agree_ekiro);

  spinner.spin();

  return 0;
}
