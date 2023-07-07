t1 = os.clock()
i = 0.0
while i < 100000000.0 do
    i = i + 1.0
end
t2 = os.clock()
print(t2 - t1)