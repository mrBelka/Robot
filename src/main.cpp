#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

// argc - number of console arguments, 
// argv - two-dimentional array of char
int main(int argc, char **argv)
{
	// initialization of node
  	ros::init(argc, argv, "talker");

  	// handle of node
  	ros::NodeHandle n;

  	// initialization of publisher
  	// templates
  	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);

  	// frequency in hz
  	ros::Rate loop_rate(1);

  	int count = 0;

	while (ros::ok())
	{
		geometry_msgs::Twist msg;
		
		if(count > 10)
		{
			msg.linear.x = 5;
			msg.linear.y = 0;
			msg.linear.z = 0;

			msg.angular.x = 0;
			msg.angular.y = 0;
			msg.angular.z = 0;

			count = 0;
		}
		else
		{
			msg.linear.x = -1;
			msg.linear.y = 0;
			msg.linear.z = 0;

			msg.angular.x = 0;
			msg.angular.y = 0;
			msg.angular.z = 0;
		}

		count++;

		// logger, printf, std::string s; s.c_str(), s.data()
		//ROS_INFO("%s", msg.data.c_str());

		// 
		pub.publish(msg);

		// multi-threading
		ros::spinOnce();

		// 100 ms
		loop_rate.sleep();
	}

  	return 0;
}