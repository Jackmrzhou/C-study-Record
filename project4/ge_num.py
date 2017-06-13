import random, subprocess

for k in range(10):
	with open('read', 'w') as fp:
		for i in range(100000):
			#fp.write(str(i)+' ')
			fp.write(str(random.randint(0,100000))+' ')
	p = subprocess.Popen(['test'], stdout=subprocess.PIPE, shell=False)

	print(p.stdout.read())