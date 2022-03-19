
/**
 *  @file laser_odom.cpp
 *  @author Arghya Chatterjee <arghyame20buet@gmail.com>
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "laser_odom_from_tf");
   
  ros::NodeHandle nh;
  
  ros::Publisher odom_publisher_ = nh.advertise<nav_msgs::Odometry>("odom", 200);
   
  tf::TransformListener tf_listener;

  //ros::Time current_time, last_time;
  //current_time = ros::Time::now();
  //last_time = ros::Time::now();

  ros::Rate rate(200.0);
  while (nh.ok()){
    
    //current_time = ros::Time::now();

    tf::StampedTransform st_transform;
    geometry_msgs::Twist gr_twist;
    
    // duration between consecutive transforms

    // double dt = (current_time - last_time).toSec();
    // ros::Duration(dt);
    
    // look up twist method calling

    // void Transformer::lookupTwist	(	const std::string & 	tracking_frame,
    // const std::string & 	observation_frame,
    // const ros::Time & 	time,
    // const ros::Duration & 	averaging_interval,
    // geometry_msgs::Twist & 	twist	 
    // )			const

     
    try{
      tf_listener.lookupTransform("/odom", "/base_link", ros::Time(0), st_transform);
      tf_listener.lookupTwist("/odom", "/base_link", ros::Time(0), ros::Duration(0.005), gr_twist);
    }
    catch (tf::TransformException &exp) {
      ROS_ERROR("%s",exp.what());
      ros::Duration(1.0).sleep();
      continue;
    }
  
    nav_msgs::Odometry odom;
    //odom.header.stamp = current_time;
    odom.header.stamp = ros::Time::now();
    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";

    // pose information
    odom.pose.pose.position.x = st_transform.getOrigin().x();
    odom.pose.pose.position.y = st_transform.getOrigin().y();
    odom.pose.pose.position.z = st_transform.getOrigin().z();
    odom.pose.pose.orientation.x = st_transform.getRotation().x();
    odom.pose.pose.orientation.y = st_transform.getRotation().y();
    odom.pose.pose.orientation.z = st_transform.getRotation().z();
    odom.pose.pose.orientation.w = st_transform.getRotation().w();
    odom.pose.covariance[0] = 0.001;
    odom.pose.covariance[7] = 0.001;
    odom.pose.covariance[14] = 0.001;
    odom.pose.covariance[21] = 0.001;
    odom.pose.covariance[28] = 0.001;
    odom.pose.covariance[35] = 0.03;

    // compute velocity in a typical way given the position of the robot
    
    // double dt = (current_time - last_time).toSec();

 
    // double (vx * cos(th) - vy * sin(th)) = delta_x / dt;
    // double (vx * sin(th) + vy * cos(th)) = delta_y / dt;
    // double vth = delta_th / dt;

    // velocity information
    
    odom.twist.twist.linear.x = gr_twist.linear.x;
    odom.twist.twist.linear.y = gr_twist.linear.y;
    odom.twist.twist.linear.z = gr_twist.linear.z;
    odom.twist.twist.angular.x = gr_twist.angular.x;
    odom.twist.twist.angular.y = gr_twist.angular.y;
    odom.twist.twist.angular.z = gr_twist.angular.z;

    odom.twist.covariance = odom.pose.covariance;

    odom_publisher_.publish(odom); 

    //last_time = current_time;
   
    rate.sleep();
  }
  return 0;
};