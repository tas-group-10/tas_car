#include "control/control.h"
#include "std_msgs/Int32.h"

int speed;
int angspeed;

void rspeed_callback(const std_msgs::Int32::ConstPtr& msg){


    speed=msg->data;

}

void angspeed_callback(const std_msgs::Int32::ConstPtr& msg){


    angspeed=msg->data;

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "autonomous_control");
    control autonomous_control;

    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("dynspeed",1000,rspeed_callback);
    ros::Subscriber sub1=nh.subscribe("angspeed",1000,angspeed_callback);
    angspeed = -1;
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
                if (infoflag2==false)
                {
                    ROS_INFO("Automatic Control!");
                    infoflag2=true;
                }

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


                ROS_INFO("normal");

                if(angspeed != -1)
                {
                    autonomous_control.control_servo.y = angspeed;
                }
                else
                {
                    if(speed > 1550 && autonomous_control.cmd_steeringAngle > 1700)
                    {

                        autonomous_control.control_servo.y = 1600;
                        ROS_INFO("Grenze 1600");
                    }
                    else if(speed > 1550 && autonomous_control.cmd_steeringAngle < 1300)
                    {
                        autonomous_control.control_servo.y = 1400;
                        ROS_INFO("Grenze 1400");
                    }
                    else
                    {
                        autonomous_control.control_servo.y = autonomous_control.cmd_steeringAngle;
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
