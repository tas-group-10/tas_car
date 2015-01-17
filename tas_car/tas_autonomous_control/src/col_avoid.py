#!/usr/bin/env python
# license removed for brevity
import rospy
import numpy as np
from std_msgs.msg import Int32
from operator import itemgetter
from sensor_msgs.msg import LaserScan

dspeed = 0

avoid_dist = 0.8
car_width = 0.30
def callback(scan):
    startidx = int(round(np.arctan(avoid_dist/car_width)*720/np.pi))
    checkscans = np.array(scan.ranges)

    idx_fi = range(0,startidx)
    idx_se = range(startidx,360)
    idx_th = range(360,720-startidx)
    idx_fo = range(720-startidx,720)

    first = checkscans[idx_fi]
    second = checkscans[idx_se]
    third = checkscans[idx_th]
    fourth = checkscans[idx_fo]


    d_fi = [first[i] - car_width*np.cos(idx_fi[i]/(720/np.pi)) for i in range(len(first))]
    d_se = [x-avoid_dist for x in second]
    d_th = [x-avoid_dist for x in third]
    d_fo = [fourth[i] - car_width*np.cos((720-idx_fo[i])/(720/np.pi)) for i in range(len(first))]

    min_list = [min(d_fi), min(d_se), min(d_th), min(d_fo)]
    min_id = np.argmin(min_list)

    sector = -1
    if min_list[min_id] < 0:
        sector = min_id
    #print d_fi
    if sector != -1:
        if min_id == 0 or min_id == 1:
            aspeed = 2000
        else:
            aspeed = 1000  
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
