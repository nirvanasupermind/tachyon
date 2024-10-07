import time
start = time.time()
for i in range(0, 10000000):
    x = 2.0 + 2.0
end = time.time()
print(f'elapsed time: {end - start}')