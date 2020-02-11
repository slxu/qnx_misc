"""
    spin up a loop of simple computation for each CPU core on the system, recording the ops per core
    and ops averagely.
    
    Each core is tested in a separate process.

    Note each process is not pinned to core, so the result may have variances caused by scheduling
    moving around processes among cores.

"""

import time;
from multiprocessing import Pool
from multiprocessing import cpu_count
import signal

stop_loop = False
processes = cpu_count()

def exit_chld(x, y):
    global stop_loop
    stop_loop = True

def f(x):
    global stop_loop
    i = 0
    ts = time.time()
    while not stop_loop:
        x*x
        i+=1
    te = time.time()
    ops = (i/(te-ts))
    return ops

signal.signal(signal.SIGINT, exit_chld)

if __name__ == '__main__':
    print('-' * 20)
    print('Running load on %d CPU core(s)' % processes)
    print('-' * 20)
    pool = Pool(processes)
    core_ops = pool.map(f, range(processes))
    avg_ops = 0.0
    for x in xrange(processes):
        ops = core_ops[x]
        print("cpu core " + str(x) + ", ops: " + str(ops))
        avg_ops += ops
    print("avg ops: " + str(avg_ops / len(core_ops)))
