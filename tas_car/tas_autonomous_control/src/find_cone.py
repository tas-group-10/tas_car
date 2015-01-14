#!/usr/bin/env python
# license removed for brevity
import rospy
import math
import numpy as np
from std_msgs.msg import Int32
from sensor_msgs.msg import LaserScan
from operator import itemgetter, attrgetter

isright = False

def callback(scan):
    
    scan_data = len(scan.ranges)
    
    step = 1
    threshold = 0.3
    scan_size=len(scan_data)

    diff_scan = scan_data[step:len(scan_data)]-scan[0:len(scan_data)-step];
    first_spike = []
    for i in xrange(0,len(diff_scan)):
        if(diff_scan[i] < -threshold):
            first_spike.append(i)
    
    spike_pairs = []
    
    print(first_spike)
    print(diff_scan[first_spike[0]])
    
    for i in xrange(0,len(first_spike)):
        point = scan_data[first_spike[i]+step]
        max_cone_size = int(math.floor(-20*x**3+80*x**2-x*120+90))
        for j in xrange(first_spike[i],min(first_spike[i]+max_cone_size,len(diff_scan))):
            if(diff_scan[j] > threshold):
                spike_pairs.append((first_spike[i], j, diff_scan[first_spike[i]]-diff_scan[j]))
    print spike_pairs
    sorted_spikes = sorted(spike_pairs, key=itemgetter(2))
    print(sorted_spikes)
    
    spike_pos = 0
    if len(sorted_spikes) != 0:
        spike_pos = (sorted_spikes[0][0]+sorted_spikes[0][1])/2
    
    if spike_pos > 4*124:
        print '5th'
    elif spike_pos > 3*124:
        print '4th'
    elif spike_pos > 2*124:
        print '3th'
    elif spike_pos > 1*124:
        print '2th'
    elif spike_pos > 1:
        print '1th'
    else:
        print 'no_cone_found'
    
    pub = rospy.Publisher('cone_position', Int32, queue_size=1000)
    sector = Int32()
    sector.data = spike_pos
    pub.publish(sector)

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
