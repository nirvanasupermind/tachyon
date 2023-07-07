import time
t1 = time.time()
i = 0.0
while i < 100000000.0:
    i = i + 1.0
t2 = time.time()
print(t2 - t1)