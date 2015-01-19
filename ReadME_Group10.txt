Features we implemented:

-Dynamical speed
-Slalom
-Colllision avoidance
-initial position through Wifi
-Oscillation surpression

We built seperate launch files for the two starting positions and the slalom task.

group10_1.launch: Start our nodes with starting position 1
group10_2.launch: Start our nodes with starting position 2

To set up the car for the race run_system.launch is needed.

For the slalom task we did some changes in the tas_autonomous_control node and therefore
run_system_s.launch has to be launched.

Run autonomous drviving:
========================
1. roslaunch group10_1.launch (starpos. 1) or roslaunch group10_2.launch (startpos.2)
2. roslaunch run_system.launch


Run Slamĺom Course:
===================
1. roslaunch run_system_s.launch
2. rosrun tas_autonomous_control slalom.py


Dynamical speed node
=====================
If the front laserscanner sees no obstacles the car is accelerating.

rosrun tas_autonomous_control speed_adjust.py


Collision avoidance
==============================
Checks in which half of the laserscanner a obstacle is and steers in the opposite direction.

rosrun tas_autonomous_control col_avoid.py





