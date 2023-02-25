#include <iostream>
#include <cstring>
#include <queue>
using namespace std;
int a = 0;//checks if the player changes position
const int map[11][15]=
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,1,0,1,0,1,0,1,0,1,0,0,0,0,1},
    {1,0,0,1,1,1,0,1,0,1,0,0,1,0,1},
    {1,0,1,1,1,0,0,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,1,1,0,1,0,0,0,0,1},
    {1,1,1,0,1,1,1,0,0,1,0,1,1,1,1},
    {1,0,1,0,1,0,1,1,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
int map2[11][15];
int pozx=1,pozy=8, direction=180;
int prevx=pozx,prevy=pozy;
int mpozx=1,mpozy=2;
int makeamove=1;
char movecombos[1001];int idx;

const int dy[]={-1,1,0,0};
const int dx[]={0,0,-1,1};
const char dir[]="SWDA";
//Legend
//0 = walkable space
//1 = wall
//2 = exit

//Graphics
void drawhall()
{
    cout<<"\t\033[95m"<<"#"<<"\033[35m"<<"######"<<"\033[95m"<<"#"<<endl;
    cout<<"\t\033[95m"<<"##"<<"\033[35m"<<"####"<<"\033[95m"<<"##"<<endl;
    cout<<"\t\033[95m"<<"##"<<"    "<<"##"<<endl;
    cout<<"\t"<<"\033[93m"<<"i"<<"\033[95m"<<"#"<<"    "<<"#"<<"\033[93m"<<"i"<<endl;
    cout<<"\t\033[95m"<<"##"<<"    "<<"##"<<endl;
    cout<<"\t\033[95m"<<"##"<<"\033[35m"<<"####"<<"\033[95m"<<"##"<<endl;
    cout<<"\t\033[95m"<<"#"<<"\033[35m"<<"######"<<"\033[95m"<<"#"<<endl;
    cout<<"\033[0m";
}

void drawwall()
{
    cout<<"\t\033[95m"<<"########"<<endl;
    cout<<"\t\033[95m"<<"########"<<endl;
    cout<<"\t\033[95m"<<"########"<<endl;
    cout<<"\t"<<"\033[95m"<<"#"<<"\033[93m"<<"i"<<"\033[95m"<<"####"<<"\033[93m"<<"i"<<"\033[95m"<<"#"<<endl;
    cout<<"\t\033[95m"<<"########"<<endl;
    cout<<"\t\033[95m"<<"########"<<endl;
    cout<<"\t\033[95m"<<"########"<<endl;
    cout<<"\033[0m";
}

void drawmonsterclose()
{
    cout<<"\t\033[95m"<<"#"<<"\033[35m"<<"######"<<"\033[95m"<<"#"<<endl;
    cout<<"\t\033[95m"<<"#"<<"\033[37m"<<"L"<<"\033[33m"<<"="<<"\033[91m"<<"@"<<" "<<"\033[33m"<<"="<<"\033[95m"<<"##"<<endl;
    cout<<"\t\033[95m"<<"##"<<"\033[33m"<<"{"<<"\033[37m"<<"WM"<<"\033[33m"<<"}"<<"\033[95m##"<<endl;
    cout<<"\t\033[95m"<<"#"<<"\033[33m"<<"<"<<"\033[90m"<<"(==)"<<"\033[33m"<<">"<<"\033[95m"<<"#"<<endl;
    cout<<"\t"<<"\033[95m"<<"#"<<"\033[33m"<<"[ "<<"\033[90m"<<"##"<<"\033[33m"<<" ]"<<"\033[95m"<<"#"<<endl;
    cout<<"\t\033[95m"<<"##"<<"\033[33m"<<"lWWl"<<"\033[95m"<<"##"<<endl;
    cout<<"\t\033[95m"<<"##"<<"\033[33m"<<"M"<<"\033[35m"<<"##"<<"\033[33m"<<"M"<<"\033[95m"<<"##"<<endl;
    cout<<"\t\033[95m"<<"#"<<"\033[35m"<<"######"<<"\033[95m"<<"#"<<endl;
    cout<<"\033[0m";
}

void gameover()
{
    cout<<"\t\033[37mL"<<endl;
    cout<<"\t\033[37mLLL"<<"\033[33m[[[]]]"<<endl;
    cout<<"\t\033[33m   ["<<"\033[91m@"<<"\033[33m[] ]"<<endl;
    cout<<"\t\033[33m   ["<<"\033[37mWWWW"<<"\033[33m]"<<endl;
    cout<<"\t\033[33m   [    ]"<<endl;
    cout<<"\t\033[33m   ["<<"\033[37mWWWW"<<"\033[33m]"<<endl;
    cout<<"\t\033[33m    MMMM"<<endl;
    cout<<"\033[91mAn eye for an eye...a tooth for a tooth. The Grievotaur has found you and devoured you."<<endl;
    cout<<"\033[0m";
}

bool inmap(int y,int x)
{
    return x >=0 && y>=0 && x<15 && y<11;
}

void road(int y,int x)
{
    if(y==mpozy && x==mpozx)
        return;
    else
        for(int d=0;d<=3;d++)
        {
            int newy = y + dy[d];
            int newx = x + dx[d];
            if(inmap(newy,newx) && map2[newy][newx]==map2[y][x]-1)
            {    
                road(newy,newx);
                movecombos[idx++]=dir[d];
                break;
            }
        }
}

void calcpos(int ys,int xs)
{
    idx=0;
    queue<pair<int,int>> Q;
    Q.push(make_pair(ys,xs));
    map2[ys][xs]=1;
    while(!Q.empty())
    {
        for(int d=0;d<=3;d++)
        {
            int newy = dy[d] + Q.front().first;
            int newx = dx[d] + Q.front().second;
            if(inmap(newy,newx) && map[newy][newx]<1 && map2[newy][newx]==0)
            {    
                map2[newy][newx]=map2[Q.front().first][Q.front().second]+1;
                Q.push(make_pair(newy,newx));
            }
        }
        Q.pop();
    }
    road(pozy,pozx);
}

int main()
{
    char input[1];
    cout<<"\033[91mAn eye for an eye...a tooth for a tooth"<<endl;
    cout<<"\033[0m"<<endl;
    cout<<"You may have taken one of the Grievotaur's horns and eye, but he took your hand as revenge! And he is not satisfied! Try to escape his dungeon!"<<endl;
    cout<<"Controls: W, A, S, D"<<endl;
    cout<<"W = Go forward"<<endl;
    cout<<"S = Go backwards"<<endl;
    cout<<"A = Turn to the left"<<endl;
    cout<<"D = Turn to the right"<<endl;
    calcpos(mpozy,mpozx);
    while(1)
    {
        //Determine the direction the player is facing
        if(direction==0)
            if(map[pozy-1][pozx]==1)drawwall();
            else if(mpozy-pozy>=2 && mpozy-pozy<=4 && mpozx==pozx)drawmonsterclose();
            else drawhall();
        else if(direction==90)
            if(map[pozy][pozx+1]==1)drawwall();
            else if(mpozx-pozx>=2 && mpozx-pozx<=4 && mpozy==pozy)drawmonsterclose();
            else drawhall();
        else if(direction==180)
            if(map[pozy+1][pozx]==1)drawwall();
            else if(mpozy-pozy>=-2 && mpozy-pozy<=0 && mpozx==pozx)drawmonsterclose();
            else drawhall();
        else if(direction==-90)
            if(map[pozy][pozx-1]==1)drawwall();
            else if(mpozx-pozx>=-2 && mpozx-pozx<=0 && mpozy==pozy)drawmonsterclose();
            else drawhall();
        cout<<"What are you going to do? ";cin>>input;
        //Reading input and changing the position
        if(strcmp("w",input)==0 || strcmp("W",input)==0)
            {
                if(direction==0 && map[pozy-1][pozx]==0)
                {
                    pozy-=1;
                }
                else if(direction==90 && map[pozy][pozx+1]==0)
                {
                    pozx+=1;
                }
                else if(direction==-90 && map[pozy][pozx-1]==0)
                {
                    pozx-=1;
                }
                else if(direction==180 && map[pozy+1][pozx]==0)
                {
                    pozy+=1;
                }

                // Special case when close to exit
                if(direction==0 && map[pozy-1][pozx]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                else if(direction==90 && map[pozy][pozx+1]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                else if(direction==-90 && map[pozy][pozx-1]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                else if(direction==180 && map[pozy+1][pozx]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                makeamove++;
            }
        else if (strcmp("s",input)==0 || strcmp("S",input)==0)
            {
                if(direction==0 && map[pozy+1][pozx]==0)
                {
                    pozy+=1;
                }
                else if(direction==90 && map[pozy][pozx-1]==0)
                {
                    pozx-=1;
                }
                else if(direction==-90 && map[pozy][pozx+1]==0)
                {
                    pozx+=1;
                }
                else if(direction==180 && map[pozy-1][pozx]==0)
                {
                    pozy-=1;
                }

                // Special case when close to exit
                if(direction==0 && map[pozy+1][pozx]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                else if(direction==90 && map[pozy][pozx-1]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                else if(direction==-90 && map[pozy][pozx+1]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                else if(direction==180 && map[pozy-1][pozx]==2)
                {
                    cout<<"Congratulations! You escaped!";
                    return 0;
                }
                makeamove++;
            }
        else if (strcmp("a",input)==0 || strcmp("A",input)==0)
        {
            direction-=90;
            makeamove++;
        }
        else if (strcmp("d",input)==0 || strcmp("D",input)==0)
        {
            direction+=90;
            makeamove++;
        }
        else cout<<"Invalid command."<<endl;
        if(direction==-180)
            direction=180;
        if(direction==270)
            direction=-90;
        if(direction==360)
            direction=0;
        if(prevx==pozx && prevy==pozy)
            a++;
        else
            {
                calcpos(mpozy,mpozx);
                a=0;
                prevx=pozx;
                prevy=pozy;
            }
        if(movecombos[a]=='S') 
        {
            if(mpozy<10)mpozy++;
        }
        else if (movecombos[a]=='W') 
        {
            if(mpozy>2)mpozy--;
        }
        else if (movecombos[a]=='D')
        {
            if(mpozx<14)mpozx++;
        }
        else if(mpozx>1)mpozx--;
        if((mpozx-pozx==1 && pozy==mpozy)||(mpozy-pozy==1 && pozx==mpozx))
        {
            gameover();
            return 0;
        }
    }
    return 0;
}