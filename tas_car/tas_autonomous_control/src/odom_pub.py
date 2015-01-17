#!/usr/bin/env python
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
import rospy
nav_msgs.Odometry odom
geometry_msgs.Quaternion odom_quat
geometry_msgs.Twist vel_twst

def velCallback(vel):
	global odom
	ROS_INFO("Entered velCallback")
	printf("Entered velCallback")
	odom.twist.twist.linear.x = vel.linear.x
	odom.twist.twist.linear.y = vel.linear.y
	odom.twist.twist.linear.z = vel.linear.z
	odom.twist.twist.angular.x = vel.angular.x
	odom.twist.twist.angular.y = vel.angular.y
	odom.twist.twist.angular.z = vel.angular.z
def poseCallback(my_odom):
	global odom
	ROS_INFO("Entered poseCallback")
	printf("Entered poseCallback")
	odom.pose.pose.position.x = my_odom.pose.pose.position.x
	odom.pose.pose.position.y = my_odom.pose.pose.position.y
	odom.pose.pose.position.z = my_odom.pose.pose.position.z
	odom_quat = tf.createQuaternionMsgFromYaw(odom.twist.twist.angular.z)
	odom.pose.pose.orientation = odom_quat
	odom.pose.covariance[0]  = 0.1
	odom.pose.covariance[7]  = 0.1
	odom.pose.covariance[35] = 0.1
	odom.pose.covariance[14] = DBL_MAX
	odom.pose.covariance[21] = DBL_MAX
	odom.pose.covariance[28] = DBL_MAX
if __name__ == '__main__':
	rospy.init_node('odom_publisher', anonymous=True)
	vel_sub = rospy.Subscriber("/velocity", 1000, velCallback)
	pose_sub = rospy.Subscriber("/slam_out_pose", 1000, poseCallback)
	odom_pub = rospy.Publisher('/odom_new', nav_msgs.Odometry, queue_size=10)
	#ros.Time current_time, last_time
	#current_time = ros.Time.now()
	#last_time = ros.Time.now()
	r = rospy.loop_rate(10)
	while ros.ok():
		#printf("Position: %f, %f, %f", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z)
		#current_time = ros::Time::now()
		odom_pub.publish(odom)
		ros.spinOnce()
		#last_time = current_time
		r.sleep()
	}
	return 0
