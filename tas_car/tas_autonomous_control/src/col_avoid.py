#!/usr/bin/env python
# license removed for brevity
import rospy
import numpy as np
from std_msgs.msg import Int32
from operator import itemgetter
from sensor_msgs.msg import LaserScan

dspeed = 0

def callback(scan):
    global dspeed
    alpha = 30
    n=10

    nVal = len(scan.ranges)
    v_min = (90-alpha)/0.25
    v_max = (90+alpha)/0.25
    tmp_arr = np.array(scan.ranges[int(v_min):int(v_max)])
    idx_arr = tmp_arr.argsort()[:n]
    med = np.median(tmp_arr[idx_arr])
    
    dspeed=1540+med*20
    if dspeed > 1600:
        dspeed = 1600
    if med < 0.3:
        dspeed=1500
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
