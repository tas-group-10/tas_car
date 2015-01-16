#!/usr/bin/env python
# license removed for brevity
import rospy
import numpy as np
from std_msgs.msg import Int32
from operator import itemgetter
from sensor_msgs.msg import LaserScan

dspeed = 0

def callback(scan):
    checkscans = np.array(scan.ranges)
    checkscans = checkscans[250:470]
    min_element = min(checkscans)
    mean_val = -1
    if min_element < 0.5:
        npscan = np.array(checkscans)
        l = len(npscan[npscan==min_element])
        ids = np.argsort(npscan)
        mean_val = round(np.mean(ids[0:l]))

    if mean_val != -1:
        if mean_val > 125:
            aspeed = 1000
        else:
            aspeed = 2000  
    else:
        aspeed = -1
    pub = rospy.Publisher('angspeed', Int32, queue_size=1000)
    rate = rospy.Rate(10) # 10hz
    angspeed = Int32()
    angspeed.data = aspeed
    pub.publish(angspeed)

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
