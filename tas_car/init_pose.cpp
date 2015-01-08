#include <iostream>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

//Declarations for wlan sensitivity
#include <stdio.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

const int PATH_MAX = 512;
const int Addr_LENGTH = 17;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "send_start");

    ros::NodeHandle n;
    ros::Publisher start = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",50);
    // Startpoint position.x = 11
    // Startpoint position.y = 19
    int pos = start_position();
    if(pos == 1)
    {
        geometry_msgs::PoseWithCovarianceStamped init;
        init.pose.pose.position.x = 11.0;
        init.pose.pose.position.y = 19.0;
        init.pose.pose.position.z = 0.0;
        init.pose.pose.orientation.x = 0.0;
        init.pose.pose.orientation.y = 0.0;
        init.pose.pose.orientation.z = -0.72;
        init.pose.pose.orientation.w = 0.69;
        init.pose.covariance[0] = 0.25;
        init.pose.covariance[7] = 0.25;
        init.pose.covariance[35] = 0.069;
    }
    else if(pos == 2)
    {
        geometry_msgs::PoseWithCovarianceStamped init;
        init.pose.pose.position.x = 11.0;
        init.pose.pose.position.y = 19.0;
        init.pose.pose.position.z = 0.0;
        init.pose.pose.orientation.x = 0.0;
        init.pose.pose.orientation.y = 0.0;
        init.pose.pose.orientation.z = -0.72;
        init.pose.pose.orientation.w = 0.69;
        init.pose.covariance[0] = 0.25;
        init.pose.covariance[7] = 0.25;
        init.pose.covariance[35] = 0.069;
    }
    else
    {
        cout << "iwconfig doesn't work or no start position found" << endl;
    }

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

int start_position()
{
    FILE *fp;
    int status;
    char path[PATH_MAX];
    int position = 0;

    fp = popen("iwconfig", "r");
    if (fp == NULL)
    {
        cout << "Handle error" << endl;
    }

    char *ptr;
    bool found_wlan = false;
    char macAddr[20];
    for(int i = 0; i<20; i++)
    {
        macAddr[i] = 0;
    }
    while (fgets(path, PATH_MAX, fp) != NULL)
    {
        //cout << path << endl;
        ptr = strstr(path,"wlan");
        if(ptr != NULL)
        {
            found_wlan = true;
        }
        if(found_wlan == true)
        {
            ptr = strstr(path,"Access Point");
            if(ptr != NULL)
            {
                strncpy(macAddr,ptr+strlen("Access Point")+1,Addr_LENGTH);
                cout << "MAC address: " << macAddr << endl;
                break;
            }
        }
    }
    if(found_wlan == false)
    {
        cout << "No Wlan found. Check your access!" << endl;
    }
    else
    {
        if(strcmp(macAddr,"54:A0:50:5B:4D:E8") == 0)
        {
            position = 1;
        }
        else if(strcmp(macAddr,"54:A0:50:5B:32:58") == 0)
        {
            position = 2;
        }
        else
        {
            position = 0;
        }
    }
    status = pclose(fp);
    if (status == -1)
    {
        /* Error reported by pclose() */
        cout << "Error closing file" << endl;
        return 0;
    }
    else
    {
        return position;
    }
}
