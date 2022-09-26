#!/usr/bin/python

import os
import time
import sys

confFile = "codes/quicksort-FullDuplication/quicksort-fulldup.conf"
detectLog = "/tmp/quick-fulldup-detection.log"

timestampFile = "summary-carolfi.log"
timestampMaxDiff=60*5 # in seconds

def killall():
    os.system("killall -9 fault_injector.py")
    os.system("killall -9 gdb")

def run():
    killall()
    os.system("./fault_injector.py -c "+confFile+" -i 9000 -d "+detectLog+"&")

try:
    print "running ..."
    run()
    time.sleep(timestampMaxDiff)
    while True:
        timestamp = int(os.path.getmtime(timestampFile))
        now = int(time.time())
        timestampDiff = now - timestamp
        if timestampDiff > timestampMaxDiff:
            print "timestamp > than expected"
            run()
        else:
            print "timestamp OK"
        
        time.sleep(timestampMaxDiff)

except KeyboardInterrupt:  # Ctrl+c
    print "\n\tKeyboardInterrupt detected, exiting gracefully!( at least trying :) )"
    killall()
    sys.exit(1)
