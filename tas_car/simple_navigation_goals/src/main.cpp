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
    waypoint1.position.x = 11.0288;//22.0;
    //waypoint1.position.x = 1.0;
    waypoint1.position.y = 16.674;//10.75;
    //waypoint1.position.y = 1.0;
    waypoint1.position.z = 0.000;
    waypoint1.orientation.x = 0.0;
    waypoint1.orientation.y = 0.0;
    waypoint1.orientation.z = -0.7076;
    waypoint1.orientation.w = 0.70666;
    waypoints.push_back(waypoint1);
    //Before first curve
    geometry_msgs::Pose waypoint2;
    waypoint2.position.x = 9.634;
    waypoint2.position.y = 7.55;
    waypoint2.position.z = 0.000;
    waypoint2.orientation.x = 0.0;
    waypoint2.orientation.y = 0.0;
    waypoint2.orientation.z = -0.71987;
    waypoint2.orientation.w = 0.69416;
    waypoints.push_back(waypoint2);  
    //After first curve
    geometry_msgs::Pose waypoint3;
    waypoint3.position.x = 12.28;
    waypoint3.position.y = 7.120;
    waypoint3.position.z = 0.000;
    waypoint3.orientation.x = 0.0;
    waypoint3.orientation.y = 0.0;
    waypoint3.orientation.z = -0.01932;
    waypoint3.orientation.w = 1.0;
    waypoints.push_back(waypoint3);
    //Before the small door, Laith office
    geometry_msgs::Pose waypoint4;
    waypoint4.position.x = 22.95397;
    waypoint4.position.y = 6.28584;
    waypoint4.position.z = 0.000;
    waypoint4.orientation.x = 0.0;
    waypoint4.orientation.y = 0.0;
    waypoint4.orientation.z = -0.03;
    waypoint4.orientation.w = 0.98454;
    waypoints.push_back(waypoint4);
    //After small door
    geometry_msgs::Pose waypoint5;
    waypoint5.position.x = 23.59;
    waypoint5.position.y = 8.130;
    waypoint5.position.z = 0.000;
    waypoint5.orientation.x = 0.0;
    waypoint5.orientation.y = 0.0;
    waypoint5.orientation.z = 0.7278;
    waypoint5.orientation.w = 0.68579;
    waypoints.push_back(waypoint5);
    //Before Lab
    geometry_msgs::Pose waypoint6;
    waypoint6.position.x = 23.6083;
    waypoint6.position.y = 17.0;
    waypoint6.position.z = 0.000;
    waypoint6.orientation.x = 0.0;
    waypoint6.orientation.y = 0.0;
    waypoint6.orientation.z = 0.675944;
    waypoint6.orientation.w = 0.73695;
    waypoints.push_back(waypoint6);
    //in Lab
    geometry_msgs::Pose waypoint7;
    waypoint7.position.x = 22.04;
    waypoint7.position.y = 19.344;
    waypoint7.position.z = 0.000;
    waypoint7.orientation.x = 0.0;
    waypoint7.orientation.y = 0.0;
    waypoint7.orientation.z = 0.6802;
    waypoint7.orientation.w = 0.73299;
    waypoints.push_back(waypoint7);
    //Goal
    geometry_msgs::Pose waypoint8;
    waypoint8.position.x = 13.512;
    waypoint8.position.y = 19.72325;
    waypoint8.position.z = 0.000;
    waypoint8.orientation.x = 0.0;
    waypoint8.orientation.y = 0.0;
    waypoint8.orientation.z = -1;
    waypoint8.orientation.w = 0.00116;
    waypoints.push_back(waypoint8);

    geometry_msgs::Pose waypoint8;
    waypoint7.position.x = 13.512;
    waypoint7.position.y = 19.72325;
    waypoint7.position.z = 0.000;
    waypoint7.orientation.x = 0.0;
    waypoint7.orientation.y = 0.0;
    waypoint7.orientation.z = -1;
    waypoint7.orientation.w = 0.00116;
    waypoints.push_back(waypoint8);

    MoveBaseClient ac("move_base", true); // action client to spin a thread by default

    while (!ac.waitForServer(ros::Duration(5.0))) { // wait for the action server to come up
        ROS_INFO("Waiting for the move_base action server to come up");
    }
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map"; // set target pose frame of coordinates
    //goal.target_pose.header.frame_id = "base_link";

    int bias = 6; //Start Position2
    //for(int i = 0; i < (waypoints.size()); ++i) { // loop over all goal points, point by point
    for(int i = 0;; ++i) { // loop over all goal points, point by point
        goal.target_pose.header.stamp = ros::Time::now(); // set current time
        goal.target_pose.pose = waypoints.at((i+bias)%8);
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
