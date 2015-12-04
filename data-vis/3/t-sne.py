import numpy as np
import matplotlib.pyplot as plt
from matplotlib import offsetbox
from sklearn import (manifold, datasets, decomposition, ensemble, random_projection)
from math import exp, log

digits = datasets.load_digits()
X = digits.data
y = digits.target
n_samples, n_features = X.shape

def plot_embedding(X, title=None):
    x_min, x_max = np.min(X, 0), np.max(X, 0)
    X = (X - x_min) / (x_max - x_min)

    plt.figure(figsize=(10, 10))
    ax = plt.subplot(111)
    for i in range(X.shape[0]):
        plt.text(X[i, 0], X[i, 1], str(digits.target[i]),
                 color=plt.cm.Set1(y[i] / 10.),
                 fontdict={'weight': 'bold', 'size': 9})
    plt.xticks([]), plt.yticks([])
    if title is not None:
        plt.title(title)
    plt.show()

euclxij = np.sum((X[:, np.newaxis, : ] - X[np.newaxis, :, : ]) ** 2, axis=2)

def pi_j(i,j,s):
    return np.exp(-euclxij[i][j]/2/s**2)/(np.sum(np.exp(-euclxij[i][:]/2/s**2))-np.sum(np.exp(-euclxij[i][i]/2/s**2)))

Perp = 30
def binSearchSigma(i):
    def func(s):
        ps = np.array([pi_j(i,j,s) for j in range(len(X))])
        return log(Perp,2) + np.sum(ps*np.log2(ps))

    s, smin, smax, step = 0, 2, 22, 20
    while func(smax) > 0:
        smin+=step
        smax+=step
    prec = 0.1

    while True:
        sres = func((smin+smax)/2)
        if abs(sres) < prec :
            s = (smin+smax)/2
            break
        if sres < 0 : smax = (smin+smax)/2
        else : smin = (smin+smax)/2
    return s


T, nu = 1000, 100
(n, d) = X.shape

Y = np.random.randn(n, 2)
Yprev = np.zeros((n, 2))
dY = np.zeros((n,2))

sigmas = [binSearchSigma(i) for i in range(n)]
pi_js = [[pi_j(i,j,sigmas[i]) for i in range(n)] for j in range(n)]
P = np.array([[(pi_js[i][j]+pi_js[j][i])/2/n for i in range(n)] for j in range(n)])

for t in range(T):

    print(t)

    alpha = 0.5 if t < 180 else 0.8

    euclyij = np.sum((Y[:, np.newaxis, : ] - Y[np.newaxis, :, : ]) ** 2, axis=2)
    sttm = (1+euclyij)**-1
    Q = sttm/(np.sum(sttm)-np.diag(sttm))
    PQ = P-Q

    for i in range(n):
        temp = PQ[i, :]*sttm[i, :]
        dY[i,:] = np.sum(np.array([temp, temp]).T * (Y[i,:] - Y), 0)

    Ynew = Y-nu*dY+alpha*(Y-Yprev)
    Yprev = Y
    Y = Ynew

plot_embedding(Y, "Result")


