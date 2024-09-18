from tkinter import Tk, Canvas
from time import time
from copy import deepcopy
from PIL import Image, ImageTk

from scripts.tools import createImage, decoupe
from scripts.joueur import Joueur
from scripts.affichage import affichage

tk=Tk()
tk.title('Polytris')
tk.geometry('1000x600')
tk.minsize(width=200, height=180)
tk.attributes('-fullscreen', False)

color=((0,0,0),(0,255,255),(51,51,255),(255,153,51),(255,255,51),(0,204,0),(153,51,255),(204,0,0),(125,125,125),(64,64,64))
gravCD=[50/100, 4/100]; moveCD=12/100; rotCD=12/100; hdCD=30/100

screenDim=[1000,600]

fpsLimiter=time()
inputs=[[False for i in range(10)] for j in range(2)]
imgGrid=[None, None]
imgBlocks=decoupe('sprites/blocksPreview.png',8,screenDim[1])
animMenu=ImageTk.PhotoImage(Image.open('sprites/fallingBlocks.png').resize((int(screenDim[1]/25*20),screenDim[1]),4))

menuIdx=[0, time()]; state='Menu'

joueur1=Joueur(0); joueur2=Joueur(1); timer=time()

def resizeSprites():
    global imgBlocks, animMenu

    imgBlocks=decoupe('sprites/blocksPreview.png',8,screenDim[1])
    animMenu=ImageTk.PhotoImage(Image.open('sprites/fallingBlocks.png').resize((int(screenDim[1]/25*20),screenDim[1]),4))

def newGame(mode):
    global state, joueur1, joueur2, timer

    if mode=='Solo':
        joueur1=Joueur(0)
        timer=[time(),None]
        state='Solo'

    if mode=='Versus':
        joueur1, joueur2=Joueur(0), Joueur(1)
        timer=[time(),None]
        state='Versus'

def main():
    global fpsLimiter, inputs, imgGrid, timer, menuIdx, state, screenDim

    cSize=['','']; wGeometry=tk.winfo_geometry(); idx=[0,0]
    while wGeometry[idx[0]]!='+':
        if wGeometry[idx[0]]=='x':idx[1]+=1
        else:cSize[idx[1]]+=wGeometry[idx[0]]
        idx[0]+=1
    cSize=[int(cSize[0]), int(cSize[1])]
    if cSize!=screenDim and cSize!=[1,1]:
        screenDim=cSize
        can.config(width=screenDim[0], height=screenDim[1])
        resizeSprites()

    if state=='Menu':
        if (time()-menuIdx[1])>1/6:
            if inputs[0][8]:menuIdx[0]=(menuIdx[0]-1 if menuIdx[0]>0 else 2); menuIdx[1]=time()
            if inputs[0][3]:menuIdx[0]=(menuIdx[0]+1 if menuIdx[0]<2 else 0); menuIdx[1]=time()
            if inputs[0][2]:
                if menuIdx[0]==0:newGame('Solo')
                elif menuIdx[0]==1:newGame('Versus')
                else:tk.destroy()
            if inputs[0][9]:tk.destroy()

    if state=='Solo':
        if joueur1.game:
            joueur1.gamePlay(inputs, gravCD, moveCD, rotCD, hdCD)
            
            if joueur1.score>=40:joueur1.game=False; timer[1]=time()
        
        elif  (time()-timer[0])<4 and (time()-timer[0])>3:joueur1.game=True

        if inputs[0][6]:newGame('Solo'); inputs[0][6]=False
        if inputs[0][9]:state='Menu'; menuIdx[1]=time()

    if state=='Versus':
        if not joueur1.game and not joueur2.game and (time()-timer[0])>3:
            joueur1.game, joueur2.game=True, True
            
        if joueur1.game and joueur2.game:
            garbage2=joueur1.gamePlay(inputs, gravCD, moveCD, rotCD, hdCD)
            garbage1=joueur2.gamePlay(inputs, gravCD, moveCD, rotCD, hdCD)  

            joueur1.addLine(garbage1)
            joueur2.addLine(garbage2)

        if inputs[0][6]:newGame('Versus'); inputs[0][6]=False
        if inputs[0][9]:state='Menu'; menuIdx[1]=time()

    if (time()-fpsLimiter)>1/60:
        imgGrid[0]=createImage(joueur1.currentBlock.renderBlock(deepcopy(joueur1.grid)), screenDim[1], color)
        imgGrid[1]=createImage(joueur2.currentBlock.renderBlock(deepcopy(joueur2.grid)), screenDim[1], color)
        fpsLimiter=time(); affichage(can, screenDim, imgBlocks, imgGrid, joueur1, joueur2, timer, state, menuIdx, animMenu)

    tk.after(1, main)

def clavier(event):
    global inputs
    var=event.keysym

    if var=='Right':inputs[0][0]=True
    elif var=='Left':inputs[0][1]=True
    elif var=='y':inputs[0][2]=True
    elif var=='Down':inputs[0][3]=True
    elif var=='t':inputs[0][4]=True
    elif var=='f':inputs[0][5]=True
    elif var=='h':inputs[0][6]=True
    elif var=='r':inputs[0][7]=True
    elif var=='Up':inputs[0][8]=True

    if var=='d':inputs[1][0]=True
    elif var=='q':inputs[1][1]=True
    elif var=='o':inputs[1][2]=True
    elif var=='s':inputs[1][3]=True
    elif var=='i':inputs[1][4]=True
    elif var=='j':inputs[1][5]=True
    elif var=='u':inputs[1][7]=True

    elif var=='Escape':inputs[0][9]=True

def clavierRelease(event):
    global inputs
    var=event.keysym

    if var=='Right':inputs[0][0]=False
    elif var=='Left':inputs[0][1]=False
    elif var=='y':inputs[0][2]=False
    elif var=='Down':inputs[0][3]=False
    elif var=='t':inputs[0][4]=False
    elif var=='f':inputs[0][5]=False
    elif var=='h':inputs[0][6]=False
    elif var=='r':inputs[0][7]=False
    elif var=='Up':inputs[0][8]=False

    if var=='d':inputs[1][0]=False
    elif var=='q':inputs[1][1]=False
    elif var=='o':inputs[1][2]=False
    elif var=='s':inputs[1][3]=False
    elif var=='i':inputs[1][4]=False
    elif var=='j':inputs[1][5]=False
    elif var=='u':inputs[1][7]=False

    elif var=='Escape':inputs[0][9]=False

can=Canvas(tk, width=screenDim[0], height=screenDim[1], bg='black')
can.focus_set()
can.bind("<Key>", clavier); can.bind("<KeyRelease>", clavierRelease)
can.pack()

main()

tk.mainloop()