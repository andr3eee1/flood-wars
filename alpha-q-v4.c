/*
   Nume program   : alpha-q.c
   Nume concurent : Pana Andrei, Paunescu Vlad
   E-mail         : panaandreimihai@gmail.com, svsvpaunescu@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAXN 50
#define MAXM 50
#define INFINIT 2147483647
#define MAXDEPTH 30
#define MAXTIME 900000 // 0.90 sec

struct timeval tv;
int cont;
long long tbase;
int maxdepth;

int killer[MAXDEPTH]; // killer moves

// timpul curent in microsecunde (milionime de secunda)
static inline long long checktime() {
  gettimeofday(&tv, NULL);
  return ((long long)tv.tv_sec) * 1000000 + tv.tv_usec;
}

char mat[MAXN+2][MAXM+2];
char juc,mutch;

char mut[]={'@','#','+','*','.'};

char frecvfill[127];

static inline int max(int a, int b){
  return a>b?a:b;
}

static inline int min(int a, int b){
  return a<b?a:b;
}

static inline int abs_(int x){
  return (x<0?-x:x);
}

int fill(char table[MAXN+2][MAXM+2],int l,int c,char f,char tf){
  int rez=1;
  table[l][c]=f;
  frecvfill[(int)table[l+1][c]]=frecvfill[(int)table[l-1][c]]=frecvfill[(int)table[l][c-1]]=frecvfill[(int)table[l][c+1]]=1;

  if(table[l+1][c]==tf||(tf==-1&&table[l+1][c]!='X'&&table[l+1][c]!='Y'&&table[l+1][c]!='Z'&&table[l+1][c]!=0)){
    rez+=fill(table,l+1,c,f,tf);
  }
  if(table[l-1][c]==tf||(tf==-1&&table[l-1][c]!='X'&&table[l-1][c]!='Y'&&table[l-1][c]!='Z'&&table[l-1][c]!=0)){
    rez+=fill(table,l-1,c,f,tf);
  }
  if(table[l][c-1]==tf||(tf==-1&&table[l][c-1]!='X'&&table[l][c-1]!='Y'&&table[l][c-1]!='Z'&&table[l][c-1]!=0)){
    rez+=fill(table,l,c-1,f,tf);
  }
  if(table[l][c+1]==tf||(tf==-1&&table[l][c+1]!='X'&&table[l][c+1]!='Y'&&table[l][c+1]!='Z'&&table[l][c+1]!=0)){
    rez+=fill(table,l,c+1,f,tf);
  }
  return rez;
}

//schimba semnul numerelor peste care trece
//returneazaz suma distantelor manhatten fata de pozitia (lfin,cfin)
int distFill(char table[MAXN+2][MAXM+2],int l,int c,char tf,int lfin,int cfin){
  int rez=abs_(l-lfin)+abs_(c-cfin);
  table[l][c]=-table[l][c];

  if(table[l-1][c]==tf){
    rez+=distFill(table,l-1,c,tf,lfin,cfin);
  }
  if(table[l][c+1]==tf){
    rez+=distFill(table,l,c+1,tf,lfin,cfin);
  }
  if(table[l+1][c]==tf){
    rez+=distFill(table,l+1,c,tf,lfin,cfin);
  }
  if(table[l][c-1]==tf){
    rez+=distFill(table,l,c-1,tf,lfin,cfin);
  }

  return rez;
}

int getScore(char table[MAXN+2][MAXM+2],int n,int m){
  char ctable[MAXN+2][MAXM+2];
  int l,c,i,pctj,pcts,pct;

  //copiez tabla in ctable
  for(l=0;l<=n+1;l++){
    for(c=0;c<=m+1;c++){
      ctable[l][c]=table[l][c];
    }
  }

  pctj=fill(ctable,n,1,'X',ctable[n][1]);
  pcts=fill(ctable,1,m,'Y',ctable[1][m]);
  // printf("Score init: %d %d\n",pctj,pcts);

  for(i=0;i<5;i++){
    frecvfill[(int)mut[i]]=0;
  }
  frecvfill['X']=frecvfill['Y']=frecvfill['Z']=0;

  for(l=1;l<=n;l++){
    for(c=1;c<=m;c++){
      if(ctable[l][c]!='X'&&ctable[l][c]!='Y'&&ctable[l][c]!='Z'){
        for(i=0;i<5;i++){
          frecvfill[(int)mut[i]]=0;
        }
        frecvfill['X']=frecvfill['Y']=frecvfill['Z']=0;
        pct=fill(ctable,l,c,'Z',-1);
        // for(int l1=1;l1<=n;l1++){
        //   for(int c1=1;c1<=m;c1++){
        //     printf("%c",ctable[l1][c1]);
        //   }
        //   printf("\n");
        // }
        // printf("\n");

        if(frecvfill['X']==1&&frecvfill['Y']==0){
          pctj+=pct;
        }else if(frecvfill['X']==0&&frecvfill['Y']==1){
          pcts+=pct;
        }
      }
    }
  }

  // printf("Score: %d %d\n",pctj,pcts);

  return pctj-pcts;
}

int getDist(char table[MAXN+2][MAXM+2],int n,int m){
  int distj,dists,dist;
  //evaluare dupa pozitionarea fata de coltul jucatorilor
  distj=distFill(table,n,1,table[n][1],1,m);
  distFill(table,n,1,table[n][1],1,m);
  dists=distFill(table,1,m,table[1][m],n,1);
  distFill(table,1,m,table[1][m],n,1);

  //diferenta este inversata, pentru ca o distanta mai mica este mai buna
  dist=dists-distj;
  if(juc=='S'){
    dist=distj-dists;
  }

  return dist;
}

int evalStatic(char table[MAXN+2][MAXM+2],int n,int m){
  int score=0;

  //evaluare dupa punctaj
  score+=getScore(table,n,m);

  //evaluare dupa pozitionarea fata de coltul jucatorilor
  // score+=getDist(table,n,m);

  // printf("Score: %d\n",score);
  // for(int l=1;l<=n;l++){
  //   for(int c=1;c<=m;c++){
  //     printf("%c",table[l][c]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  return score;
}

int negamax(int depth,char table[MAXN+2][MAXM+2],int n,int m,int alpha,int beta,int jucl,int jucc){
  int icolor,score,l,c;
  char ctable[MAXN+2][MAXM+2];

  if(maxdepth-depth==5){
    cont=((checktime()-tbase)<MAXTIME);
  }

  if(cont&&depth==maxdepth){
    return (((depth+(1-(juc=='J'?0:1)))&1)*2-1)*evalStatic(table,n,m);
  }

  if(cont&&killer[depth]>=0){
    icolor=killer[depth];
    if(mut[icolor]!=table[n][1]&&mut[icolor]!=table[1][m]){
      for(l=0;l<=n+1;l++){
        for(c=0;c<=m+1;c++){
          ctable[l][c]=table[l][c];
        }
      }
      fill(ctable,jucl,jucc,mut[icolor],ctable[jucl][jucc]);

      score=-negamax(depth+1,ctable,n,m,-beta,-alpha,(jucl==1?n:1),(jucc==1?m:1));

      if(score>alpha){
        alpha=score;
      }
    }
  }

  icolor=0;
  while(cont&&alpha<beta&&icolor<5){
    if(icolor!=killer[depth]&&mut[icolor]!=table[n][1]&&mut[icolor]!=table[1][m]){
      for(l=0;l<=n+1;l++){
        for(c=0;c<=m+1;c++){
          ctable[l][c]=table[l][c];
        }
      }
      fill(ctable,jucl,jucc,mut[icolor],ctable[jucl][jucc]);

      score=-negamax(depth+1,ctable,n,m,-beta,-alpha,(jucl==1?n:1),(jucc==1?m:1));

      if(score>alpha){
        alpha=score;
        killer[depth]=icolor;
      }
    }
    icolor++;
  }

  return alpha<beta?alpha:beta;
}

int main(){
  int n,m,l,c,ljuc,cjuc,maxicolor;
  char ch,chjuc;

  juc=fgetc(stdin);
  fgetc(stdin);//'\n'

  n=1;
  ch=fgetc(stdin);
  while(!feof(stdin)){
    m=1;
    while(ch!='\n'){
      mat[n][m++]=ch;
      ch=fgetc(stdin);
    }
    ch=fgetc(stdin);
    n++;
  }
  n--;
  m--;

  chjuc=mat[n][1];
  ljuc=n;
  cjuc=1;
  if(juc=='S'){
    chjuc=mat[1][m];
    ljuc=1;
    cjuc=m;
  }

  for(l=0;l<MAXDEPTH;l++){
    killer[l]=-1;
  }
  tbase=checktime();

  maxicolor=100;//ceva fictional, pentru debug
  cont=maxdepth=1;
  while((checktime()-tbase)<MAXTIME&&maxdepth<=MAXDEPTH){
    negamax(0,mat,n,m,-INFINIT,INFINIT,ljuc,cjuc);
    maxdepth++;
    if(cont){
      maxicolor=killer[0];
    }
  }
  // printf("%d %d\n",maxicolor,maxdepth);
  fill(mat,ljuc,cjuc,mut[maxicolor],chjuc);

  if(juc=='J'){
    fputc('S',stdout);
  }else{
    fputc('J',stdout);
  }
  fputc('\n',stdout);
  for(l=1;l<=n;l++){
    for(c=1;c<=m;c++){
      fputc(mat[l][c],stdout);
    }
    fputc('\n',stdout);
  }

  return 0;
}
/*
J
.#+#.*.@.@
@.+*@.+*#+
**#++@**#@
#@#@.@@+@#
++@++@#.@.
**##*@*#++
#@@*.+*.*+
*/