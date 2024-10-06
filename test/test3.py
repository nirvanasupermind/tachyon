import time
a = time.time()
for i in range(0,1000000000):
    x = 2.0 + 2.0
b = time.time()
print(f'elapsed time:{b - a}')