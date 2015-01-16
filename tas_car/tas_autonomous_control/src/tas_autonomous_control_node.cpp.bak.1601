#include "control/control.h"
#include "std_msgs/Int32.h"

int speed;

void rspeed_callback(const std_msgs::Int32::ConstPtr& msg){


    speed=msg->data;

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "autonomous_control");
    control autonomous_control;

    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("dynspeed",1000,rspeed_callback);

    speed=1550;
    ros::Rate loop_rate(50);

    while(ros::ok())
    {
        if(autonomous_control.control_Mode.data==0)
        {
            ROS_INFO("Manually Control!");
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
                ROS_INFO("Automatic Control!");
                if(autonomous_control.cmd_linearVelocity>0)
                {
                    autonomous_control.control_servo.x = speed;
                }
                else if(autonomous_control.cmd_linearVelocity<0)
                {
                    autonomous_control.control_servo.x = 1450;
                }
                else
                {
                    autonomous_control.control_servo.x = 1500;
                }

                autonomous_control.control_servo.y = autonomous_control.cmd_steeringAngle;
            }

            autonomous_control.control_servo_pub_.publish(autonomous_control.control_servo);

        }

        ros::spinOnce();
        loop_rate.sleep();

    }

    return 0;

}
