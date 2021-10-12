# This script is written by Minhyuk Kweon
# 2020.07.07
#
# mh.kweon@postech.ac.kr

import os
import signal
import sys
import shlex
import subprocess as sp
import argparse as ap
import multiprocessing as mp
from datetime import datetime
from time import time as timer, sleep

#1) Identity mapping
# TCAD main mapping algorithm
# no reverse traversal
RESULT_I =( ("4mod5-v1_22"      , 18        , 0     ),
            ("mod5mils_65"      , 36        , 0     ),
            ("alu-v0_27"        , 36        , 0     ),
            ("decod24-v2_43"    , 45        , 0     ),
            ("4gt13_92"         , 63        , 0     ),
            ("ising_model_10"   , 87        , 0     ),
            ("ising_model_13"   , 123       , 0     ),
            ("ising_model_16"   , 159       , 0     ),
            ("qft_10"           , 96        , 0     ),
            ("qft_13"           , 171       , 0     ),
            ("qft_16"           , 261       , 0     ),
            ("qft_20"           , 450       , 0.015 ),
            ("rd84_142"         , 231       , 0     ),
            ("adr4_197"         , 1575      , 0.016 ),
            ("radd_250"         , 1548      , 0.015 ),
            ("z4_268"           , 1482      , 0.015 ),
            ("sym6_145"         , 2289      , 0.015 ),
            ("misex1_241"       , 2124      , 0.031 ),
            ("rd73_252"         , 2622      , 0.016 ),
            ("cycle10_2_110"    , 2889      , 0.031 ),
            ("square_root_7"    , 2661      , 0.031 ),
            ("sqn_258"          , 5688      , 0.063 ),
            ("rd84_253"         , 5784      , 0.063 ),
            ("co14_215"         , 8427      , 0.094 ),
            ("sym9_193"         , 15438     , 0.156 ),
            ("urf5_158"         , 76413     , 0.702 ),
            ("hwb9_119"         , 96057     , 0.890 ),
            ("urf4_187"         , 228588    , 2.487 )
        )

#2) Identity mapping
# TCAD main mapping algorithm
# reverse traversal (7)
RESULT_IR =(("4mod5-v1_22"      , 15        , 0     ),
            ("mod5mils_65"      , 30        , 0     ),
            ("alu-v0_27"        , 30        , 0     ),
            ("decod24-v2_43"    , 42        , 0     ),
            ("4gt13_92"         , 54        , 0     ),
            ("ising_model_10"   , 78        , 0     ),
            ("ising_model_13"   , 39        , 0     ),
            ("ising_model_16"   , 72        , 0     ),
            ("qft_10"           , 69        , 0     ),
            ("qft_13"           , 129       , 0     ),
            ("qft_16"           , 249       , 0.015 ),
            ("qft_20"           , 396       , 0.031 ),
            ("rd84_142"         , 234       , 0.015 ),
            ("adr4_197"         , 1236      , 0.046 ),
            ("radd_250"         , 1494      , 0.046 ),
            ("z4_268"           , 1527      , 0.046 ),
            ("sym6_145"         , 2244      , 0.046 ),
            ("misex1_241"       , 2157      , 0.063 ),
            ("rd73_252"         , 1839      , 0.062 ),
            ("cycle10_2_110"    , 3042      , 0.092 ),
            ("square_root_7"    , 2304      , 0.094 ),
            ("sqn_258"          , 5751      , 0.141 ),
            ("rd84_253"         , 5595      , 0.187 ),
            ("co14_215"         , 6819      , 0.265 ),
            ("sym9_193"         , 14085     , 0.452 ),
            ("urf5_158"         , 75468     , 2.092 ),
            ("hwb9_119"         , 87537     , 2.562 ),
            ("urf4_187"         , 221481    , 7.820 )
        )


#3) TCAD initial mapping
# TCAD main mapping algorithm
# no reverse traversal
RESULT =(   ("4mod5-v1_22"      , 0         , 0     ),
            ("mod5mils_65"      , 0         , 0     ),
            ("alu-v0_27"        , 3         , 0     ),
            ("decod24-v2_43"    , 0         , 0     ),
            ("4gt13_92"         , 21        , 0     ),
            ("ising_model_10"   , 0         , 0     ),
            ("ising_model_13"   , 0         , 0     ),
            ("ising_model_16"   , 0         , 0     ),
            ("qft_10"           , 39        , 0     ),
            ("qft_13"           , 99        , 0     ),
            ("qft_16"           , 225       , 0     ),
            ("qft_20"           , 375       , 0.015 ),
            ("rd84_142"         , 153       , 0      ),
            ("adr4_197"         , 1566      , 0.015 ),
            ("radd_250"         , 1353      , 0.015 ),
            ("z4_268"           , 1071      , 0.015 ),
            ("sym6_145"         , 1017      , 0.015 ),
            ("misex1_241"       , 2118      , 0.031 ),
            ("rd73_252"         , 2352      , 0.016 ),
            ("cycle10_2_110"    , 2226      , 0.031 ),
            ("square_root_7"    , 2061      , 0.031 ),
            ("sqn_258"          , 3708      , 0.046 ),
            ("rd84_253"         , 6411      , 0.063 ),
            ("co14_215"         , 5634      , 0.062 ),
            ("sym9_193"         , 15420     , 0.156 ),
            ("urf5_158"         , 69852     , 0.640 ),
            ("hwb9_119"         , 93219     , 0.891 ),
            ("urf4_187"         , 220329    , 2.123 )
        )

#4) ramdom initial mapping
# TCAD main mapping algorithm
# reverse traversal
RESULT_RR =(("4mod5-v1_22"      , 0         , 0     ),
            ("mod5mils_65"      , 0         , 0     ),
            ("alu-v0_27"        , 3         , 0     ),
            ("decod24-v2_43"    , 0         , 0     ),
            ("4gt13_92"         , 0         , 0     ),
            ("ising_model_10"   , 0         , 0     ),
            ("ising_model_13"   , 0         , 0     ),
            ("ising_model_16"   , 0         , 0     ),
            ("qft_10"           , 51        , 0     ),
            ("qft_13"           , 93        , 0     ),
            ("qft_16"           , 177       , 0.015 ),
            ("qft_20"           , 348       , 0.016 ),
            ("rd84_142"         , 132       , 0.015 ),
            ("adr4_197"         , 1377      , 0.046 ),
            ("radd_250"         , 1233      , 0.046 ),
            ("z4_268"           , 969       , 0.046 ),
            ("sym6_145"         , 1050      , 0.045 ),
            ("misex1_241"       , 1233      , 0.047 ),
            ("rd73_252"         , 1854      , 0.062 ),
            ("cycle10_2_110"    , 2151      , 0.078 ),
            ("square_root_7"    , 2565      , 0.103 ),
            ("sqn_258"          , 3573      , 0.094 ),
            ("rd84_253"         , 5658      , 0.156 ),
            ("co14_215"         , 6903      , 0.234 ),
            ("sym9_193"         , 13920     , 0.374 ),
            ("urf5_158"         , 70113     , 1.841 ),
            ("hwb9_119"         , 81657     , 2.248 ),
            ("urf4_187"         , 187785    , 6.690 )
        )




#5) TCAD initial mapping
# TCAD main mapping algorithm
# reverse traversal
RESULT_OP =(("4mod5-v1_22"      , 0         , 0     ),
            ("mod5mils_65"      , 0         , 0     ),
            ("alu-v0_27"        , 3         , 0     ),
            ("decod24-v2_43"    , 0         , 0     ),
            ("4gt13_92"         , 21        , 0     ),
            ("ising_model_10"   , 0         , 0     ),
            ("ising_model_13"   , 0         , 0     ),
            ("ising_model_16"   , 0         , 0     ),
            ("qft_10"           , 39        , 0.015 ),
            ("qft_13"           , 96        , 0.031 ),
            ("qft_16"           , 192       , 0.062 ),
            ("qft_20"           , 360       , 0.109 ),
            ("rd84_142"         , 108       , 0.046 ),
            ("adr4_197"         , 1224      , 0.218 ),
            ("radd_250"         , 1047      , 0.186 ),
            ("z4_268"           , 855       , 0.202 ),
            ("sym6_145"         , 1017      , 0.202 ),
            ("misex1_241"       , 1098      , 0.249 ),
            ("rd73_252"         , 2193      , 0.343 ),
            ("cycle10_2_110"    , 1968      , 0.348 ),
            ("square_root_7"    , 1788      , 0.406 ),
            ("sqn_258"          , 3057      , 0.563 ),
            ("rd84_253"         , 5697      , 0.892 ),
            ("co14_215"         , 5061      , 1.062 ),
            ("sym9_193"         , 13746     , 2.091 ),
            ("urf5_158"         , 58947     , 9.312 ),
            ("hwb9_119"         , 89355     , 12.909),
            ("urf4_187"         , 168366    , 38.42 )
        )

# define paths
dirpos = "../example"
binaryName = "./QC2021_Base"
outpos = "../output"
revpos = "../reverse"
#logpos = "../log/initial_mapping_upgrade"
logpos = "../log"

#
curTime = datetime.now().strftime('%m_%d_%H_%M')
benchlist_dir = []
benchlist = []
benchlist_initial = ["qft_13.qasm", "qft_20.qasm", "adr4_197.qasm", "z4_268.qasm",
    "square_root_7.qasm", "co14_215.qasm", "urf5_158.qasm", ]
benchDir = sorted(os.listdir(dirpos))

# result include tuple (baseline result, our result) with template ()
benchresult = []


### function define ###
def ExecuteCommand( curBench, result, alpha, beta, gamma, time_limit):
    inputPath = "%s/%s" % (dirpos, curBench)
    outputPath = "%s/%s" % (outpos, curBench)
    reversePath = "%s/%s" % (revpos, curBench)
    #logPath = "%s/%s.%s.___%s_%s_%s.log" % (logpos, curBench, curTime, alpha, beta, gamma)
    logPath = "%s/%s.%s.log" % (logpos, curBench, curTime)
    exeStr = "%s %s %s %s %s %s %s | tee %s" % (binaryName, inputPath, outputPath, reversePath, alpha, beta, gamma, logPath)
    print( exeStr )
    run = RunCommand(exeStr, time_limit)
    if(run!=None):
        ourResult = ParseLog(logPath)
        TCADResult = FindTCAD(curBench[:-5])
        result.append( [(curBench[:-5], alpha, beta, gamma), TCADResult, ourResult] )
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



def FindTCAD( name ):
    swap, time = 0, 0
    for rslt in RESULT_OP:
        curBench = rslt[0]
        if curBench == name:
            swap = rslt[1]
            time = rslt[2]
    return (swap, time)

def ParseLog( logPath ):
    swap, time = 0, 0
    f = open(logPath)
    for line in f:
        if line[0:16] == "# add_cnot_num: " :
            swap = int(line[16:])
        #if line[0:13] == "# add_depth: " :
        #    depth = int(line[13:])
        if line[0:11] == "END Program":
            time = line.rstrip('\n').split(' ')[-1]
    f.close()
    #cnot_num = [ swap, depth, time ]
    cnot_num = [ swap, time ]
    return cnot_num

def getdirList( ):
    for curBench in benchDir:
        if curBench[-5:] == ".qasm":
            benchlist_dir.append(curBench)


########################
beta_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]


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
    ExecuteCommand( benchlist_dir[benchNum], benchresult, 0.8, 9, 3, 50)
        
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
                    args=(curBench, results, 0.8, 9, 3, 50))
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
                    args=(curBench, results, 0.8, 9, 3, 50))
            procs.append(proc)
            proc.start()
        for proc in procs:
            proc.join()
    benchresult = results[:]

else:
    print("ERROR: NO ACCEPTABLE ARGUMENT")
    sys.exit(1)

benchresult = sorted(benchresult, key=lambda row: row[0][0])
avg_set = []
print(" ")
print(" ")
print("----------------------------- Result --------------------------------------------------")
print("               |       TCAD        |                 Ours                  ")
#print("  bench_name   |  #CNOT  | RunTime |  #CNOT  | RunTime |   + Depth  | improve")
print("  bench_name   |  #CNOT  | RunTime |  #CNOT  | RunTime | improve")
print("---------------------------------------------------------------------------------------")
for benchResult in benchresult:
    s1 = '%-14s' % benchResult[0][0]
    s_beta  = '%4s' % benchResult[0][2]
    s2 = '%7s' % benchResult[1][0]
    s3 = '%7s' % benchResult[1][1]
    s4 = '%7s' % benchResult[2][0]  #CNOT
    s5 = '%7s' % benchResult[2][1]  #RunTime
    #s5 = '%7s' % benchResult[2][2]  #RunTime
    #s6 = '%10s' % benchResult[2][1]  #Depth
    

    try:
        avg = round(int(s4) / int(s2), 6)
        avg_set.append(avg)
        print( s1, "|",
                s2, "|", s3, "|", 
                #s4, "|", s5, "|", s6, "|",
                s4, "|", s5, "|",
                avg)
    except ZeroDivisionError as e:
        print( s1, "|",
                s2, "|", s3, "|",
                #s4, "|", s5, "|", s6, "|",
                s4, "|", s5, "|",
                e)
avg_improve = round(sum(avg_set)/len(avg_set), 6)
print("---------------------------------------------------------------------------------------")
print("==> avg_improve: ", avg_improve)
