E = [0.7, 0, 0, 0]
E_Z = [0.7,0,0,0]
N_Z = [0.3,0,0,0]
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
Z += [cross(R[1],C[1])];

for i in range(1,4):
	print i, ": "
	E_Z[i] = 0.8 * E_Z[i-1] + 0.3 * (1-E_Z[i-1])
	print ' ',"P(E) =",E_Z[i]
	o = O[i]
	_o = str(o)
	print ' ',"P(Z_"+_o+"|E) =", Z[0][o]	
	print ' ',"P(Z_"+_o+"|N) =", Z[1][o]
	z = Z[0][o]*E_Z[i] + Z[1][o]*(1-E_Z[i])
	E_Z[i] = Z[0][o]*E_Z[i] / z
	N_Z[i] = Z[1][o]*(1-E_Z[i]) / z
	print "P(E|Z_"+_o+") =", E_Z[i]
	#E[i] = E_Z[i]
	

print E_Z
