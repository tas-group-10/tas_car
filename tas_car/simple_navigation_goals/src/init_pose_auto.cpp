#include <iostream>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "std_msgs/Int32.h"

//Declarations for wlan sensitivity
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using std::cout;
using std::endl;

int find_init_pos();

int main(int argc, char **argv)
{
    ros::init(argc, argv, "send_start_wlan");

    ros::NodeHandle n;
    ros::Publisher start = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",50);
    ros::Publisher init_pub = n.advertise<std_msgs::Int32>("init_Position", 50);
    // Startpoint position.x = 11
    // Startpoint position.y = 19
    std_msgs::Int32 pos;
    pos.data = find_init_pos();
    init_pub.publish(pos);
    geometry_msgs::PoseWithCovarianceStamped init;
    if(pos.data == 1)
    {
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
    else if(pos.data == 2)
    {        
        init.pose.pose.position.x = 23.94;
        init.pose.pose.position.y = 19.0677;
        init.pose.pose.position.z = 0.0;
        init.pose.pose.orientation.x = 0.0;
        init.pose.pose.orientation.y = 0.0;
        init.pose.pose.orientation.z = 0.99974;
        init.pose.pose.orientation.w = 0.022479;
        init.pose.covariance[0] = 0.25;
        init.pose.covariance[7] = 0.25;
        init.pose.covariance[35] = 0.068538;
    }
    else
    {
        cout << "No start position found" << endl;
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

int find_init_pos()
{
    FILE *fp;
    int value_return = 0;
    fp = popen("iwlist wlan1 scan", "r");
    //fp = fopen("iwlist.txt", "r");
    if (fp == NULL)
    {
        cout << "Handle error" << endl;
        return 0;
    }
    else
    {
        char *ptr;
        char path[512] = {0};
        char Addr[3][30] = {0};
        char id[20] = {0};
        char level[3][10] = {0};
        unsigned int wlan_count = 0;
        while(fgets(path, 512, fp) != NULL)
        {
            //cout << path << endl;
            ptr = strstr(path,"Address: ");
            if(ptr != NULL)
            {
                strncpy(Addr[wlan_count],ptr+strlen("Address: "),17);
                //cout << Addr[0] << endl;
                fgets(path, 512, fp);
                ptr = strstr(path,"ESSID:");
                if(ptr != NULL)
                {
                    strncpy(id,ptr+strlen("ESSID:"),9);
                    //cout << "ID: " << id << endl;
                    if(strcmp(id,"\"TAS_NET\"") == 0)
                    {
                        while(fgets(path, 512, fp) != NULL)
                        {
                            ptr = strstr(path,"Signal level=");
                            if(ptr != NULL)
                            {
                                strncpy(level[wlan_count],ptr+strlen("Signal level="),7);
                                //cout << Addr[wlan_count] << endl;
                                //cout << "Signal level " << level[wlan_count] << endl;
                                wlan_count++;
                                break;
                            }
                        }
                    }
                }
            }
            if(wlan_count == 3)
                break;
        }
        if(wlan_count != 3)
        {
            cout << "Couldn' find all access Points!" << endl;
            return 0;
        }
        else
        {
            int pos_level[3] = {0};
            for(int i=0; i<3; i++)
            {
                cout << Addr[i] << "  " << level[i] << endl;
                if(strcmp(Addr[i],"54:A0:50:5B:4D:E8") == 0)
                {
                    ptr = strstr(level[i],"/");
                    char tmp[3] = {0};
                    strncpy(tmp,(ptr-2),2);
                    pos_level[0] = atoi(tmp);
                    if(pos_level[0] == 0)
                        pos_level[0] = 100;
                    cout << "Level Pos 1: " << pos_level[0] << endl;
                }
                else if(strcmp(Addr[i],"54:A0:50:5B:20:80") == 0)
                {
                    ptr = strstr(level[i],"/");
                    char tmp[3] = {0};
                    strncpy(tmp,(ptr-2),2);
                    pos_level[1] = atoi(tmp);
                    if(pos_level[1] == 0)
                        pos_level[1] = 100;
                    cout << "Level Pos 2: " << pos_level[1] << endl;
                }
                else if(strcmp(Addr[i],"54:A0:50:5B:32:58") == 0)
                {
                    ptr = strstr(level[i],"/");
                    char tmp[3] = {0};
                    strncpy(tmp,(ptr-2),2);
                    pos_level[2] = atoi(tmp);
                    if(pos_level[2] == 0)
                        pos_level[2] = 100;
                    cout << "Level Pos 3: " << pos_level[2] << endl;
                }
            }
            if((pos_level[0] > pos_level[1]) || (pos_level[2] > pos_level[1]))
            {
                cout << "Position 1" << endl;
                value_return = 1;
            }
            else if((pos_level[1] > pos_level[0]) || (pos_level[1] > pos_level[2]))
            {
                cout << "Position 2" << endl;
                value_return = 2;
            }
            else
            {
                cout << "No Position" << endl;
                value_return = 0;
            }
        }
        int status = pclose(fp);
        if (status == -1)
        {
            /* Error reported by pclose() */
            cout << "Error closing file" << endl;
        }
    }
    return value_return;
}
