from pygraphml import GraphMLParser
import numpy as np
import matplotlib.pyplot as plt

# parse xml and init some variables
parser = GraphMLParser()
# g = parser.parse('test_tree.graphml')
g = parser.parse('test_complete_tree.graphml')
nodes = sorted(g.nodes(), key=lambda node: node['label'])
nodesCoord = [[0,0] for i in range(len(nodes))]
nodesId = [node.id for node in nodes]
nodesLabel = [node['label'] for node in nodes]
edges = [(nodesLabel.index(e.node1['label']), nodesLabel.index(e.node2['label'])) for e in g.edges()]
edgesVisited = []

#get child nodes
def getCandidates(node):
    candidates = []
    for e in edges:
        if e[0] == node or e[1] == node :
            if e not in edgesVisited :
                edgesVisited.append(e)
                candidates.append(e[0] if e[1] == node else e[1])
    return candidates

#go algo
def calculate(node, w, hmax):
    nodesCoord[node] = [w, hmax]
    candidates = getCandidates(node)
    if len(candidates) > 0 :
        h = calculate(candidates[0], w+1, hmax)
        hmax = max(h,hmax)
    if len(candidates) > 1 :
        h = calculate(candidates[1], w, hmax+1)
        hmax = max(h,hmax)
    return hmax

calculate(nodesId[0],0,0)

#calculate maxdepth and maxwidth, recalculate coordinates
maxx, maxy = 2+max([nc[0] for nc in nodesCoord]), 2+max([nc[1] for nc in nodesCoord])
delimx, delimy = 400./maxx, 400./maxy
nodesCoord = [[(nc[0]+1)*delimx, 400-(nc[1]+1)*delimy] for nc in nodesCoord]

# draw plot
plt.axis([0, 400, 0, 400])
for e in edges :
    plt.plot([nodesCoord[e[0]][0], nodesCoord[e[1]][0]], [nodesCoord[e[0]][1], nodesCoord[e[1]][1]], 'bo-')
plt.show()
