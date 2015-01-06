#include "caspeedc.h"


STATES state;

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
    //Computation of the index Values of scan.ranges that need to be checked for ostacles
    int anzValues = int((round(fabs(scan->angle_min)+scan->angle_max)/scan->angle_increment));
    int indexmin = (anzValues/2) - (AVOIDANGLE/2)/((scan->angle_increment/PI)*180);
    int indexmax = (anzValues/2) + (AVOIDANGLE/2)/((scan->angle_increment/PI)*180);
    int i =0;
    bool checkdistance[4]={false};
/*
    for (i=indexmin-1;i<=(indexmax-1);i++){

        if (scan->ranges[i] < DSMAX){ checkdistance[0]=true;}
        if (scan->ranges[i] < DSMED){ checkdistance[1]=true;}
        if (scan->ranges[i] < DSSLOW){ checkdistance[2]=true;}
        if (scan->ranges[i] < DSBREAK){ checkdistance[3]=true;}

    }

    if (checkdistance[3]==true){
        state=BREAK;
        return;
    }
    if (checkdistance[2]==true){
        state=SLOW;
        return;
    }
    if (checkdistance[1]==true){
        state=MED;
        return;
    }
    if (checkdistance[0]==true){
        state=MAX;
        return;
    }
    */

}

int main(int argc, char **argv){

    ros::init(argc, argv, "caspeedc");
    ros::NodeHandle n;

    ros::Publisher dspeed_publisher = n.advertise<std_msgs::Int32>("dynspeed", 1000);
    ros::Subscriber sub = n.subscribe("scan", 1000, scanCallback);

    std_msgs::Int32 speed;
    ros::Rate loop_rate(10);

    while (ros::ok()){

        switch (state){

            case MAX:
                speed.data = MAXSPEED;
                dspeed_publisher.publish(speed);
                break;

            case MED:
                speed.data = MEDSPEED;
                dspeed_publisher.publish(speed);
                break;

            case SLOW:
                speed.data = SLOWSPEED;
                dspeed_publisher.publish(speed);
                break;

            case BREAK:
                speed.data = SBREAK;
                dspeed_publisher.publish(speed);
                break;
        }

       ros::spinOnce();
       loop_rate.sleep();
    }





    return 0;
}
