from functools import partial
from scipy.io import loadmat
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from pycpd import rigid_registration
import numpy as np
import time
import math

def visualize(iteration, error, X, Y, ax):
    plt.cla()
    ax.scatter(X[:,0],  X[:,1], X[:,2], color='red')
    ax.scatter(Y[:,0],  Y[:,1], Y[:,2], color='blue')
    plt.draw()
    print "iteration %d, error %.5f" % (iteration, error)
    plt.pause(0.001)

def cpdCorresp(X, T, sigma2, outlier, N, M, D):
	ksig = 0.0
	diff = 0.0
	razn = 0.0
	outlier_tmp = 0.0
	temp_x = 0.0
	sp = 0.0
	P = np.zeros(shape=(M,1))
	P1 = np.zeros(shape=(M,1))
	Pc = np.zeros(shape=(M,1))

	ksig = -2.0 * (sigma2 + 1e-3)
	outlier_tmp = (outlier * M * pow(-ksig*3.14159265358979,0.5*D)) / ((1 - outlier) * N)

	for n in range(0,N):
		sp=0
		for m in range(0,M):
			razn=0
			for d in range(0,D):
				diff=(X[n][d])-(T[m][d])
				diff=diff*diff
				razn+=diff

			P[m][0] = math.exp(razn/ksig)
			sp+=(P[m][0])

		sp+=outlier_tmp

		for m in range(0,M):
			temp_x=(P[m][0])/sp

			if n == 0:
				P1[m][0] = (P[m][0])/ sp
				Pc[m][0] = n+1

			if (temp_x > (P1[m][0])):
				P1[m][0] = (P[m][0])/ sp
				Pc[m][0]=n+1
			#print m,' ',n,' ',Pc[m]
	return Pc

def main():
    #fish = loadmat('./bunny.mat')
    #X = fish['X']
    #print "Number of X Vertices: %d" % (len(X))
    #Y = X + 1
    #print "Number of Y Vertices: %d" % (len(Y))
    
    X = np.array([]).reshape(0,3)
    with open("Base_01_decimated.obj", "r") as filestream:
        for line in filestream:
            if line.startswith("v "):
                line = line[2:]
                currentLine = line.split(" ")
                currentLine[2] = currentLine[2][:-1]
                currentVertex = np.array(map(float, currentLine))
                X = np.vstack([X,currentVertex])

	print "Number of Vertices: %d" % (len(X))
	#print 'Type of X data struct: ',type(X)
	#print X

	Y = np.array([]).reshape(0,3)
	with open("Patient_02_decimated.obj", "r") as filestream:
		for line in filestream:
			if line.startswith("v "):
				line = line[2:]
				currentLine = line.split(" ")
				currentLine[2] = currentLine[2][:-1]
				currentVertex = np.array(map(float, currentLine))
				Y = np.vstack([Y,currentVertex])

    print "Number of Vertices: %d" % (len(Y))
    #print Y

##    fig = plt.figure()
##    ax = fig.add_subplot(111, projection='3d')
##    callback = partial(visualize, ax=ax)

    reg = rigid_registration(X, Y)
    reg.maxIterations=150
##    reg.register(callback)
    reg.register(None)
    print 'Total Iterations: ',reg.iteration
    print 'End Error: ',reg.err
    np.savetxt("X.txt", X)
    np.savetxt("Y.txt", Y)
    np.savetxt("T.txt", reg.Y)
    np.savetxt("Corr.txt",cpdCorresp(reg.X, reg.Y, reg.sigma2, 0.6, reg.X.shape[0], reg.Y.shape[0], reg.X.shape[1]))
    #print reg.X
    #print reg.Y
##    plt.show()

if __name__ == '__main__':
    main()
##	arr = np.loadtxt("02_to_01.txt")
##	arrNew = np.loadtxt("Corr.txt")
##	for num in range(0,len(arr)):
##		print abs(arrNew[num]-arr[num])