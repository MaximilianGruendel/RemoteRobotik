#include <ros/ros.h>
#include <angle_detector/AngleDetector.h>
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "angle_client");
  if (argc != 2)
  {
    ROS_INFO("usage: angle_client X");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<angle_detector::AngleDetector>("angle_detector");
  angle_detector::AngleDetector srv;
  srv.request.inputMotor = atoll(argv[1]);
  if (client.call(srv))
  {
    ROS_INFO("Angles: %s", srv.response.outResponse.c_str());
  }
  else
  {
    ROS_ERROR("Failed to call service angle_detector");
    return 1;
  }

  return 0;
}
