import random

def discrete(weights, count):
	eta = sum(weights)
	for i in range(0,count):
		r = random.random() * eta
		index = 0
		for w in weights:
			if r > w:
				r -= w
				index += 1
			else:
				yield index
				break

E_E = [[0.8,0.2],[0.3,0.7]]
E_Z = [[0.7],[0.3]]
R = [[0.2,0.8],[0.7,0.3]]
C = [[0.1,0.9],[0.3,0.7]]
Z = []
O = [0,3,1,0]
def cross(a,b):
	results = []
	for x in a:
		for y in b:
			results += [x*y]
	return results

Z += [cross(R[0],C[0])]
Z += [cross(R[1],C[1])]

n = 10;
X = ["E","N"]

def pretty(array):
	result = "["
	for a in array:
		result += str(a) + ","
	result += "]"
	return result

particles = [0] * 7 + [1] * 3
print particles
weights = []
for i in range(1,4):
	print i, ": "
	#propagation
	particles = [[x for x in discrete(E_E[p],1)][0] for p in particles]

	print pretty([X[p] for p in particles])
	#weighting
	o = O[i]
	weights = [Z[p][o] for p in particles]
	print "%.3f, "*len(weights)%tuple(weights)
	eta = sum(weights)
	print eta
	weights = [w / eta for w in weights]
	print "%.3f, "*len(weights)%tuple(weights)

	#resampling
	particles = [particles[j] for j in discrete(weights,n)]
	print pretty([X[p] for p in particles])

	#E[i] = E_Z[0][i]
	
