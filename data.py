'''
from time import time
import tracemalloc
import sys
tracemalloc.start()
                 
n=1
def a(b,c):
    if b<c:
        return 0
    x = 0
    for _ in range(n):
        x += 1
    return a(b-c, c)+1

sys.setrecursionlimit(10005)
a(10000,1)
snapshot = tracemalloc.take_snapshot()
top_stats = snapshot.statistics('lineno')
print("[ Top 10 ]")
for stat in top_stats[:10]:
    print(stat)
'''

'''
import time
start = time.time()
n = 100000000
x = 0
for _ in range(n):
    x += 1
print(time.time()-start)
'''


import sys
from time import time
n=100000
def fibo(a):
    if a<=1:
        return 1
    return fibo(a-1)+fibo(a-2)

for _ in range(50):
    st = time()
    fibo(35)
    print(time()-st)

