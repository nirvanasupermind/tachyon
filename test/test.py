import time
t1 = time.time()
def sqrtBisect(x):
    lo = 1
    hi = x
    i = 0
    while i < 50:
        avg = (lo + hi)/2
        if avg * avg < x:
            lo = avg
        else:
            hi = avg
        
        i = i + 1
    
    return lo

i = 0
while i < 1000000:
    sqrtBisect(2)
    i = i + 1

t2 = time.time()
print(t2 - t1)