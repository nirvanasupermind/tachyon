import time
t1 = time.time()
i = 0
while i < 1e+7:
    i = i + 1
t2 = time.time()
print(t2 - t1)
print(i)