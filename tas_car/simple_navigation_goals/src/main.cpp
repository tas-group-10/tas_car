/**
 * This node sends fixed goals to move base via ROS Action API and receives feedback via callback functions.
 */

#include <iostream>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <actionlib/client/simple_action_client.h>
#include "std_msgs/Int16.h"
using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

/**
 * Callback function
 */
void doneCb(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result) {
    ROS_INFO("Finished in state [%s]", state.toString().c_str());
}

/**
 * Callback function, called once when the goal becomes active
 */
void activeCb() {
    ROS_INFO("Goal just went active");
}

/**
 * Callback function, called every time feedback is received for the goal
 */
void feedbackCb(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback) {
    ROS_INFO("[X]:%f [Y]:%f [W]: %f [Z]: %f", feedback->base_position.pose.position.x,feedback->base_position.pose.position.y,feedback->base_position.pose.orientation.w, feedback->base_position.pose.orientation.z);
}


/**
 * Main function
 */
int main(int argc, char** argv){
    ros::init(argc, argv, "simple_navigation_goals"); // init and set name
    std::vector<geometry_msgs::Pose> waypoints; // vector of goals, with position and orientation

    //Set first Goalpoint
    geometry_msgs::Pose waypoint1;
    waypoint1.position.x = 10.27;//22.0;
    //waypoint1.position.x = 1.0;
    waypoint1.position.y = 7.05;//10.75;
    //waypoint1.position.y = 1.0;
    waypoint1.position.z = 0.000;
    waypoint1.orientation.x = 0.0;
    waypoint1.orientation.y = 0.0;
    waypoint1.orientation.z = -0.73;
    waypoint1.orientation.w = 0.676;
    waypoints.push_back(waypoint1);
    //After first curve
    geometry_msgs::Pose waypoint2;
    waypoint2.position.x = 11.67;
    waypoint2.position.y = 6.32;
    waypoint2.position.z = 0.000;
    waypoint2.orientation.x = 0.0;
    waypoint2.orientation.y = 0.0;
    waypoint2.orientation.z = -0.03;
    waypoint2.orientation.w = 1.0;
    waypoints.push_back(waypoint2);  
    //Before the small door, Laith office
    geometry_msgs::Pose waypoint3;
    waypoint3.position.x = 22.72;
    waypoint3.position.y = 6.02;
    waypoint3.position.z = 0.000;
    waypoint3.orientation.x = 0.0;
    waypoint3.orientation.y = 0.0;
    waypoint3.orientation.z = -0.03;
    waypoint3.orientation.w = 1.0;
    waypoints.push_back(waypoint3);
    //After small door
    geometry_msgs::Pose waypoint4;
    waypoint4.position.x = 23.48;
    waypoint4.position.y = 7.35;
    waypoint4.position.z = 0.000;
    waypoint4.orientation.x = 0.0;
    waypoint4.orientation.y = 0.0;
    waypoint4.orientation.z = 0.69;
    waypoint4.orientation.w = 0.72;
    waypoints.push_back(waypoint4);
    //Before Lab
    geometry_msgs::Pose waypoint5;
    waypoint5.position.x = 23.7;
    waypoint5.position.y = 18.44;
    waypoint5.position.z = 0.000;
    waypoint5.orientation.x = 0.0;
    waypoint5.orientation.y = 0.0;
    waypoint5.orientation.z = 0.68;
    waypoint5.orientation.w = 0.73;
    waypoints.push_back(waypoint5);
    //in Lab
    geometry_msgs::Pose waypoint6;
    waypoint6.position.x = 22.6;
    waypoint6.position.y = 19.032;
    waypoint6.position.z = 0.000;
    waypoint6.orientation.x = 0.0;
    waypoint6.orientation.y = 0.0;
    waypoint6.orientation.z = 1.0;
    waypoint6.orientation.w = 0.02;
    waypoints.push_back(waypoint6);
    //Goal
    geometry_msgs::Pose waypoint7;
    waypoint7.position.x = 12.3;
    waypoint7.position.y = 19.6;
    waypoint7.position.z = 0.000;
    waypoint7.orientation.x = 0.0;
    waypoint7.orientation.y = 0.0;
    waypoint7.orientation.z = 1.0;
    waypoint7.orientation.w = 0.02;
    waypoints.push_back(waypoint7);

    MoveBaseClient ac("move_base", true); // action client to spin a thread by default

    while (!ac.waitForServer(ros::Duration(5.0))) { // wait for the action server to come up
        ROS_INFO("Waiting for the move_base action server to come up");
    }
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map"; // set target pose frame of coordinates
    //goal.target_pose.header.frame_id = "base_link";

    for(int i = 0; i < (waypoints.size()); ++i) { // loop over all goal points, point by point
        goal.target_pose.header.stamp = ros::Time::now(); // set current time
        goal.target_pose.pose = waypoints.at(i);
        ROS_INFO("Sending goal");
        ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb); // send goal and register callback handler
        ac.waitForResult(); // wait for goal result

        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("The base moved to %d goal", i);
        } else {
            ROS_INFO("The base failed to move to %d goal for some reason", i);
        }
    }
    return 0;
}
