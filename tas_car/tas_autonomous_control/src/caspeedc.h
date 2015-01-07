#ifndef DYNSPEED_H
#define DYNSPEED_H

#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Int32.h"


#define PI                     3.14159265

//Define the angle of the area in fornt of the car in that collisions are detected
//Angle in Degrees for further information see documentation
#define AVOIDANGLE 30

/*
* The speed of the car is controlled via a pwm-signal.
* To drive the car forward (positive speed) the PWM-signal has to be: 1500< speed <2000
* To drive the car backwards (negative speed) the PWM-singal has to be: 1000< speed <1500
* halt is for speed = 1500
*
* TO-DO:  find out if speed increases linear between 1500 - 2000
*/
#define MAXSPEED    1750
#define MEDSPEED    1650
#define SLOWSPEED   1550
#define DOORAVOID   1520
#define SBREAK     1500

/*  Describtion of different zones for collision avoidance in front of the car
 *  The values of the laserscan in the given angle (avoidangle) are searched for a value smaller than the distance for every zone.
 *  If this is ture the appropriate speed is set.
 * ________________________________________________________________________________________    ________________
 * |    DSMAX      |        max. speed --> no obstacle in view                             |   \              /
 * |_______________|_______________________________________________________________________|    \____________/
 * |    DSMAX      |        medium speed --> obstacle coming closer                        |     \          /
 * |_______________|_______________________________________________________________________|      \________/
 * |    DSSLOW     |        slow speed(default value) --> need to drive slow for avoidance |       \      /
 * |_______________|_______________________________________________________________________|        \____/
 * |    DSBREAK    |        break --> speed = 0    :  obstacle in dangerous range          |         \  /
 * |_______________|_______________________________________________________________________|          \/Position of Laserscanner
*/

#define DSMAX   8
#define DSMED   4
#define DSSLOW  1.2
#define DSBREAK 0.2


/*
 * To avoid collisions with the wall especially by going through the different doors.
 * Therefore an the collision avoidance is manipulationg the steering angle to higher the distance to the wall.
 *
 *
 *
*/

//collision avoidance for the wall


typedef enum {MAX,MED,SLOW,AVOID,BREAK} STATES;



#endif // DYNSPEED_H
