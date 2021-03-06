/**
 * This node sends fixed goals to move base via ROS Action API and receives feedback via callback functions.
 */

#include <iostream>
#include <tf/transform_listener.h>
#include <math.h>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <actionlib/client/simple_action_client.h>

#define PI 3.14159265

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
    //ROS_INFO("[X]:%f [Y]:%f [W]: %f [Z]: %f", feedback->base_position.pose.position.x,feedback->base_position.pose.position.y,feedback->base_position.pose.orientation.w, feedback->base_position.pose.orientation.z);
}

/**
 * Main function
 */
int main(int argc, char** argv){
    ros::init(argc, argv, "slalom"); // init and set name
    tf::TransformListener listener;

    std::vector<geometry_msgs::Pose> waypoints; // vector of goals, with position and orientation
    geometry_msgs::Pose waypoint;
    double b = 2.2;
    double l = 1.5;
    double initx = 23.5;
    double inity = 18.85;

    for (size_t i = 0; i < 20; i++){

        waypoint.position.x = initx + b/4.0*sin((double)i*3.0/8.0*PI/l);
        waypoint.position.y = inity - 0.5 - (double)i*3.0/8.0;
        waypoint.position.z = 0;
        waypoint.orientation = tf::createQuaternionMsgFromYaw(b/4.0*3.0/8.0*PI/l*cos((double)i*3.0/8.0*PI/l)-PI/2.0);
        //listener.transformPose("map",waypoint,waypoint_new);
        printf("Sending goal: x: %f, y: %f, z: %f\n", waypoint.position.x, waypoint.position.y, waypoint.position.z);
        printf("Sending goal: x: %f, y: %f, z: %f, w: %f\n", waypoint.orientation.x, waypoint.orientation.y, waypoint.orientation.z, waypoint.orientation.w);
        waypoints.push_back(waypoint);
    }
    for (size_t i = 0; i < 20; i++){
        printf("Sending goal %d: x: %f, y: %f, z: %f\n",(int)i, waypoints[i].position.x, waypoints[i].position.y, waypoints[i].position.z);
        printf("Sending Or: x: %f, y: %f, z: %f, w: %f\n", waypoints[i].orientation.x, waypoints[i].orientation.y, waypoints[i].orientation.z, waypoints[i].orientation.w);
    }


    MoveBaseClient ac("move_base", true); // action client to spin a thread by default

    while (!ac.waitForServer(ros::Duration(5.0))) { // wait for the action server to come up
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map"; // set target pose frame of coordinates
    //goal.target_pose.header.frame_id = "base_link";

    for(int i = 0; i < waypoints.size(); ++i) { // loop over all goal points, point by point
        goal.target_pose.header.stamp = ros::Time::now(); // set current time
        goal.target_pose.pose = waypoints.at(i);
        ROS_INFO("Sending goal %d: x: %f",(int)i, goal.target_pose.pose.position.x);
        ROS_INFO("Sending goal %d: y: %f",(int)i, goal.target_pose.pose.position.y);
        ROS_INFO("Sending goal %d: z: %f",(int)i, goal.target_pose.pose.position.z);
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
