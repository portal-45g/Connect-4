#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
// windows.h is windows exclusive file,used here for "system("<cmd commands>")" ,"Beep(,)"
// and "memset(,,,)" functions;
#include<MMsystem.h>
// this is a c++ header file,working in my machine as i use Code::Blocks compiler
// used here for "playsound(,,)" ,


COORD coord={0,0};
typedef struct {int pt;int u1c;int u2c;} Undo; //"U"ndo player "1" "C"ount

void drop_coin(char*, int*, int, int*,int*,int*);
void printb(char*);//print board
char symbol(int*);//X or O
char player(int*);// 1 or 2
int horizontalCheck(char*);
int checkFour(char*, int , int , int , int );
int verticalCheck(char*);
int sdiagonalCheck(char*);
int pdiagonalCheck(char*);
int checkWin(char*);
void popout_coin(char*,int*,Undo*,int*);
void gotoxy(int ,int );
void save_game(char*,int*,Undo*,int*);
int savecheck();
int title(int);
void tie();
void flip_coin();
void how_to_play();

void main()
{
    char *board;
    int n=42;//7 columns *6 rows
    board=(char*)malloc(n*sizeof(char));
    if(board==NULL)
        {
            gotoxy(28,10);
            printf("Unable to start the game.");
            gotoxy(29,11);
            printf("(Press any key to exit)");
            getchar();
            return;
        }
    system("color 0f");
    char com[3];
    com[2]=' ';
    Undo undo_record;
    undo_record.pt=-1;
    undo_record.u1c=0;
    undo_record.u2c=0;
    memset(board,' ',42);
    int i;
    int top[7]={-1,-1,-1,-1,-1,-1,-1};
    int *op;
    *op =1;
    int f=0;
    int t;
    A:
    switch (title(savecheck()))
    {
        case 0:
            {
            FILE *fs;
            fs = fopen("save.dat","r");
            fgets(board,42,fs);
            fclose(fs);
            fflush(fs);
            fs = fopen("top.dat","r");
            fscanf(fs,"%d %d %d %d %d %d %d",&top[0],&top[1],&top[2],&top[3],&top[4],&top[5],&top[6]);
            fclose(fs);
            fflush(fs);
            fs = fopen("count.dat","r");
            fscanf(fs,"%d %d %d %d",op,&undo_record.pt,&undo_record.u1c,&undo_record.u2c);
            fclose(fs);
            fflush(fs);
            break;
            }
        case 1:
            {
                flip_coin();
                break;
            }
        case 2:
            {
            how_to_play();
            goto A;
            }

        case 3:
            {
                return;
            }
        }
    system("cls");
    system("color f7");
    delay(75);
    system("color 0f");
    printb(board);
    printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op),symbol(op));
    while(f!=1)
    {
        if (top[0]==5 && top[1]==5 &&top[2]==5 &&top[3]==5 &&top[4]==5 && top[5]==5 && top[6]==5)
        {
            tie();
            return;
        }
        scanf("%s",com);
        if(com[2]!=' ')
        {
            printb(board);
            printf("\n       Enter a valid command!\n\a");
            *(com+2)=' ';
            printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
        }
        else if(*com>='1' && *com<='7')
        {
            drop_coin(board,top,*com - 48,op,&f,&undo_record.pt);
        }
        else if(*com=='U' || *com=='u')
        {
            popout_coin(board,top,&undo_record,op);
        }
        else if(*com=='Q' || *com=='q')
        {
            printf("\n       Are you sure?(Y/N) :\a");
            char yn[2];
            scanf("%s",yn);
            if (*yn=='y' || *yn =='Y')
            {
                printf("       Do you want to save? (Y/N) :\a");
                scanf("%s",yn);
                if(*yn=='y' || *yn =='Y')
                {
                    save_game(board,top,&undo_record,op);
                    return;
                }
                else
                {
                    system("cls");
                    FILE *fp;
                    fp = fopen("save.dat","w");
                    fprintf(fp,"%d",EOF);
                    fclose(fp);
                    fflush(fp);
                    return;
                }
            }
            else
            {
                printb(board);
                printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
            }
        }
        else
        {
            printb(board);
            printf("\n       Enter a valid command!\n\a");
            printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
        }
    }
}


void gotoxy(int x,int y)
 {
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
 }

void printb(char *board)
{
    int row,col;
    system("cls");
    gotoxy(29,1);
    printf("****Connect Four****\n\n");
    for(row = 0; row < 6; row++)
    {
        for(col = 0; col < 7; col++)
        {
             gotoxy(25+4*col,3+2*row);
            printf("%c %c ",179,  board[7*row + col]);
        }
        printf("%c\n",179);
        gotoxy(25,4+2*row);
        printf("%c---%c---%c---%c---%c---%c---%c---%c\n",195,197,197,197,197,197,197,180);

    }
    gotoxy(25,4+2*(row-1));
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",192,196,196,196,193,196,196,196,193,196,196,196,193,196,196,196,193,196,196,196,193,196,196,196,193,196,196,196,217);
    gotoxy(25,3+2*row);
    printf("  1   2   3   4   5   6   7\n\n");


}
void drop_coin(char *board, int *top,int i, int *op,int *f,int *pt)
{
    while (*(top+i-1)>=5)
    {
        printb(board);
        printf("\n      **Column full!**\n\a");
        printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
        scanf("%d",&i);
    }
    *(top+i-1) = *(top+i-1)+1;
    board[(34+i) - *(top+i-1)*7]=symbol(op);
    *pt=i;
    printb(board);
    if(checkWin(board))
    {
        PlaySound("tada.wav", SND_RESOURCE, SND_SYNC);
        system("color 7f");
        delay(75);
        int j;
        char s[28];
        system("color 0e");
        gotoxy(31,3*6);
        char txtcol[7];
        srand(time(NULL));
        strcpy(s,"****Congratulations !!!!****");
        strcpy(txtcol,"color 4");
        for(j=0;j<15;j++)
        {
            gotoxy(26+j,3*6);
            printf("%c",s[j]);
            gotoxy(54-j,3*6);
            printf("%c",s[28-j]);
            txtcol[6]= (rand()%6)+ 65;
            system(txtcol);
            Beep(rand(),50);
        }
        PlaySound("Congratulations.wav", SND_RESOURCE, SND_SYNC);
        gotoxy(31,3+3*6);
        char s2[17];
        strcpy(s2,"Player       Wins");
        strcpy(txtcol,"color 4");
        for(j=0;j<6;j++)
        {
            gotoxy(31+j,3*7);
            printf("%c",s2[j]);
            gotoxy(48-j,3*7);
            printf("%c",s2[16-j]);
            txtcol[6]= (rand()%6)+ 65;
            system(txtcol);
            Beep(rand(),50);
        }
        gotoxy(49,3+3*6);
        printf("!");
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        Beep(rand(),50);
        gotoxy(50,3+3*6);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        Beep(rand(),50);
        gotoxy(38,3+3*6);
        printf("%c",player(op));
        gotoxy(41,3+3*6);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        Beep(rand(),50);
        printf("%c",symbol(op));
        gotoxy(40,3+3*6);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        Beep(rand(),50);
        printf("(");
        gotoxy(42,3+3*6);
        printf(")");
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        if(symbol(op)=='X')
        {
             PlaySound("p1.wav", SND_RESOURCE, SND_SYNC);
        }
        else
        {
             system("color d");
             PlaySound("p2.wav", SND_RESOURCE, SND_SYNC);
        }
        _getch();
        *f=1;
        FILE *fp;
        fp = fopen("save.dat","w");
        fprintf(fp,"%d",EOF);
        fclose(fp);
        fflush(fp);
        return;
    }
    *op = *op +1;
    printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
}

char player(int *op)
{
    if(*op%2==1)
    {
        return ('1');
    }
    else
    {
        return('2');
    }
}

char symbol(int *op)
{
    if(*op%2==1)
    {
        system("color a");
        return ('X');
    }
    else
    {
        system("color d");
        return('O');
    }
}

int horizontalCheck(char *board)
{
    int row, col, idx;//rows, column , index
    for(row = 0; row < 6; row++)
    {
       for(col = 0; col < 4; col++)
        {
          idx = 7*row + col;
          if(checkFour(board, idx, idx+1, idx + 2, idx + 3))
            {
             return 1;
            }
       }
    }
    return 0;
}

int checkFour(char *board, int a, int b, int c, int d)
{
    return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');
}
int verticalCheck(char *board)
{
    int row, col, idx;
    for(row = 0; row < 3; row++)
    {
       for(col = 0; col < 7; col++)
       {
          idx = 7 * row + col;
          if(checkFour(board, idx, idx + 7, idx + 14, idx + 21))
            {
              return 1;
            }
       }
    }
    return 0;
}

int sdiagonalCheck(char *board)//secondary diagonal of matrix
{
    int row, col, idx;
    for(row = 0; row < 3; row++)
    {
       for(col = 3; col < 7; col++)
       {
          idx = 7 * row + col;
          if(checkFour(board, idx, idx + 6, idx + 12, idx + 18))//
          {
              return 1;
          }
       }
    }
    return 0;
}

int pdiagonalCheck(char *board)//primary diagonal of matrix
{
    int row, col, idx;
    for(row = 0; row < 3; row++)
    {
       for(col = 0; col < 4; col++)
       {
          idx = 7 * row + col;
          if(checkFour(board, idx, idx + 8, idx + 16, idx + 24))
          {
              return 1;
          }
       }
    }
    return 0;
}

int checkWin(char *board)
{
    return (horizontalCheck(board) || verticalCheck(board) || sdiagonalCheck(board)|| pdiagonalCheck(board));
}

void popout_coin(char *board,int *top,Undo *rec,int *op)
{
    if(rec->pt==-1)
    {
        printb(board);
        printf("\n       Undo-Lifeline can't used as first move.\n\a");
        printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
        return;
    }
    *op=*op-1;
    int i;
    i=rec->pt;
    if(player(op)=='1')
    {
        if((rec->u1c)>=1)
        {
            printb(board);
            printf("\n       Player 1 (X) has already used Undo-Lifeline.\n\a");
            *op=*op+1;
            printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
            return;
        }
        else
        {
            rec->u1c=rec->u1c+1;
            board[(34+i) - *(top+i-1)*7]=' ';
            *(top+i-1) = *(top+i-1)-1;
            printb(board);
            printf("\n       Player %c (%c):\n       Enter New Column No. : ", player(op), symbol(op));
            return;
        }
    }
    else if(player(op)=='2')
    {
        if((rec->u2c)>=1)
        {
            printb(board);
            printf("\n       Player 2 (O) has already used Undo-Lifeline.\n\a");
            *op=*op+1;
            printf("\n       Player %c (%c):\n       Enter Column No. : ", player(op), symbol(op));
            return;
        }
        else
        {
            rec->u2c=rec->u2c+1;
            board[(34+i) - *(top+i-1)*7 ]=' ';
            *(top+i-1) = *(top+i-1)-1;
            printb(board);
            printf("\n       Player %c (%c):\n       Enter New Column No. : ", player(op), symbol(op));
            return;
        }
    }
}

void save_game(char *board,int *top,Undo *rec,int *op)
{
    FILE *fs;
    int i;
    fs = fopen("save.dat","w");
    if (fs==NULL)
    {
        printf("Unable to Save.");
        return;
    }
    for(i=0;i<42;i++)
    {
        fprintf(fs,"%c",board[i]);
    }
    fprintf(fs,"%d",EOF);
    fclose(fs);
    fflush(fs);
    fs = fopen("top.dat","w");
    if (fs==NULL)
    {
        printf("Unable to Save.");
        return;
    }
    for(i=0;i<7;i++)
    {
        fprintf(fs,"%d ",top[i]);
    }
    fclose(fs);
    fflush(fs);
    fs = fopen("count.dat","w");
    if (fs==NULL)
    {
        printf("Unable to Save.");
        return;
    }
    fprintf(fs,"%d %d %d %d",*op,rec->pt,rec->u1c,rec->u2c);
    fclose(fs);
    fflush(fs);
}

int savecheck()
{
    FILE *fp;
    int d;
    fp=fopen("save.dat","r");
    fscanf(fp,"%d",&d);
    if(d==EOF)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int title(int x)
{
    system("cls");
    int val=5;
    gotoxy(18,2);
    int pt=0,n;
    PlaySound("tada.wav", SND_RESOURCE, SND_SYNC);
    printf(" _  _             _ ___       __  _      __");
    gotoxy(18,3);
    printf("(_ (_) %c\\%c %c\\%c %c (_  %c   %c%c  %c-  (_) %c_%c %c<",179,179,179,179,238,179,196,196,179,179,179,179);
    gotoxy(72,23);
    printf("- <RA>");
    PlaySound("c4.wav", SND_RESOURCE, SND_SYNC);
    int i;
    char txtcol[7];
    srand(time(NULL));
    strcpy(txtcol,"color 3");
    if(x==1)
        {
        n=4;
        gotoxy(12,7);
        printf("%c",218);
        gotoxy(66,18);
        printf("%c",217);
        for (i=0;i<53;i++)
        {
        gotoxy(13+i,7);
        printf("%c",196);
        gotoxy(65-i,18);
        printf("%c",196);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        Beep(5000+i*100,50);
        }
        for (i=0;i<10;i++)
        {
        gotoxy(66,17-i);
        printf("%c",179);
        gotoxy(12,8+i);
        printf("%c\a\a",179);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        delay(50);
        }
        gotoxy(66,17-i);
        printf("%c",191);
        gotoxy(12,8+i);
        printf("%c",192);
        char s[43];s[43]='\0';
        strcpy(s,"Connect to the former Game-Play Continuum!");
        gotoxy(18,9);
        for(i=0;i<42;i++)
        {
            printf("%c",*(s+i));
            delay(25);
        }
        strcpy(s,"Initialize a New Game!");
        gotoxy(27,11);
        for(i=0;i<22;i++)
        {
            printf("%c",*(s+i));
            delay(35);
        }
       gotoxy(22,13);
       strcpy(s,"Discover the Laws of this Game!");
       for(i=0;i<31;i++)
        {
            printf("%c",*(s+i));
            delay(30);
        }
       gotoxy(28,15);
        strcpy(s,"Withdraw to Windows!");
       for(i=0;i<20;i++)
        {
            printf("%c",*(s+i));
            delay(35);
        }
   }
   else
   {
        n=3;
        gotoxy(12,7);
        printf("%c",218);
        gotoxy(66,16);
        printf("%c",217);
        for (i=0;i<53;i++)
        {
        gotoxy(13+i,7);
        printf("%c",196);
        gotoxy(65-i,16);
        printf("%c",196);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        Beep(5000+i*100,50);
        }
        for (i=0;i<8;i++)
        {
        gotoxy(66,15-i);
        printf("%c\a\a",179);
        gotoxy(12,8+i);
        printf("%c",179);
        txtcol[6]= (rand()%6)+ 65;
        system(txtcol);
        delay(50);
        }
        gotoxy(66,15-i);
        printf("%c",191);
        gotoxy(12,8+i);
        printf("%c",192);
        char s[32];
        s[32]='\0';
        strcpy(s,"Initialize a New Game!");
        gotoxy(27,9);
        for(i=0;i<22;i++)
        {
            printf("%c",*(s+i));
            delay(50);
        }
       gotoxy(22,11);
       strcpy(s,"Discover the Laws of this Game!");
       for(i=0;i<31;i++)
        {
            printf("%c",*(s+i));
            delay(30);
        }
       gotoxy(28,13);
       strcpy(s,"Withdraw to Windows!");
       for(i=0;i<20;i++)
        {
            printf("%c",*(s+i));
            delay(50);
        }
    }
    gotoxy(14,9);
    printf("%c",16);
    int ch;
    ch = _getch ();
    if(ch==27)
      {
        return 3;
      }
    while(ch!=27)
    {
        ch = _getch ();
        switch (ch)
        {
            case 72:
                for (i=0;i<n;i++)
                {
                   gotoxy(14,9 + i*2);
                   printf(" ");
                }
                pt=(n+pt-1)%n;
                gotoxy(14,9 + pt*2);
                printf("%c",16);
                break;

            case 80:
                for (i=0;i<n;i++)
                {
                   gotoxy(14,9 + i*2);
                   printf(" ");
                }
                pt=(pt+1)%n;
                gotoxy(14,9+pt*2);
                printf("%c",16);
                break;
        }
    if(ch==27)
    {
        return 3;
    }
    val=pt;
    if(n==3)
    {
        val=val+1;
    }
    if(ch==13)
    {
        return val;
    }
    }
}

void tie()
{
            PlaySound("tada.wav", SND_RESOURCE, SND_SYNC);
            system("color 7f");
            delay(75);
            system("color 0e");
            system("cls");
            gotoxy(26,5);
            printf("****   Well Played ,,,   ****");
            gotoxy(35,12);
            printf("It's a tie!");
            gotoxy(29,20);
            printf("(Press any key to exit)");
            PlaySound("tie.wav", SND_RESOURCE, SND_SYNC);
            _getch();
}

void flip_coin()
{
    system("cls");
    int i;
    char s[11];
    system("color 7f");
    delay(75);
    system("color 0f");
    gotoxy(30,5);
    printf("Toss for Player 1(X)");
    srand(time(NULL));
    for(i=0;i<2;i++)
    {
        gotoxy(39,12);
        printf("   ");
        delay(300);
        gotoxy(39,12);
        printf(".");
        delay(300);
        printf(".");
        delay(300);
        printf(".");
        delay(300);
    }
    if(rand()%2)
    {
        strcpy(s,"It's a Heads!");
    }
    else
    {
        strcpy(s,"It's a Tails!");
    }
    gotoxy(34,12);
    for(i=0;i<11;i++)
    {
        printf("%c",s[i]);
        delay(50);
    }
    gotoxy(29,20);
    printf("(Press any key to exit)");
    _getch();
}

void how_to_play()
{
    system("cls");
    system("color 7f");
    delay(75);
    system("color 0f");
    int i;
    gotoxy(2,2);
    printf("%c",218);
    for(i=0;i<74;i++)
    {
        printf("-");
    }
    printf("%c",191);
    for(i=0;i<18;i++)
    {
        gotoxy(2,3+i);
        printf("|");
        gotoxy(77,3+i);
        printf("|");
    }
    gotoxy(77,3+i);
    printf("%c",217);
    gotoxy(2,3+i);
    printf("%c",192);
    for(i=0;i<74;i++)
    {
        printf("-");
    }
    gotoxy(24,2);
    printf(" Discover the Laws of this Game! ");
    gotoxy(5,23);
    printf("*Enter 'U' to Undo*");
    gotoxy(56,23);
    printf("*Enter 'Q' to Exit*");
    gotoxy(29,23);
    printf("(Press any key to exit)");
    gotoxy(5,5);
    printf("Objective :");
    gotoxy(7,6);
    printf("To win Connect Four you must be the first player to get four of your");
    gotoxy(7,7);
    printf("'marked coin' in a row either horizontally, vertically or diagonally.");
    gotoxy(5,10);
    printf("How to Play :");
    gotoxy(7,11);
    printf("Game starts with a coin toss, One who wins may play as player 1.");
    gotoxy(7,12);
    printf("To drop the coin in a specific column, One must enter the column no.");
    gotoxy(7,13);
    printf("Once the column is filled, you further cannot drop any coin in it.");
    gotoxy(5,16);
    printf("Undo-Lifeline");
    gotoxy(7,17);
    printf("Both players are given a Undo-Lifeline which can correct your wrong");
    gotoxy(7,18);
    printf("move, However only once. This life must be used with consent of both");
    gotoxy(7,19);
    printf("players, So this life it to be used when other player gets the chance.");
    gotoxy(34,21);
    printf(" Have Fun! ");
    _getch();
}
