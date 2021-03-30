# Spencer Smith
# CS 380
# AI_HW3. Mouse Maze
# Last Updated: 3/31/2020

from tkinter import *
import time

class StateSpaceNode:
    def __init__(self, letter, coords, goal = False):
        self.letter = letter
        self.goal = goal
        self.coords = coords
        self.connections = []
    
    def connect(self, newNode):
        if newNode in self.connections:
            return
        else:
            self.connections.append(newNode)
            newNode.connect(self)

class SearchSpaceNode:
    def __init__(self, node = StateSpaceNode, weight = 0, orientation = 0):
        self.orientation = orientation
        self.node = node
        self.weight = weight
        self.paths = []
        self.goal = False
        self.parent = None

    def addPath(self, newNode):
        if newNode not in self.paths and newNode != self.node:
            self.paths.append(newNode)
            newNode.parent = self

    def setGoalPath(self):
        self.goal = True
        if self.parent != None:
            self.parent.setGoalPath()

    def getTree(self):
        returnTree = [self.node.letter, self.weight, self.goal]
        for path in self.paths:
            child = path.getTree()
            returnTree.append(child)

        return returnTree

    def getSuccessPath(self):
        if self.goal:
            returnPath = [self.node.letter, self.weight, True]
            for path in self.paths:
                if path.goal:
                    returnPath.append(path.getSuccessPath())
                else:
                    returnPath.append([path.node.letter, path.weight, False])
            return returnPath

        else:
            return None

def isIntersection(tile):
    opening = 0
    for wall in tile:
        if wall == 2 or wall == 3:
            opening += 1
                    
    if opening >= 3:
        return True

    return False

def findWeight(startCoords, targetCoords, nodes, maze, startOrientation = 0):
    allNodeCoords = []
    for node in nodes:
        allNodeCoords.append(node.coords)
    
    currentCoords = startCoords
    weight = 0
    tryCount = 0

    searching = True
    initOrientation = startOrientation
    orientation = initOrientation
    visited = [currentCoords]
    prevOrientation = initOrientation

    while(searching):
        #print("in:", currentCoords, "orientation:", orientation)
        currentNode = maze[currentCoords[0]][currentCoords[1]]
        #time.sleep(.2)
        extraWeight = 0

        if isIntersection(currentNode) and currentCoords not in allNodeCoords and currentCoords != startCoords:
            extraWeight =  findWeight(currentCoords, targetCoords, nodes, maze, orientation)
            if extraWeight != -1:
                weight = extraWeight[0] + weight
                orientation = extraWeight[1]
                return (weight, orientation)

        if currentCoords == targetCoords:
            searching == False
            #print("Success, weight = ", weight)
            
            return (weight, orientation)

        elif extraWeight == -1 or (currentCoords in allNodeCoords and currentCoords != startCoords):
            #print("Restarting")
            weight = 0
            currentCoords = startCoords
            initOrientation += 1
            initOrientation = initOrientation % 4
            orientation = initOrientation
            prevOrientation = startOrientation
            if startOrientation == initOrientation:
                print("ERROR: couldn't find path from", startCoords, "to", targetCoords)
                return -1
            
        else:
            
            
            #checkforward
            if currentNode[orientation] == 2:
                tempCoords = currentCoords
                if orientation == 0: currentCoords = [currentCoords[0] - 1, currentCoords[1]]
                elif orientation == 1: currentCoords = [currentCoords[0], currentCoords[1] + 1]
                elif orientation == 2: currentCoords = [currentCoords[0] + 1, currentCoords[1]]
                elif orientation == 3: currentCoords = [currentCoords[0], currentCoords[1] - 1]
                if currentCoords in visited:
                    currentCoords = tempCoords
                    orientation += 1
                    orientation = orientation % 4
                    tryCount += 1
                    if tryCount > 3:
                        return -1
                        print("ERROR: couldn't find path from", startCoords, "to", targetCoords)
                #Path found and not traversed
                else:
                    tryCount = 0
                    weight += 1

                    turns = abs(orientation - prevOrientation)
                    if turns == 3: turns = 1
                    #if turns > 0: print("turns at:", currentCoords, "=", turns, "(", orientation, "-", prevOrientation, ")")
                    weight += turns


                    prevOrientation = orientation
                    visited.append(currentCoords)
            
            #turnright
            else:
                orientation += 1
                orientation = orientation % 4

    return 1

def ucsSearch(nodes, maze):
    currentNode = nodes[0]
    visited = [currentNode]
    searching = True
    
    currentSearchNode = SearchSpaceNode(currentNode)
    searchSpace = currentSearchNode
    parentSearchNode = searchSpace

    currentPath = 0
    paths = [currentSearchNode]

    while(searching):
        #Breaks if it found goal node
        if (currentNode.goal):
            currentSearchNode.setGoalPath()
            searching = False
            break

        #Explores new possible paths
        for connection in currentNode.connections:
            if connection not in visited:
                finalOrientation = currentSearchNode.orientation
                visited.append(connection)
                searchData = findWeight(currentNode.coords, connection.coords, nodes, maze, finalOrientation)
                weight = searchData[0] + paths[currentPath].weight
                finalOrientation = searchData[1]

                newSearchNode = SearchSpaceNode(connection, weight, finalOrientation)
                currentSearchNode.addPath(newSearchNode)
                paths.append(newSearchNode)
        
        #removes current path from possible paths
        paths.pop(currentPath)

        #Finds new lowest path
        if len(paths) > 0:
            lowestIndex = 0
            lowestWeight = paths[0].weight
            index = 0
            for path in paths:
                if path.weight < lowestWeight:
                    lowestIndex = index
                    lowestWeight = path.weight
                index += 1

            #sets current path and current node
            currentNode = paths[lowestIndex].node
            currentPath = lowestIndex
            currentSearchNode = paths[lowestIndex]
        else:
            searching = False
            break

    return searchSpace

def dlsSearch(nodes, maze, limit):
    depth = 0
    currentNode = nodes[0]
    searching = True
    
    currentSearchNode = SearchSpaceNode(currentNode)
    searchSpace = currentSearchNode

    while(searching):
        if (currentNode.goal):
            currentSearchNode.setGoalPath()
            searching = False
            break

        #searches while below limit
        elif depth < limit:
            currentPath = []
            #generates the current path
            tempNode = currentSearchNode
            while tempNode.parent != None:
                tempNode = tempNode.parent
                currentPath.append(tempNode.node)


            #adds visited nodes to the path
            for path in currentSearchNode.paths:
                currentPath.append(path.node)

            visitList = []
            #find from current Node
            for node in currentNode.connections:
                if node not in currentPath:
                    visitList.append(node)

            #So long as the visitList is not empty, moves to next node
            if len(visitList) > 0:
                connection = visitList[0]

                finalOrientation = currentSearchNode.orientation
                searchData = findWeight(currentNode.coords, connection.coords, nodes, maze, finalOrientation)

                weight = searchData[0]
                finalOrientation = searchData[1]

                newSearchNode = SearchSpaceNode(connection, weight, finalOrientation)
                currentSearchNode.addPath(newSearchNode)

                currentNode = connection
                currentSearchNode = newSearchNode
                depth += 1
            #Backtracks if no possible visits
            elif currentSearchNode.parent != None:
                currentSearchNode = currentSearchNode.parent
                currentNode = currentSearchNode.node
                depth -= 1
            else:
                print("No possible goal path at given limit")
                break
        #Backtracks if depth is reached
        elif currentSearchNode.parent != None:
            currentSearchNode = currentSearchNode.parent
            currentNode = currentSearchNode.node
            depth -= 1
        else:
            print("No possible goal path at given limit")
            break
                
    return searchSpace

def bfsSearch(nodes, maze):
    currentNode = nodes[0]
    searching = True
    
    currentSearchNode = SearchSpaceNode(currentNode)
    searchSpace = currentSearchNode

    currentLevel = [currentSearchNode]
    while searching:
        newLevel = []
        if len(currentLevel) == 0:
            print("error, no possible goal path")
            break

        for node in currentLevel:
            if node.node.goal:
                node.setGoalPath()
                searching = False
                break

            currentPath = []
            tempNode = node
            while tempNode.parent != None:
                tempNode = tempNode.parent
                #print(tempNode.node.letter)
                currentPath.append(tempNode.node)
                #time.sleep(0.05)
                
            #time.sleep(.5)
            #print ("here")
            
            for connection in node.node.connections:
                if connection not in currentPath and connection != node.node:
                    orientation = node.orientation
                    searchData = findWeight(node.node.coords, connection.coords, nodes, maze, orientation)

                    if searchData == -1:
                        print("ERROR: no path from", node.node.letter, "to", connection.letter)
                        return
                    else:
                        weight = searchData[0]
                        orientation = searchData[1]

                    newSearchNode = SearchSpaceNode(connection, weight, orientation)
                    node.addPath(newSearchNode)
                    newLevel.append(newSearchNode)
        currentLevel = newLevel


    return searchSpace

def printTree(tree, level = 0):
    print(tree[0], tree[1], level)
    for node in tree:
        if type(node) == list:
            printTree(node, level + 1)

def mazeNG():
    verticalHall = [2, 1, 2, 1]
    horizontalHall = [1, 2, 1, 2]
    structure = [ 
        [ [1, 2, 2, 1], horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, [3, 1, 2, 2]],
        [ [2, 2, 1, 1], horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, [1, 2, 2, 2]  , horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, [2, 1, 2, 2]],
        [ [1, 2, 2, 1], horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, [2, 1, 1, 2]  , [1, 2, 1, 1]  , horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, [2, 1, 1, 2]],
        [ [2, 2, 2, 1], horizontalHall, horizontalHall, [1, 2, 2, 2]  , [1, 2, 2, 2]  , [1, 1, 2, 2]  , [1, 2, 2, 1]  , [1, 1, 2, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, horizontalHall, [1, 2, 2, 2]  , horizontalHall, horizontalHall, horizontalHall, [1, 2, 2, 2]  , [1, 1, 2, 2]  , [1, 2, 2, 1]  , [1, 1, 2, 2]],
        [ [2, 2, 1, 1], horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ [1, 2, 2, 1], horizontalHall, [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ [2, 2, 1, 1], horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ [1, 2, 2, 1], horizontalHall, [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 2, 1]  , [1, 1, 2, 2]  , [1, 2, 2, 1]  , [1, 1, 2, 2]  , [2, 2, 2, 1]  , [1, 1, 2, 2]  , [1, 2, 2, 1]  , [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ [2, 2, 1, 1], horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall],
        [ [1, 2, 2, 1], [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 1, 1]  , [2, 1, 1, 2]  , [2, 2, 1, 1]  , [2, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [1, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 2, 1]  , [2, 1, 1, 2]  , [2, 1, 1, 1]],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , verticalHall  , [2, 1, 1, 1]  , [2, 2, 1, 1]  , [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 2, 1]  , [1, 2, 2, 2]  , [1, 1, 2, 2]],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, [2, 1, 1, 2]  , [2, 2, 2, 1]  , horizontalHall, horizontalHall, horizontalHall, [2, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, [1, 2, 2, 2]  , [2, 1, 2, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, [2, 1, 1, 2]  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, [1, 1, 2, 2]  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , verticalHall  , [2, 2, 1, 1]  , [2, 1, 1, 2]  , [2, 1, 1, 1]  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, [2, 1, 1, 2]  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, verticalHall  , verticalHall  , verticalHall  , verticalHall  , [2, 2, 1, 1]  , horizontalHall, [1, 1, 2, 2]  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, horizontalHall, [2, 1, 1, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [2, 1, 1, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ verticalHall, [2, 2, 1, 1]  , [2, 1, 1, 2]  , verticalHall  , verticalHall  , [1, 2, 2, 1]  , horizontalHall, [2, 1, 1, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, horizontalHall, horizontalHall, [2, 2, 1, 2]  , horizontalHall, horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall],
        [ [2, 2, 1, 1], horizontalHall, horizontalHall, [2, 2, 1, 2]  , [2, 1, 1, 2]  , [2, 2, 1, 1]  , horizontalHall, horizontalHall, [2, 2, 1, 2]  , horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, horizontalHall, [1, 1, 1, 2]  , [2, 2, 1, 1]  , [2, 1, 1, 2]  , [2, 1, 1, 1]  , [2, 1, 3, 1]]
        ]
    
    return structure      

def nodesNG():

    nodeA = StateSpaceNode('A', [0, 19])
    nodeB = StateSpaceNode('B', [0, 9])
    nodeC = StateSpaceNode('C', [0, 3])
    nodeD = StateSpaceNode('D', [3, 3])
    nodeE = StateSpaceNode('E', [8, 9])
    nodeF = StateSpaceNode('F', [7, 10])
    nodeG = StateSpaceNode('G', [19, 19], True)
    nodeH = StateSpaceNode('H', [12, 3])
    nodeJ = StateSpaceNode('J', [7, 13])
    nodeK = StateSpaceNode('K', [3, 19])
    nodeL = StateSpaceNode('L', [5, 19])
    nodeM = StateSpaceNode('M', [8, 17])
    nodeN = StateSpaceNode('N', [12, 12])
    nodeO = StateSpaceNode('O', [16, 3])
    nodeP = StateSpaceNode('P', [13, 18])
    nodeQ = StateSpaceNode('Q', [12, 17])
    nodeR = StateSpaceNode('R', [16, 15])
    nodeS = StateSpaceNode('S', [16, 19])
    nodeT = StateSpaceNode('T', [17, 10])
    nodeU = StateSpaceNode('U', [17, 11])
    nodeV = StateSpaceNode('V', [19, 10])
    nodeW = StateSpaceNode('W', [18, 19])
    nodeX = StateSpaceNode('X', [0, 0])
    nodeY = StateSpaceNode('Y', [0, 1])
    nodeZ = StateSpaceNode('Z', [10, 2])
    nodea = StateSpaceNode('a', [13, 7])
    nodeb = StateSpaceNode('b', [9, 1])
    noded = StateSpaceNode('d', [4, 3])
    nodee = StateSpaceNode('e', [9, 7])
    nodef = StateSpaceNode('f', [18, 1])
    nodeg = StateSpaceNode('g', [19, 0], True)
    nodeh = StateSpaceNode('h', [9, 11])
    nodem = StateSpaceNode('m', [8, 18])
    noden = StateSpaceNode('n', [8, 12])
    nodeq = StateSpaceNode('q', [9, 19])
    noder = StateSpaceNode('r', [16, 17])
    nodes = StateSpaceNode('s', [15, 19])
    nodet = StateSpaceNode('t', [19, 2])

    nodeA.connect(nodeB)
    nodeA.connect(nodeK)
    nodeB.connect(nodeC)
    nodeK.connect(nodeD)
    nodeK.connect(nodeJ)
    nodeC.connect(nodeb)
    nodeC.connect(nodeD)
    nodeD.connect(noded)
    nodeJ.connect(noden)
    nodeJ.connect(nodeM)
    nodeJ.connect(nodeL)
    nodeb.connect(nodeY)
    nodeb.connect(nodef)
    noded.connect(nodeF)
    noded.connect(nodeE)
    noden.connect(nodeE)
    noden.connect(nodeN)
    noden.connect(nodeM)
    nodeM.connect(nodeQ)
    nodeL.connect(nodem)
    nodeL.connect(nodeq)
    nodeY.connect(nodeX)
    nodef.connect(nodet)
    nodef.connect(nodeg)
    nodeE.connect(nodeH)
    nodeN.connect(nodee)
    nodeN.connect(nodeQ)
    nodem.connect(nodeP)
    nodeq.connect(nodes)
    nodeX.connect(nodeg)
    nodet.connect(nodeZ)
    nodeH.connect(nodeO)
    nodeH.connect(nodee)
    nodee.connect(nodeh)
    nodeP.connect(noder)
    nodeP.connect(nodeS)
    nodeO.connect(nodeT)
    nodeO.connect(nodea)
    noder.connect(nodea)
    nodeS.connect(nodeU)
    nodea.connect(nodeR)
    nodeT.connect(nodeV)
    nodeT.connect(nodeU)
    nodeU.connect(nodeG)
    nodeU.connect(nodeW)

    nodes  = [nodeA, nodeB, nodeC, nodeD, nodeE, nodeF, nodeG, nodeH, nodeJ, nodeK, nodeL, nodeM, nodeN, nodeO, nodeP, nodeQ, nodeR, nodeS, nodeT, nodeU, nodeV, nodeW, nodeX, nodeY, nodeZ, nodea, nodeb, noded, nodee, nodef, nodeg, nodeh, nodem, noden, nodeq, noder, nodes, nodet]
    for node in nodes: node.coords.reverse()

    return nodes

def mazeSMITH():
    verticalHall = [2, 1, 2, 1]
    horizontalHall = [1, 2, 1, 2]
    structure = [ 
        [[1, 2, 2, 1], [1, 1, 2, 2]  , [1, 2, 2, 1]  , horizontalHall, horizontalHall, [3, 1, 1, 2]  , [1, 1, 2, 1]  , [1, 1, 2, 1]],
        [verticalHall, verticalHall  , [2, 2, 1, 1]  , [1, 1, 2, 2]  , [1, 1, 2, 1]  , [1, 2, 2, 1]  , [2, 1, 2, 2]  , verticalHall],
        [verticalHall, [2, 2, 1, 1]  , horizontalHall, [2, 2, 2, 2,] , [2, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall],
        [[2, 2, 1, 1], horizontalHall, [1, 1, 2, 2]  , verticalHall  , verticalHall  , verticalHall  , verticalHall  , verticalHall],
        [[1, 2, 1, 3], [1, 2, 2, 2]  , [2, 1, 1, 2]  , [2, 1, 1, 1]  , [2, 2, 1, 1]  , [2, 1, 1, 2]  , verticalHall  , verticalHall],
        [[1, 2, 2, 1], [2, 2, 1, 2]  , horizontalHall, horizontalHall, horizontalHall, [1, 1, 1, 2]  , [2, 2, 2, 1]  , [2, 1, 1, 2]],
        [[2, 2, 2, 1], horizontalHall, horizontalHall, [1, 2, 2, 2]  , horizontalHall, [1, 1, 2, 2]  , [2, 2, 1, 1]  , [1, 1, 2, 2]],
        [[2, 2, 1, 1], horizontalHall, [1, 1, 1, 2]  , [2, 1, 1, 1]  , [1, 2, 1, 1]  , [2, 2, 1, 2]  , horizontalHall, [2, 1, 1, 2]]
        ]
    return structure      

def nodesSMITH():
    nodeA = StateSpaceNode('A', [3, 7])
    nodeB = StateSpaceNode('B', [1, 4])
    nodeC = StateSpaceNode('C', [3, 2])
    nodeD = StateSpaceNode('D', [3, 4])
    nodeE = StateSpaceNode('E', [4, 2])
    nodeF = StateSpaceNode('F', [4, 1])
    nodeG = StateSpaceNode('G', [5, 0], True)
    nodeH = StateSpaceNode('H', [4,4])
    nodeJ = StateSpaceNode('J', [6,5])
    nodeK = StateSpaceNode('K', [7,0])
    nodeL = StateSpaceNode('L', [4, 7])
    nodeM = StateSpaceNode('M', [3, 6])
    nodeN = StateSpaceNode('N', [0, 6])
    nodeO = StateSpaceNode('O', [5, 5])
    nodeP = StateSpaceNode('P', [2, 7])
    nodeQ = StateSpaceNode('Q', [6, 0])
    nodeg = StateSpaceNode('g', [0, 4], True)

    nodeA.connect(nodeM)
    nodeM.connect(nodeN)
    nodeM.connect(nodeL)
    nodeM.connect(nodeJ)
    nodeN.connect(nodeB)
    nodeN.connect(nodeO)
    nodeN.connect(nodeP)
    nodeJ.connect(nodeK)
    nodeJ.connect(nodeQ)
    nodeJ.connect(nodeH)
    nodeB.connect(nodeg)
    nodeB.connect(nodeC)
    nodeH.connect(nodeE)
    nodeC.connect(nodeG)
    nodeC.connect(nodeD)
    nodeC.connect(nodeE)
    nodeE.connect(nodeF)

    nodes = [nodeA, nodeB, nodeC, nodeD, nodeE, nodeF, nodeG, nodeH, nodeJ, nodeK, nodeL, nodeM, nodeN, nodeO, nodeP, nodeQ, nodeg]
    for node in nodes: node.coords.reverse()
    return nodes

class MazeCanvas:
    def __init__(self, NGmaze, NGnodes, SMITHmaze, SMITHnodes):
        window = Tk()
        window.title('Maze Solver')

        self.maze = NGmaze
        self.nodes = NGnodes

        self.NGmaze = NGmaze
        self.NGnodes = NGnodes

        self.SMITHmaze = SMITHmaze
        self.SMITHnodes = SMITHnodes

        self.tileScale = 20
        self.buffer = 4

        self.mazeCanvas  = Canvas(window, width = 410, height = 410)
        self.treeCanvas = Canvas(window, width = 1210, height = 610)
        self.controls = Frame(window, width = 200, height = 200)

        self.mazeCanvas.grid(row = 0, column = 0)
        self.treeCanvas.grid(row = 0, column = 1)
        self.controls.grid(row = 1)

        bNewMaze = Button(self.controls, text = "Run Maze", command = self.newMaze)
        bNewMaze.grid(row = 0, column = 1)

        self.algoChoice = IntVar()
        self.algoChoice.set(1)

        Radiobutton(self.controls, text = "Uniform-Cost", variable = self.algoChoice, value = 1).grid(column = 1)
        Radiobutton(self.controls, text = "Depth Limited", variable = self.algoChoice, value = 2).grid(column = 1)
        Radiobutton(self.controls, text = "Breadth First", variable = self.algoChoice, value = 3).grid(column = 1)

        self.depth = IntVar()
        self.depth.set(5)

        depthSlider = Scale(self.controls, from_ = 0, to_ = 10, variable = self.depth, orient = HORIZONTAL)
        depthSlider.grid(row = 2, column = 2)

        self.mazeChoice = IntVar()
        self.mazeChoice.set(1)

        Radiobutton(self.controls, text = "Ng's Maze", variable = self.mazeChoice, value = 1).grid(row = 1, column = 0)
        Radiobutton(self.controls, text = "Smith's Maze", variable = self.mazeChoice, value = 2).grid(row = 2, column = 0)

        window.mainloop()



    #### MAZE DEFINITIONS ###
    def drawTile(self, walls, coords):
        tileScale = self.tileScale
        buffer = self.buffer
        widthWeight = 2
        dashPattern = (2,1)

        upperLeftX = coords[0] * tileScale + buffer
        upperLeftY = coords[1] * tileScale + buffer

        upperRightX = coords[0] * tileScale + tileScale + buffer
        upperRightY = coords[1] * tileScale + buffer

        lowerLeftX = coords[0] * tileScale + buffer
        lowerLeftY = coords[1] * tileScale + tileScale + buffer

        lowerRightX = coords[0] * tileScale + tileScale + buffer
        lowerRightY = coords[1] * tileScale + tileScale + buffer

        #top
        if walls[0] == 1:
            self.mazeCanvas.create_line(upperLeftX, upperLeftY, upperRightX, upperRightY, width = widthWeight)
        elif walls[0] == 2:
            self.mazeCanvas.create_line(upperLeftX, upperLeftY, upperRightX, upperRightY, dash = dashPattern)
        #right
        if walls[1] == 1:
            self.mazeCanvas.create_line(upperRightX, upperRightY, lowerRightX, lowerRightY, width = widthWeight)
        elif walls[1] == 2:
            self.mazeCanvas.create_line(upperRightX, upperRightY, lowerRightX, lowerRightY, dash = dashPattern)
        #bot
        if walls[2] == 1:
            self.mazeCanvas.create_line(lowerLeftX, lowerLeftY, lowerRightX, lowerRightY, width = widthWeight)
        elif walls[2] == 2:
            self.mazeCanvas.create_line(lowerLeftX, lowerLeftY, lowerRightX, lowerRightY, dash = dashPattern)
        #left
        if walls[3] == 1:
            self.mazeCanvas.create_line(upperLeftX, upperLeftY, lowerLeftX, lowerLeftY, width = widthWeight)
        elif walls[3] == 2:
            self.mazeCanvas.create_line(upperLeftX, upperLeftY, lowerLeftX, lowerLeftY, dash = dashPattern)

    def drawNode(self, node):
        letter = node.letter
        coordScale = self.tileScale
        buffer = self.buffer + self.tileScale/2

        y = node.coords[0] * coordScale + buffer 
        x = node.coords[1] * coordScale + buffer

        self.mazeCanvas.create_text(x, y, text = letter)

    def displayMaze(self, maze):
        rowNum = 0
        colNum = 0
        for row in maze:
            for collumn in row:
                coords = [colNum, rowNum]
                walls = maze[rowNum][colNum]
                self.drawTile(walls, coords)
                
                colNum += 1
            rowNum +=1
            colNum = 0

    def displayNodes(self, nodes, level = 0):
        for node in nodes:
            self.drawNode(node)


    #### TREE DEFINITIONS ###
    def drawTreeNode(self, letter, number, level, isGoal = False):
        center = self.treeCanvas.winfo_reqwidth()/2

        newText = letter + ', ' + str(number)
        radius = 15
        buffer = self.buffer + radius
        shiftValue = radius * 2
        levelTag = "Level" + str(level)

        self.treeCanvas.move(levelTag, -shiftValue, 0)
        selfShift = shiftValue * (len(self.treeCanvas.find_withtag(levelTag)) / 2)

        x1 = center - radius + selfShift
        x2 = center + radius + selfShift
        y1 = (level * 2) * (radius * 2) - radius + buffer
        y2 = (level * 2) * (radius * 2) + radius + buffer

        #print(self.treeCanvas.find_enclosed(x1, y1, x2, y2))

        thisFill = ""
        if(isGoal): thisFill = "Green"
        id = self.treeCanvas.create_oval(x1, y1, x2, y2, tag = levelTag, fill = thisFill)
        self.treeCanvas.create_text(x1 + radius, y1 + radius, text = newText, tag = levelTag)
        
        return  id

    def drawConnection(self, connectionList):
        parentID = connectionList[0]
        
        for childID in connectionList:
            if type(childID) is list:

                parentCoords = self.treeCanvas.coords(parentID)
            
                childCoords = self.treeCanvas.coords(childID[0])
                r = 15

                self.treeCanvas.create_line(parentCoords[0] + r, parentCoords[1] + 2 * r, childCoords[0] + r, childCoords[1])

                if len(childID) > 1:
                    self.drawConnection(childID)

        return

    def displayTree(self, successPath, level = 0):
        ownID = [self.drawTreeNode(successPath[0], successPath[1], level, successPath[2])]
        for node in successPath:
            if type(node) == list:
                newID = self.displayTree(node, level + 1)
                ownID.append(newID)

        return ownID

    ### Button Definitions ###
    def newMaze(self):
        self.mazeCanvas.delete(ALL)
        self.treeCanvas.delete(ALL)

        mazeSelect = self.mazeChoice.get()
        if mazeSelect == 1:
            self.maze = self.NGmaze
            self.nodes = self.NGnodes
        else:
            self.maze = self.SMITHmaze
            self.nodes = self.SMITHnodes

        algoSelect = self.algoChoice.get()
        successPath = None

        if algoSelect == 1:
            successPath = ucsSearch(self.nodes, self.maze)
            pathList = self.displayTree(successPath.getTree())
            self.drawConnection(pathList)
        elif algoSelect == 2:
            depth = self.depth.get()
            successPath = dlsSearch(self.nodes, self.maze, depth)
            pathList = self.displayTree(successPath.getTree())
            self.drawConnection(pathList)

            
        else:
            successPath = bfsSearch(self.nodes, self.maze)
            pathList = self.displayTree(successPath.getTree())
            self.drawConnection(pathList)
            
        self.displayMaze(self.maze)
        self.displayNodes(self.nodes)
        


def main():
    maze1 = mazeNG()
    nodes1 = nodesNG()

    maze2 = mazeSMITH()
    nodes2 = nodesSMITH()

    MazeCanvas(maze1, nodes1, maze2, nodes2)

main()