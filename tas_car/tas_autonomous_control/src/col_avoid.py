#!/usr/bin/env python
# license removed for brevity
import rospy
import numpy as np
from std_msgs.msg import Int32
from operator import itemgetter
from sensor_msgs.msg import LaserScan


#distance to avoid obstacles in the front
avoid_dist = 0.8

#estimated half of the car width (inflate it a bit)
car_width = 0.30
def callback(scan):
    #getting the index for the front collision check
    startidx = int(round(np.arctan(avoid_dist/car_width)*720/np.pi))
    
    #transform scans into np array
    checkscans = np.array(scan.ranges)

    
    idx_fi = range(0,startidx)          #scan indices for right side
    idx_se = range(startidx,360)        #scan indices for right front half
    idx_th = range(360,720-startidx)    #scan indices for left front half
    idx_fo = range(720-startidx,720)    #scan indices for left side

    #get all the scan values
    first = checkscans[idx_fi]
    second = checkscans[idx_se]
    third = checkscans[idx_th]
    fourth = checkscans[idx_fo]

    #get the penetration depth for possible obstacles
    d_fi = [first[i] - car_width*np.cos(idx_fi[i]/(720/np.pi)) for i in range(len(first))]
    d_se = [x-avoid_dist for x in second]
    d_th = [x-avoid_dist for x in third]
    d_fo = [fourth[i] - car_width*np.cos((720-idx_fo[i])/(720/np.pi)) for i in range(len(first))]

    #get the closes obstacle for each sector
    min_list = [min(d_fi), min(d_se), min(d_th), min(d_fo)]
    min_id = np.argmin(min_list)

    #depending on sector with closest obstacle steer right or left
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

    #publish steering command
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
