#define MINDISTGOAL 2.0

#include "control/control.h"
#include "std_msgs/Int32.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "math.h"
int speed;
int angspeed;
int cone_pos;
double goal[2];
double pose[2];

double cal_dist(double* goal, double* pose);

void rspeed_callback(const std_msgs::Int32::ConstPtr& msg){


    speed=msg->data;

}

void angspeed_callback(const std_msgs::Int32::ConstPtr& msg){


    angspeed=msg->data;

}

void cpspeed_callback(const std_msgs::Int32::ConstPtr& msg){


    cone_pos=msg->data;

}
void getgoal_callback(const move_base_msgs::MoveBaseActionGoal::ConstPtr& msg){


    goal[0]=double (msg->goal.target_pose.pose.position.x);
    goal[1]=double (msg->goal.target_pose.pose.position.y);


}
void pose_callback (const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){

    pose[0]=double (msg->pose.pose.position.x);
    pose[1]=double (msg->pose.pose.position.y);



}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "autonomous_control");
    control autonomous_control;

    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("dynspeed",1000,rspeed_callback);
    ros::Subscriber sub1=nh.subscribe("angspeed",1000,angspeed_callback);
    ros::Subscriber sub2=nh.subscribe("cone_position",1000,cpspeed_callback);
    ros::Subscriber sub3=nh.subscribe("amcl_pose",1000,pose_callback);
    ros::Subscriber sub4=nh.subscribe("move_base/goal",1000,getgoal_callback);

    angspeed = -1;
    cone_pos = -1;
    speed=1550;
    ros::Rate loop_rate(50);
    bool infoflag = false;
    bool infoflag2 = false;

    while(ros::ok())
    {
        if(autonomous_control.control_Mode.data==0)
        {
             if (infoflag == false){
             ROS_INFO("Manually Control!");
             infoflag=true;
             }
        }
        else
        {
            if(autonomous_control.control_Brake.data==1)
            {
                autonomous_control.control_servo.x=1500;
                autonomous_control.control_servo.y=1500;
            }
            else
            {
                if(1)
                {
                    autonomous_control.control_servo.x = 1550;
                    if(cone_pos == 0)
                        autonomous_control.control_servo.y = 1000;
                    else if(cone_pos == -1)
                        autonomous_control.control_servo.y = 1500;
                    else if(cone_pos == 1)
                        autonomous_control.control_servo.y = 2000;
                }
                else
                {
                    if (infoflag2==false)
                    {
                        ROS_INFO("Automatic Control!");
                        infoflag2=true;
                    }

                    if(autonomous_control.cmd_linearVelocity>0)
                    {
                        if (cal_dist(goal,pose) > MINDISTGOAL){
                            ROS_INFO("dynspeed");
                            autonomous_control.control_servo.x = speed;
                            }

                        else {

                            autonomous_control.control_servo.x = 1550;

                            if (speed == 1500){
                                autonomous_control.control_servo.x = 1500;
                                ROS_INFO("emergency brake");

                            }
                        }
                    }
                    else if(autonomous_control.cmd_linearVelocity<0)
                    {

                        autonomous_control.control_servo.x = 1450;
                    }
                    else
                    {
                        autonomous_control.control_servo.x = 1500;
                        ROS_INFO("fail");

                    }



                    if(angspeed != -1)
                    {
                        autonomous_control.control_servo.y = angspeed;
                    }
                    else
                    {
                        if (cal_dist(goal,pose) > MINDISTGOAL)
                        {
                            if(speed > 1550 && autonomous_control.cmd_steeringAngle > 1700)
                            {

                                autonomous_control.control_servo.y = 1700;
                                ROS_INFO("Grenze 1600");
                            }
                            else if(speed > 1550 && autonomous_control.cmd_steeringAngle < 1300)
                            {
                                autonomous_control.control_servo.y = 1300;
                                ROS_INFO("Grenze 1400");
                            }
                            else
                            {
                                autonomous_control.control_servo.y = autonomous_control.cmd_steeringAngle;
                            }
                        }
                        else
                        {
                            autonomous_control.control_servo.y = autonomous_control.cmd_steeringAngle;
                        }
                    }
                }
            }

            autonomous_control.control_servo_pub_.publish(autonomous_control.control_servo);

        }

        ros::spinOnce();
        loop_rate.sleep();

    }

    return 0;

}

double cal_dist(double* goal, double* pose){


return sqrt((goal[0]-pose[0])*(goal[0]-pose[0])+(goal[1]-pose[1])*(goal[1]-pose[1]));


}
