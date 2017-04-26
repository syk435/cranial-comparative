import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

#arr = np.loadtxt("02_to_01.txt")
arrNew = np.loadtxt("Corr.txt")
#for num in range(0,len(arr)):
#	print abs(arrNew[num]-arr[num])
X = np.loadtxt("X.txt")
Y = np.loadtxt("Y.txt")
T = np.loadtxt("T.txt")
Y = Y+130

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
plt.cla()
ax.scatter(X[:,0],  X[:,1], X[:,2], color='red')
ax.scatter(Y[:,0],  Y[:,1], Y[:,2], color='orange')
#ax.scatter(T[:,0],  T[:,1], T[:,2], color='green')
for idx in range(0,len(arrNew)):
	if idx%500 == 0:
		ptX = X[int(arrNew[idx]-1)]
		ptY = Y[idx]
		ax.plot([ptX[0],ptY[0]],[ptX[1],ptY[1]],[ptX[2],ptY[2]])

plt.draw()
plt.show()