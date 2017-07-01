with open('write', 'r') as fp:
	temp = 0
	for num in fp.readline().split():
		if temp > int(num):
			print("Not ordered!")
		temp = int(num)
	else:
		print("Ordered!")