#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <sstream>
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */

nav_msgs::Odometry odom;
geometry_msgs::Quaternion odom_quat;
geometry_msgs::TwistStamped vel_twst;


void velCallback(geometry_msgs::TwistStamped vel)
{
	ROS_INFO("Entered velCallback");
	printf("Entered velCallback");

	odom.twist.twist.linear.x = vel.twist.linear.x;
	odom.twist.twist.linear.y = vel.twist.linear.y;
	odom.twist.twist.linear.z = vel.twist.linear.z;
	odom.twist.twist.angular.x = vel.twist.angular.x;
	odom.twist.twist.angular.y = vel.twist.angular.y;
	odom.twist.twist.angular.z = vel.twist.angular.z;

}


void poseCallback(geometry_msgs::PoseStamped my_pose)
{	
	ROS_INFO("Entered poseCallback");
	printf("Entered poseCallback");

    odom.pose.pose.position.x = my_pose.pose.position.x;
    odom.pose.pose.position.y = my_pose.pose.position.y;
    odom.pose.pose.position.z = my_pose.pose.position.z;
    //odom.pose.pose.position.w = my_pose.pose.position.w;
    odom_quat = tf::createQuaternionMsgFromYaw(odom.twist.twist.angular.z);

    odom.pose.pose.orientation = odom_quat;
    
	odom.pose.covariance[0]  = 0.1;
	odom.pose.covariance[7]  = 0.1;
	odom.pose.covariance[35] = 0.1;
	odom.pose.covariance[14] = DBL_MAX;
	odom.pose.covariance[21] = DBL_MAX;
	odom.pose.covariance[28] = DBL_MAX;
}

int main(int argc, char **argv)
{
//    double x_old = odom.pose.pose.position.x;
//    double y_old = odom.pose.pose.position.y;
//    double z_old = odom.pose.pose.position.z;

//	ros::init(argc, argv, "odom_publisher");
//	ros::NodeHandle n;
//	ROS_INFO("Outside");
	
//    //ros::Subscriber vel_sub = n.subscribe("/velocity", 1000, velCallback);
//	ros::Subscriber pose_sub = n.subscribe("/slam_out_pose", 1000, poseCallback);
//	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/odom_new", 50);
	
//	ros::Time current_time, last_time;
//	current_time = ros::Time::now();
//	last_time = ros::Time::now();
//	ros::Rate loop_rate(10);
	
//	while (n.ok())
//    {
//        double roll, pitch, yaw;

//        current_time = ros::Time::now();
//        double t_diff = current_time - last_time;
//        odom.twist.twist.linear.x = (odom.pose.pose.position.x - x_old)/t_diff;
//        odom.twist.twist.linear.y = (odom.pose.pose.position.y - y_old)/t_diff;
//        odom.twist.twist.linear.z = (odom.pose.pose.position.z - z_old)/t_diff;

//        tf::Matrix3x3(odom_quat).getRPY(roll, pitch, yaw);

//        odom.twist.twist.angular.x = (roll - r_old)/t_diff;
//        odom.twist.twist.angular.y = (pitch - p_old)/t_diff;
//        odom.twist.twist.angular.z = (yaw - y_old)/t_diff;

//		//printf("Position: %f, %f, %f", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
//		odom_pub.publish(odom);
//		ros::spinOnce();

//        odom.pose.pose.position.x - x_old;
//        odom.pose.pose.position.y - y_old;
//        odom.pose.pose.position.z - z_old;
//        roll = r_old;
//        pitch = p_old;
//        yaw = y_old;




//		last_time = current_time;
//		loop_rate.sleep();
//	}
	
	return 0;
}
