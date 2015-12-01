from pygraphml import GraphMLParser
import numpy as np
import matplotlib.pyplot as plt

# parse xml
parser = GraphMLParser()
g = parser.parse('myGraph3.graphml')
nodes = sorted(g.nodes(), key=lambda node: node['label'])
nodesLabel = [node['label'] for node in nodes]
edges = [(nodesLabel.index(e.node1['label']), nodesLabel.index(e.node2['label'])) for e in g.edges()]

pf = 3 #points fixed - according to task
n = len(nodes)

# calculate node degree
def deg(ver):
    counter = 0
    for e in edges:
        if ver == e[0] or ver == e[1]:
            counter += 1
    return counter

# check if there is en edge between nodes
def edge(ver1, ver2):
    for e in edges:
        if (e[0] == ver1 and e[1] == ver2) or (e[1] == ver1 and e[0] == ver2):
            return 1
    return 0

# make equations
a = []
for i in range(pf):
    ai = [1 if j == i else 0 for j in range(n)]
    a.append(ai)
for i in range(pf,n):
    ai = [-deg(i) if i == j else edge(i,j) for j in range(n)]
    a.append(ai)

bx = [10, 200, 390]+[0 for i in range(n-pf)]
by = [10, 390, 10]+[0 for i in range(n-pf)]

# solve equations
x = np.linalg.solve(np.array(a),np.array(bx))
y = np.linalg.solve(np.array(a),np.array(by))

# draw plot
plt.axis([0, 400, 0, 400])
for e in edges :
    plt.plot([x[e[0]], x[e[1]]], [y[e[0]], y[e[1]]], 'bo-')
plt.show()
