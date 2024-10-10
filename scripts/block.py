from time import time
from random import randint

from scripts.tools import getSRS

class Block():
    SRS, wkA, wkB=getSRS()

    def __init__(self, type, rot=0, X=3, Y=-3):
        self.type=type
        self.rot=rot
        self.X, self.Y=X, Y
        self.gravTic, self.moveTic, self.rotTic=time(), time(), time()
        self.tSpin=False

    def gravity(self, gridR):
        collide=False
        for i in range(4 if self.type==1 else 3):
            for j in range(4 if self.type==1 else 3):
                if Block.SRS[self.type][self.rot][j][i]=='1' and self.Y+j+1>=0:
                    if self.Y+j+1==20:collide=True
                    elif gridR[self.Y+j+1][self.X+i]!=0:collide=True
        
        self.gravTic=time()

        if not collide:self.Y+=1; self.tSpin=False
        return collide

    def hardDrop(self, gridR, preview=False):
        collide=True; drop=0

        while collide:
            drop+=1

            for i in range(4 if self.type==1 else 3):
                for j in range(4 if self.type==1 else 3):
                    if Block.SRS[self.type][self.rot][j][i]=='1' and self.Y+j+drop>=0:
                        if self.Y+j+drop==20:collide=False
                        elif gridR[self.Y+j+drop][self.X+i]!=0:collide=False

        if preview:
            return self.Y+drop-1
        else:
            self.Y+=drop-1; self.gravTic-=1/2

    def move(self, gridR, dir):
        collide=False
        for i in range(len(Block.SRS[self.type][0])):
            for j in range(len(Block.SRS[self.type][0])):
                if Block.SRS[self.type][self.rot][j][i]=='1':
                    if self.X+i+dir<0 or self.X+i+dir>9:collide=True
                    elif gridR[self.Y+j][self.X+i+dir]!=0 and self.Y+j>=0:collide=True
        
        self.moveTic=time()

        if not collide:self.X+=dir

    def rotate(self, gridR, dir):
        newRot=self.rot+dir
        if newRot<0:newRot=3
        if newRot>3:newRot=0

        collide=True; var=-1; wk=(Block.wkB if self.type==1 else Block.wkA)
        while collide and var<4:

            collide=False; var+=1
            for i in range(4 if self.type==1 else 3):
                for j in range(4 if self.type==1 else 3):
                    if Block.SRS[self.type][newRot][j][i]=='1':
                        if (self.X+i+wk[0 if dir==1 else 1][self.rot][var][0]<0 or self.X+i+wk[0 if dir==1 else 1][self.rot][var][0]>9 
                            or self.Y+j+wk[0 if dir==1 else 1][self.rot][var][1]>19):
                            collide=True
                        elif self.Y+j+wk[0 if dir==1 else 1][self.rot][var][1]>=0:
                            if gridR[self.Y+j+wk[0 if dir==1 else 1][self.rot][var][1]][self.X+i+wk[0 if dir==1 else 1][self.rot][var][0]]!=0:
                                collide=True
        
        self.rotTic=time()  

        if not collide:
            if self.type==6:self.tSpin=True
            self.rot=newRot
            self.X+=wk[0 if dir==1 else 1][self.rot][var][0]
            self.Y-=wk[0 if dir==1 else 1][self.rot][var][1]

    def isOut(self):
        out=False
        for i in range(4 if self.type==1 else 3):
            for j in range(4 if self.type==1 else 3):
                if Block.SRS[self.type][self.rot][j][i]=='1':
                    if self.Y+j<0:out=True

        return out
    
    def correctHeight(self, gridR):
        notResolved=True; n=0

        while notResolved:
            collide=False
            for i in range(len(Block.SRS[self.type][0])):
                for j in range(len(Block.SRS[self.type][0])):
                    if Block.SRS[self.type][self.rot][j][i]=='1' and self.Y+j-n>=0:
                        if gridR[self.Y+j-n][self.X+i]!=0:collide=True
            if collide:n+=1
            else:notResolved=False

        self.Y-=n

    def renderBlock(self, gridR):
        shadowDrop=self.hardDrop(gridR, True)

        for i in range(4 if self.type==1 else 3):
            for j in range(4 if self.type==1 else 3):
                if Block.SRS[self.type][self.rot][j][i]=='1':
                    if shadowDrop+j>=0:gridR[shadowDrop+j][self.X+i]=9
                    if self.Y+j>=0:gridR[self.Y+j][self.X+i]=self.type

        return gridR.copy()
    
    def carveGarbage(self, garbage, garbageY):
        for i in range(4 if self.type==1 else 3):
            for j in range(4 if self.type==1 else 3):
                if Block.SRS[self.type][self.rot][j][i]=='1':
                    if self.Y+j>=garbageY and self.Y+j<garbageY+len(garbage):
                        garbage[self.Y+j-garbageY][self.X+i]=0
        rand=randint(0,9)
        for i in garbage:
            if i.count(0)==0:i[rand]=0 

        return garbage