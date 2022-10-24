#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

void chatterCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  ROS_INFO("I heard: [%lf]", msg->linear.x);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  // create subscriber, dependency injection
  ros::Subscriber sub = n.subscribe("turtle1/cmd_vel", 1000, chatterCallback);

  ros::spin();

  while(ros::ok())
  {
    ros::spinOnce();
  }

  return 0;
}