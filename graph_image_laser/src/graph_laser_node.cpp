﻿#include <thread>

#include <ros/ros.h>

#include "graph_image_laser/graph_laser.hpp"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "LaserMapper");
  ros::NodeHandle node_handle("~");

  graphImageLaser mapper(node_handle);

  //mapping线程
  std::thread publish_map_thread(&graphImageLaser::publishMapThread, &mapper);
  //发布变换线程
  std::thread publish_tf_thread(&graphImageLaser::publishTfThread, &mapper);
  //分割闭环线程
  std::thread segmatch_thread(&graphImageLaser::segMatchThread, &mapper);

  try {
    ros::spin();
  }
  catch (const std::exception& e)
  {
    ROS_ERROR_STREAM("Exception: " << e.what());
    return 1;
  }
  catch (...)
  {
    ROS_ERROR_STREAM("Unknown Exception");
    return 1;
  }

  publish_map_thread.join();
  publish_tf_thread.join();
  segmatch_thread.join();

  return 0;
}
