#include <iostream>
#include <winbgim.h>
#include <stdlib.h>
#include <windows.h>
#include <cmath>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

fstream f1("settings.txt");

int stanga,sus,width,height,latura,numar,dimensiune,matchesCont,whiteWins,blackWins,totalMatches,table[100][100],auxtable[100][100],winCorner[100][100],movingSide=1,textSize,page,textH,menuOption,playerSide=1,experience,level,lastexp,lastpos,currency,inventorypages=1,undorem,shoppages,wh,bl;
float progress;
bool selection,chainJump,possibleChain,mainMenu=true,endMatch=false,bonusMove=false,inGame=false,vsPC=false,replaying,saved,undoing,finalPosition,escapeChain,typing;
char tasta,wequipped[1000],bequipped[1000],numejucator1[1000],numejucator2[1000];
long int prevl[100000],prevc[100000],pcont;

struct previousMoves
{
    int cont;
    struct points
    {
        int lin,col;
    } p[1000];

} moves;

struct currentMoves
{
    int cont,global;
    struct positions
    {
        char letr[10000];
    } t[10000];

} teamMoves[3];

struct replay
{
    int tableDimension,movesCont;
    char whoWon[1000];
    struct whiteBlackMoves
    {
        int fl,fc,tl,tc;
    } moves[1000];


} match[1000];

void checkExitProgram()
{
    if(kbhit())
        tasta=getch();

}

struct Button
{
    bool isClicked=false,arrow=false;
    int quantity=0,item=0,lvl=0,price=0;
    int xg,yg,xg1,yg1,xg2,yg2,xg3,yg3,gborder,gincolor,gtextsize,gtype,glen;

    void drawButton(int x1,int y1,int x2,int y2,char* text,int textsize,int border,int incolor)
    {
        settextstyle(10,0,textsize);
        xg1=(x1+(x2-x1)/2-textwidth(text)/2)>x1? x1 : x1+(x2-x1)/2-textwidth(text)/2-textwidth("o")/2;
        xg2=(x1+(x2-x1)/2+textwidth(text)/2)<x2? x2 : x1+(x2-x1)/2+textwidth(text)/2+textwidth("o")/2;
        yg1=(y1+(y2-y1)/2-textheight(text)/2)>y1? y1 : (y1+(y2-y1)/2-textheight(text)/2)-textheight("o")/2;
        yg2=(y1+(y2-y1)/2+textheight(text)/2)<y2? y2 : (y1+(y2-y1)/2+textheight(text)/2)+textheight("o")/2;
        gborder=border;
        gincolor=incolor;
        gtextsize=textsize;
        setcolor(border);
        setlinestyle(SOLID_LINE,0,4);
        rectangle(xg1,yg1,xg2,yg2);
        setfillstyle(SOLID_FILL,incolor);
        bar(xg1+2,yg1+2,xg2-2,yg2-2);
        if(quantity>0 && price==0)
        {
            char txt[1000]="assets/",conv[1000];
            itoa(item,conv,10);
            strcat(txt,conv);
            strcat(txt,".gif");
            readimagefile(txt,x1+2+(gtextsize>=2?gtextsize:2),y1+2+(gtextsize>=2?gtextsize:2),x2-2-(gtextsize>=2?gtextsize:2),y2-2-(gtextsize>=2?gtextsize:2));
            if(quantity>1)
            {
                itoa(quantity,txt,10);
                settextstyle(10,0,1);
                setbkcolor(incolor);
                outtextxy(xg2-textwidth(txt)-(gtextsize>=2?gtextsize:2)-1,yg2-textheight(txt)-(gtextsize>=2?gtextsize:2),txt);
            }
        }
        if(price>0)
        {
            char txt[1000]="assets/",conv[1000];
            itoa(item,conv,10);
            strcat(txt,conv);
            strcat(txt,".gif");
            settextstyle(10,0,1);
            readimagefile(txt,x1+2+(gtextsize>=2?gtextsize:2)+textheight(txt),y1+textheight(txt)+2+(gtextsize>=2?gtextsize:2),x2-textheight(txt)-2-(gtextsize>=2?gtextsize:2),y2-textheight(txt)-2-(gtextsize>=2?gtextsize:2));
            itoa(price,conv,10);
            setbkcolor(incolor);
            outtextxy(xg2-textwidth(conv)-textheight(conv)-(gtextsize>=2?gtextsize:2)-4,yg2-textheight(conv)-(gtextsize>=2?gtextsize:2),conv);
            readimagefile("assets/smoney.gif",xg2-textheight(conv)-(gtextsize>=2?gtextsize:2)-4,yg2-textheight(conv)-(gtextsize>=2?gtextsize:2)-4,xg2-(gtextsize>=2?gtextsize:2)-4,yg2-(gtextsize>=2?gtextsize:2)-4);
        }
        if(lvl>0)
        {
            char txt[1000]="Nivel:",conv[1000];
            itoa(lvl,conv,10);
            strcat(txt,conv);
            settextstyle(10,0,1);
            setbkcolor(incolor);
            outtextxy(xg1+(gtextsize>=2?gtextsize:2)+1,yg1+(gtextsize>=2?gtextsize:2),txt);
        }
        setbkcolor(incolor);
        outtextxy(xg1+(xg2-xg1)/2-textwidth(text)/2,yg1+(yg2-yg1)/2-textheight(text)/2,text);
        isClicked=false;
    }

    void drawTriangle(int x1,int y1,int x2,int y2,int x3,int y3)
    {
        line(x1,y1,x2,y2);
        line(x1,y1,x3,y3);
        line(x2,y2,x3,y3);
    }

    void drawLines(int x1,int y1,int x2,int y2,int x3,int y3)
    {
        line(x1,y1,x3,y3);
        line(x3,y3,x2,y2);
    }

    void drawArrow(int x,int y,int len,int border,int incolor,int type)
    {
        gborder=border;
        gincolor=incolor;
        gtype=type;
        glen=len;
        xg=x;
        yg=y;
        setcolor(border);
        setfillstyle(SOLID_FILL,incolor);
        setlinestyle(SOLID_LINE,0,4);
        arrow=true;
        switch(type)
        {
        case 0: //up arrow
            xg1=x-len/2;
            yg1=y;
            xg2=x+len/2;
            yg2=y;
            xg3=x;
            yg3=y-len*sqrt(3)/2;
            drawTriangle(xg1,yg1,xg2,yg2,xg3,yg3);
            floodfill(x,y-len/4,border);
            break;
        case 1: //down arrow
            xg1=x-len/2;
            yg1=y;
            xg2=x+len/2;
            yg2=y;
            xg3=x;
            yg3=y+len*sqrt(3)/2;
            drawTriangle(xg1,yg1,xg2,yg2,xg3,yg3);
            floodfill(x,y+len/4,border);
            break;
        case 2: //left arrow
            xg1=x;
            yg1=y-len/2;
            xg2=x;
            yg2=y+len/2;
            xg3=x-len*sqrt(3)/2;
            yg3=y;
            drawTriangle(xg1,yg1,xg2,yg2,xg3,yg3);
            floodfill(x-len/4,y,border);
            break;
        case 3: //right arrow
            xg1=x;
            yg1=y-len/2;
            xg2=x;
            yg2=y+len/2;
            xg3=x+len*sqrt(3)/2;
            yg3=y;
            drawTriangle(xg1,yg1,xg2,yg2,xg3,yg3);
            floodfill(x+len/4,y,border);
            break;
        }
    }

    bool is_in_triangle(int x,int y, int xg1,int yg1, int xg2,int yg2, int xg3,int yg3)
    {
        int as_x = x-xg1;
        int as_y = y-yg1;

        bool s_ab = (xg2-xg1)*as_y-(yg2-yg1)*as_x > 0;

        if((xg3-xg1)*as_y-(yg3-yg1)*as_x > 0 == s_ab)
            return false;

        if((xg3-xg2)*(y-yg2)-(yg3-yg2)*(x-xg2) > 0 != s_ab)
            return false;

        return true;
    }

    void animationParts(int i)
    {
        if(!arrow)
            rectangle(xg1+i-1,yg1+i-1,xg2-i+1,yg2-i+1);
        else
        {
            switch(gtype)
            {
            case 0:
                drawLines(xg-(glen-i)/2+3,yg-3,xg+(glen-i)/2-3,yg-3,xg,yg-(glen-i)*sqrt(3)/2+2);
                break;
            case 1:
                drawLines(xg-(glen-i)/2+3,yg+2,xg+(glen-i)/2-3,yg+2,xg,yg+(glen-i)*sqrt(3)/2-2);
                break;
            case 2:
                drawLines(xg-3,yg-(glen-i)/2+3,xg-3,yg+(glen-i)/2-3,xg-(glen-i)*sqrt(3)/2+2,yg);
                break;
            case 3:
                drawLines(xg+2,yg-(glen-i)/2+3,xg+2,yg+(glen-i)/2-3,xg+(glen-i)*sqrt(3)/2-2,yg);
                break;
            }
        }
    }

    void animation(int color)
    {
        int x,y,i,j;
        x=mousex();
        y=mousey();
        setcolor(gborder);
        rectangle(xg1,yg1,xg2,yg2);
        if(((x>=xg1 && x<=xg2 && y>=yg1 && y<=yg2) || (arrow==true && is_in_triangle(x,y,xg1,yg1,xg2,yg2,xg3,yg3))) && tasta!=27 && isClicked==false && !(kbhit()==true && typing==true))
        {
            setcolor(color);
            setbkcolor(gincolor);
            for(i=2; i<=(arrow?(glen/10):(gtextsize>=2?gtextsize:2)) && !(kbhit()==true && typing==true); i++)
            {
                checkExitProgram();
                x=mousex();
                y=mousey();
                if(!((x>=xg1 && x<=xg2 && y>=yg1 && y<=yg2)  || (arrow==true && is_in_triangle(x,y,xg1,yg1,xg2,yg2,xg3,yg3))) || tasta==27 || (kbhit() && typing==true) || (ismouseclick(WM_LBUTTONDOWN)))
                {
                    setcolor(gincolor);
                    for(j=i; j>=2; j--)
                        animationParts(j);
                    setcolor(gborder);
                    if(!arrow)
                        rectangle(xg1,yg1,xg2,yg2);
                    else
                        drawTriangle(xg1,yg1,xg2,yg2,xg3,yg3);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        isClicked=true;
                    }
                    return;
                }
                else
                    clearmouseclick(WM_LBUTTONDOWN);
                animationParts(i);
                delay(50);
            }
            for(i=(arrow?(glen/10):(gtextsize>=2?gtextsize:2)); i>=2; i--)
            {
                x=mousex();
                y=mousey();
                if(!((x>=xg1 && x<=xg2 && y>=yg1 && y<=yg2) || (arrow==true && is_in_triangle(x,y,xg1,yg1,xg2,yg2,xg3,yg3))) || tasta==27 || (kbhit() && typing==true) || (ismouseclick(WM_LBUTTONDOWN)))
                {
                    setcolor(gincolor);
                    for(j=i; j>=2; j--)
                        animationParts(j);
                    setcolor(gborder);
                    if(!arrow)
                        rectangle(xg1,yg1,xg2,yg2);
                    else
                        drawTriangle(xg1,yg1,xg2,yg2,xg3,yg3);
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        clearmouseclick(WM_LBUTTONDOWN);
                        isClicked=true;
                    }
                    return;
                }
                else
                    clearmouseclick(WM_LBUTTONDOWN);
                setcolor(gincolor);
                animationParts(i);
                delay(50);
            }
            if(ismouseclick(WM_LBUTTONDOWN) && isClicked==false)
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isClicked=true;
            }
        }
        else if(ismouseclick(WM_LBUTTONDOWN) && undoing==false)
            clearmouseclick(WM_LBUTTONDOWN);

    }
} start,magazin,inventar,meciuri,iesire,inventorymatrix[100][100][100],shopmatrix[100][100][100],matchesToShow[1000],rarrow,larrow,inp,undo,backm;

struct experienceBar
{
    int xg1,yg1,xg2,yg2,gcolor,gborder,gincolor;
    float gprogress;

    void drawBar(int x1,int y1,int x2,int y2,int border,int incolor,int color)
    {
        xg1=x1;
        yg1=y1;
        xg2=x2;
        yg2=y2;
        gborder=border;
        gincolor=incolor;
        gcolor=color;
        setcolor(border);
        setfillstyle(SOLID_FILL,incolor);
        setlinestyle(SOLID_LINE,0,4);
        rectangle(x1,y1,x2,y2);
        floodfill(x1+(x2-x1)/2,y1+(y2-y1)/2,border);
        setfillstyle(SOLID_FILL,gcolor);
        setcolor(gcolor);
        setlinestyle(SOLID_LINE,0,1);
        if(lastpos==0)
            lastpos=xg1;
        else if(lastpos!=xg1)
        {
            for(int i=xg1; i<=lastpos; i++)
                line(i,yg1+2,i,yg2-3);
            setcolor(gborder);
            for(int j=lastpos-1; j>=lastpos-3; j--)
                line(j,yg1-(yg2-yg1)/8,j,yg2+(yg2-yg1)/8);
        }
    }
    void resizeBar(int x1,int y1,int x2,int y2,int border,int incolor,int color)
    {
        char conv[1000],txt[1000]="Nivelul ";
        itoa(level,conv,10);
        strcat(txt,conv);
        settextstyle(10,0,1);
        setcolor(incolor);
        setbkcolor(LIGHTGRAY);
        char txt2[1000]="Nivelul ";
        itoa(level+1,conv,10);
        strcat(txt2,conv);
        if(x1+textwidth(txt)/2<x2-textwidth(txt2)/2)
        {
            outtextxy(x1-textwidth(txt)/2,y2+textheight(txt)/2,txt);
            outtextxy(x2-textwidth(txt)/2,y2+textheight(txt2)/2,txt2);
        }
        else
            outtextxy(x1+(x2-x1)/2-textwidth(txt)/2,y2+textheight(txt)/2,txt);
        setcolor(border);
        setfillstyle(SOLID_FILL,incolor);
        setlinestyle(SOLID_LINE,0,4);
        rectangle(x1,y1,x2,y2);
        floodfill(x1+(x2-x1)/2,y1+(y2-y1)/2,border);
        setfillstyle(SOLID_FILL,color);
        setcolor(color);
        setlinestyle(SOLID_LINE,0,1);
        if(lastpos!=0)
            gprogress=progress*(x2-x1);
        else
            gprogress=0;
        for(int i=x1; i<=x1+gprogress; i++)
            line(i,y1+2,i,y2-3);
        setcolor(border);
        for(int j=x1+gprogress; j>=x1+gprogress-3; j--)
            line(j,y1,j,y2);
    }
    void animation(int color,float exp,char* reason)
    {
        int i,dispos;
        char txt[1000]="+",conv[1000],txt3[1000];
        itoa(exp,conv,10);
        strcat(txt,conv);
        strcat(txt," experienta");
        dispos=lastpos;
        for(i=lastpos; i<=lastpos+(exp/experience)*(xg2-xg1); i++)
        {
            if(i>=xg2)
            {
                setbkcolor(LIGHTGRAY);
                setfillstyle(SOLID_FILL,LIGHTGRAY);
                setcolor(LIGHTGRAY);
                settextstyle(10,0,1);
                bar3d(xg1-4,yg1-(yg2-yg1)/8,xg2+4+textwidth(strlen(txt)>strlen(reason)?txt:reason),yg2+4+2*textheight(txt),1,1);
                setcolor(gborder);
                setfillstyle(SOLID_FILL,gincolor);
                setlinestyle(SOLID_LINE,0,4);
                rectangle(xg1,yg1,xg2,yg2);
                floodfill(xg1+(xg2-xg1)/2,yg1+(yg2-yg1)/2,gborder);
                exp=exp-(experience-lastexp);
                lastexp=0;
                txt[0]='+';
                txt[1]=NULL;
                itoa(exp,conv,10);
                strcat(txt,conv);
                strcat(txt," experienta");
                lastpos=xg1;
                i=xg1;
                dispos=xg1;
                level++;
                setcolor(LIGHTGRAY);
                setfillstyle(SOLID_FILL,LIGHTGRAY);
                bar3d(xg2+4,yg1-textheight(txt3),xg2+4+textwidth(txt3),yg1,1,1);
                settextstyle(10,0,3);
                setcolor(gincolor);
                setbkcolor(LIGHTGRAY);
                outtextxy(xg1+(xg2-xg1)/2-textwidth("Ai crescut in nivel!")/2,yg1-(yg2-yg1)/8-textheight("Ai crescut in nivel!"),"Ai crescut in nivel!");
                itoa(level,conv,10);
                char txt2[1000]="Nivelul ";
                strcat(txt2,conv);
                setcolor(gcolor);
                setbkcolor(gincolor);
                if(xg1+(xg2-xg1)/2-textwidth(txt2)/2>xg1)
                    outtextxy(xg1+(xg2-xg1)/2-textwidth(txt2)/2,yg1+(yg2-yg1)/2-textheight(txt2)/2,txt2);
                experience=100*(level+1);
                delay(500);
                setcolor(LIGHTGRAY);
                setfillstyle(SOLID_FILL,LIGHTGRAY);
                bar3d(xg1-textwidth("Ai crescut in nivel!"),yg1-(yg2-yg1)/8-textheight("Ai crescut in nivel!"),xg2+textwidth("Ai crescut in nivel!"),yg1-(yg2-yg1)/8,1,1);
            }
            if(exp>0)
            {
                setbkcolor(LIGHTGRAY);
                setcolor(color);
                setfillstyle(SOLID_FILL,color);
                setlinestyle(SOLID_LINE,0,1);
                line(i,yg1+2,i,yg2-3);
                settextstyle(10,0,1);
                setcolor(gincolor);
                outtextxy(i+4,yg2+4,txt);
                outtextxy(i+4,yg2+4+textheight(txt),reason);
                itoa(((experience-lastexp-exp)>0?(experience-lastexp-exp):0),txt3,10);
                strcat(txt3," xp ramasa");
                outtextxy(i+4,yg1-4-textheight(txt3),txt3);
                delay(1);

            }
        }
        setcolor(gborder);
        setfillstyle(SOLID_FILL,gcolor);
        if(i<xg2 && (i-1)!=xg1)
            for(int j=i-1; j>=i-3; j--)
                line(j,yg1-(yg2-yg1)/8,j,yg2+(yg2-yg1)/8);
        setcolor(gcolor);
        if(dispos<i-4)
            for(int j=dispos; j<=i-4; j++)
            {
                line(j,yg1+2,j,yg2-3);
                delay(1);
            }
        else
            for(int j=xg1; j<=i-4; j++)
            {
                line(j,yg1+2,j,yg2-3);
                delay(1);
            }
        lastpos=i;
        progress=float(lastpos-xg1)/(xg2-xg1);
        lastexp=lastexp+exp;
        delay(500);
        setbkcolor(LIGHTGRAY);
        setfillstyle(SOLID_FILL,LIGHTGRAY);
        setcolor(LIGHTGRAY);
        settextstyle(10,0,1);
        bar3d(i+4,yg2+4,i+4+textwidth(strlen(txt)>strlen(reason)?txt:reason),yg2+4+2*textheight(txt),1,1);
        bar3d(i+4,yg1-4-textheight(txt3),i+4+textwidth(txt3),yg1-4,1,1);
        delay(1000);
    }

} expbar;

void initializeTable()
{
    numar=dimensiune;
    width=getmaxx()/2;
    height=width;
    latura=width/numar;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
    if(latura/20>=1)
        textSize=latura/20;
    else
        textSize=1;
    if(wh!=0)
    {
        char txt[1000]="assets/",conv[1000];
        itoa(wh,conv,10);
        strcat(conv,".gif");
        strcat(txt,conv);
        strcpy(wequipped,txt);
    }
    if(bl!=0)
    {
        char txt[1000]="assets/",conv[1000];
        itoa(bl,conv,10);
        strcat(conv,".gif");
        strcat(txt,conv);
        strcpy(bequipped,txt);
    }
    if(!inGame && !endMatch)
    {
        for(int i=1; i<=dimensiune; i++)
            for(int j=1; j<=dimensiune; j++)
                table[i][j]=0;
        winCorner[dimensiune][1]=1;
        winCorner[dimensiune][2]=1;
        winCorner[dimensiune][3]=1;
        winCorner[dimensiune-1][1]=1;
        winCorner[dimensiune-2][1]=1;
        winCorner[dimensiune-1][2]=1;
        winCorner[1][dimensiune]=2;
        winCorner[2][dimensiune]=2;
        winCorner[3][dimensiune]=2;
        winCorner[1][dimensiune-1]=2;
        winCorner[1][dimensiune-2]=2;
        winCorner[2][dimensiune-1]=2;
        table[dimensiune][1]=1;
        table[dimensiune][2]=1;
        table[dimensiune][3]=1;
        table[dimensiune-1][1]=1;
        table[dimensiune-2][1]=1;
        table[dimensiune-1][2]=1;
        table[3][dimensiune]=2;
        table[1][dimensiune]=2;
        table[2][dimensiune]=2;
        table[1][dimensiune-1]=2;
        table[1][dimensiune-2]=2;
        table[2][dimensiune-1]=2;
        teamMoves[1].global=0;
        teamMoves[2].global=0;
        movingSide=1;
        for(int i=0; i<=dimensiune+1; i++)
        {
            table[0][i]=-1;
            table[i][0]=-1;
            table[dimensiune+1][i]=-1;
            table[i][dimensiune+1]=-1;
        }
    }
    teamMoves[1].cont=0;
    teamMoves[2].cont=0;
    endMatch=false;
    bonusMove=false;
    selection=false;
    chainJump=false;
}

int gameWon()
{
    if((table[1][dimensiune]==1 && table[2][dimensiune]==1 && table[3][dimensiune]==1 && table[1][dimensiune-1]==1 && table[1][dimensiune-2]==1 && table[2][dimensiune-1]==1) && (table[dimensiune][1]==2 && table[dimensiune][2]==2 && table[dimensiune][3]==2 && table[dimensiune-1][1]==2 && table[dimensiune-2][1]==2 && table[dimensiune-1][2]==2))
        return 3;
    else if(table[1][dimensiune]==1 && table[2][dimensiune]==1 && table[3][dimensiune]==1 && table[1][dimensiune-1]==1 && table[1][dimensiune-2]==1 && table[2][dimensiune-1]==1)
        return 1;
    else if(table[dimensiune][1]==2 && table[dimensiune][2]==2 && table[dimensiune][3]==2 && table[dimensiune-1][1]==2 && table[dimensiune-2][1]==2 && table[dimensiune-1][2]==2)
        return 2;
    return 0;
}

void checkMenuReturn()
{
    if(kbhit())
        if(getch()==27)
            mainMenu=true;
}

bool isWithinPossibleMoves(int linia,int coloana,int lines[17],int columns[17],int cont) ///Check if the move is possible by normal move or by chain jump
{
    for(int i=1; i<=cont; i++)
        if(lines[i]==linia && columns[i]==coloana)
            return true;
    return false;
}

bool inPrevious(int linia,int coloana) ///Check if the nearby squares were visited by a chain jump
{
    if(moves.cont>=1)
        for(int i=1; i<=moves.cont; i++)
            if(moves.p[i].lin==linia && moves.p[i].col==coloana)
                return true;
    return false;
}

void checkPossibleMoves(int linia,int coloana,int lines[17],int columns[17],int &cont) ///Check all possible moves
{
    int dx[]= {-1,0,1,0,-1,-1,1,1},dy[]= {0,1,0,-1,1,-1,1,-1};
    cont=0;
    for(int i=0; i<8; i++)
    {
        if(table[linia+dx[i]][coloana+dy[i]]==0 && chainJump==false)
        {
            cont++;
            lines[cont]=linia+dx[i];
            columns[cont]=coloana+dy[i];
        }
        if((table[linia+dx[i]][coloana+dy[i]]==1 || table[linia+dx[i]][coloana+dy[i]]==2) && table[linia+2*dx[i]][coloana+2*dy[i]]==0 && !inPrevious(linia+2*dx[i],coloana+2*dy[i]))
        {
            cont++;
            lines[cont]=linia+2*dx[i];
            columns[cont]=coloana+2*dy[i];
            possibleChain=true;
        }
    }
}

void scoreForMove(int cl,int cc,long int prevl[100000],long int prevc[100000],long int pcont,int &maxi)
{
    int dx[]= {-1,0,1,0,-1,-1,1,1},dy[]= {0,1,0,-1,1,-1,1,-1};
    int nr=0,winlin[10],wincol[10];
    //Get available win positions
    if(playerSide==2)
    {
        for(int i=1; i<=3; i++)
            for(int j=dimensiune+i-3; j<=dimensiune; j++)
                if(table[i][j]==0)
                {
                    nr++;
                    winlin[nr]=i;
                    wincol[nr]=j;
                }
    }
    else
    {
        for(int i=dimensiune-2; i<=dimensiune; i++)
            for(int j=1; j<=i-dimensiune+3; j++)
                if(table[i][j]==0)
                {
                    nr++;
                    winlin[nr]=i;
                    wincol[nr]=j;
                }
    }
    //find the closest win position
    int closestwin=200;
    for(int i=1; i<=nr; i++)
        if(abs(winlin[i]-cl)+abs(wincol[i]-cc)<closestwin)
            closestwin=abs(winlin[i]-cl)+abs(wincol[i]-cc);
    //compare with the other positions found
    if(closestwin<maxi)
        maxi=closestwin;
    //reiterate for the rest
    for(int i=0; i<8; i++)
    {
        if((cl+2*dx[i]>=1 && cl+2*dx[i]<=dimensiune && cc+2*dy[i]>=1 && cc+2*dy[i]<=dimensiune && cl+dx[i]>=1 && cl+dx[i]<=dimensiune && cc+dy[i]>=1 && cc+dy[i]<=dimensiune) && table[cl+2*dx[i]][cc+2*dy[i]]==0 && table[cl+dx[i]][cc+dy[i]]!=0)
        {
            bool foundInPrev=false;
            for(int j=1; j<=pcont; j++)
                if(prevl[j]==cl+2*dx[i] && prevc[j]==cc+2*dy[i])
                    foundInPrev=true;
            if(foundInPrev==false)
            {
                pcont++;
                prevl[pcont]=cl+2*dx[i];
                prevc[pcont]=cc+2*dy[i];
                scoreForMove(cl+2*dx[i],cc+2*dy[i],prevl,prevc,pcont,maxi);
                pcont--;
            }
        }
    }
    return;
}

void chooseARandomPiece(int &linia,int &coloana)
{
    int lines[100],columns[100],cont=0,r;
    for(int i=1; i<=dimensiune; i++)
        for(int j=1; j<=dimensiune; j++)
            if(table[i][j]!=playerSide && table[i][j]>0)
            {
                cont++;
                lines[cont]=i;
                columns[cont]=j;
            }
    int dx[]= {-1,0,1,0,-1,-1,1,1},dy[]= {0,1,0,-1,1,-1,1,-1};
    //Check if a piece is close to the winning corner
    for(int i=1; i<=cont; i++)
    {
        if(playerSide==2 && winCorner[lines[i]][columns[i]]!=2)
        {
            for(int j=0; j<8; j++)
                if((winCorner[lines[i]+dx[j]][columns[i]+dy[j]]==2 && table[lines[i]+dx[j]][columns[i]+dy[j]]==0) || (winCorner[lines[i]+2*dx[j]][columns[i]+2*dy[j]]==2 && table[lines[i]+2*dx[j]][columns[i]+2*dy[j]]==0 && table[lines[i]+dx[j]][columns[i]+dy[j]]!=0))
                {
                    linia=lines[i];
                    coloana=columns[i];
                    return;
                }
        }
        else if(playerSide==1 && winCorner[lines[i]][columns[i]]!=1)
        {
            for(int j=0; j<8; j++)
                if((winCorner[lines[i]+dx[j]][columns[i]+dy[j]]==1 && table[lines[i]+dx[j]][columns[i]+dy[j]]==0) || (winCorner[lines[i]+2*dx[j]][columns[i]+2*dy[j]]==1 && table[lines[i]+2*dx[j]][columns[i]+2*dy[j]]==0 && table[lines[i]+dx[j]][columns[i]+dy[j]]!=0))
                {
                    linia=lines[i];
                    coloana=columns[i];
                    return;
                }
        }
    }
    //Check if the piece can jump diagonally towards the enemy corner
    for(int i=1; i<=cont; i++)
    {

        if(playerSide==2 && winCorner[lines[i]][columns[i]]!=2 && lines[i]-2>=1 && columns[i]+2<=dimensiune && table[lines[i]-2][columns[i]+2]==0 && table[lines[i]-1][columns[i]+1]!=0 )
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
        else if(playerSide==1 && winCorner[lines[i]][columns[i]]!=1 && lines[i]+2<=dimensiune && columns[i]-2>=1 && table[lines[i]+2][columns[i]-2]==0 && table[lines[i]+1][columns[i]-1]!=0)
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
    }
    //Check if a piece can jump closer to the closest enemy corner
    linia=0;
    coloana=0;
    int minimizer=200,nr=0,winlin[10],wincol[10];
    if(playerSide==2)
    {
        for(int i=1; i<=3; i++)
            for(int j=dimensiune+i-3; j<=dimensiune; j++)
                if(table[i][j]==0)
                {
                    nr++;
                    winlin[nr]=i;
                    wincol[nr]=j;
                }
    }
    else
    {
        for(int i=dimensiune-2; i<=dimensiune; i++)
            for(int j=1; j<=i-dimensiune+3; j++)
                if(table[i][j]==0)
                {
                    nr++;
                    winlin[nr]=i;
                    wincol[nr]=j;
                }
    }
    for(int i=1; i<=nr; i++)
        if(abs(winlin[i]-lines[1])+abs(wincol[i]-columns[1])<minimizer)
            minimizer=abs(winlin[i]-lines[1])+abs(wincol[i]-columns[1]);
    for(int i=1; i<=cont; i++)
    {
        pcont=0;
        pcont++;
        prevl[pcont]=lines[i];
        prevc[pcont]=columns[i];
        if(winCorner[lines[i]][columns[i]]!=playerSide)
            for(int j=0; j<8; j++)
            {
                if((table[lines[i]+dx[j]][columns[i]+dy[j]]==1 || table[lines[i]+dx[j]][columns[i]+dy[j]]==2) && table[lines[i]+2*dx[j]][columns[i]+2*dy[j]]==0)
                {
                    int score=200;
                    pcont++;
                    prevl[pcont]=lines[i]+2*dx[j];
                    prevc[pcont]=columns[i];
                    scoreForMove(lines[i]+2*dx[j],columns[i]+2*dy[j],prevl,prevc,pcont,score);
                    pcont--;
                    if(score<=minimizer)
                    {
                        minimizer=score;
                        linia=lines[i];
                        coloana=columns[i];
                    }
                }
            }
    }
    if(linia!=0 && coloana!=0)
        return;
    //Check if the piece can move diagonally
    for(int i=1; i<=cont; i++)
    {
        if(playerSide==2 && winCorner[lines[i]][columns[i]]!=2 && lines[i]-1>=1 && columns[i]+1<=dimensiune && table[lines[i]-1][columns[i]+1]==0)
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
        else if(playerSide==1 && winCorner[lines[i]][columns[i]]!=1 && lines[i]+1<=dimensiune && columns[i]-1>=1 && table[lines[i]+1][columns[i]-1]==0)
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
    }
    //choose the closest to the winning corner that can move
    int minline=101,mincolumn=101,maxline=-1,maxcolumn=-1;
    linia=0;
    coloana=0;
    for(int i=1; i<=cont; i++)
    {
        bool restore=possibleChain;
        int ls[17],cls[17],ct;
        checkPossibleMoves(lines[i],columns[i],ls,cls,ct);
        possibleChain=restore;
        if(playerSide==2 && winCorner[lines[i]][columns[i]]!=2 && lines[i]+dimensiune-columns[i]+1<minline+mincolumn && ct>0)
        {
            minline=lines[i];
            mincolumn=dimensiune-columns[i]+1;
            linia=lines[i];
            coloana=columns[i];
        }
        else if(playerSide==1 && winCorner[lines[i]][columns[i]]!=1 && lines[i]+dimensiune-columns[i]+1>maxline+maxcolumn && ct>0)
        {
            maxline=lines[i];
            maxcolumn=dimensiune-columns[i]+1;
            linia=lines[i];
            coloana=columns[i];
        }
    }
    if(linia!=0 && coloana!=0)
        return;
    //if nothing is available choose a piece that can move
    for(int i=1; i<=cont; i++)
    {
        bool restore=possibleChain;
        int ls[17],cls[17],ct;
        checkPossibleMoves(lines[i],columns[i],ls,cls,ct);
        possibleChain=restore;
        if(ct>0)
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
    }
    //Random choice for a lower difficulty
    srand (time(NULL));
    r=rand()%cont+1;
    linia=lines[r];
    coloana=columns[r];
}

void chooseAvailablePosition(int &linia,int &coloana,int lines[17],int columns[17],int cont)
{
    finalPosition=false;
    escapeChain=false;
    //Select the position if its in the winning corner
    for(int i=1; i<=cont; i++)
        if(playerSide==2 && winCorner[lines[i]][columns[i]]==2)
        {
            linia=lines[i];
            coloana=columns[i];
            finalPosition=true;
            return;
        }
        else if(playerSide==1 && winCorner[lines[i]][columns[i]]==1)
        {
            linia=lines[i];
            coloana=columns[i];
            finalPosition=true;
            return;
        }
    //If jumping check if the piece can go further or else stop here

    if(chainJump==true)
    {
        linia=0;
        coloana=0;
        int minimizer=200,nr=0,winlin[10],wincol[10];
        if(playerSide==2)
        {
            for(int i=1; i<=3; i++)
                for(int j=dimensiune+i-3; j<=dimensiune; j++)
                    if(table[i][j]==0)
                    {
                        nr++;
                        winlin[nr]=i;
                        wincol[nr]=j;
                    }
        }
        else
        {
            for(int i=dimensiune-2; i<=dimensiune; i++)
                for(int j=1; j<=i-dimensiune+3; j++)
                    if(table[i][j]==0)
                    {
                        nr++;
                        winlin[nr]=i;
                        wincol[nr]=j;
                    }
        }
        for(int i=1; i<=nr; i++)
            if(abs(winlin[i]-lines[0])+abs(wincol[i]-columns[0])<minimizer)
                minimizer=abs(winlin[i]-lines[0])+abs(wincol[i]-columns[0]);
        for(int i=1; i<=cont; i++)
        {
            int score=200;
            pcont++;
            prevl[pcont]=lines[0];
            prevc[pcont]=columns[0];
            pcont++;
            prevl[pcont]=lines[i];
            prevc[pcont]=columns[i];
            scoreForMove(lines[i],columns[i],prevl,prevc,pcont,score);
            pcont--;
            if(score<=minimizer)
            {
                minimizer=score;
                linia=lines[i];
                coloana=columns[i];
            }
        }
        if(linia!=0 && coloana!=0)
            return;
        else
        {
            pcont=0;
            escapeChain=true;
            return;
        }
    }

    //Jump diagonally if possible
    for(int i=1; i<=cont; i++)
        if(playerSide==2 && lines[0]-2==lines[i] && columns[0]+2==columns[i])
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
        else if(playerSide==1 && lines[0]+2==lines[i] && columns[0]-2==columns[i])
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
    //Check if the piece can be placed in a position so it can jump diagonally after
    for(int i=1; i<=cont; i++)
        if(playerSide==2 && lines[i]-2>=1 && columns[i]+2<=dimensiune && (lines[i]!=lines[0]+1 || columns[i]!=columns[0]-1) && (table[lines[i]-2][columns[i]+2]==0 && (table[lines[i]-1][columns[i]+1]==table[lines[0]][columns[0]] || (table[lines[i]-1][columns[i]+1]==playerSide && table[lines[i]+1][columns[i]-1]!=0))))
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
        else if(playerSide==1 && lines[i]+2<=dimensiune && columns[i]-2>=1 && (lines[i]!=lines[0]-1 || columns[i]!=columns[0]+1) && (table[lines[i]+2][columns[i]-2]==0 && (table[lines[i]+1][columns[i]-1]==table[lines[0]][columns[0]] || (table[lines[i]+1][columns[i]-1]==playerSide && table[lines[i]-1][columns[i]+1]!=0))))
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
    //Check if the piece can be placed in a position so it can help another to jump
    for(int i=1; i<=cont; i++)
        if(playerSide==2 && table[lines[i]+1][columns[i]-1]==1 && table[lines[i]-1][columns[i]+1]==0 && ((lines[0]-1==lines[i] && columns[0]+1==lines[i])?false:true))
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
        else if(playerSide==1 && table[lines[i]-1][columns[i]+1]==2 && table[lines[i]+1][columns[i]-1]==0 && ((lines[0]+1==lines[i] && columns[0]-1==lines[i])?false:true))
        {
            linia=lines[i];
            coloana=columns[i];
            return;
        }
    //choose the position closest to an non-ocuppied win position
    int nr=0,winlin[10],wincol[10];
    if(playerSide==2)
    {
        for(int i=1; i<=3; i++)
            for(int j=dimensiune+i-3; j<=dimensiune; j++)
                if(table[i][j]==0)
                {
                    nr++;
                    winlin[nr]=i;
                    wincol[nr]=j;
                }
    }
    else
    {
        for(int i=dimensiune-2; i<=dimensiune; i++)
            for(int j=1; j<=i-dimensiune+3; j++)
                if(table[i][j]==0)
                {
                    nr++;
                    winlin[nr]=i;
                    wincol[nr]=j;
                }
    }
    int minline=101,mincolumn=101,maxline=-1,maxcolumn=-1;
    linia=0;
    coloana=0;
    for(int j=1; j<=nr; j++)
        for(int i=1; i<=cont; i++)
        {
            if(abs(lines[i]-winlin[j])+abs(columns[i]-wincol[j])<minline+mincolumn)
            {
                minline=abs(lines[i]-winlin[j]);
                mincolumn=abs(columns[i]-wincol[j]);
                linia=lines[i];
                coloana=columns[i];
            }
        }
    if(linia!=0 && coloana!=0)
        return;
    //random selection for a lower difficulty
    int r;
    srand (time(NULL));
    r=rand()%cont+1;
    linia=lines[r];
    coloana=columns[r];
}

void drawTable()
{
    int i,j,p,mij;
    char s[20],muta[1000]="Mutari alb",mutn[1000]="Mutari negru",ma[1000]="Muta alb",mn[1000]="Muta negru",le1[]="a",le2[]="a1 - a1";
    if(numejucator1[0]!=NULL)
    {
        strcpy(muta,"Mutarile lui ");
        strcat(muta,numejucator1);
        strcpy(ma,"Muta ");
        strcat(ma,numejucator1);
    }
    if(numejucator2[0]!=NULL)
    {
        strcpy(mutn,"Mutarile lui ");
        strcat(mutn,numejucator2);
        strcpy(mn,"Muta ");
        strcat(mn,numejucator2);
    }
    cleardevice();
    setbkcolor(LIGHTGRAY);
    setfillstyle(0,WHITE);
    floodfill(1,1,DARKGRAY);
    for(i=1; i<=numar; i++)
    {
        for(j=1; j<=numar; j++)
        {
            setcolor(DARKGRAY);
            setlinestyle(SOLID_LINE,0,2);
            rectangle(stanga+latura*(j-1),sus+latura*(i-1),stanga+latura*j,sus+latura*i);
            setfillstyle(SOLID_FILL,WHITE);
            floodfill(stanga+latura*(j-1)+latura/2,sus+latura*(i-1)+latura/2,DARKGRAY);
            if((table[i][j]==1 && replaying==false) || (auxtable[i][j]==1 && replaying==true))
            {
                if(wh==0 || (auxtable[i][j]==1 && replaying==true))
                {
                    circle(stanga+latura*(j-1)+latura/2,sus+latura*(i-1)+latura/2,9*latura/21-2);
                    setbkcolor(WHITE);
                    setfillstyle(CLOSE_DOT_FILL,DARKGRAY);
                    floodfill(stanga+latura*(j-1)+latura/2,sus+latura*(i-1)+latura/2,DARKGRAY);
                }
                else
                    readimagefile(wequipped,stanga+latura*(j-1)+latura/10,sus+latura*(i-1)+latura/10,stanga+latura*j-latura/10,sus+latura*i-latura/10);
            }
            else if((table[i][j]==2 && replaying==false) || (auxtable[i][j]==2 && replaying==true))
            {
                if(bl==0 || (auxtable[i][j]==2 && replaying==true))
                {
                    setcolor(DARKGRAY);
                    circle(stanga+latura*(j-1)+latura/2,sus+latura*(i-1)+latura/2,9*latura/21-2);
                    setbkcolor(BLACK);
                    setfillstyle(LTSLASH_FILL,DARKGRAY);
                    floodfill(stanga+latura*(j-1)+latura/2,sus+latura*(i-1)+latura/2,DARKGRAY);
                }
                else
                    readimagefile(bequipped,stanga+latura*(j-1)+latura/10,sus+latura*(i-1)+latura/10,stanga+latura*j-latura/10,sus+latura*i-latura/10);
            }
        }
        setcolor(WHITE);
        char s[100],c[2];
        itoa(numar-i+1,s,10);
        setbkcolor(LIGHTGRAY);
        settextstyle(10,1,latura/30>1?latura/30:1);
        textH=textheight(le1);
        outtextxy(stanga-textH-2,sus+latura*(i-1)+latura/2+textwidth(s)/2,s);
        settextstyle(10,0,latura/30>1?latura/30:1);
        c[0]=96+i;
        c[1]=NULL;
        outtextxy(stanga+latura*(i-1)+latura/2-textwidth(le1)/2,sus+latura*numar+2,c);
    }
    setcolor(WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,width/230>0?width/230:1);
    if(textwidth(muta)<stanga)
    {
        mij=stanga/2;
        outtextxy(mij-textwidth(muta)/2,sus-textheight(muta),muta);
        line(mij-textwidth(muta)/2,sus,mij+textwidth(muta)/2,sus);
        mij=getmaxx()-mij;
        outtextxy(mij-textwidth(mutn)/2,sus-textheight(mutn),mutn);
        line(mij-textwidth(mutn)/2,sus,mij+textwidth(mutn)/2,sus);
    }
    mij=stanga+latura*(numar)/2;
    settextstyle(10,0,width/170>1?width/170:1);
    if(movingSide==1)
        outtextxy(mij-textwidth(ma)/2,sus-textheight(ma)-textSize-2,ma);
    else
        outtextxy(mij-textwidth(mn)/2,sus-textheight(mn)-textSize-2,mn);
    settextstyle(10,0,width/230>0?width/230:1);
    if(sus+teamMoves[1].cont*(textheight(le2)+textSize-2) >= getmaxy())
    {
        teamMoves[1].t[1]=teamMoves[1].t[teamMoves[1].cont];
        teamMoves[1].cont=1;
    }
    if(sus+teamMoves[2].cont*(textheight(le2)+textSize-2) >= getmaxy())
    {
        teamMoves[2].t[1]=teamMoves[2].t[teamMoves[2].cont];
        teamMoves[2].cont=1;
    }
}

void setpage()
{
    if (page == 0)
    {
        setactivepage(1);
        cleardevice();
        drawTable();
    }
    else if (page == 1)
    {
        setactivepage(0);
        cleardevice();
        drawTable();
    }
}

void showpage()
{
    if (page == 0)
    {
        page = 1;
        setvisualpage(page);
    }
    else if (page == 1)
    {
        page = 0;
        setvisualpage(page);
    }
}

void rotateTable()
{
    int i,j,aux;
    for(i=1; i<=dimensiune; i++)
        for(j=i+1; j<=dimensiune; j++)
        {
            aux=table[j][i];
            table[j][i]=table[i][j];
            table[i][j]=aux;
        }
}

void redrawMoves()
{
    int mij;
    char s[100],le[]="a1 - a1",ma[]="Muta alb",mn[]="Muta negru";
    if(numejucator1[0]!=NULL)
    {
        strcpy(ma,"Muta ");
        strcat(ma,numejucator1);
    }
    if(numejucator2[0]!=NULL)
    {
        strcpy(mn,"Muta ");
        strcat(mn,numejucator2);
    }
    setbkcolor(LIGHTGRAY);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    mij=stanga+latura*(numar)/2;
    settextstyle(10,0,width/170>1?width/170:1);
    if(movingSide==1)
    {
        bar(stanga,0,stanga+latura*numar,sus-2);
        outtextxy(mij-textwidth(ma)/2,sus-textheight(ma)-2,ma);
    }
    else
    {
        bar(stanga,0,stanga+latura*numar,sus-2);
        outtextxy(mij-textwidth(mn)/2,sus-textheight(mn)-2,mn);
    }
    if(textwidth(teamMoves[1].t[1].letr)<stanga-2*textheight(le))
    {
        settextstyle(10,0,width/230>0?width/230:1);
        if(sus+(teamMoves[1].cont+1)*textheight(le) > sus+latura*dimensiune)
        {
            teamMoves[1].t[1]=teamMoves[1].t[teamMoves[1].cont];
            teamMoves[1].cont=1;
            setbkcolor(LIGHTGRAY);
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,LIGHTGRAY);
            mij=stanga/2;
            bar(0,sus+2,stanga-latura/5-textH,sus+latura*dimensiune);
        }
        if(sus+(teamMoves[2].cont+1)*textheight(le) > sus+latura*dimensiune)
        {
            teamMoves[2].t[1]=teamMoves[2].t[teamMoves[2].cont];
            teamMoves[2].cont=1;
            setbkcolor(LIGHTGRAY);
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,LIGHTGRAY);
            mij=stanga/2;
            mij=getmaxx()-mij;
            bar(stanga+latura*numar+2,sus+2,getmaxx(),sus+latura*dimensiune);
        }
        mij=stanga/2;
        if(movingSide==2)
        {
            bar(0,sus+((teamMoves[1].cont-1)>=0?(teamMoves[1].cont-1):0)*textheight(le)+2,stanga-latura/5-textH,sus+latura*dimensiune);
            settextstyle(10,0,width/230>0?width/230:1);
            outtextxy(mij-textwidth(teamMoves[1].t[teamMoves[1].cont].letr)/2,sus+(teamMoves[1].cont-1)*textheight(le)+2,teamMoves[1].t[teamMoves[1].cont].letr);
            itoa(teamMoves[1].global,s,10);
            if(teamMoves[1].cont>0)
            {
                line(mij-textwidth(teamMoves[1].t[teamMoves[1].cont].letr)/2,sus+(teamMoves[1].cont)*textheight(le)+2,mij+textwidth(teamMoves[1].t[teamMoves[1].cont].letr)/2,sus+(teamMoves[1].cont)*textheight(le)+2);
                strcat(s," mutari");
                outtextxy(mij-textwidth(s)/2,sus+(teamMoves[1].cont)*textheight(le)+4,s);
            }
        }
        else
        {
            mij=getmaxx()-mij;
            bar(stanga+latura*numar+2,sus+((teamMoves[2].cont-1)>=0?(teamMoves[2].cont-1):0)*textheight(le)+2,getmaxx(),sus+latura*dimensiune);
            settextstyle(10,0,width/230>0?width/230:1);
            outtextxy(mij-textwidth(teamMoves[2].t[teamMoves[2].cont].letr)/2,sus+(teamMoves[2].cont-1)*textheight(le)+2,teamMoves[2].t[teamMoves[2].cont].letr);
            itoa(teamMoves[2].global,s,10);
            if(teamMoves[2].cont>0)
            {
                line(mij-textwidth(teamMoves[2].t[teamMoves[2].cont].letr)/2,sus+teamMoves[2].cont*textheight(le)+2,mij+textwidth(teamMoves[2].t[teamMoves[2].cont].letr)/2,sus+teamMoves[2].cont*textheight(le)+2);
                strcat(s," mutari");
                outtextxy(mij-textwidth(s)/2,sus+(teamMoves[2].cont)*textheight(le)+4,s);
            }
        }

    }
}

void placeAnimation(int linia,int coloana)
{
    int i,j;
    setlinestyle(SOLID_LINE,0,4);
    for(i=9*latura/21+4; i<=latura/2-3; i++)
    {
        setcolor(BLUE);
        circle(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,i);
        delay(10);
        setcolor(WHITE);
        circle(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,i-2);
    }
    setcolor(WHITE);
    for(j=i-2; j<=latura/2-3; j++)
    {
        circle(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,j);
        delay(10);
    }

}

void hoverAtPoint(int x,int y,int lines[17],int columns[17],int cont)
{
    int linia,coloana,linia2,coloana2,i,j,k;
    if(selection==false ? (x>=stanga && x<=stanga+latura*dimensiune-1 && y>=sus && y<=sus+latura*dimensiune-1) : true)
    {
        if(vsPC?(movingSide==playerSide):true)
        {
            x=mousex();
            y=mousey();
            linia=(y-sus)/latura+1;
            coloana=(x-stanga)/latura+1;
        }
        if(table[linia][coloana]==movingSide && selection==false)
            setcolor(GREEN);
        else if(table[linia][coloana]!=movingSide && selection==false)
            setcolor(RED);
        else
            setcolor(GREEN);
        setlinestyle(SOLID_LINE,0,2);
        for(i=2; i<=latura/2-9*latura/21; i++)
        {
            checkExitProgram();
            if(selection==false && (!(mousex()>=stanga+latura*(coloana-1) && mousex()<=stanga+latura*coloana && mousey()>=sus+latura*(linia-1) && mousey()<=sus+latura*linia) || tasta==27 || (ismouseclick(WM_LBUTTONDOWN) && table[linia][coloana]==movingSide)))
            {
                setcolor(WHITE);
                for(j=i; j>=2; j--)
                    rectangle(stanga+latura*(coloana-1)+j-1,sus+latura*(linia-1)+j-1,stanga+latura*(coloana)-j+1,sus+latura*(linia)-j+1);
                setcolor(DARKGRAY);
                rectangle(stanga+latura*(coloana-1),sus+latura*(linia-1),stanga+latura*(coloana),sus+latura*(linia));
                return;
            }
            else if(selection==true && ((ismouseclick(WM_LBUTTONDOWN) && (mousex()>stanga+1 && mousex()<=stanga+latura*dimensiune-1 && mousey()>sus+1 && mousey()<=sus+latura*dimensiune-1)) || (ismouseclick(WM_RBUTTONDOWN) && (vsPC ? movingSide==playerSide : true))))
            {
                if(vsPC?(movingSide==playerSide):true)
                {
                    x=mousex();
                    y=mousey();
                    linia=(y-sus)/latura+1;
                    coloana=(x-stanga)/latura+1;
                }
                if(isWithinPossibleMoves(linia,coloana,lines,columns,cont) || ismouseclick(WM_RBUTTONDOWN))
                {
                    setcolor(WHITE);
                    for(j=i; j>=2; j--)
                        for(k=1; k<=cont; k++)
                            rectangle(stanga+latura*(columns[k]-1)+j-1,sus+latura*(lines[k]-1)+j-1,stanga+latura*(columns[k])-j+1,sus+latura*(lines[k])-j+1);
                    for(j=9*latura/21; j<=latura/2; j++)
                        circle(stanga+latura*(columns[0]-1)+latura/2,sus+latura*(lines[0]-1)+latura/2,j);
                    return;
                }
                else
                    clearmouseclick(WM_LBUTTONDOWN);
            }
            else
                clearmouseclick(WM_LBUTTONDOWN);
            if(selection==false)
                rectangle(stanga+latura*(coloana-1)+i-1,sus+latura*(linia-1)+i-1,stanga+latura*(coloana)-i+1,sus+latura*(linia)-i+1);
            else
            {
                setcolor(GREEN);
                for(k=1; k<=cont; k++)
                    rectangle(stanga+latura*(columns[k]-1)+i-1,sus+latura*(lines[k]-1)+i-1,stanga+latura*(columns[k])-i+1,sus+latura*(lines[k])-i+1);
                setcolor(BLUE);
                circle(stanga+latura*(columns[0]-1)+latura/2,sus+latura*(lines[0]-1)+latura/2,latura/2-i>9*latura/21? latura/2-i : 9*latura/21);
            }
            delay((latura*dimensiune)/40);
        }
        if(selection==false)
        {
            setcolor(DARKGRAY);
            rectangle(stanga+latura*(coloana-1),sus+latura*(linia-1),stanga+latura*(coloana),sus+latura*(linia));
        }
        for(i=latura/2-9*latura/21; i>=2; i--)
        {
            checkExitProgram();
            if(selection==false && (!(mousex()>=stanga+latura*(coloana-1) && mousex()<=stanga+latura*coloana && mousey()>=sus+latura*(linia-1) && mousey()<=sus+latura*linia) || tasta==27 || (ismouseclick(WM_LBUTTONDOWN) && table[linia][coloana]==movingSide)))
            {
                setcolor(WHITE);
                for(j=i; j>=2; j--)
                    rectangle(stanga+latura*(coloana-1)+j-1,sus+latura*(linia-1)+j-1,stanga+latura*(coloana)-j+1,sus+latura*(linia)-j+1);
                setcolor(DARKGRAY);
                rectangle(stanga+latura*(coloana-1),sus+latura*(linia-1),stanga+latura*(coloana),sus+latura*(linia));
                return;
            }
            else if(selection==true && ((ismouseclick(WM_LBUTTONDOWN) && (mousex()>stanga+1 && mousex()<=stanga+latura*dimensiune-1 && mousey()>sus+1 && mousey()<=sus+latura*dimensiune-1)) || (ismouseclick(WM_RBUTTONDOWN) && (vsPC ? movingSide==playerSide : true))))
            {
                if(vsPC?(movingSide==playerSide):true)
                {
                    x=mousex();
                    y=mousey();
                    linia=(y-sus)/latura+1;
                    coloana=(x-stanga)/latura+1;
                }
                if(isWithinPossibleMoves(linia,coloana,lines,columns,cont) || ismouseclick(WM_RBUTTONDOWN))
                {
                    setcolor(WHITE);
                    for(j=i; j>=2; j--)
                        for(k=1; k<=cont; k++)
                            rectangle(stanga+latura*(columns[k]-1)+j-1,sus+latura*(lines[k]-1)+j-1,stanga+latura*(columns[k])-j+1,sus+latura*(lines[k])-j+1);
                    for(j=9*latura/21; j<=latura/2; j++)
                        circle(stanga+latura*(columns[0]-1)+latura/2,sus+latura*(lines[0]-1)+latura/2,j);
                    return;
                }
                else
                    clearmouseclick(WM_LBUTTONDOWN);
            }
            else
                clearmouseclick(WM_LBUTTONDOWN);
            if(selection==false)
            {
                setcolor(WHITE);
                rectangle(stanga+latura*(coloana-1)+i-1,sus+latura*(linia-1)+i-1,stanga+latura*(coloana)-i+1,sus+latura*(linia)-i+1);
            }
            else
            {
                setcolor(WHITE);
                for(k=1; k<=cont; k++)
                    rectangle(stanga+latura*(columns[k]-1)+i-1,sus+latura*(lines[k]-1)+i-1,stanga+latura*(columns[k])-i+1,sus+latura*(lines[k])-i+1);
                circle(stanga+latura*(columns[0]-1)+latura/2,sus+latura*(lines[0]-1)+latura/2,latura/2-i >9*latura/21 ? latura/2-i : 9*latura/21);
            }
            delay((latura*dimensiune)/40);
        }
        if(selection==false)
        {
            setcolor(DARKGRAY);
            rectangle(stanga+latura*(coloana-1),sus+latura*(linia-1),stanga+latura*(coloana),sus+latura*(linia));
        }
    }
}

void movePiece()
{
    int linia,coloana,lines[17],columns[17],cont,x,y,i,q;
    char p[100],c[100];
    pcont=0;
    if(ismouseclick(WM_LBUTTONDOWN) || (vsPC && movingSide!=playerSide))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        if(vsPC && movingSide!=playerSide)
            chooseARandomPiece(linia,coloana);
        else
        {
            x=mousex();
            y=mousey();
            linia=(y-sus)/latura+1;
            coloana=(x-stanga)/latura+1;
        }
        if (((x>=stanga && x<=stanga+width-textSize-4 && y>=sus && y<=sus+height-textSize-4) && table[linia][coloana]==movingSide) || (vsPC && movingSide!=playerSide))
        {
            selection=true;
            cont=0;
            lines[cont]=linia;
            columns[cont]=coloana;
            possibleChain=false;
            checkPossibleMoves(linia,coloana,lines,columns,cont);
            while(selection==true && tasta!=27)
            {
                if(possibleChain==false && chainJump==true)
                {
                    selection=false;
                    chainJump=false;
                    inGame=true;
                    strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr," - ");
                    q=numar-linia+1;
                    itoa(q,p,10);
                    c[0]=97+coloana-1;
                    c[1]=NULL;
                    strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,c);
                    strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,p);
                    movingSide=movingSide==1 ? 2 : 1;
                    moves.cont=0;
                    redrawMoves();
                    //rotateTable();
                    if(gameWon()==1 && bonusMove==false)
                        bonusMove=true;
                    else if(gameWon()==2 || ((gameWon()==1 || gameWon()==3) && bonusMove==true))
                    {
                        endMatch=true;
                        return;
                    }
                    break;
                }
                if(vsPC && movingSide!=playerSide)
                    chooseAvailablePosition(linia,coloana,lines,columns,cont);
                else
                {
                    x=mousex();
                    y=mousey();
                    linia=(y-sus)/latura+1;
                    coloana=(x-stanga)/latura+1;
                }
                hoverAtPoint(x,y,lines,columns,cont);
                setcolor(DARKGRAY);
                for(int k=0; k<=cont; k++)
                    rectangle(stanga+latura*(columns[k]-1),sus+latura*(lines[k]-1),stanga+latura*(columns[k]),sus+latura*(lines[k]));
                if((ismouseclick(WM_LBUTTONDOWN) && isWithinPossibleMoves(linia,coloana,lines,columns,cont) && (x>stanga+1 && x<=stanga+latura*dimensiune-1 && y>sus+1 && y<=sus+latura*dimensiune-1)) || (vsPC && movingSide!=playerSide && escapeChain==false))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    setcolor(WHITE);
                    setfillstyle(SOLID_FILL,WHITE);
                    bar(stanga+latura*(columns[0]-1)+2,sus+latura*(lines[0]-1)+2,stanga+latura*(columns[0])-2,sus+latura*(lines[0])-2);
                    setlinestyle(SOLID_LINE,0,2);
                    setcolor(DARKGRAY);
                    rectangle(stanga+latura*(columns[0]-1),sus+latura*(lines[0]-1),stanga+latura*(columns[0]),sus+latura*(lines[0]));
                    if(table[lines[0]][columns[0]]==1)
                    {
                        if(wh==0)
                        {
                            setlinestyle(SOLID_LINE,0,2);
                            setcolor(DARKGRAY);
                            circle(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,9*latura/21-2);
                            setbkcolor(WHITE);
                            setfillstyle(CLOSE_DOT_FILL,DARKGRAY);
                            floodfill(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,DARKGRAY);
                        }
                        else
                            readimagefile(wequipped,stanga+latura*(coloana-1)+latura/10,sus+latura*(linia-1)+latura/10,stanga+latura*coloana-latura/10,sus+latura*linia-latura/10);
                        setlinestyle(SOLID_LINE,0,2);
                        setcolor(DARKGRAY);
                        rectangle(stanga+latura*(coloana-1),sus+latura*(linia-1),stanga+latura*(coloana),sus+latura*(linia));
                    }
                    else if(table[lines[0]][columns[0]]==2)
                    {
                        if(bl==0)
                        {
                            setlinestyle(SOLID_LINE,0,2);
                            setcolor(DARKGRAY);
                            circle(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,9*latura/21-2);
                            setbkcolor(BLACK);
                            setfillstyle(LTSLASH_FILL,DARKGRAY);
                            floodfill(stanga+latura*(coloana-1)+latura/2,sus+latura*(linia-1)+latura/2,DARKGRAY);
                        }
                        else
                            readimagefile(bequipped,stanga+latura*(coloana-1)+latura/10,sus+latura*(linia-1)+latura/10,stanga+latura*coloana-latura/10,sus+latura*linia-latura/10);
                        setlinestyle(SOLID_LINE,0,2);
                        setcolor(DARKGRAY);
                        rectangle(stanga+latura*(coloana-1),sus+latura*(linia-1),stanga+latura*(coloana),sus+latura*(linia));
                    }
                    ///Mark only the chain jump
                    if((abs(linia-lines[0])==2 && abs(coloana-columns[0])==2) || (abs(linia-lines[0])==0 && abs(coloana-columns[0])==2) || (abs(linia-lines[0])==2 && abs(coloana-columns[0])==0))
                    {
                        moves.cont++;
                        moves.p[moves.cont].lin=linia;
                        moves.p[moves.cont].col=coloana;
                        moves.cont++;
                        moves.p[moves.cont].lin=lines[0];
                        moves.p[moves.cont].col=columns[0];
                        table[linia][coloana]=table[lines[0]][columns[0]];
                        table[lines[0]][columns[0]]=0;

                        match[matchesCont].movesCont++;
                        match[matchesCont].moves[match[matchesCont].movesCont].fl=lines[0];
                        match[matchesCont].moves[match[matchesCont].movesCont].fc=columns[0];
                        match[matchesCont].moves[match[matchesCont].movesCont].tl=linia;
                        match[matchesCont].moves[match[matchesCont].movesCont].tc=coloana;

                        if(chainJump==false)
                        {
                            teamMoves[movingSide].cont++;
                            teamMoves[movingSide].global++;
                            q=numar-lines[0]+1;
                            itoa(q,p,10);
                            c[0]=97+columns[0]-1;
                            c[1]=NULL;
                            teamMoves[movingSide].t[teamMoves[movingSide].cont].letr[0]=NULL;
                            strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,c);
                            strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,p);

                        }
                        chainJump=true;
                        cont=0;
                        lines[cont]=linia;
                        columns[cont]=coloana;
                        possibleChain=false;
                        checkPossibleMoves(linia,coloana,lines,columns,cont);
                    }
                    else
                    {
                        selection=false;
                        chainJump=false;
                        inGame=true;
                        teamMoves[movingSide].cont++;
                        teamMoves[movingSide].global++;
                        itoa(numar-lines[0]+1,p,10);
                        c[0]=97+columns[0]-1;
                        c[1]=NULL;
                        teamMoves[movingSide].t[teamMoves[movingSide].cont].letr[0]=NULL;
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,c);
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,p);
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr," - ");
                        q=numar-linia+1;
                        itoa(q,p,10);
                        c[0]=97+coloana-1;
                        c[1]=NULL;
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,c);
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,p);

                        movingSide=movingSide==1 ? 2 : 1;
                        moves.cont=0;

                        table[linia][coloana]=table[lines[0]][columns[0]];
                        table[lines[0]][columns[0]]=0;

                        match[matchesCont].movesCont++;
                        match[matchesCont].moves[match[matchesCont].movesCont].fl=lines[0];
                        match[matchesCont].moves[match[matchesCont].movesCont].fc=columns[0];
                        match[matchesCont].moves[match[matchesCont].movesCont].tl=linia;
                        match[matchesCont].moves[match[matchesCont].movesCont].tc=coloana;

                        redrawMoves();
                        //rotateTable();
                        if(gameWon()==1 && bonusMove==false)
                            bonusMove=true;
                        else if(gameWon()==2 || ((gameWon()==1 || gameWon()==3) && bonusMove==true))
                        {
                            endMatch=true;
                            return;
                        }
                    }
                    placeAnimation(linia,coloana);
                }
                if((ismouseclick(WM_RBUTTONDOWN) && (vsPC ? movingSide==playerSide : true)) || (finalPosition==true && vsPC==true && movingSide!=playerSide) || escapeChain==true)
                {
                    clearmouseclick(WM_RBUTTONDOWN);
                    selection=false;
                    if(escapeChain==true && chainJump==false)
                    {
                        escapeChain=false;
                        movingSide=movingSide==1 ? 2 : 1;
                    }
                    finalPosition=false;
                    if(chainJump==true)
                    {
                        chainJump=false;
                        inGame=true;
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr," - ");
                        q=numar-lines[0]+1;
                        itoa(q,p,10);
                        c[0]=97+columns[0]-1;
                        c[1]=NULL;
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,c);
                        strcat(teamMoves[movingSide].t[teamMoves[movingSide].cont].letr,p);
                        movingSide=movingSide==1 ? 2 : 1;
                        moves.cont=0;
                        redrawMoves();
                        //rotateTable();
                        if(gameWon()==1 && bonusMove==false)
                            bonusMove=true;
                        else if(gameWon()==2 || ((gameWon()==1 || gameWon()==3) && bonusMove==true))
                        {
                            endMatch=true;
                            return;
                        }
                    }
                }
            }
            selection=false;
            chainJump=false;
            escapeChain=false;
        }
    }
    else
    {
        clearmouseclick(WM_RBUTTONDOWN);
        x=mousex();
        y=mousey();
        linia=(y-sus)/latura+1;
        coloana=(x-stanga)/latura+1;
        hoverAtPoint(x,y,lines,columns,cont);
    }
}

void chestanimation(int x1,int y1,int x2,int y2)
{
    for(int i=1; i<=35; i++)
    {
        char txt[1000]="assets/animations/chestanimations/achest (",conv[1000];
        itoa(i,conv,10);
        strcat(conv,").gif");
        strcat(txt,conv);
        readimagefile(txt,x1+2,y1+2,x2-2,y2-2);
        delay(0.1);
    }
}

void coinsanimation(int x1,int y1,int x2,int y2,int coins)
{
    int last,i;
    if(coins==1)
        last=10;
    else if(coins==2)
        last=17;
    else if(coins==3)
        last=23;
    else if(coins==4)
        last=27;
    else if(coins==5)
        last=29;
    else if(coins==6)
        last=32;
    else if(coins==7)
        last=36;
    else if(coins==8)
        last=38;
    else if(coins==9)
        last=41;
    else if(coins==10)
        last=50;
    else
        last=58;
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    for(i=1; i<=last; i++)
    {
        char txt[1000]="assets/animations/coinsanimations/acoins (",conv[1000],txt2[1000]="+";
        itoa(i,conv,10);
        strcat(conv,").gif");
        strcat(txt,conv);
        readimagefile(txt,x1+2,y1+2,x2-2,y2-2);
        itoa(i,conv,10);
        strcat(txt2,conv);
        strcat(txt2," banuti");
        if(i<=coins)
            outtextxy(x1+(x2-x1)/2-textwidth(txt2)/2,y1-textheight(txt2),txt2);
        delay(1);
    }
    for(int j=i; j<=coins; j++)
    {
        char txt2[1000]="+",conv[1000];
        itoa(i,conv,10);
        strcat(txt2,conv);
        strcat(txt2," banuti");
        outtextxy(x1+(x2-x1)/2-textwidth(txt2)/2,y1-textheight(txt2),txt2);
    }
}

void loadingscreen()
{
    int r;
    srand(time(NULL));
    r=rand()%144+1;
    for(int i=1; i<=r; i++)
    {
        char txt[1000]="assets/animations/loadingscreen/ls (",conv[1000],txt2[1000]="+";
        itoa(i,conv,10);
        strcat(conv,").gif");
        strcat(txt,conv);
        readimagefile(txt,0,0,getmaxx(),getmaxy());
        if(kbhit())return;
        delay(1);
    }
}

void findininventory(int &p1,int &p2,int &p3,int item)
{
    for(int k=1; k<=inventorypages; k++)
        for(int i=1; i<=4; i++)
            for(int j=1; j<=4; j++)
                if(inventorymatrix[k][i][j].item==item)
                {
                    p1=k;
                    p2=i;
                    p3=j;
                    return;
                }
    if(p1==0)
        for(int k=1; k<=inventorypages; k++)
            for(int i=1; i<=4; i++)
                for(int j=1; j<=4; j++)
                    if(inventorymatrix[k][i][j].quantity==0)
                    {
                        p1=k;
                        p2=i;
                        p3=j;
                        return;
                    }
}

void endMatchScreen()
{
    int r;
    char nc[]="Negrul Castiga",re[]="Remiza",ac[]="Albul castiga";
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    if(teamMoves[1].global==teamMoves[2].global && gameWon()==3)
        outtextxy(getmaxx()/2-textwidth(re)/2,textheight(re)/2,re);
    else if(teamMoves[1].global<=teamMoves[2].global && gameWon()==1)
    {
        if(numejucator1[0]!=NULL)
        {
            strcpy(ac,numejucator1);
            strcat(ac," castiga");
        }
        outtextxy(getmaxx()/2-textwidth(ac)/2,textheight(ac)/2,ac);
    }
    else if(teamMoves[2].global<=teamMoves[1].global && gameWon()==2)
    {
        if(numejucator2[0]!=NULL)
        {
            strcpy(nc,numejucator1);
            strcat(nc," castiga");
        }
        outtextxy(getmaxx()/2-textwidth(nc)/2,textheight(nc)/2,nc);
    }
    else
    {
        if(numejucator2[0]!=NULL)
        {
            strcpy(nc,numejucator1);
            strcat(nc," castiga");
        }
        outtextxy(getmaxx()/2-textwidth(nc)/2,textheight(nc)/2,nc);
    }
    expbar.drawBar(getmaxx()/2-textwidth(nc)/2,getmaxy()/2-textheight(nc)/2,getmaxx()/2+textwidth(nc)/2,getmaxy()/2+textheight(nc)/4,DARKGRAY,WHITE,CYAN);
    showpage();
    expbar.animation(COLOR(255, 184, 77),teamMoves[1].global*(level+1)," pentru mutari alb");
    expbar.animation(YELLOW,teamMoves[2].global*(level+1)," pentru mutari negru");
    settextstyle(10,0,textSize);
    srand (time(NULL));
    r=rand()%3;
    int p1=0,p2=0,p3=0;
    findininventory(p1,p2,p3,1);
    if(r==1 && p1!=0)
    {
        setpage();
        char txt[1000]="Ai gasit un cufar!";
        inventorymatrix[p1][p2][p3].item=1;
        inventorymatrix[p1][p2][p3].quantity++;
        cleardevice();
        textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
        setfillstyle(SOLID_FILL,LIGHTGRAY);
        floodfill(1,1,WHITE);
        setbkcolor(LIGHTGRAY);
        settextstyle(10,0,textSize);
        outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
        readimagefile("assets/animations/chestanimations/achest (1).gif",getmaxx()/2-getmaxy()/4,getmaxy()/2-getmaxy()/4,getmaxx()/2+getmaxy()/4,getmaxy()/2+getmaxy()/4);
        showpage();
        delay(1000);
    }
    srand (time(NULL));
    r=rand()%10;
    p1=0;
    p2=0;
    p3=0;
    findininventory(p1,p2,p3,2);
    if((r==1 || level%5==0) && p1!=0)
    {
        setpage();
        char txt[1000]="Ai gasit o cheie!";
        inventorymatrix[p1][p2][p3].item=2;
        inventorymatrix[p1][p2][p3].quantity++;
        cleardevice();
        textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
        setfillstyle(SOLID_FILL,LIGHTGRAY);
        floodfill(1,1,WHITE);
        setbkcolor(LIGHTGRAY);
        settextstyle(10,0,textSize);
        outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
        readimagefile("assets/animations/key.jpg",getmaxx()/2-getmaxy()/4,getmaxy()/2-getmaxy()/4,getmaxx()/2+getmaxy()/4,getmaxy()/2+getmaxy()/4);
        showpage();
        delay(1000);
    }
    srand (time(NULL));
    r=rand()%2;
    if(r==1 || level%2==0)
    {
        setpage();
        srand (time(NULL));
        int r2=rand()%10+1;
        currency+=r2;
        cleardevice();
        setfillstyle(SOLID_FILL,LIGHTGRAY);
        floodfill(1,1,WHITE);
        setbkcolor(LIGHTGRAY);
        showpage();
        coinsanimation(getmaxx()/2-getmaxy()/4,getmaxy()/2-getmaxy()/4,getmaxx()/2+getmaxy()/4,getmaxy()/2+getmaxy()/4,r2);
        delay(1000);
    }
    Button inapoi,saveGame;
    inapoi.drawButton(getmaxx()/2+textSize,getmaxy()-textheight("I")/2-textSize,getmaxx()/2+getmaxy()/3+textSize,getmaxy()-textSize,"Inapoi",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    saveGame.drawButton(getmaxx()/2-getmaxy()/3-textSize,inapoi.yg1,getmaxx()/2-textSize,inapoi.yg2,"Salveaza meciul",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    do
    {
        saveGame.animation(CYAN);
        inapoi.animation(CYAN);
        if(saveGame.isClicked)
        {
            saveGame.isClicked=false;
            if(saved==false)
            {
                saved=true;
                saveGame.drawButton(saveGame.xg1,saveGame.yg1,saveGame.xg2,saveGame.yg2,"Meci salvat!",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
                delay(500);
                saveGame.drawButton(saveGame.xg1,saveGame.yg1,saveGame.xg2,saveGame.yg2,"Salveaza meciul",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
            }
            else
            {
                saveGame.drawButton(saveGame.xg1,saveGame.yg1,saveGame.xg2,saveGame.yg2,"Meci deja salvat!",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
                delay(500);
                saveGame.drawButton(saveGame.xg1,saveGame.yg1,saveGame.xg2,saveGame.yg2,"Salveaza meciul",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
            }
        }
        checkExitProgram();
    }
    while(inapoi.isClicked==false && tasta!=27);
    mainMenu=true;
}

void GamePvP()
{
    initializeTable();
    setpage();
    drawTable();
    //char pattern[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};
    //setfillpattern(pattern,CYAN);
    showpage();
    char txt[1000]="Undo | ",conv[1000];
    itoa(undorem,conv,10);
    strcat(txt,conv);
    bool undoagain=false;
    undoing=true;
    undo.drawButton(stanga+latura*dimensiune+4,sus+latura*dimensiune+4,getmaxx()-4,getmaxy()-4,txt,width/170>1?width/170:1,DARKGRAY,WHITE);
    backm.drawButton(4,sus+latura*dimensiune+4,stanga-4,getmaxy()-4,"Iesire",width/170>1?width/170:1,DARKGRAY,WHITE);
    do
    {
        if(undoagain==false)
            movePiece();
        undo.animation(CYAN);
        backm.animation(CYAN);
        if(undo.isClicked || undoagain)
        {
            undo.isClicked=false;
            if(vsPC && undoagain==false)
            {
                undoagain=true;
                undorem++;
            }
            else
                undoagain=false;
            if(match[matchesCont].movesCont>0 && undorem>0)
            {
                undorem--;
                int toline=match[matchesCont].moves[match[matchesCont].movesCont].tl;
                int tocolumn=match[matchesCont].moves[match[matchesCont].movesCont].tc;
                int i=match[matchesCont].movesCont;
                while(match[matchesCont].moves[i].fl==match[matchesCont].moves[i-1].tl && match[matchesCont].moves[i].fc==match[matchesCont].moves[i-1].tc)
                    i--;
                int frline=match[matchesCont].moves[i].fl;
                int frcolumn=match[matchesCont].moves[i].fc;
                if(i!=match[matchesCont].movesCont)
                    match[matchesCont].movesCont=i-1;
                else
                    match[matchesCont].movesCont--;
                if(bonusMove==true)
                    bonusMove=false;
                table[frline][frcolumn]=table[toline][tocolumn];
                table[toline][tocolumn]=0;
                setcolor(WHITE);
                setfillstyle(SOLID_FILL,WHITE);
                bar(stanga+latura*(tocolumn-1)+2,sus+latura*(toline-1)+2,stanga+latura*(tocolumn)-2,sus+latura*(toline)-2);
                setlinestyle(SOLID_LINE,0,2);
                setcolor(DARKGRAY);
                rectangle(stanga+latura*(tocolumn-1),sus+latura*(toline-1),stanga+latura*(tocolumn),sus+latura*(toline));
                if(table[frline][frcolumn]==1)
                {
                    if(wh==0)
                    {
                        setlinestyle(SOLID_LINE,0,2);
                        setcolor(DARKGRAY);
                        circle(stanga+latura*(frcolumn-1)+latura/2,sus+latura*(frline-1)+latura/2,9*latura/21-2);
                        setbkcolor(WHITE);
                        setfillstyle(CLOSE_DOT_FILL,DARKGRAY);
                        floodfill(stanga+latura*(frcolumn-1)+latura/2,sus+latura*(frline-1)+latura/2,DARKGRAY);
                    }
                    else
                        readimagefile(wequipped,stanga+latura*(frcolumn-1)+latura/10,sus+latura*(frline-1)+latura/10,stanga+latura*frcolumn-latura/10,sus+latura*frline-latura/10);
                    setlinestyle(SOLID_LINE,0,2);
                    setcolor(DARKGRAY);
                    rectangle(stanga+latura*(frcolumn-1),sus+latura*(frline-1),stanga+latura*(frcolumn),sus+latura*(frline));
                    teamMoves[1].global--;
                    teamMoves[1].cont--;
                }
                else if(table[frline][frcolumn]==2)
                {
                    if(bl==0)
                    {
                        setlinestyle(SOLID_LINE,0,2);
                        setcolor(DARKGRAY);
                        circle(stanga+latura*(frcolumn-1)+latura/2,sus+latura*(frline-1)+latura/2,9*latura/21-2);
                        setbkcolor(BLACK);
                        setfillstyle(LTSLASH_FILL,DARKGRAY);
                        floodfill(stanga+latura*(frcolumn-1)+latura/2,sus+latura*(frline-1)+latura/2,DARKGRAY);
                    }
                    else
                        readimagefile(bequipped,stanga+latura*(frcolumn-1)+latura/10,sus+latura*(frline-1)+latura/10,stanga+latura*frcolumn-latura/10,sus+latura*frline-latura/10);
                    setlinestyle(SOLID_LINE,0,2);
                    setcolor(DARKGRAY);
                    rectangle(stanga+latura*(frcolumn-1),sus+latura*(frline-1),stanga+latura*(frcolumn),sus+latura*(frline));
                    teamMoves[2].global--;
                    teamMoves[2].cont--;
                }
                redrawMoves();
                movingSide=(movingSide==1?2:1);
                itoa(match[matchesCont].movesCont,conv,10);
                strcpy(txt,conv);
                strcat(txt," in meci");
                undo.drawButton(undo.xg1,undo.yg1,undo.xg2,undo.yg2,txt,undo.gtextsize,DARKGRAY,WHITE);
                delay(500);
                strcpy(txt,"Undo | ");
                itoa(undorem,conv,10);
                strcat(txt,conv);
                undo.drawButton(undo.xg1,undo.yg1,undo.xg2,undo.yg2,txt,undo.gtextsize,DARKGRAY,WHITE);
            }
        }
        checkExitProgram();
    }
    while (endMatch==false && tasta!=27 && backm.isClicked==false);
    tasta=26;
    backm.isClicked=false;
    undoing=false;
    if(endMatch==true)
    {
        inGame=false;
        endMatch=false;
        if(teamMoves[1].global<=teamMoves[2].global && gameWon()==1)
            whiteWins++;
        else
            blackWins++;
        totalMatches++;
        endMatchScreen();
        if(saved==true && matchesCont<1000)
        {
            saved=false;
            matchesCont++;
            match[matchesCont-1].tableDimension=dimensiune;
            if(teamMoves[1].global==teamMoves[2].global && gameWon()==3)
                strcpy(match[matchesCont-1].whoWon,"Remiza");
            else if(teamMoves[1].global<=teamMoves[2].global && gameWon()==1)
            {
                if(numejucator1[0]!=NULL)
                    strcpy(match[matchesCont-1].whoWon,numejucator1);
                else
                    strcpy(match[matchesCont-1].whoWon,"Albul");
            }
            else
            {
                if(numejucator2[0]!=NULL)
                    strcpy(match[matchesCont-1].whoWon,numejucator2);
                else
                    strcpy(match[matchesCont-1].whoWon,"Negrul");
            }
        }
    }
}

void menu()
{
    int x,y,lastheight,lastwidth,cont=1,opt=1;
    if(inGame==true)
        opt=2;
    char st[]="Start joc",jn[]="Joc nou",ct[]="Continua joc",tit[]="Din Colt In Colt",inv[]="Inventar",mg[]="Magazin",le[]="o";
    Button sarrow1,sarrow2;
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(tit)/2,textheight(tit)/2,tit);
    lastheight=2*textheight(tit);
    lastwidth=textwidth(tit)/4;
    expbar.resizeBar(getmaxx()/2-lastwidth,lastheight/2+lastheight/4,getmaxx()/2+lastwidth,lastheight/2+lastheight/3,DARKGRAY,WHITE,CYAN);
    settextstyle(10,0,1);
    lastheight+=lastheight/4+2*textheight(tit);
    settextstyle(10,0,textSize-2>1? textSize-2:1);
    if(!inGame)
        start.drawButton(getmaxx()/2-lastwidth,lastheight,getmaxx()/2+lastwidth,lastheight+textheight(st)+textheight(le)/3,st,textSize-2>1?textSize-2:1,DARKGRAY,WHITE);
    else
    {
        start.drawButton(getmaxx()/2-lastwidth,lastheight,getmaxx()/2+lastwidth,lastheight+textheight(jn)+textheight(le)/3,jn,textSize-2>1?textSize-2:1,DARKGRAY,WHITE);
        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
    }
    lastheight+=2*(textheight(st)+textheight(le)/3);
    magazin.drawButton(getmaxx()/2-lastwidth,lastheight,getmaxx()/2+lastwidth,lastheight+textheight(st)+textheight(le)/3,mg,textSize-2>1?textSize-2:1,DARKGRAY,WHITE);
    lastheight+=2*(textheight(st)+textheight(le)/3);
    inventar.drawButton(getmaxx()/2-lastwidth,lastheight,getmaxx()/2+lastwidth,lastheight+textheight(st)+textheight(le)/3,inv,textSize-2>1?textSize-2:1,DARKGRAY,WHITE);
    settextstyle(10,0,textSize-6>1?textSize-6:1);
    meciuri.drawButton(getmaxx()-2*textwidth("Meciuri")-16,getmaxy()-2*textheight("Meciuri")-4,getmaxx()-4,getmaxy()-4,"Meciuri",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    iesire.drawButton(4,getmaxy()-2*textheight("Iesire")-4,2*textwidth("Iesire")+12,getmaxy()-4,"Iesire",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    showpage();
    do
    {
        start.animation(CYAN);
        magazin.animation(CYAN);
        inventar.animation(CYAN);
        meciuri.animation(CYAN);
        iesire.animation(CYAN);
        if(inGame)
        {
            sarrow1.animation(CYAN);
            sarrow2.animation(CYAN);
        }
        if(sarrow1.isClicked)
        {
            sarrow1.isClicked=false;
            if(cont==1)
                cont=opt;
            else
                cont--;
            if(cont==1)
            {
                if(inGame)
                    start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Joc nou",start.gtextsize,start.gborder,start.gincolor);
                else
                    start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Start joc",start.gtextsize,start.gborder,start.gincolor);
            }
            else if(cont==2)
                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Continua joc",start.gtextsize,start.gborder,start.gincolor);
            sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
            sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
        }
        if(sarrow2.isClicked)
        {
            sarrow2.isClicked=false;
            if(cont==opt)
                cont=1;
            else
                cont++;
            if(cont==1)
            {
                if(inGame)
                    start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Joc nou",start.gtextsize,start.gborder,start.gincolor);
                else
                    start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Start joc",start.gtextsize,start.gborder,start.gincolor);
            }
            else if(cont==2)
                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Continua joc",start.gtextsize,start.gborder,start.gincolor);
            sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
            sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
        }
        if(start.isClicked)
        {
            if(cont==1)
            {
                match[matchesCont].movesCont=0;
                start.isClicked=false;
                char txtnm1[1000]="Nume alb:",txtnm2[1000]="Nume negru:";
                int ct=1,lungime1=-1,lungime2=-1;
                typing=true;
                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,txtnm1,start.gtextsize,start.gborder,start.gincolor);
                sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                do
                {
                    sarrow1.animation(CYAN);
                    sarrow2.animation(CYAN);
                    start.animation(CYAN);
                    if(ismouseclick(WM_RBUTTONDOWN))
                    {
                        clearmouseclick(WM_RBUTTONDOWN);
                        return;
                    }
                    if(ct==1 && kbhit())
                    {
                        char key=getch();
                        if(lungime1<10 && key!=8 && key!=27)
                        {
                            lungime1++;
                            numejucator1[lungime1]=key;
                            numejucator1[lungime1+1]=NULL;
                            strcpy(txtnm1,numejucator1);
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,txtnm1,start.gtextsize,start.gborder,start.gincolor);
                        }
                        if(key==8 && lungime1>-1)
                        {
                            numejucator1[lungime1]=NULL;
                            lungime1--;
                            strcpy(txtnm1,numejucator1);
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,txtnm1,start.gtextsize,start.gborder,start.gincolor);
                        }
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                    else if(ct==2 && kbhit())
                    {
                        char key=getch();
                        if(lungime2<10 && key!=8 && key!=27)
                        {
                            lungime2++;
                            numejucator2[lungime2]=key;
                            numejucator2[lungime2+1]=NULL;
                            strcpy(txtnm2,numejucator2);
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,txtnm2,start.gtextsize,start.gborder,start.gincolor);
                        }
                        if(key==8 && lungime2>-1)
                        {
                            numejucator2[lungime2]=NULL;
                            lungime2--;
                            strcpy(txtnm2,numejucator2);
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,txtnm2,start.gtextsize,start.gborder,start.gincolor);
                        }
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                    if(sarrow1.isClicked)
                    {
                        sarrow1.isClicked=false;
                        if(ct==1)
                            ct=2;
                        else
                            ct--;
                        start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,ct==1?txtnm1:txtnm2,start.gtextsize,start.gborder,start.gincolor);
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                    if(sarrow2.isClicked)
                    {
                        sarrow2.isClicked=false;
                        if(ct==2)
                            ct=1;
                        else
                            ct++;
                        start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,ct==1?txtnm1:txtnm2,start.gtextsize,start.gborder,start.gincolor);
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                }
                while(start.isClicked==false && tasta!=27);
                start.isClicked=false;
                typing=false;
                int cont1=4;
                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"Tabla:4x4",start.gtextsize,start.gborder,start.gincolor);
                sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                do
                {
                    sarrow1.animation(CYAN);
                    sarrow2.animation(CYAN);
                    start.animation(CYAN);
                    checkExitProgram();
                    if(ismouseclick(WM_RBUTTONDOWN))
                    {
                        clearmouseclick(WM_RBUTTONDOWN);
                        return;
                    }
                    if(sarrow1.isClicked)
                    {
                        sarrow1.isClicked=false;
                        if(cont1==4)
                            cont1=26;
                        else
                            cont1--;
                        char conv[1000],s[1000]="Tabla:";
                        itoa(cont1,conv,10);
                        strcat(s,conv);
                        strcat(s,"x");
                        strcat(s,conv);
                        start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,s,start.gtextsize,start.gborder,start.gincolor);
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                    if(sarrow2.isClicked)
                    {
                        sarrow2.isClicked=false;
                        if(cont1==26)
                            cont1=4;
                        else
                            cont1++;
                        char conv[1000],s[1000]="Tabla:";
                        itoa(cont1,conv,10);
                        strcat(s,conv);
                        strcat(s,"x");
                        strcat(s,conv);
                        start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,s,start.gtextsize,start.gborder,start.gincolor);
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                }
                while(start.isClicked==false && tasta!=27);
                dimensiune=cont1;
                sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                start.isClicked=false;
                int cont2=1;
                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PvP",start.gtextsize,start.gborder,start.gincolor);
                do
                {
                    sarrow1.animation(CYAN);
                    sarrow2.animation(CYAN);
                    start.animation(CYAN);
                    checkExitProgram();
                    if(ismouseclick(WM_RBUTTONDOWN))
                    {
                        clearmouseclick(WM_RBUTTONDOWN);
                        return;
                    }
                    if(sarrow1.isClicked)
                    {
                        sarrow1.isClicked=false;
                        if(cont2==1)
                            cont2=2;
                        else
                            cont2--;
                        if(cont2==1)
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PvP",start.gtextsize,start.gborder,start.gincolor);
                        else if(cont2==2)
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PvPC",start.gtextsize,start.gborder,start.gincolor);
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                    if(sarrow2.isClicked)
                    {
                        sarrow2.isClicked=false;
                        if(cont2==2)
                            cont2=1;
                        else
                            cont2++;
                        if(cont2==1)
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PvP",start.gtextsize,start.gborder,start.gincolor);
                        else if(cont2==2)
                            start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PvPC",start.gtextsize,start.gborder,start.gincolor);
                        sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                    }
                }
                while(start.isClicked==false && tasta!=27);
                if(cont2==1)
                {
                    inGame=false;
                    vsPC=false;
                }
                else if(cont2==2)
                {
                    inGame=false;
                    vsPC=true;
                    start.isClicked=false;
                    int cont3=1;
                    start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PC:alb",start.gtextsize,start.gborder,start.gincolor);
                    do
                    {
                        sarrow1.animation(CYAN);
                        sarrow2.animation(CYAN);
                        start.animation(CYAN);
                        checkExitProgram();
                        if(ismouseclick(WM_RBUTTONDOWN))
                        {
                            clearmouseclick(WM_RBUTTONDOWN);
                            return;
                        }
                        if(sarrow1.isClicked)
                        {
                            sarrow1.isClicked=false;
                            if(cont3==1)
                                cont3=2;
                            else
                                cont3--;
                            if(cont3==1)
                                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PC:alb",start.gtextsize,start.gborder,start.gincolor);
                            else if(cont3==2)
                                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PC:negru",start.gtextsize,start.gborder,start.gincolor);
                            sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                            sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                        }
                        if(sarrow2.isClicked)
                        {
                            sarrow2.isClicked=false;
                            if(cont3==2)
                                cont3=1;
                            else
                                cont3++;
                            if(cont3==1)
                                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PC:alb",start.gtextsize,start.gborder,start.gincolor);
                            else if(cont3==2)
                                start.drawButton(start.xg1,start.yg1,start.xg2,start.yg2,"PC:negru",start.gtextsize,start.gborder,start.gincolor);
                            sarrow1.drawArrow(start.xg1-textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,2);
                            sarrow2.drawArrow(start.xg2+textSize,start.yg1+(start.yg2-start.yg1)/2,start.yg2-start.yg1,DARKGRAY,WHITE,3);
                        }
                    }
                    while(start.isClicked==false && tasta!=27);
                    if(cont3==1)
                    {
                        playerSide=2;
                    }
                    else if(cont3==2)
                    {
                        playerSide=1;
                    }
                }
                teamMoves[1].cont=0;
                teamMoves[1].global=0;
                teamMoves[2].cont=0;
                teamMoves[2].global=0;
            }
            else if(cont==2)
                inGame=true;
        }
        if(iesire.isClicked)
            tasta=27;
        checkExitProgram();
    }
    while (tasta!=27 && start.isClicked==false && inventar.isClicked==false && magazin.isClicked==false && meciuri.isClicked==false);
}

void rewardScreen()
{
    int r;
    char txt[1000]="Se deschide cufarul",txt2[]="Ai gasit o piesa!",conv[1000];
    Button urmatorul;
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    showpage();
    chestanimation(getmaxx()/2-getmaxy()/4,getmaxy()/2-getmaxy()/4,getmaxx()/2+getmaxy()/4,getmaxy()/2+getmaxy()/4);
    strcpy(txt,"Ai gasit");
    delay(500);
    setpage();
    cleardevice();
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    showpage();
    srand (time(NULL));
    r=rand()%100+1;
    currency+=r;
    coinsanimation(getmaxx()/2-getmaxy()/6,getmaxy()/2-getmaxy()/6,getmaxx()/2+getmaxy()/6,getmaxy()/2+getmaxy()/6,r);
    delay(50);
    urmatorul.drawButton(getmaxx()/2-getmaxy()/6,getmaxy()-textheight(txt)/2-textSize,getmaxx()/2+getmaxy()/6,getmaxy()-textSize,"Inainte",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    do
    {
        urmatorul.animation(CYAN);
        checkExitProgram();
    }
    while(urmatorul.isClicked==false && tasta!=27);
    urmatorul.isClicked=false;
    srand(time(NULL));
    r=1;
    if(r==1)
    {
        srand(time(NULL));
        r=rand()%48+3;
        int p1=0,p2=0,p3=0;
        findininventory(p1,p2,p3,r);
        if(p1!=0 && p2!=0 && p3!=0)
        {
            inventorymatrix[p1][p2][p3].item=r;
            inventorymatrix[p1][p2][p3].quantity++;
            Button rewardpiece;
            rewardpiece.quantity=1;
            rewardpiece.item=r;
            setpage();
            cleardevice();
            rewardpiece.drawButton(getmaxx()/2-getmaxy()/6,getmaxy()/2-getmaxy()/6,getmaxx()/2+getmaxy()/6,getmaxy()/2+getmaxy()/6,"",10,DARKGRAY,WHITE);
            textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
            setfillstyle(SOLID_FILL,LIGHTGRAY);
            floodfill(1,1,WHITE);
            setbkcolor(LIGHTGRAY);
            setcolor(WHITE);
            settextstyle(10,0,textSize);
            outtextxy(getmaxx()/2-textwidth(txt2)/2,textheight(txt2)/2,txt2);
            showpage();
            delay(50);
            urmatorul.drawButton(getmaxx()/2-getmaxy()/6,getmaxy()-textheight(txt)/2-textSize,getmaxx()/2+getmaxy()/6,getmaxy()-textSize,"Inainte",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
            do
            {
                rewardpiece.animation(CYAN);
                if(rewardpiece.isClicked)rewardpiece.isClicked=false;
                urmatorul.animation(CYAN);
                checkExitProgram();
            }
            while(urmatorul.isClicked==false && tasta!=27);
        }
    }
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    srand (time(NULL));
    r=rand()%100+1;
    expbar.drawBar(getmaxx()/2-textwidth("Negrul Castiga")/2,getmaxy()/2-textheight("N")/2,getmaxx()/2+textwidth("Negrul Castiga")/2,getmaxy()/2+textheight("N")/4,DARKGRAY,WHITE,CYAN);
    showpage();
    expbar.animation(BLUE,r*(level+1)," din cufar");
    settextstyle(10,0,textSize);
    Button inapoi;
    inapoi.drawButton(getmaxx()/2-getmaxy()/6,getmaxy()-textheight(txt)/2-textSize,getmaxx()/2+getmaxy()/6,getmaxy()-textSize,"Inapoi",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    do
    {
        inapoi.animation(CYAN);
        checkExitProgram();
    }
    while(inapoi.isClicked==false && tasta!=27);
    inventar.isClicked=true;
}

void loadmarket()
{
    int p=3,maxi=47,k,i,j;
    shoppages=3;
    shopmatrix[1][1][1].item=-1;
    shopmatrix[1][1][1].lvl=0;
    shopmatrix[1][1][1].price=50;
    shopmatrix[1][1][2].item=0;
    shopmatrix[1][1][2].lvl=0;
    shopmatrix[1][1][2].price=800;
    shopmatrix[1][1][3].item=2;
    shopmatrix[1][1][3].lvl=0;
    shopmatrix[1][1][3].price=200;
    for(k=1; k<=shoppages && p<=maxi; k++)
        for(i=1; i<=4 && p<=maxi; i++)
            for((k==1 && i==1)?(j=4):(j=1); j<=4 && p<=maxi; j++)
            {
                shopmatrix[k][i][j].item=p;
                shopmatrix[k][i][j].lvl=p-2;
                shopmatrix[k][i][j].price=p*13+i+j;
                p++;
            }

}

void drawMarket(int cont)
{
    int i,j,lastheight,lastwidth;
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    char txt[1000]="Magazin",conv[1000];
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    itoa(currency,conv,10);
    outtextxy(getmaxx()-textwidth(conv)-textheight(conv)-textSize,textheight(txt)/2-2,conv);
    readimagefile("assets/money.gif",getmaxx()-textSize-textheight(conv),textheight(txt)/2-textSize,getmaxx()-textSize,textheight(txt)/2+textheight(conv)-textSize);
    char txt2[1000]="Lvl:";
    itoa(level,conv,10);
    strcat(txt2,conv);
    outtextxy(textSize,textheight(txt)/2-2,txt2);
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE,0,4);
    line(0,textheight(txt)+textheight(txt)/2,getmaxx(),textheight(txt)+textheight(txt)/2);
    width=getmaxx()/2;
    height=(getmaxy()-textheight(txt)-textheight(txt)/2-textSize);
    latura=width>height? height/4: width/4;
    sus=textheight(txt)+textheight(txt)/2;
    stanga=getmaxx()/2-latura*2;
    Button leftArrow,inapoi;
    Button rightArrow;
    leftArrow.drawArrow(stanga,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,2);
    rightArrow.drawArrow(stanga+latura*4+textSize,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,3);
    inapoi.drawButton(getmaxx()-latura-textSize,getmaxy()-latura/4-textSize,getmaxx()-textSize,getmaxy()-textSize,"Inapoi",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    lastheight=textheight(txt)-latura/6+(sus+latura*4+textSize)/2;
    lastwidth=stanga+latura*4+textSize+latura/3;
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            shopmatrix[cont][i][j].drawButton(stanga+latura*(j-1)+textSize,sus+latura*(i-1)+textSize,stanga+latura*j,sus+latura*i,"",textSize,DARKGRAY,WHITE);
}

void market()
{
    int i,j,cont=1,lastheight,lastwidth,xg1,yg1;
    struct point
    {
        int p1,p2,p3;
    } aux1,aux2;
    mainMenu=false;
    magazin.isClicked=false;
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    char txt[1000]="Magazin",conv[1000];
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    itoa(currency,conv,10);
    xg1=getmaxx()-textwidth(conv)-textheight(conv)-textSize;
    yg1=textheight(txt)/2-2;
    outtextxy(xg1,yg1,conv);
    readimagefile("assets/money.gif",getmaxx()-textSize-textheight(conv),textheight(txt)/2-textSize,getmaxx()-textSize,textheight(txt)/2+textheight(conv)-textSize);
    char txt2[1000]="Lvl:";
    itoa(level,conv,10);
    strcat(txt2,conv);
    outtextxy(textSize,yg1,txt2);
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE,0,4);
    line(0,textheight(txt)+textheight(txt)/2,getmaxx(),textheight(txt)+textheight(txt)/2);
    width=getmaxx()/2;
    height=(getmaxy()-textheight(txt)-textheight(txt)/2-textSize);
    latura=width>height? height/4: width/4;
    sus=textheight(txt)+textheight(txt)/2;
    stanga=getmaxx()/2-latura*2;
    Button leftArrow,inapoi;
    Button rightArrow;
    leftArrow.drawArrow(stanga,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,2);
    rightArrow.drawArrow(stanga+latura*4+textSize,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,3);
    inapoi.drawButton(getmaxx()-latura-textSize,getmaxy()-latura/4-textSize,getmaxx()-textSize,getmaxy()-textSize,"Inapoi",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    lastheight=textheight(txt)-latura/6+(sus+latura*4+textSize)/2;
    lastwidth=stanga+latura*4+textSize+latura/3;
    loadmarket();
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            shopmatrix[cont][i][j].drawButton(stanga+latura*(j-1)+textSize,sus+latura*(i-1)+textSize,stanga+latura*j,sus+latura*i,"",textSize,DARKGRAY,WHITE);
    showpage();
    do
    {
        for(i=1; i<=4; i++)
            for(j=1; j<=4; j++)
            {
                if(shopmatrix[cont][i][j].price>0)
                    shopmatrix[cont][i][j].animation(CYAN);
                if(shopmatrix[cont][i][j].isClicked)
                {
                    shopmatrix[cont][i][j].isClicked=false;
                    Button buy;
                    buy.drawButton(shopmatrix[cont][i][j].xg1,shopmatrix[cont][i][j].yg1,shopmatrix[cont][i][j].xg2,shopmatrix[cont][i][j].yg2,"Cumpara",textSize-5>1?textSize-5:1,shopmatrix[cont][i][j].gborder,shopmatrix[cont][i][j].gincolor);
                    do
                    {
                        buy.animation(CYAN);
                        if(buy.isClicked)
                        {
                            buy.isClicked=false;
                            if(shopmatrix[cont][i][j].price<=currency && cont==1 && i==1 && j<=2)
                            {
                                if(shopmatrix[cont][i][j].item==0 && inventorypages<100)
                                {
                                    inventorypages++;
                                    currency-=shopmatrix[cont][i][j].price;
                                }
                                else if(shopmatrix[cont][i][j].item==-1)
                                {
                                    undorem+=10;
                                    currency-=shopmatrix[cont][i][j].price;
                                }
                                break;
                            }
                            else if(shopmatrix[cont][i][j].price<=currency && shopmatrix[cont][i][j].lvl<=level)
                            {
                                currency-=shopmatrix[cont][i][j].price;
                                int p1=0,p2=0,p3=0;
                                findininventory(p1,p2,p3,shopmatrix[cont][i][j].item);
                                inventorymatrix[p1][p2][p3].item=shopmatrix[cont][i][j].item;
                                inventorymatrix[p1][p2][p3].quantity++;
                                break;
                            }
                            else if(shopmatrix[cont][i][j].price>currency)
                            {
                                settextstyle(10,0,textSize);
                                setcolor(RED);
                                setbkcolor(LIGHTGRAY);
                                char c[1000];
                                itoa(currency,c,10);
                                outtextxy(xg1,yg1,c);
                                delay(100);
                                setcolor(WHITE);
                                setbkcolor(LIGHTGRAY);
                                outtextxy(xg1,yg1,c);
                                delay(100);
                                setcolor(RED);
                                setbkcolor(LIGHTGRAY);
                                outtextxy(xg1,yg1,c);
                                delay(100);
                                setcolor(WHITE);
                                setbkcolor(LIGHTGRAY);
                                outtextxy(xg1,yg1,c);
                                delay(100);
                            }
                            else if(shopmatrix[cont][i][j].lvl>level)
                            {
                                settextstyle(10,0,textSize);
                                setcolor(RED);
                                setbkcolor(LIGHTGRAY);
                                char c[1000],txt[1000]="Lvl:";
                                itoa(level,c,10);
                                strcat(txt,c);
                                outtextxy(textSize,yg1,txt);
                                delay(100);
                                setcolor(WHITE);
                                setbkcolor(LIGHTGRAY);
                                outtextxy(textSize,yg1,txt);
                                delay(100);
                                setcolor(RED);
                                setbkcolor(LIGHTGRAY);
                                outtextxy(textSize,yg1,txt);
                                delay(100);
                                setcolor(WHITE);
                                setbkcolor(LIGHTGRAY);
                                outtextxy(textSize,yg1,txt);
                                delay(100);
                            }
                        }
                    }
                    while(mousex()>=shopmatrix[cont][i][j].xg1 && mousex()<=shopmatrix[cont][i][j].xg2 && mousey()>=shopmatrix[cont][i][j].yg1 && mousey()<=shopmatrix[cont][i][j].yg2);
                    setpage();
                    drawMarket(cont);
                    showpage();
                }
            }
        inapoi.animation(CYAN);
        if(leftArrow.isClicked)
        {
            leftArrow.isClicked=false;
            if(cont==1)
                cont=shoppages;
            else
                cont--;
            setpage();
            drawMarket(cont);
            showpage();
        }
        else if(rightArrow.isClicked)
        {
            rightArrow.isClicked=false;
            if(cont==shoppages)
                cont=1;
            else
                cont++;
            setpage();
            drawMarket(cont);
            showpage();
        }
        rightArrow.animation(CYAN);
        leftArrow.animation(CYAN);
        checkExitProgram();
    }
    while(tasta!=27 && inapoi.isClicked==false);
    inapoi.isClicked=false;
    mainMenu=true;
    return;
}

void drawInventory(int cont)
{
    int i,j,lastheight,lastwidth;
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    char txt[1000]="Inventar",conv[1000];
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    itoa(currency,conv,10);
    outtextxy(getmaxx()-textwidth(conv)-textheight(conv)-textSize,textheight(txt)/2,conv);
    readimagefile("assets/money.gif",getmaxx()-textSize-textheight(conv),textheight(txt)/2-textSize,getmaxx()-textSize,textheight(txt)/2+textheight(conv)-textSize);
    char txt2[1000]="Lvl:";
    itoa(level,conv,10);
    strcat(txt2,conv);
    outtextxy(textSize,textheight(txt)/2-2,txt2);
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE,0,4);
    line(0,textheight(txt)+textheight(txt)/2,getmaxx(),textheight(txt)+textheight(txt)/2);
    width=getmaxx()/2;
    height=(getmaxy()-textheight(txt)-textheight(txt)/2-textSize);
    latura=width>height? height/4: width/4;
    sus=textheight(txt)+textheight(txt)/2;
    stanga=latura/3;
    Button leftArrow,inapoi;
    Button rightArrow;
    if(inventorypages>1)
    {
        leftArrow.drawArrow(stanga,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,2);
        rightArrow.drawArrow(stanga+latura*4+textSize,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,3);
    }
    inapoi.drawButton(getmaxx()-latura-textSize,getmaxy()-latura/4-textSize,getmaxx()-textSize,getmaxy()-textSize,"Inapoi",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    lastheight=textheight(txt)-latura/6+(sus+latura*4+textSize)/2;
    lastwidth=stanga+latura*4+textSize+latura/3;
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            inventorymatrix[cont][i][j].drawButton(stanga+latura*(j-1)+textSize,sus+latura*(i-1)+textSize,stanga+latura*j,sus+latura*i,"",textSize,DARKGRAY,WHITE);
}

void inventory()
{
    int i,j,cont=1,cont2=1,lastheight,lastwidth,openstate=0;
    bool equip=false;
    struct point
    {
        int p1,p2,p3;
    } aux1,aux2;
    mainMenu=false;
    inventar.isClicked=false;
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    char txt[1000]="Inventar",conv[1000];
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,WHITE);
    setbkcolor(LIGHTGRAY);
    settextstyle(10,0,textSize);
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    itoa(currency,conv,10);
    outtextxy(getmaxx()-textwidth(conv)-textheight(conv)-textSize,textheight(txt)/2,conv);
    readimagefile("assets/money.gif",getmaxx()-textSize-textheight(conv),textheight(txt)/2-textSize,getmaxx()-textSize,textheight(txt)/2+textheight(conv)-textSize);
    char txt2[1000]="Lvl:";
    itoa(level,conv,10);
    strcat(txt2,conv);
    outtextxy(textSize,textheight(txt)/2-2,txt2);
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE,0,4);
    line(0,textheight(txt)+textheight(txt)/2,getmaxx(),textheight(txt)+textheight(txt)/2);
    width=getmaxx()/2;
    height=(getmaxy()-textheight(txt)-textheight(txt)/2-textSize);
    latura=width>height? height/4: width/4;
    sus=textheight(txt)+textheight(txt)/2;
    stanga=latura/3;
    Button leftArrow,inapoi;
    Button rightArrow;
    if(inventorypages>1)
    {
        leftArrow.drawArrow(stanga,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,2);
        rightArrow.drawArrow(stanga+latura*4+textSize,textheight(txt)-latura/6+(sus+latura*4+textSize)/2,latura/3,DARKGRAY,WHITE,3);
    }
    inapoi.drawButton(getmaxx()-latura-textSize,getmaxy()-latura/4-textSize,getmaxx()-textSize,getmaxy()-textSize,"Inapoi",textSize-6>1?textSize-6:1,DARKGRAY,WHITE);
    lastheight=textheight(txt)-latura/6+(sus+latura*4+textSize)/2;
    lastwidth=stanga+latura*4+textSize+latura/3;
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            inventorymatrix[1][i][j].drawButton(stanga+latura*(j-1)+textSize,sus+latura*(i-1)+textSize,stanga+latura*j,sus+latura*i,"",textSize,DARKGRAY,WHITE);
    showpage();
    Button chest,key,open,equipbox,equiptxt1,equiptxt2,equiptxt3,sarrow1,sarrow2;
    do
    {
        for(i=1; i<=4; i++)
            for(j=1; j<=4; j++)
            {
                inapoi.animation(CYAN);
                if(inventorymatrix[cont][i][j].quantity>0)
                    inventorymatrix[cont][i][j].animation(CYAN);
                if(inventorymatrix[cont][i][j].isClicked)
                {
                    inventorymatrix[cont][i][j].isClicked=false;
                    if(inventorymatrix[cont][i][j].item==1 && inventorymatrix[cont][i][j].quantity>0 && openstate!=1)
                    {
                        aux1.p1=cont;
                        aux1.p2=i;
                        aux1.p3=j;
                        if(openstate==2)
                        {
                            chest.item=1;
                            chest.quantity=1;
                            openstate=3;
                        }
                        else if(openstate==0)
                        {
                            openstate=1;
                            chest.item=1;
                            chest.quantity=1;
                            key.quantity=0;
                        }
                        setpage();
                        drawInventory(cont);
                        chest.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                        key.drawButton(lastwidth+(getmaxx()-lastwidth)/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                        open.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+latura/3+textSize,"Deschide",textSize>5?textSize-5:1,DARKGRAY,WHITE);
                        showpage();
                    }
                    if(inventorymatrix[cont][i][j].item==2 && inventorymatrix[cont][i][j].quantity>0 && openstate!=2)
                    {
                        aux2.p1=cont;
                        aux2.p2=i;
                        aux2.p3=j;
                        if(openstate==1)
                        {
                            key.item=2;
                            key.quantity=1;
                            openstate=3;
                        }
                        else if(openstate==0)
                        {
                            openstate=2;
                            key.item=2;
                            key.quantity=1;
                            chest.quantity=0;
                        }
                        setpage();
                        drawInventory(cont);
                        chest.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                        key.drawButton(lastwidth+(getmaxx()-lastwidth)/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                        open.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+latura/3+textSize,"Deschide",textSize>5?textSize-5:1,DARKGRAY,WHITE);
                        showpage();
                    }
                    if(inventorymatrix[cont][i][j].item>=3 && inventorymatrix[cont][i][j].quantity>0)
                    {
                        equip=true;
                        openstate=0;
                        equipbox.item=inventorymatrix[cont][i][j].item;
                        equipbox.quantity=1;
                        setpage();
                        drawInventory(cont);
                        equipbox.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                        equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                        sarrow1.drawArrow(equiptxt1.xg1-textSize,equiptxt1.yg1+(equiptxt1.yg2-equiptxt1.yg1)/2,equiptxt1.yg2-equiptxt1.yg1,DARKGRAY,WHITE,2);
                        sarrow2.drawArrow(equiptxt1.xg2+textSize,equiptxt1.yg1+(equiptxt1.yg2-equiptxt1.yg1)/2,equiptxt1.yg2-equiptxt1.yg1,DARKGRAY,WHITE,3);
                        showpage();
                    }
                }
            }
        if(openstate!=0)
        {
            open.animation(CYAN);
            if(open.isClicked)
            {
                open.isClicked=false;
                if(key.quantity>0 && chest.quantity>0 && openstate==3)
                {
                    inventorymatrix[aux1.p1][aux1.p2][aux1.p3].quantity--;
                    inventorymatrix[aux2.p1][aux2.p2][aux2.p3].quantity--;
                    openstate=0;
                    key.quantity=0;
                    chest.quantity=0;
                    setfillstyle(SOLID_FILL,LIGHTGRAY);
                    bar(lastwidth,sus+textSize,getmaxx(),getmaxy());
                    rewardScreen();
                    return;
                }
                else if(key.quantity==0)
                    open.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+latura/3+textSize,"Introdu o cheie!",textSize>5?textSize-5:1,DARKGRAY,WHITE);

                else if(chest.quantity==0)
                    open.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+latura/3+textSize,"Introdu un cufar!",textSize>5?textSize-5:1,DARKGRAY,WHITE);
            }
        }
        if(equip && openstate==0)
        {

            if(cont2==1)
                equiptxt1.animation(CYAN);
            else if(cont2==2)
                equiptxt2.animation(CYAN);
            else if(cont2==3)
                equiptxt3.animation(CYAN);
            sarrow1.animation(CYAN);
            sarrow2.animation(CYAN);
            if(sarrow1.isClicked)
            {
                sarrow1.isClicked=false;
                if(cont2==1)
                    cont2=3;
                else
                    cont2--;
                if(cont2==1)
                    equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                else if(cont2==2)
                    equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Negru",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                else if(cont2==3)
                    equiptxt3.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Reset",textSize>7?textSize-7:1,DARKGRAY,WHITE);
            }
            if(sarrow2.isClicked)
            {
                sarrow2.isClicked=false;
                if(cont2==3)
                    cont2=1;
                else
                    cont2++;
                if(cont2==1)
                    equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                else if(cont2==2)
                    equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Negru",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                else if(cont2==3)
                    equiptxt3.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Reset",textSize>7?textSize-7:1,DARKGRAY,WHITE);
            }
            if(equiptxt3.isClicked)
            {
                equiptxt3.isClicked=false;
                wh=0;
                bl=0;
                equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Resetat!",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                delay(500);
                equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Reset",textSize>7?textSize-7:1,DARKGRAY,WHITE);
            }
            if(equiptxt1.isClicked && equipbox.item!=bl && equipbox.item!=wh)
            {
                equiptxt1.isClicked=false;
                wh=equipbox.item;
                equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Echipat!",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                delay(500);
                equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
            }
            else if(equiptxt1.isClicked)
            {
                equiptxt1.isClicked=false;
                equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Deja echipat!",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                delay(500);
                equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
            }
            if(equiptxt2.isClicked && equipbox.item!=wh && equipbox.item!=bl)
            {
                equiptxt2.isClicked=false;
                bl=equipbox.item;
                equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Echipat!",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                delay(500);
                equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Negru",textSize>7?textSize-7:1,DARKGRAY,WHITE);

            }
            else if(equiptxt2.isClicked)
            {
                equiptxt2.isClicked=false;
                equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Deja echipat!",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                delay(500);
                equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Negru",textSize>7?textSize-7:1,DARKGRAY,WHITE);
            }
        }
        if(leftArrow.isClicked)
        {
            leftArrow.isClicked=false;
            if(cont==1)
                cont=inventorypages;
            else
                cont--;
            if(inventorypages>1)
            {
                setpage();
                if(openstate==0 && equip==false)
                    drawInventory(cont);
                else if(openstate>0)
                {
                    drawInventory(cont);
                    chest.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                    key.drawButton(lastwidth+(getmaxx()-lastwidth)/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                    open.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+latura/3+textSize,"Deschide",textSize>5?textSize-5:1,DARKGRAY,WHITE);
                }
                else if(equip)
                {
                    drawInventory(cont);
                    equipbox.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                    sarrow1.drawArrow(equiptxt1.xg1-textSize,equiptxt1.yg1+(equiptxt1.yg2-equiptxt1.yg1)/2,equiptxt1.yg2-equiptxt1.yg1,DARKGRAY,WHITE,2);
                    sarrow2.drawArrow(equiptxt1.xg2+textSize,equiptxt1.yg1+(equiptxt1.yg2-equiptxt1.yg1)/2,equiptxt1.yg2-equiptxt1.yg1,DARKGRAY,WHITE,3);
                    if(cont2==1)
                        equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                    else if(cont2==2)
                        equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Negru",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                    else if(cont2==3)
                        equiptxt3.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Reset",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                }
                showpage();
            }
        }
        else if(rightArrow.isClicked)
        {
            rightArrow.isClicked=false;
            if(cont==inventorypages)
                cont=1;
            else
                cont++;
            if(inventorypages>1)
            {
                setpage();
                if(openstate==0 && equip==false)
                    drawInventory(cont);
                else if(openstate>0)
                {
                    drawInventory(cont);
                    chest.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                    key.drawButton(lastwidth+(getmaxx()-lastwidth)/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                    open.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura,sus+latura+latura/3+textSize,"Deschide",textSize>5?textSize-5:1,DARKGRAY,WHITE);
                }
                else if(equip)
                {
                    drawInventory(cont);
                    equipbox.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+textSize,"",textSize,DARKGRAY,WHITE);
                    sarrow1.drawArrow(equiptxt1.xg1-textSize,equiptxt1.yg1+(equiptxt1.yg2-equiptxt1.yg1)/2,equiptxt1.yg2-equiptxt1.yg1,DARKGRAY,WHITE,2);
                    sarrow2.drawArrow(equiptxt1.xg2+textSize,equiptxt1.yg1+(equiptxt1.yg2-equiptxt1.yg1)/2,equiptxt1.yg2-equiptxt1.yg1,DARKGRAY,WHITE,3);
                    if(cont2==1)
                        equiptxt1.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Alb",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                    else if(cont2==2)
                        equiptxt2.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Negru",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                    else if(cont2==3)
                        equiptxt3.drawButton(lastwidth+(getmaxx()-lastwidth)/2-latura/2,sus+latura+2*textSize,lastwidth+(getmaxx()-lastwidth)/2+latura/2,sus+latura+latura/3,"Reset",textSize>7?textSize-7:1,DARKGRAY,WHITE);
                }
                showpage();
            }
        }
        if(inventorypages>1)
        {
            leftArrow.animation(CYAN);
            rightArrow.animation(CYAN);
        }
        checkExitProgram();
    }
    while (tasta!=27 && inapoi.isClicked==false);
    inapoi.isClicked=false;
    mainMenu=true;
}

void replayMatch(int m)
{
    int lines[17],columns[17],restore=movingSide,restore1=teamMoves[1].global,restore2=teamMoves[2].global,restore3=dimensiune,restore4,bl,bc,el,ec;
    bool endJump=true,restore5=vsPC;
    vsPC=false;
    clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_RBUTTONDOWN);
    numar=match[m].tableDimension;
    dimensiune=match[m].tableDimension;
    width=getmaxx()/2;
    height=width;
    latura=width/numar;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
    if(latura/20>=1)
        textSize=latura/20;
    else
        textSize=1;
    for(int i=1; i<=match[m].tableDimension; i++)
        for(int j=1; j<=match[m].tableDimension; j++)
            auxtable[i][j]=0;
    auxtable[match[m].tableDimension][1]=1;
    auxtable[match[m].tableDimension][2]=1;
    auxtable[match[m].tableDimension][3]=1;
    auxtable[match[m].tableDimension-1][1]=1;
    auxtable[match[m].tableDimension-2][1]=1;
    auxtable[match[m].tableDimension-1][2]=1;
    auxtable[1][match[m].tableDimension]=2;
    auxtable[2][match[m].tableDimension]=2;
    auxtable[3][match[m].tableDimension]=2;
    auxtable[1][match[m].tableDimension-1]=2;
    auxtable[1][match[m].tableDimension-2]=2;
    auxtable[2][match[m].tableDimension-1]=2;
    teamMoves[1].global=0;
    teamMoves[2].global=0;
    teamMoves[1].cont=0;
    teamMoves[2].cont=0;
    movingSide=1;
    for(int i=0; i<=match[m].tableDimension+1; i++)
    {
        auxtable[0][i]=-1;
        auxtable[i][0]=-1;
        auxtable[match[m].tableDimension+1][i]=-1;
        auxtable[i][match[m].tableDimension+1]=-1;
    }
    replaying=true;
    setpage();
    drawTable();
    showpage();
    selection=true;
    for(int i=1; i<=match[m].movesCont && tasta!=27; i++)
    {
        checkExitProgram();
        lines[0]=match[m].moves[i].fl;
        columns[0]=match[m].moves[i].fc;
        lines[1]=match[m].moves[i].tl;
        columns[1]=match[m].moves[i].tc;
        if(endJump==true)
        {
            bl=match[m].moves[i].fl;
            bc=match[m].moves[i].fc;
        }
        hoverAtPoint(0,0,lines,columns,1);
        auxtable[match[m].moves[i].tl][match[m].moves[i].tc]=auxtable[match[m].moves[i].fl][match[m].moves[i].fc];
        auxtable[match[m].moves[i].fl][match[m].moves[i].fc]=0;
        if(auxtable[match[m].moves[i+1].fl][match[m].moves[i+1].fc]!=auxtable[match[m].moves[i].tl][match[m].moves[i].tc])
        {
            char c[100],p[100];
            int q;
            endJump=true;
            el=match[m].moves[i].tl;
            ec=match[m].moves[i].tc;
            teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont++;
            teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].global++;
            itoa(numar-bl+1,p,10);
            c[0]=97+bc-1;
            c[1]=NULL;
            teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].t[teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont].letr[0]=NULL;
            strcat(teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].t[teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont].letr,c);
            strcat(teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].t[teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont].letr,p);
            strcat(teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].t[teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont].letr," - ");
            q=numar-el+1;
            itoa(q,p,10);
            c[0]=97+ec-1;
            c[1]=NULL;
            strcat(teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].t[teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont].letr,c);
            strcat(teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].t[teamMoves[auxtable[match[m].moves[i].tl][match[m].moves[i].tc]].cont].letr,p);
            movingSide=movingSide==1 ? 2 : 1;

        }
        else
            endJump=false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL,WHITE);
        bar(stanga+latura*(columns[0]-1)+2,sus+latura*(lines[0]-1)+2,stanga+latura*(columns[0])-2,sus+latura*(lines[0])-2);
        setlinestyle(SOLID_LINE,0,2);
        setcolor(DARKGRAY);
        rectangle(stanga+latura*(columns[0]-1),sus+latura*(lines[0]-1),stanga+latura*(columns[0]),sus+latura*(lines[0]));
        if(auxtable[lines[1]][columns[1]]==1)
        {
            setlinestyle(SOLID_LINE,0,2);
            setcolor(DARKGRAY);
            circle(stanga+latura*(columns[1]-1)+latura/2,sus+latura*(lines[1]-1)+latura/2,9*latura/21-2);
            setbkcolor(WHITE);
            setfillstyle(CLOSE_DOT_FILL,DARKGRAY);
            floodfill(stanga+latura*(columns[1]-1)+latura/2,sus+latura*(lines[1]-1)+latura/2,DARKGRAY);
            setlinestyle(SOLID_LINE,0,2);
            setcolor(DARKGRAY);
            rectangle(stanga+latura*(columns[1]-1),sus+latura*(lines[1]-1),stanga+latura*(columns[1]),sus+latura*(lines[1]));
        }
        else if(auxtable[lines[1]][columns[1]]==2)
        {
            setlinestyle(SOLID_LINE,0,2);
            setcolor(DARKGRAY);
            circle(stanga+latura*(columns[1]-1)+latura/2,sus+latura*(lines[1]-1)+latura/2,9*latura/21-2);
            setbkcolor(BLACK);
            setfillstyle(LTSLASH_FILL,DARKGRAY);
            floodfill(stanga+latura*(columns[1]-1)+latura/2,sus+latura*(lines[1]-1)+latura/2,DARKGRAY);
            setlinestyle(SOLID_LINE,0,2);
            setcolor(DARKGRAY);
            rectangle(stanga+latura*(columns[1]-1),sus+latura*(lines[1]-1),stanga+latura*(columns[1]),sus+latura*(lines[1]));
        }
        redrawMoves();
    }
    if(tasta==27)
        tasta=26;
    selection=false;
    replaying=false;
    movingSide=restore;
    vsPC=restore5;
    teamMoves[1].global=restore1;
    teamMoves[2].global=restore2;
    dimensiune=restore3;
}

void showMatches(int first,int last,bool drawarrows)
{
    int i=first,upheight,downheight,lat;
    bool morepages=false;
    setpage();
    cleardevice();
    textSize=getmaxx()/190>=1 ? getmaxx()/190 : 1;
    char txt[1000]="A castigat: ",conv[1000];
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(1,1,LIGHTGRAY);
    setbkcolor(LIGHTGRAY);
    setcolor(WHITE);
    settextstyle(10,0,textSize-2>0?textSize-2:1);
    itoa(whiteWins,conv,10);
    strcat(txt,conv);
    strcat(txt,"/");
    itoa(totalMatches,conv,10);
    strcat(txt,conv);
    strcat(txt," Alb | ");
    itoa(blackWins,conv,10);
    strcat(txt,conv);
    strcat(txt,"/");
    itoa(totalMatches,conv,10);
    strcat(txt,conv);
    strcat(txt," Negru");
    outtextxy(getmaxx()/2-textwidth(txt)/2,textheight(txt)/2,txt);
    lat=textwidth(txt)/2;
    setcolor(DARKGRAY);
    setlinestyle(SOLID_LINE,0,4);
    line(0,textheight(txt)+textheight(txt)/2,getmaxx(),textheight(txt)+textheight(txt)/2);
    upheight=textheight(txt)+textheight(txt)/2+textSize;
    settextstyle(10,0,textSize-3>0?textSize-3:1);
    strcpy(txt,"Inapoi");
    textSize=textSize-6>1?textSize-6:1;
    inp.drawButton(getmaxx()/2-textwidth(txt)/2,getmaxy()-textheight(txt)-4,getmaxx()/2+textwidth(txt)/2,getmaxy()-4,txt,textSize,DARKGRAY,WHITE);
    downheight=upheight+2*textheight(txt);
    while(i<matchesCont && downheight<getmaxy()-2*textheight(txt)-4)
    {
        char conv[1000],txt2[1000]="Meciul ";
        itoa(i+1,conv,10);
        strcat(txt2,conv);
        strcat(txt2," | ");
        itoa(match[i].tableDimension,conv,10);
        strcat(txt2,"Tabla ");
        strcat(txt2,conv);
        strcat(txt2,"x");
        strcat(txt2,conv);
        strcat(txt2," | ");
        if(strcmp(match[i].whoWon,"Remiza")==0)
            strcat(txt2,"Remiza");
        else
        {
            strcat(txt2,match[i].whoWon);
            strcat(txt2," a castigat");
        }
        matchesToShow[i].drawButton(getmaxx()/2-lat,upheight,getmaxx()/2+lat,downheight,txt2,textSize,DARKGRAY,WHITE);
        upheight=downheight+2*textSize;
        downheight=upheight+2*textheight(txt);
        i++;
    }
    if((downheight>getmaxy()-2*textheight(txt)-4 && i<matchesCont) || drawarrows==true)
    {
        morepages=true;
        if(last==0)
            last=i;
        if(i<matchesCont)
            rarrow.drawArrow(inp.xg2+2*textSize,inp.yg1+(inp.yg2-inp.yg1)/2,inp.yg2-inp.yg1,DARKGRAY,WHITE,3);
        if(first!=0)
            larrow.drawArrow(inp.xg1-2*textSize,inp.yg1+(inp.yg2-inp.yg1)/2,inp.yg2-inp.yg1,DARKGRAY,WHITE,2);
    }
    showpage();
    do
    {
        for(int j=first; j<i; j++)
        {
            matchesToShow[j].animation(CYAN);
            if(matchesToShow[j].isClicked)
            {
                matchesToShow[j].isClicked=false;
                replayMatch(j);
                return;
            }
        }
        if(morepages==true)
        {
            if(i<matchesCont)
                rarrow.animation(CYAN);
            if(first!=0)
                larrow.animation(CYAN);
        }
        if(rarrow.isClicked && i<matchesCont)
        {
            rarrow.isClicked=false;
            showMatches(i,last,true);
            break;
        }
        if(larrow.isClicked)
        {
            first-=last;
            larrow.isClicked=false;
            showMatches(first,last,true);
            break;
        }
        inp.animation(CYAN);
        checkExitProgram();
    }
    while(inp.isClicked==false && tasta!=27);
    inp.isClicked=false;
}

void readData()
{
    f1>>inGame;
    f1>>lastexp;
    f1>>lastpos;
    f1>>progress;
    f1>>level;
    f1>>experience;
    f1>>inventorypages;
    f1>>undorem;
    f1>>currency;
    f1>>wh;
    f1>>bl;
    f1>>totalMatches;
    f1>>whiteWins;
    f1>>blackWins;
    f1>>matchesCont;

    for(int i=0; i<matchesCont; i++)
    {
        f1>>match[i].tableDimension;
        f1>>match[i].movesCont;
        int lungime;
        char c;
        f1>>lungime;
        for(int j=0; j<lungime; j++)
        {
            f1>>c;
            match[i].whoWon[j]=c;
        }
        match[i].whoWon[lungime]=NULL;
        for(int j=1; j<=match[i].movesCont; j++)
            f1>>match[i].moves[j].fl>>match[i].moves[j].fc>>match[i].moves[j].tl>>match[i].moves[j].tc;
    }

    for(int k=1; k<=inventorypages; k++)
        for(int i=1; i<=4; i++)
            for(int j=1; j<=4; j++)
                f1>>inventorymatrix[k][i][j].quantity>>inventorymatrix[k][i][j].item;
    if(inGame)
    {
        f1>>dimensiune;
        f1>>playerSide;
        f1>>vsPC;
        for(int i=0; i<=dimensiune+1; i++)
        {
            for(int j=0; j<=dimensiune+1; j++)
                f1>>table[i][j];
        }
        f1>>match[matchesCont].movesCont;
        for(int i=1; i<=match[matchesCont].movesCont; i++)
            f1>>match[matchesCont].moves[i].fl>>match[matchesCont].moves[i].fc>>match[matchesCont].moves[i].tl>>match[matchesCont].moves[i].tc;
        f1>>teamMoves[1].global;
        f1>>teamMoves[2].global;
        f1>>movingSide;

        int lungime1,lungime2;
        char c;
        f1>>lungime1;
        for(int i=0; i<lungime1; i++)
        {
            f1>>c;
            numejucator1[i]=c;
        }
        numejucator1[lungime1]=NULL;
        f1>>lungime2;
        for(int i=0; i<lungime2; i++)
        {
            f1>>c;
            numejucator2[i]=c;
        }
        numejucator2[lungime2]=NULL;
    }
    if(experience==0)
        experience=100;
}

void writeData()
{
    f1.clear();
    f1.seekp(0, ios_base::beg);
    f1<<inGame<<"\n";
    f1<<lastexp<<"\n";
    f1<<lastpos<<"\n";
    f1<<progress<<"\n";
    f1<<level<<"\n";
    f1<<experience<<"\n";
    f1<<inventorypages<<"\n";
    f1<<undorem<<"\n";
    f1<<currency<<"\n";
    f1<<wh<<"\n";
    f1<<bl<<"\n";
    f1<<totalMatches<<"\n";
    f1<<whiteWins<<"\n";
    f1<<blackWins<<"\n";
    f1<<matchesCont<<"\n";

    for(int i=0; i<matchesCont; i++)
    {
        f1<<match[i].tableDimension<<" ";
        f1<<match[i].movesCont<<" ";
        f1<<strlen(match[i].whoWon)<<" ";
        f1<<match[i].whoWon<<" ";
        for(int j=1; j<=match[i].movesCont; j++)
            f1<<match[i].moves[j].fl<<" "<<match[i].moves[j].fc<<" "<<match[i].moves[j].tl<<" "<<match[i].moves[j].tc<<" ";
        f1<<"\n";
    }

    for(int k=1; k<=inventorypages; k++)
    {
        for(int i=1; i<=4; i++)
        {
            for(int j=1; j<=4; j++)
                if(inventorymatrix[k][i][j].quantity>0)
                    f1<<inventorymatrix[k][i][j].quantity<<" "<<inventorymatrix[k][i][j].item<<" ";
                else
                    f1<<0<<" "<<0<<" ";
            f1<<"\n";
        }
    }
    if(inGame)
    {
        f1<<dimensiune<<"\n";
        f1<<playerSide<<"\n";
        f1<<vsPC<<"\n";

        for(int i=0; i<=dimensiune+1; i++)
        {
            for(int j=0; j<=dimensiune+1; j++)
                f1<<table[i][j]<<" ";
            f1<<"\n";
        }
        f1<<match[matchesCont].movesCont<<" ";
        for(int i=1; i<=match[matchesCont].movesCont; i++)
            f1<<match[matchesCont].moves[i].fl<<" "<<match[matchesCont].moves[i].fc<<" "<<match[matchesCont].moves[i].tl<<" "<<match[matchesCont].moves[i].tc<<" ";
        f1<<"\n";
        f1<<teamMoves[1].global<<"\n";
        f1<<teamMoves[2].global<<"\n";
        f1<<movingSide<<"\n";

        f1<<strlen(numejucator1)<<"\n";
        f1<<numejucator1<<"\n";
        f1<<strlen(numejucator2)<<"\n";
        f1<<numejucator2<<"\n";
    }
}

int main()
{
    DWORD screenwidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenheight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenwidth,screenheight,"",-3,-3);
    loadingscreen();
    //initwindow(400,300);
    readData();
    inventorymatrix[1][1][2].quantity=1;
    inventorymatrix[1][1][2].item=1;
    inventorymatrix[1][2][2].quantity=1;
    inventorymatrix[1][2][2].item=2;
    inventorymatrix[1][3][3].quantity=1;
    inventorymatrix[1][3][3].item=3;
    do
    {
        if(mainMenu==true)
            menu();
        if(start.isClicked)
            GamePvP();
        if(magazin.isClicked)
            market();
        if(inventar.isClicked)
            inventory();
        if(meciuri.isClicked)
            showMatches(0,0,false);
        checkExitProgram();
    }
    while(tasta!=27);
    writeData();
    closegraph();
    return 0;
}
