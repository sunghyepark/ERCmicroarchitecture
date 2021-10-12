#

import os
import signal
import sys
import shlex
import subprocess as sp
import argparse as ap
import multiprocessing as mp
from datetime import datetime
from time import time as timer, sleep

# define paths
dirpos = "../example"
binaryName = "./ERCdecoder"
outpos = "../output"
logpos = "../log"

#
curTime = datetime.now().strftime('%m_%d_%H_%M')
benchlist_dir = []
benchlist = []
benchDir = sorted(os.listdir(dirpos))

# result include tuple (baseline result, our result) with template ()
benchresult = []


### function define ###
def ExecuteCommand( curBench, result, time_limit):
    inputPath = "%s/%s" % (dirpos, curBench)
    outputPath = "%s/%s" % (outpos, curBench)
    logPath = "%s/%s.%s.log" % (logpos, curBench, curTime)
    exeStr = "%s %s %s | tee %s" % (binaryName, inputPath, outputPath, logPath)
    print( exeStr )
    run = RunCommand(exeStr, time_limit)
    if(run!=None):
        result.append( [(curBench[:-5])] )
    else:
        print('time out')
    
def RunCommand(exeStr, time_limit):
    try:
        proc = sp.Popen(exeStr, shell=True)
        return proc.communicate(timeout = time_limit)
    except sp.TimeoutExpired:
        print('Time out!')
        pid = proc.pid
        proc.terminate()
        proc.kill()
        return None


def getdirList( ):
    for curBench in benchDir:
        if curBench[-5:] == ".qasm":
            benchlist_dir.append(curBench)


########################


getdirList()
if len(sys.argv) <= 1 or sys.argv[1] == "help":
    print("usage: python run.py <arg>")
    print("")
    print("list of <arg>")
    print("     help        : print how to use run.py")
    print("     list        : print file number of bench directory")
    print("     all         : execute all bench file in directory")
    print("     #           : execute one bench file(or more) in directory")
    print("                   to look file number do list")
    print("     <benchname> : execute one bench file of benchname")
    sys.exit(1)
elif sys.argv[1] == "list":
    for index, curBench in enumerate(benchlist_dir):
        print( str(index) + " : " + curBench )
    sys.exit(1)
elif sys.argv[1].isdigit() and len(sys.argv) == 2:
    benchNum = int(sys.argv[1])
    try:
        benchlist_dir[benchNum]
    except:
        print("ERROR: NO EXISTING BENCH FILE")
        print("Please do python run.py list")
        sys.exit(1)
    ExecuteCommand( benchlist_dir[benchNum], benchresult, 50)
        
elif sys.argv[1] == "all" or len(sys.argv) >= 3:
    if len(sys.argv) >= 3:
        for argnum in range(1, len(sys.argv)):
            num = int(sys.argv[argnum])
            try:
                benchlist_dir[num]
            except:
                print("ERROR: NO EXISTING BENCH FILE")
                print("Please do python run.py list")
                sys.exit(1)
            benchlist.append(benchlist_dir[num])
    elif sys.argv[1] == "all":
        benchlist = benchlist_dir
        #print("print all!")

    if __name__ == '__main__':
        procs = []
        manager = mp.Manager()
        results = manager.list()
        for index, curBench in enumerate(benchlist):
            proc = mp.Process(target=ExecuteCommand,
                    args=(curBench, results, 50))
            procs.append(proc)
            proc.start()
        for proc in procs:
            proc.join()
    benchresult = results[:]

elif sys.argv[1] == "initial" or len(sys.argv) >= 3:
    if len(sys.argv) >= 3:
        for argnum in range(1, len(sys.argv)):
            num = int(sys.argv[argnum])
            try:
                benchlist_dir[num]
            except:
                print("ERROR: NO EXISTING BENCH FILE")
                print("Please do python run.py list")
                sys.exit(1)
            benchlist.append(benchlist_dir[num])
    elif sys.argv[1] == "all":
        benchlist = benchlist_dir
        #print("print all!")

    if __name__ == '__main__':
        procs = []
        manager = mp.Manager()
        results = manager.list()
        for index, curBench in enumerate(benchlist_initial):
            proc = mp.Process(target=ExecuteCommand,
                    args=(curBench, results, 50))
            procs.append(proc)
            proc.start()
        for proc in procs:
            proc.join()
    benchresult = results[:]

else:
    print("ERROR: NO ACCEPTABLE ARGUMENT")
    sys.exit(1)

benchresult = sorted(benchresult)
print(" ")
print(" ")
print("===== Result =====")
print(" bench_name")
print("==================")
for benchResult in benchresult:
    s1 = '%-14s' % benchResult[0][0]

print("-------------")    
