from time import time
from random import shuffle, randint

from scripts.block import Block

class Joueur:

    def __init__(self, inputIdx):
        
        self.game=False; self.score=0
        self.grid=[[0 for i in range(10)] for j in range(20)]
        self.hdTic=time()
        self.queue=[i for i in range(1,8)]; shuffle(self.queue); self.hold=[0, True]
        self.currentBlock=Block(self.queue[0])
        self.inputIdx=inputIdx
        self.btb=False
        self.animIdx=[]
        self.updateQueue()

    def gamePlay(self, inputs, gravCD, moveCD, rotCD, hdCD):
        garbage=[]

        if self.hold[1] and inputs[self.inputIdx][5]:
            if self.hold[0]==0:
                self.hold=[self.currentBlock.type,False]; self.currentBlock=Block(self.queue[0])
                self.updateQueue()
            else:var=self.hold[0]; self.hold=[self.currentBlock.type,False]; self.currentBlock=Block(var)

        if (time()-self.hdTic)>hdCD:
            if inputs[self.inputIdx][4]:self.currentBlock.hardDrop(self.grid); self.hdTic=time()

        if (time()-self.currentBlock.moveTic)>moveCD:
            if inputs[self.inputIdx][0] and not inputs[self.inputIdx][1]:self.currentBlock.move(self.grid, 1)
            if inputs[self.inputIdx][1] and not inputs[self.inputIdx][0]:self.currentBlock.move(self.grid, -1)
                
        if (time()-self.currentBlock.rotTic)>rotCD:        
            if inputs[self.inputIdx][2]:self.currentBlock.rotate(self.grid, 1); inputs[self.inputIdx][2]=False
            if inputs[self.inputIdx][7]:self.currentBlock.rotate(self.grid, -1); inputs[self.inputIdx][2]=False

        if (time()-self.currentBlock.gravTic)>gravCD[1 if inputs[self.inputIdx][3] else 0]:
            if self.currentBlock.gravity(self.grid):

                if self.currentBlock.isOut():
                    self.game=False
                else:
                    self.grid=self.currentBlock.renderBlock(self.grid)
                    garbageY,garbage=self.delLine()
                    if len(garbage)>0:garbage=self.currentBlock.carveGarbage(garbage, garbageY)
                    self.currentBlock=Block(self.queue[0])
                    self.updateQueue()
                    self.hold[1]=True

        if len(self.animIdx)>0:self.finishAnim()

        return garbage

    def updateQueue(self):
        self.queue.pop(0)
        while(len(self.queue)<7):
            rand=randint(1,7)
            if not self.queue.count(rand)>1:self.queue.append(rand)

    def delLine(self):
        n=0; garbageY=None; cTime=time()

        for j in range(20):
            if 0 not in self.grid[j] and 9 not in self.grid[j]:
                self.score+=1; n+=1
                self.grid[j]=[9 for i in range(10)]
                self.animIdx.append([j,cTime])
                if garbageY==None:garbageY=j 

        if n!=0:
            if n==1:n=0; self.btb=False
            elif n==2:n=1; self.btb=False
            elif n==3:n=2; self.btb=False
            elif self.btb:n=6
            else:self.btb=True

            if self.currentBlock.tSpin:n=(n+1)*2

            if self.grid[19]==[0 for i in range(10)]:n+=10; print('TspinOk')

        return garbageY,[[8 for i in range(10)] for j in range(n)]
    
    def finishAnim(self):
        #Bloc moche mais fonctionnel ?
        compt=0; finAnim=[]
        for j in range(len(self.animIdx)):
            if (time()-self.animIdx[j][1])>0.3 or not self.game:
                self.grid[self.animIdx[j][0]]=None
                finAnim.append(j)
        for j in finAnim:
            self.animIdx.pop(j-compt); compt+=1
        for j in range(20):
            if self.grid[j]==None:
                self.grid.pop(j)
                self.grid.insert(0, [0 for i in range(10)])   

    def addLine(self, garbage):
        for i in garbage:
            self.grid.pop(0)
            self.grid.append(i)
        self.currentBlock.correctHeight(self.grid)