#!/usr/bin/env python
# license removed for brevity

"""
dynamical adjustment of the car speed
subscriber: /scan (type LaserScan
publisher: /dynspeed (type Int32) 
"""

import rospy
import numpy as np
from sensor_msgs.msg import LaserScan
from std_msgs.msg import Int32


dspeed = 0

def callback(scan):
    global dspeed

    # angle from the middle of the car to the left or right.
    # obstacles outside this are ignored
    alpha = 30
    n=10  # number of minimal values from laserscan
    #calculate the number of the lowest and highest index of the area defined by alpha
    nVal = len(scan.ranges)
    v_min = (90-alpha)/0.25
    v_max = (90+alpha)/0.25
    tmp_arr = np.array(scan.ranges[int(v_min):int(v_max)])
    #get the indexes of the n points with the lowest distance
    idx_arr = tmp_arr.argsort()[:n]
    #we calculate the median because it is independent of noise
    med = np.median(tmp_arr[idx_arr])
    
    dspeed=1535+med*20  #calculate the new speed value
    #set a maxium speed of 1600
    if dspeed > 1600:
        dspeed = 1600
    #if the distance is lower than 0.3 we have to stop
    if med < 0.3:
        dspeed=1500
    print med 
    pub = rospy.Publisher('dynspeed', Int32, queue_size=1000)
    rate = rospy.Rate(10) # 10hz
    speed = Int32()
    speed.data = dspeed
    pub.publish(speed)

def listener():
    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber('scan', LaserScan, callback)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
if __name__ == '__main__':
    listener()
