#include<bits/stdc++.h>
#define PI 3.1416
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
float sunx,suny;
float spin1=0;
float speedflag=1,leftclick=0,speedcount=0;
float birdx=0, birdy=-10, theta = 130, v0=40, timepos=0, collision=0,speedup=-20,inc=.2;
vector< pair<float, float> > projectile;
int run=0,checkingclick=0;
float wall[4][4]=
{
    {324,102,300,214},
    {350,102,326,214},
    {324,-12,300,100},
    {350,-12,326,100}

};
int wallmove=0,wallmovecurrent=0,tempwallmovecurrent=0;
int wallvanish[4]= {0,0,0,0};
float weathermove=0;
float dynamic_bird_tx=0,dynamic_bird_ty=340,dynamic_bird_tz=0,dynamic_bird_angle=30,dynamic_bird_angle_indx=0,dynamic_bird_incy=-.05;
int point=0;
int hitflag[]= {20,40,60,100};
int turn=0;
int angle = 5;



int numPoints = 4;
void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);         // black background
    glMatrixMode(GL_PROJECTION);              // setup viewing projection
    glLoadIdentity();
    dynamic_bird_tx=0;
    // glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);   // setup a 10x10x2 viewing world
    glOrtho(-40.0, 360.0, -40.0, 360.0, -40.0, 360.0);   // setup a 10x10x2 viewing world

}

void land()
{
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.392156f,0.0f);
    glVertex3f( -40,0,-40 );
    glColor3f(0.0f,0.392156f,0.0f);
    glVertex3f( 360,0,-40);
    glColor3f(0.0f,0.803921f,1.0f);
    glVertex3f( 360,-80,+100 );
    glColor3f(0.0f,0.803921f,1.0f);
    glVertex3f( -40,-80,100 );
    glEnd();
    glPopMatrix();
}

void sky()
{
    glColor3f(0.0f,0.75f,1.0f);
    glRectd(360,0,-40,360);
    glPushMatrix();
    glBegin(GL_POLYGON);
    for(int i=-90; i<=10; i++)
    {
        glColor3f(1.0f,0.45f,0.0f);
        glVertex3f(i,0,0);
    }
    int x1=-360,y1=0,x2=-1000,y2=500,x3=500,y3=1000,x4=360*1.55,y4=0;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*(1-i)*x1+3*(1-i)*(1-i)*i*x2+3*(1-i)*i*i*x3+i*i*i*x4;
        float tempy=(1-i)*(1-i)*(1-i)*y1+3*(1-i)*(1-i)*i*y2+3*(1-i)*i*i*y3+i*i*i*y4;
        glColor3f(0.0f,0.75f,1.0f);
        glVertex3f( tempx, tempy,0 );
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_POLYGON);
    float radius_x=110;
    float radius_y=220;
    float maxi=0;
    float mini=1000000000;
    for(float i = 100; i >=1; i--)
    {
        float angle = 2 * PI * i / 100;
        //glColor3f(1.0f,0.45f,0.0f);
        glColor3f(1.0f,.4980392156f,0.0f);
        glVertex3f ( 0 + cos(angle) * radius_x, -40+sin(angle) * radius_y,20);
        maxi=max(maxi,cos(angle) * radius_x);
        mini=min(mini,cos(angle) * radius_x);
    }


    glEnd();
    glPopMatrix();

}

void dynamic_bird(int x,int y)
{
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f,1.0f);
    float radius_x=3.5;
    float radius_y=3.5;
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * PI * i / 100;
        glVertex3f ( x + cos(angle) * radius_x,y+ sin(angle) * radius_y,0);
    }
    glEnd();

}

void dynamic_right_wing(int x, int y)
{
    float radius_x=3;
    float radius_y=3;
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f,1.0f);
    glVertex3f(x+radius_x,y,0);
    glVertex3f(x+3*radius_x,y+2*radius_y,-30);
    glVertex3f(x+3*radius_x,y+3*radius_y,-30);
    glVertex3f(x,y+radius_y,0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f,1.0f);
    glVertex3f(x+3*radius_x,y+2*radius_y,-30);
    glVertex3f(x+6.5*radius_x,y+radius_y,-50);
    glVertex3f(x+3*radius_x,y+3*radius_y,-30);
    glEnd();

}

void dynamic_left_wing(int x, int y)
{
    float radius_x=3;
    float radius_y=3;
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f,1.0f);
    glVertex3f(x-radius_x,y,0);
    glVertex3f(x-3*radius_x,y+2*radius_y,30);
    glVertex3f(x-3*radius_x,y+3*radius_y,30);
    glVertex3f(x,y+radius_y,0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f,1.0f);
    glVertex3f(x-3*radius_x,y+2*radius_y,30);
    glVertex3f(x-6.5*radius_x,y+radius_y,30);
    glVertex3f(x-3*radius_x,y+3*radius_y,30);
    glEnd();

}
void Bigbird(float x, float y)
{
    float lims=0;
    float x1,y1,x2,y2,x3,y3;
    ///hair
    glLineWidth(1.0);
    glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0,0.0,0.0);
    glPointSize(1.0);
    x1=x;
    y1=y+51.5;
    x2=x+8;
    y2=y+55;
    x3=x-3;
    y3=y+73;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glVertex3f(x-.5,y+60,0);
    x1=x-.5;
    y1=y+60;
    x2=x-3;
    y2=y+70;
    x3=x-19;
    y3=y+65;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glVertex3f(x-6,y+60,0);
    x1=x-6;
    y1=y+60;
    x2=x-7;
    y2=y+62;
    x3=x-14;
    y3=y+52;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glVertex3f(x-7,y+55.5,0);
    glVertex3f(x-.5,y+60,0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(x-7,y+52.5,0);
    glVertex3f(x-9,y+48,0);
    x1=x-9;
    y1=y+48;
    x2=x-5;
    y2=y+55;
    x3=x;
    y3=y+43;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glVertex2d(x,y+51.5);
    glEnd();
    glColor3f(255.0,255.0,0.0);
    glPointSize(5);
    glBegin(GL_POLYGON);
    /// right curve
    x1=x+20;
    y1=y;
    x2=x+24;
    y2=y;
    x3=x+20;
    y3=y+10;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glVertex3f(x+4,y+50,0);
    /// upper curve
    x1=x+4;
    y1=y+50;
    x2=x;
    y2=y+56;
    x3=x-4;
    y3=y+50;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glVertex3f(x-20,y+10,0);


    ///left curve
    x1=x-20;
    y1=y+10;
    x2=x-24;
    y2=y;
    x3=x-20;
    y3=y;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    /// lower curve
    x1=x-20;
    y1=y;
    x2=x-9;
    y2=y-4;
    x3=x+20;
    y3=y;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glVertex3f( tempx, tempy,0 );
    }
    glEnd();
    /// white part
    glBegin(GL_POLYGON);
    x1=x-10;
    y1=y;
    x2=x;
    y2=y+10;
    x3=x+17;
    y3=y;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glColor3f(1,1,1);
        glVertex3f( tempx, tempy,0 );
    }
    x1=x-20;
    y1=y;
    x2=x-9;
    y2=y-4;
    x3=x+20;
    y3=y;
    for( float i = 0 ; i < 1 ; i += 0.01 )
    {
        float tempx=(1-i)*(1-i)*x1+2*(1-i)*i*x2+i*i*x3;
        float tempy=(1-i)*(1-i)*y1+2*(1-i)*i*y2+i*i*y3;
        glColor3f(1,1,1);
        if(tempx>=x-11)
            glVertex3f( tempx, tempy,0 );
    }
    glEnd();


    ///eyes
    ///EYE RIGHT
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f,1.0f);
    float radius_x=3;
    float radius_y=5;
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * PI * i / 100;
        glVertex3f ( x +10+ cos(angle) * radius_x,y+20+ sin(angle) * radius_y,20);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glPointSize(10);
    glColor3f(0.0f, 0.0f,0.0f);
    radius_x=1;
    radius_y=1;
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * PI * i / 100;

        glVertex3f ( x +12+ cos(angle) * radius_x,y+20+ sin(angle) * radius_y,20);
    }
    glEnd();


    /// peak
    glColor3f(1.0f, 0.5f,0.0f);
    glBegin(GL_POLYGON);
    glVertex3f(x-2,y+12,0);
    glVertex3f(x+6,y+20,0);
    glVertex3f(x+4,y+15,0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(x+6,y+20,0);
    glVertex3f(x+4,y+15,0);
    glVertex3f(x+18,y+10,1);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(x-2,y+12,0);
    glVertex3f(x+6,y+5,0);
    glVertex3f(x+12,y+12.5,0);
    glVertex3f(x+4,y+15,0);
    glEnd();
    /// peak border
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0,0,0);
    glVertex3f(x-2,y+12,0);
    glVertex3f(x+6,y+20,0);
    glVertex3f(x+18,y+10,1);
    glVertex3f(x+4,y+15,0);
    glVertex3f(x-2,y+12,0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0,0,0);
    glVertex3f(x-2,y+12,0);
    glVertex3f(x+6,y+5,0);
    glVertex3f(x+12,y+12.5,0);
    glVertex3f(x+4,y+15,0);
    glVertex3f(x-2,y+12,0);
    glEnd();

    ///EYE LEFT
    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
    radius_x=3;
    radius_y=5;
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * PI * i / 100;
        glVertex3f ( x + cos(angle) * radius_x,y+20+ sin(angle) * radius_y,20);
    }
    glEnd();

    ///eye Right black
    glBegin(GL_POLYGON);
    glPointSize(10);
    glColor3f(0.0f, 0.0f,0.0f);
    radius_x=1;
    radius_y=1;
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * PI * i / 100;
        glVertex3f ( x +2+ cos(angle) * radius_x,y+20+ sin(angle) * radius_y,20);
    }
    glEnd();

    ///eyebrow
    glPushMatrix();
    glLineWidth(7.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(.5f,.35f,.05f);
    glVertex3f(x+6,y+23,0);
    glVertex3f(x+18,y+27,10);
    glEnd();

    glLineWidth(9.0f);
    glBegin(GL_POINTS);
    glColor3f(.5f,.35f,.05f);

    //glVertex3f(x+3,y+25,0);
    //glVertex3f(x-8,y+30,10);

    for(float i=8.0,j=3,k=24; i<=10.0; i+=.2,j-=1.1,k+=.5)
    {
        glPointSize(10.0);
        glVertex3f(x+j,y+k,0);

    }
    glEnd();
    glPopMatrix();


}

void speedBar()
{

    glBegin(GL_POLYGON);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(-30,-20,0);
    glVertex3f(-35,-20,0);
    glVertex3f(-35,speedup,0);
    glVertex3f(-30,speedup,0);
    glEnd();

    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0,0.0,0.0);
    glVertex3f(-30,-20,0);
    glVertex3f(-35,-20,0);
    glVertex3f(-35,80,0);
    glVertex3f(-30,80,0);
    glVertex3f(-30,-20,0);
    glEnd();

}

void buildWall(int wallmove)
{
    for(int i=3; i>=0; i--)
    {
        int tmp=i;
        if(wallvanish[i])
        {
            continue;
        }
        float a,b,c,d;
        if(wallmove==-1 || wallmove!=i)
        {
            a=wall[i][0];
            b=wall[i][1];
            c=wall[i][2];
            d=wall[i][3];
        }
        else if(wallmove==i)
        {
            a=wall[i][0];
            b=wall[i][1]-wallmovecurrent;
            c=wall[i][2];
            d=wall[i][3]-wallmovecurrent;
            // cout<<a<<" "<<b<<endl;
        }
        glColor3f(0.0980392156,0.0980392156,0.43921568);
        glRectd(a,b,c,d);
        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0,0.0,0.0);

        /// glPointSize(1.0f);
        glVertex3f(a,b,10);
        glVertex3f(a,d,10);
        glVertex3f(c,d,10);
        glVertex3f(c,b,10);
        glVertex3f(a,b,10);
        glEnd();
    }
}

void wallUptodown()
{
    tempwallmovecurrent++;
    wallmovecurrent=tempwallmovecurrent/20;
    if(wallmovecurrent==112)
    {
        wallvanish[wallmove+2]=0;
        wallvanish[wallmove]=1;
        tempwallmovecurrent=0;
        wallmove=-1;
        wallmovecurrent=0;
        run=0;
        glutIdleFunc(NULL);
    }
    glutPostRedisplay();
}
void showname(char* write,int x,int y)
{
    char s[256];
    sprintf( s, write );
    glRasterPos2f(x, y);
    for (int i = 0; s[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
}

void pointtable(int point, int turn)
{
    char s[256];
    sprintf( s, "Point %d\n",point );
    glRasterPos2f(330, 330);
    for (int i = 0; s[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    sprintf( s, "Turn %d\n",turn );
    glRasterPos2f(330, 310);
    for (int i = 0; s[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    sky();
    land();
    dynamic_bird_tx+=.1;
    dynamic_bird_tz+=.1;
    dynamic_bird_angle_indx++;
    if(dynamic_bird_tx>360.1)
        dynamic_bird_tx=-40;

    if(dynamic_bird_ty>340)
        dynamic_bird_incy=-.05;
    else if(dynamic_bird_ty<290)
        dynamic_bird_incy=.05;

    dynamic_bird_ty+=dynamic_bird_incy;
    dynamic_bird_angle+=inc;
    if(dynamic_bird_angle>30.0)
    {
        inc=-.2;
    }
    else if(dynamic_bird_angle<-30)
    {
        inc=.2;
    }
    glPushMatrix();
    glTranslatef(dynamic_bird_tx,dynamic_bird_ty,0);
    glRotatef(dynamic_bird_angle,0,0,1);
    dynamic_left_wing(0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(dynamic_bird_tx,dynamic_bird_ty,0);
    glRotatef(-dynamic_bird_angle,0,0,1);
    dynamic_right_wing(0,0);
    dynamic_bird(0,0);
    glPopMatrix();
    ///2nd
    glPushMatrix();
    glTranslatef(dynamic_bird_tx,dynamic_bird_ty-20,0);
    glRotatef(dynamic_bird_angle,0,0,1);
    dynamic_left_wing(0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(dynamic_bird_tx,dynamic_bird_ty-20,0);
    glRotatef(-dynamic_bird_angle,0,0,1);
    dynamic_right_wing(0,0);
    dynamic_bird(0,0);
    glPopMatrix();
    buildWall(wallmove);
    Bigbird(birdx,birdy);
    speedBar();
    pointtable(point,turn);
    if(point == 80)
    {
        showname("Well Done",160,330);
    }
    glutPostRedisplay();
    glFlush();/// to show what we have drawn. eta na dile ashbena kisui. shada screen ashbe

}

void goBird()
{
    if(collision==0 && timepos/30<projectile.size())
    {
        birdx=projectile[timepos/30].first;
        birdy=projectile[timepos/30].second;
        glutPostRedisplay();
        timepos++;
        float limity=birdy;
        float limitposx=birdx+20;
        float limitnegx=birdx-20;
        for(int i=0; i<=3; i++)
        {
            if(birdx-22>350)
                continue;
            if(wallvanish[i])
                continue;
            if(limity<=wall[i][3] && limity>=wall[i][1])
            {
                if((limitposx>=wall[i][2] && limitposx<=wall[i][0]) || (limitnegx<=wall[i][2] && limitposx>=wall[i][0]) || (limitnegx>=wall[i][2] && limitnegx<=wall[i][0]) )
                {
                    PlaySound("break.wav",NULL,SND_SYNC);
                    PlaySound("main_theme.wav",NULL,SND_ASYNC|SND_LOOP);
                    point+=20;
                    birdx=0;
                    birdy=-10;
                    glutIdleFunc(NULL);
                    collision=1;
                    projectile.clear();
                    if(i>=2 && wallvanish[i-2]==0)
                    {
                        wallmove=i-2;
                        tempwallmovecurrent=1;
                        wallvanish[i]=1;
                        glutIdleFunc(wallUptodown);
                    }
                    else
                    {
                        run=0;
                        wallvanish[i]=1;
                    }
                }
            }
        }
        timepos++;
    }
    else if(timepos/30>=projectile.size())
    {
        PlaySound("main_theme.wav",NULL,SND_ASYNC|SND_LOOP);
        collision++;
        run=0;
        birdx=0;
        birdy=-10;
        glutPostRedisplay();
        glutIdleFunc(NULL);
        projectile.clear();
    }
    else
    {
        PlaySound("main_theme.wav",NULL,SND_ASYNC|SND_LOOP);
        collision=0;
        run=0;
        birdx=0;
        birdy=-10;
        glutPostRedisplay();
        glutIdleFunc(NULL);
        projectile.clear();
        return;
    }

}
void setSpeed()
{

    if(speedflag==1)
    {
        if(speedcount<21)
        {
            speedcount++;
        }
        else
        {
            speedcount=0;
            speedup++;
        }
        if(speedup==81)
        {
            speedflag=0;
        }
    }
    else
    {
        if(speedcount<21)
        {
            speedcount++;
        }
        else
        {
            speedcount=0;
            speedup--;
        }
        if(speedup==-19)
        {
            speedflag=1;
        }
    }
    v0=speedup+20;
    glutPostRedisplay();
}
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'p':
        cout<<run<<" "<<v0<<endl;
        if(point==80)
            break;
        if(run==1)
            break;
        turn++;
        PlaySound("fly.wav",NULL,SND_ASYNC);
        projectile.clear();
        for(float i=0;; i+=.1)
        {
            float randx=v0* cos(theta*PI/360) * i;
            float randy=v0* sin(theta*PI/360) * i - 9.8 * i * i / 2;
            if(randy<-12)
                break;
            projectile.push_back(make_pair(randx,randy));
        }
        timepos=0;
        run=1;
        collision=0;
        glutIdleFunc(goBird);
        break;

    case 'r':
        PlaySound("main_theme.wav",NULL,SND_ASYNC|SND_LOOP);
        turn =0;
        point=0;
        spin1=0;
        run=0;
        birdx=0;
        birdy=-10;
        wallvanish[0]=0;
        wallvanish[1]=0;
        wallvanish[2]=0;
        wallvanish[3]=0;
        glutIdleFunc(NULL);
        glutPostRedisplay();
        break;

    case 's':
        break;

    default:
        break;
    }
}
void myMouse(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            if(point==80)
                break;
            if(run==0)
            {
                if(checkingclick%2==0)
                    glutIdleFunc(setSpeed);
                else
                    glutIdleFunc(NULL);
            }
            checkingclick++;
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            if(run==0)
                break;
    default:
        break;
    }
}
int main(int argc, char *argv[])
{
    PlaySound("main_theme.wav",NULL,SND_ASYNC|SND_LOOP);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1400,700);
    glutInitWindowPosition(20,10);
    glutCreateWindow("Angry Bird");
    init();
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutDisplayFunc(myDisplay);
    glutMainLoop();
}

