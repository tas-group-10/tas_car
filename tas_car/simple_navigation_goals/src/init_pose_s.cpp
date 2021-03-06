#include <iostream>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "slalom_start");

    ros::NodeHandle n;
    ros::Publisher start = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",50);
    // Startpoint position.x = 11
    // Startpoint position.y = 19
    geometry_msgs::PoseWithCovarianceStamped init;
    init.pose.pose.position.x = 23.5;
    init.pose.pose.position.y = 19.5;
    init.pose.pose.position.z = 0.0;
    init.pose.pose.orientation.x = 0.0;
    init.pose.pose.orientation.y = 0.0;
    init.pose.pose.orientation.z = -0.72;
    init.pose.pose.orientation.w = 0.69;
    init.pose.covariance[0] = 0.25;
    init.pose.covariance[7] = 0.25;
    init.pose.covariance[35] = 0.069;

    ros::Rate poll_rate(100);
    while(start.getNumSubscribers() == 0)
    {
        poll_rate.sleep();
    }
    init.header.stamp = ros::Time::now(); // set current time
    start.publish(init);
    ros::spinOnce();
    /*ros::Rate loop_rate(1);

    while (ros::ok())
    {


        loop_rate.sleep();
    }*/

    return 0;
}
