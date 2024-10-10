from time import time

def affichage(can, screenDim, imgBlocks, imgGrid, joueur1, joueur2, timer, state, menuIdx, animMenu):
    can.delete('all')

    if state=='Menu':
        can.create_text(screenDim[0]/2, screenDim[1]/2-int(screenDim[1]/25)*7,
                        text='POLYTRIS', anchor='center',
                        fill=('gainsboro'), font=('Arial', int(screenDim[1]/15), 'bold'))
        can.create_text(screenDim[0]/2, screenDim[1]/2-int(screenDim[1]/25)*2.5,
                        text='Marathon', anchor='center',
                        fill=('yellow' if menuIdx[0]==0 else 'white'), font=('Arial', int(screenDim[1]/30*(1.5 if menuIdx[0]==0 else 1)), 'bold'))
        can.create_text(screenDim[0]/2, screenDim[1]/2+int(screenDim[1]/25),
                        text='Sprint 40L', anchor='center',
                        fill=('yellow' if menuIdx[0]==1 else 'white'), font=('Arial', int(screenDim[1]/30*(1.5 if menuIdx[0]==1 else 1)), 'bold'))
        can.create_text(screenDim[0]/2, screenDim[1]/2+int(screenDim[1]/25)*4.5,
                        text='Versus JcJ', anchor='center',
                        fill=('yellow' if menuIdx[0]==2 else 'white'), font=('Arial', int(screenDim[1]/30*(1.5 if menuIdx[0]==2 else 1)), 'bold'))
        can.create_text(screenDim[0]/2, screenDim[1]/2+int(screenDim[1]/25)*8,
                        text='Quitter', anchor='center',
                        fill=('yellow' if menuIdx[0]==3 else 'white'), font=('Arial', int(screenDim[1]/30*(1.5 if menuIdx[0]==3 else 1)), 'bold'))

    if state[:4]=='Solo':
        can.create_image(screenDim[0]/2, screenDim[1]/2, image=imgGrid[0], anchor='center')

        for i in joueur1.animIdx:
            can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*5, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*(i[0]+1),
                                screenDim[0]/2+int(screenDim[1]/25)*5, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*i[0], width='0', fill='black')
            can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*5*(1-min(((time()-i[1])/0.3),1)), screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*(i[0]+1),
                                screenDim[0]/2+int(screenDim[1]/25)*5*(1-min(((time()-i[1])/0.3),1)), screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*i[0], width='0', fill='white')

        can.create_rectangle(screenDim[0]/2+int(screenDim[1]/25)*5, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2+int(screenDim[1]/25)*7.5, screenDim[1]/2-int(screenDim[1]/25)*2.5, width='5', outline='white', fill='black')
        can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*5, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2-int(screenDim[1]/25)*7.5, screenDim[1]/2-int(screenDim[1]/25)*7.5, width='5', outline='white', fill='black')
        can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*5, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2+int(screenDim[1]/25)*5, screenDim[1]/2+int(screenDim[1]/25)*10, width='5', outline='white')

        for i in range(3):
            can.create_image(screenDim[0]/2+int(screenDim[1]/25)*6.25, screenDim[1]/2-int(screenDim[1]/25)*(8.75-2.5*i),
                            image=imgBlocks[joueur1.queue[i]], anchor='center')
        can.create_image(screenDim[0]/2-int(screenDim[1]/25)*6.25, screenDim[1]/2-int(screenDim[1]/25)*8.75,
                        image=imgBlocks[joueur1.hold[0]], anchor='center')

        if state[4:]=='Sprint':
            can.create_text(screenDim[0]/2-int(screenDim[1]/25)*6.25, screenDim[1]/2-int(screenDim[1]/25)*6.25,
                            text=str(joueur1.score)+'/40', anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/40), 'bold'))
        elif state[4:]=='Marath':
            can.create_text(screenDim[0]/2-int(screenDim[1]/25)*6.25, screenDim[1]/2-int(screenDim[1]/25)*6.25,
                            text='lvl '+str(joueur1.level[0]), anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/40), 'bold'))   

        if joueur1.game:
            if state[4:]=='Sprint':
                can.create_text(screenDim[0]/2-int(screenDim[1]/25)*6.25, screenDim[1]/2-int(screenDim[1]/25)*4.75,
                                text=str(int((time()-timer[0]-3)/60))+':'+('0' if int(time()-timer[0]-3)%60<10 else '')+str(int(time()-timer[0]-3)%60), 
                                anchor='center',fill='white', font=('Arial', int(screenDim[1]/40)))
            elif state[4:]=='Marath':
                can.create_text(screenDim[0]/2-int(screenDim[1]/25)*5.5, screenDim[1]/2-int(screenDim[1]/25)*4.75,
                                text=str(joueur1.points)+'pts', 
                                anchor='e',fill='white', font=('Arial', int(screenDim[1]/40)))
        else:
            if int(4-(time()-timer[0]))>0:
                can.create_text(screenDim[0]/2, screenDim[1]/2-int(screenDim[1]/25),
                                text=str(int(4-(time()-timer[0]))), anchor='center',
                                fill='white', font=('Arial', int(screenDim[1]/10), 'bold'))   
            elif joueur1.score>=40 and state[4:]=='Sprint':
                can.create_text(screenDim[0]/2, screenDim[1]/2-int(screenDim[1]/25)*1.25,
                                text='Terminé', anchor='center',
                                fill='white', font=('Arial', int(screenDim[1]/15), 'bold'))
                can.create_text(screenDim[0]/2, screenDim[1]/2+int(screenDim[1]/25)*1.25,
                                text=str(int((timer[1]-timer[0]-3)/60))+':'+('0' if int(timer[1]-timer[0]-3)%60<10 else '')+str(int(timer[1]-timer[0]-3)%60), 
                                anchor='center', fill='white', font=('Arial', int(screenDim[1]/15)))
            elif (time()-timer[0])>4:
                can.create_text(screenDim[0]/2, screenDim[1]/2-int(screenDim[1]/25)*(1.25 if state[4:]=='Marath' else 1),
                                text='Hors Jeu', anchor='center',
                                fill='white', font=('Arial', int(screenDim[1]/15), 'bold'))
                if state[4:]=='Marath':
                    can.create_text(screenDim[0]/2, screenDim[1]/2+int(screenDim[1]/25)*1.25,
                                    text=str(joueur1.points)+'pts', 
                                    anchor='center', fill='white', font=('Arial', int(screenDim[1]/18)))              

    if state=='Versus':
        can.create_image(screenDim[0]/2-int(screenDim[1]/25)*8, screenDim[1]/2, image=imgGrid[0], anchor='center')
        can.create_image(screenDim[0]/2+int(screenDim[1]/25)*8, screenDim[1]/2, image=imgGrid[1], anchor='center')

        for i in joueur1.animIdx:
            can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*13, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*(i[0]+1),
                                screenDim[0]/2-int(screenDim[1]/25)*3, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*i[0], width='0', fill='black')
            can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*5*(1-min(((time()-i[1])/0.3),1))-int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*(i[0]+1),
                                screenDim[0]/2+int(screenDim[1]/25)*5*(1-min(((time()-i[1])/0.3),1))-int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*i[0], width='0', fill='white')
        for i in joueur2.animIdx:
            can.create_rectangle(screenDim[0]/2+int(screenDim[1]/25)*3, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*(i[0]+1),
                                screenDim[0]/2+int(screenDim[1]/25)*13, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*i[0], width='0', fill='black')
            can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*5*(1-min(((time()-i[1])/0.3),1))+int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*(i[0]+1),
                                screenDim[0]/2+int(screenDim[1]/25)*5*(1-min(((time()-i[1])/0.3),1))+int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25)*10+int(screenDim[1]/25)*i[0], width='0', fill='white')


        can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*3, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2-int(screenDim[1]/25)*0.5, screenDim[1]/2-int(screenDim[1]/25)*2.5, width='5', outline='white', fill='black')
        can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*13, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2-int(screenDim[1]/25)*15.5, screenDim[1]/2-int(screenDim[1]/25)*7.5, width='5', outline='white', fill='black')
        can.create_rectangle(screenDim[0]/2-int(screenDim[1]/25)*13, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2-int(screenDim[1]/25)*3, screenDim[1]/2+int(screenDim[1]/25)*10, width='5', outline='white')
        
        can.create_rectangle(screenDim[0]/2+int(screenDim[1]/25)*13, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2+int(screenDim[1]/25)*15.5, screenDim[1]/2-int(screenDim[1]/25)*2.5, width='5', outline='white', fill='black')
        can.create_rectangle(screenDim[0]/2+int(screenDim[1]/25)*3, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2+int(screenDim[1]/25)*0.5, screenDim[1]/2-int(screenDim[1]/25)*7.5, width='5', outline='white', fill='black')
        can.create_rectangle(screenDim[0]/2+int(screenDim[1]/25)*3, screenDim[1]/2-int(screenDim[1]/25)*10,
                            screenDim[0]/2+int(screenDim[1]/25)*13, screenDim[1]/2+int(screenDim[1]/25)*10, width='5', outline='white')

        for i in range(3):
            can.create_image(screenDim[0]/2-int(screenDim[1]/25)*1.75, screenDim[1]/2-int(screenDim[1]/25)*(8.75-2.5*i),
                            image=imgBlocks[joueur1.queue[i]], anchor='center')
            can.create_image(screenDim[0]/2+int(screenDim[1]/25)*14.25, screenDim[1]/2-int(screenDim[1]/25)*(8.75-2.5*i),
                            image=imgBlocks[joueur2.queue[i]], anchor='center')
        can.create_image(screenDim[0]/2-int(screenDim[1]/25)*14.25, screenDim[1]/2-int(screenDim[1]/25)*8.75,
                        image=imgBlocks[joueur1.hold[0]], anchor='center')
        can.create_image(screenDim[0]/2+int(screenDim[1]/25)*1.75, screenDim[1]/2-int(screenDim[1]/25)*8.75,
                        image=imgBlocks[joueur2.hold[0]], anchor='center')

        if not joueur1.game and not joueur2.game and int(4-(time()-timer[0]))>0:
            can.create_text(screenDim[0]/2-int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25),
                            text=str(int(4-(time()-timer[0]))), anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/10), 'bold'))    
            can.create_text(screenDim[0]/2+int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25),
                            text=str(int(4-(time()-timer[0]))), anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/10), 'bold'))   
        elif not joueur1.game and joueur2.game:
            can.create_text(screenDim[0]/2-int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25),
                            text='Perdu :(', anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/15), 'bold'))    
            can.create_text(screenDim[0]/2+int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25),
                            text='Bravo !', anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/15), 'bold'))    
        elif joueur1.game and not joueur2.game:
            can.create_text(screenDim[0]/2-int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25),
                            text='Bravo !', anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/15), 'bold'))    
            can.create_text(screenDim[0]/2+int(screenDim[1]/25)*8, screenDim[1]/2-int(screenDim[1]/25),
                            text='Perdu :(', anchor='center',
                            fill='white', font=('Arial', int(screenDim[1]/15), 'bold'))    