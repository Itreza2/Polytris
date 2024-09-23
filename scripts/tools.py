from PIL import Image, ImageTk
from os import path, getcwd
from csv import reader

def getSRS():
    SRS={}; wallKickA=[{},{}]; wallKickB=[{},{}]

    for i in range(7):
        SRS[i+1]=[[]]
        with open(path.join (getcwd(), 'SRS/'+str(i)+'.txt'),'r') as file:
            for line in file:
                if line[0]=='n':
                    SRS[i+1].append([])
                else:
                    SRS[i+1][-1].append(list(line.replace('\n','')))

    with open('SRS/wallKickA.csv', newline='') as file:
        lecteur=reader(file, delimiter=','); n=0
        for line in lecteur:
            if n<=3:wallKickA[0][n]=(line)
            else:wallKickA[1][n-4]=(line)
            n+=1

    with open('SRS/wallKickB.csv', newline='') as file:
        lecteur=reader(file, delimiter=','); n=0
        for line in lecteur:
            if n<=3:wallKickB[0][n]=(line)
            else:wallKickB[1][n-4]=(line)
            n+=1

    for i in range(4):
        newList=[[]]
        for j in wallKickA[0][i]:
            newList[-1].append(int(j))
            if len(newList[-1])==2 and len(newList)<5:newList.append([])
        wallKickA[0][i]=newList
        newList=[[]]
        for j in wallKickA[1][i]:
            newList[-1].append(int(j))
            if len(newList[-1])==2 and len(newList)<5:newList.append([])
        wallKickA[1][i]=newList
        newList=[[]]
        for j in wallKickB[0][i]:
            newList[-1].append(int(j))
            if len(newList[-1])==2 and len(newList)<5:newList.append([])
        wallKickB[0][i]=newList
        newList=[[]]
        for j in wallKickB[1][i]:
            newList[-1].append(int(j))
            if len(newList[-1])==2 and len(newList)<5:newList.append([])
        wallKickB[1][i]=newList

    return SRS, wallKickA, wallKickB

def createImage(gridR, screenHeight, color):
    img=Image.new(mode='RGB', size=(10,20)); imgData=[]

    for j in range(20):
        for i in range(10):
            imgData.append(color[gridR[j][i]])

    img.putdata(imgData)

    return ImageTk.PhotoImage(img.resize((int(screenHeight/25)*10, int(screenHeight/25)*20), 4))

def decoupe(path, lenght, screenHeight):
    im=Image.open(path)

    return  [ImageTk.PhotoImage(
        im.crop((i*(im.size[0]/lenght),0,(i+1)*(im.size[0]/lenght),im.size[1])
                ).resize((int(screenHeight/25)*3,int(screenHeight/25)*3),4)) 
        for i in range(lenght)]

def toHex(n):
    alphabet=('0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f')
    
    return(alphabet[int(n/16)]+alphabet[n%16])