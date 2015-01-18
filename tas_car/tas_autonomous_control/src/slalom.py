#!/usr/bin/env python
# license removed for brevity
import rospy
import math
import numpy as np
from std_msgs.msg import Int32
from sensor_msgs.msg import LaserScan
from operator import itemgetter, attrgetter

isright = 0
old_dist = 0
dist = 0
def callback(scan):
    #indicate if passing cone on the right or left
    global isright
    global old_dist
    global dist
    #transform scan data to np array
    scan_data = np.array(scan.ranges)
    
    #define which neighbors to compare for spikes in scan (every third)
    step = 3
    #define threshold of scan difference to detect cones
    threshold = 1.1
    scan_size=len(scan_data)

    #get distance from scan to its third neighbor
    diff_scan = scan_data[step:len(scan_data)]-scan_data[0:len(scan_data)-step];
    first_spike = []
    
    #find peak for right corner of cone
    for i in xrange(0,len(diff_scan)):
        if(diff_scan[i] < -threshold):
            first_spike.append(i)
    
    spike_pairs = []
    
    print(first_spike)
    
    #find second peak for left corner
    for i in xrange(0,len(first_spike)):
        x = scan_data[first_spike[i]+step]
	
        max_cone_size = int(math.floor(-20*x**3+80*x**2-x*120+90))
        for j in xrange(first_spike[i],min(first_spike[i]+max_cone_size,len(diff_scan))):
            if(diff_scan[j] > threshold):
                print (j,scan_data[j], diff_scan[j])
                spike_pairs.append((first_spike[i], j, diff_scan[first_spike[i]]-diff_scan[j]))
    
    #sort possible cones by biggest difference between first and second spike
    sorted_spikes = sorted(spike_pairs, key=itemgetter(2))
    my_spike_pos = -1;
    spike_pos = 0
    
    #depending on array indices allocate cone to sector
    if len(sorted_spikes) != 0:
        spike_pos = (sorted_spikes[0][0]+sorted_spikes[0][1])/2
        print spike_pos
        if spike_pos > 4*144:
            my_spike_pos = 4
            dist = scan_data[my_spike_pos]
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
        dist = scan_data[spike_pos]
    else:
        print 'no_cone_found'
    #if old cone scan value is very different from current, a new cone is found
    if(abs(dist-old_dist) > 0.5):
        #switch passing side for the next cone
        isright = not isright
        print 'switch'
    print ('isright', isright)
    
    #depending on sector and passing side, send -1,0 or 1 (straight, left, right)
    min_val = scan_data[my_spike_pos]
    print min_val
    if (my_spike_pos == 0 or my_spike_pos == 1 or my_spike_pos == 2 or my_spike_pos == 3) and isright:
        vel_cmd = 1
        print 'left'
    elif (my_spike_pos == 0 or my_spike_pos == 1 or my_spike_pos == 2 or my_spike_pos == 3) and not isright:
        vel_cmd = 0
        print 'right'
    else:
        vel_cmd = -1;
        print 'straight'

    #publish
    pub = rospy.Publisher('cone_position', Int32, queue_size=1000)
    sector = Int32()
    sector.data = vel_cmd
    pub.publish(sector)
    old_dist = dist
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
