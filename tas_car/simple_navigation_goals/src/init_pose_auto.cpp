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

int find_init_pos(); //Prototype to find the position over Wifi

int main(int argc, char **argv)
{
    ros::init(argc, argv, "send_start_wlan");

    ros::NodeHandle n;
    ros::Publisher start = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose",50);
    //Publisher to main.cpp to set the goals appropriate
    ros::Publisher init_pub = n.advertise<std_msgs::Int32>("init_Position", 50);
    // Startpoint position.x = 11
    // Startpoint position.y = 19
    std_msgs::Int32 pos;
    pos.data = find_init_pos(); //Call function to find the initial Position
    init_pub.publish(pos);  //Publish 
    geometry_msgs::PoseWithCovarianceStamped init;
    // Set start Position 1
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
    // Set start Position 2
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
// Function to find the initial position over Wifi
int find_init_pos()
{
    FILE *fp;
    int value_return = 0;
    // execute the linux command "iwlist wlan1 scan" to get all access points refer it to the File Pointer
    fp = popen("iwlist wlan1 scan", "r");
    // if the file pointer is zero
    if (fp == NULL)
    {
        cout << "Handle error" << endl;
        return 0;
    }
    else
    {
        char *ptr;  // Pointer for the search string
        char path[512] = {0};   // path for every line
        char Addr[3][30] = {0}; // 2D array for three access points in the TAS_NET
        char id[20] = {0};  // array for the name (TAS_NET)
        char level[3][10] = {0};    // 2D array for three signal level
        unsigned int wlan_count = 0;    // counter for the three access points
        while(fgets(path, 512, fp) != NULL) // get every line
        {
            ptr = strstr(path,"Address: "); // search the line for Address
            if(ptr != NULL) // Address found
            {
                strncpy(Addr[wlan_count],ptr+strlen("Address: "),17);   // copy the address into the array at the current position
                fgets(path, 512, fp);   // get the next line
                ptr = strstr(path,"ESSID:");    // serach for line with the ID
                if(ptr != NULL) // ID found
                {
                    strncpy(id,ptr+strlen("ESSID:"),9);
                    if(strcmp(id,"\"TAS_NET\"") == 0)   // correct access Point found
                    {
                        while(fgets(path, 512, fp) != NULL) // get next line
                        {
                            ptr = strstr(path,"Signal level="); // search for the signal level
                            if(ptr != NULL) // signal level found
                            {
                                // copy the signal level into the array at the current position
                                strncpy(level[wlan_count],ptr+strlen("Signal level="),7);
                                wlan_count++;   // increase the counter after one access point was found
                                break;
                            }
                        }
                    }
                }
            }
            if(wlan_count == 3) // if three access points are found don't search any more
                break;
        }
        if(wlan_count != 3) // could not find three access points
        {
            cout << "Couldn' find all access Points!" << endl;
            return 0;
        }
        else
        {
            // array for the signal level
            int pos_level[3] = {0};
            for(int i=0; i<3; i++)
            {
                // Position 1 found with the corresponding level and sort it into correct position of pos_level
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
                // Position 2 found with the corresponding level and sort it into correct position of pos_level
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
                // Position 3 found with the corresponding level and sort it into correct position of pos_level
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
            // Check which Position
            // signal level of position 1 > than signal level of position 2
            // or signal level of position 3 (not near to the start position) >Position 2
            if((pos_level[0] > pos_level[1]) || (pos_level[2] > pos_level[1]))
            {
                cout << "Position 1" << endl;
                value_return = 1;   // retrun position 1
            }
            // signal level of position 2 > than signal level of position 1
            // or signal level of position 2 (not near to the start position) > Position 3
            else if((pos_level[1] > pos_level[0]) || (pos_level[1] > pos_level[2]))
            {
                cout << "Position 2" << endl;
                value_return = 2;   // retrun position 2
            }
            else
            {
                cout << "No Position" << endl;
                value_return = 0;   // No Position found
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
