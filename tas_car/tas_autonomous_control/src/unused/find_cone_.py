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
    
    scan_data = np.array(scan.ranges)
    
    switch_dist = 1.1
    step = 2
    threshold = 1.1
    scan_size=len(scan_data)

    diff_scan = scan_data[step:len(scan_data)]-scan_data[0:len(scan_data)-step];
    first_spike = []
    for i in xrange(0,len(diff_scan)):
        if(diff_scan[i] < -threshold):
            first_spike.append(i)
    
    spike_pairs = []
    
    #print(first_spike)
    #print(diff_scan[first_spike[0]])
    
    for i in xrange(0,len(first_spike)):
        x = scan_data[first_spike[i]+step]
	
        max_cone_size = int(math.floor(-20*x**3+80*x**2-x*120+90))
	#max_cone_size = int(math.floor(-25*scan_data[first_spike[i]+step]+65))
	#print(max_cone_size)
        for j in xrange(first_spike[i],min(first_spike[i]+max_cone_size,len(diff_scan))):
            if(diff_scan[j] > threshold):
                spike_pairs.append((first_spike[i], j, diff_scan[first_spike[i]]-diff_scan[j]))
    #print spike_pairs
    sorted_spikes = sorted(spike_pairs, key=itemgetter(2))
    #print(sorted_spikes)
    my_spike_pos = -1;
    spike_pos = 0
    if len(sorted_spikes) != 0:
        spike_pos = (sorted_spikes[0][0]+sorted_spikes[0][1])/2
    print spike_pos
    my_spike_pos = -1;
    if spike_pos > 4*144:
        my_spike_pos = 4
        print '4th'
    elif spike_pos > 3*144:
        my_spike_pos = 3
        print '3th'
    elif spike_pos > 2*144:
        my_spike_pos = 2
        print '2th'
    elif spike_pos > 1*144:
        my_spike_pos = 1
        print '1th'
    elif spike_pos > 1:
        my_spike_pose = 0
        print '0th'
    else:
        print 'no_cone_found'
    vc2 = 1
    min_val = scan_data[my_spike_pos]
    print min_val
    if my_spike_pos == 2:
        vel_cmd = vc2;
    elif (my_spike_pos == 3 or my_spike_pos == 4) and min_val <= switch_dist:
        vel_cmd = -1
    elif (my_spike_pos == 3 or my_spike_pos == 4) and min_val > switch_dist:
        vel_cmd = 1
        vc2 = 1
    elif (my_spike_pos == 0 or my_spike_pos == 1) and min_val <= switch_dist:
        vel_cmd = -1
    elif (my_spike_pos == 0 or my_spike_pos == 1) and min_val > switch_dist:
        vel_cmd = 0
        vc2 = 0
    else:
        vel_cmd = -4
    
    pub = rospy.Publisher('cone_position', Int32, queue_size=1000)
    sector = Int32()
    sector.data = vel_cmd
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