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

int killer[MAXDEPTH+1]; // killer moves

// timpul curent in microsecunde (milionime de secunda)
static inline long long checktime() {
  gettimeofday(&tv, NULL);
  return ((long long)tv.tv_sec) * 1000000 + tv.tv_usec;
}

int mat[MAXN+2][MAXM+2];
int n,m,scorej,scores;
char juc,mutch;

char mut[]={'@','#','+','*','.'};

char atj,ats;

char int2char[5];
int char2int[127];

static inline int max(int a, int b){
  return a>b?a:b;
}

static inline int min(int a, int b){
  return a<b?a:b;
}

static inline int abs_(int x){
  return (x<0?-x:x);
}

int fill(int table[MAXN+2][MAXM+2],int l,int c,int tf,int f){
  int rez=1;

  table[l][c]=f;

  if(table[l-1][c]==tf){
    rez+=fill(table,l-1,c,tf,f);
  }
  if(table[l][c+1]==tf){
    rez+=fill(table,l,c+1,tf,f);
  }
  if(table[l+1][c]==tf){
    rez+=fill(table,l+1,c,tf,f);
  }
  if(table[l][c-1]==tf){
    rez+=fill(table,l,c-1,tf,f);
  }

  return rez;
}

int inglobFill(int table[MAXN+2][MAXM+2],int l,int c,int f){
  int rez=1;
  table[l][c]=f;

  if(table[l-1][c]==-2||table[l][c+1]==-2||table[l+1][c]==-2||table[l][c-1]==-2){
    atj=1;
  }else if(table[l-1][c]==-3||table[l][c+1]==-3||table[l+1][c]==-3||table[l][c-1]==-3){
    ats=1;
  }

  if(table[l-1][c]!=-1&&table[l-1][c]!=-2&&table[l-1][c]!=-3&&table[l-1][c]!=f){
    rez+=inglobFill(table,l-1,c,f);
  }
  if(table[l][c+1]!=-1&&table[l][c+1]!=-2&&table[l][c+1]!=-3&&table[l][c+1]!=f){
    rez+=inglobFill(table,l,c+1,f);
  }
  if(table[l+1][c]!=-1&&table[l+1][c]!=-2&&table[l+1][c]!=-3&&table[l+1][c]!=f){
    rez+=inglobFill(table,l+1,c,f);
  }
  if(table[l][c-1]!=-1&&table[l][c-1]!=-2&&table[l][c-1]!=-3&&table[l][c-1]!=f){
    rez+=inglobFill(table,l,c-1,f);
  }

  return rez;
}

int getScore(int table[MAXN+2][MAXM+2]){
  int ctable[MAXN+2][MAXM+2];
  int l,c,pctj,pcts,pct;

  //copiez tabla in ctable
  for(l=0;l<=n+1;l++){
    for(c=0;c<=m+1;c++){
      ctable[l][c]=table[l][c];
    }
  }

  pctj=fill(ctable,n,1,ctable[n][1],-2);
  pcts=fill(ctable,1,m,ctable[1][m],-3);
  // printf("Score init: %d %d\n",pctj,pcts);

  // for(l=1;l<=n;l++){
  //   for(c=1;c<=m;c++){
  //     if(ctable[l][c]!=-2&&ctable[l][c]!=-3&&ctable[l][c]!=-4){
  //       atj=ats=0;
  //       // pct=inglobFill(ctable,l,c,-4);

  //       if(atj==1&&ats==0){
  //         pctj+=pct;
  //       }else if(atj==0&&ats==1){
  //         pcts+=pct;
  //       }
  //     }
  //   }
  // }

  // printf("Score: %d %d\n",pctj,pcts);

  return pctj-pcts;
}

int evalStatic(int table[MAXN+2][MAXM+2]){
  int score=0;

  //evaluare dupa punctaj
  score+=getScore(table);

  return score;
}

int negamax(int depth,int table[MAXN+2][MAXM+2],int alpha,int beta,int jucl,int jucc){
  int icolor,score,l,c;
  int ctable[MAXN+2][MAXM+2];

  if(maxdepth-depth==5){
    cont=((checktime()-tbase)<MAXTIME);
  }

  if(cont&&depth==maxdepth){
    return (((depth+(1-(juc=='J'?0:1)))&1)*2-1)*evalStatic(table);
  }

  if(cont&&killer[depth]>=0){
    icolor=killer[depth];
    if(mut[icolor]!=int2char[table[n][1]]&&mut[icolor]!=int2char[table[1][m]]){
      for(l=0;l<=n+1;l++){
        for(c=0;c<=m+1;c++){
          ctable[l][c]=table[l][c];
        }
      }
      fill(ctable,jucl,jucc,ctable[jucl][jucc],char2int[mut[icolor]]);

      score=-negamax(depth+1,ctable,-beta,-alpha,(jucl==1?n:1),(jucc==1?m:1));

      if(score>alpha){
        alpha=score;
      }
    }
  }

  icolor=0;
  while(cont&&alpha<beta&&icolor<5){
    if(icolor!=killer[depth]&&mut[icolor]!=int2char[table[n][1]]&&mut[icolor]!=int2char[table[1][m]]){
      for(l=0;l<=n+1;l++){
        for(c=0;c<=m+1;c++){
          ctable[l][c]=table[l][c];
        }
      }
      fill(ctable,jucl,jucc,ctable[jucl][jucc],char2int[mut[icolor]]);

      score=-negamax(depth+1,ctable,-beta,-alpha,(jucl==1?n:1),(jucc==1?m:1));

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
  int l,c,ljuc,cjuc,maxicolor;
  char ch,chjuc;

  for(l=0;l<5;l++){
    char2int[mut[l]]=l;
    int2char[l]=mut[l];
  }

  juc=fgetc(stdin);
  fgetc(stdin);//'\n'

  n=1;
  ch=fgetc(stdin);
  while(!feof(stdin)){
    m=1;
    while(ch!='\n'){
      mat[n][m++]=char2int[ch];
      ch=fgetc(stdin);
    }
    ch=fgetc(stdin);
    n++;
  }
  n--;
  m--;

  for(l=0;l<=n+1;l++){
    mat[l][0]=mat[l][m+1]=-1;
  }
  for(c=0;c<=m+1;c++){
    mat[0][c]=mat[n+1][c]=-1;
  }

  chjuc=int2char[mat[n][1]];
  ljuc=n;
  cjuc=1;
  if(juc=='S'){
    chjuc=int2char[mat[1][m]];
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
    negamax(0,mat,-INFINIT,INFINIT,ljuc,cjuc);
    maxdepth++;
    if(cont){
      maxicolor=killer[0];
    }
  }
  // printf("%d %d\n",maxicolor,maxdepth);
  fill(mat,ljuc,cjuc,char2int[chjuc],char2int[mut[maxicolor]]);

  if(juc=='J'){
    fputc('S',stdout);
  }else{
    fputc('J',stdout);
  }
  fputc('\n',stdout);
  for(l=1;l<=n;l++){
    for(c=1;c<=m;c++){
      fputc(int2char[mat[l][c]],stdout);
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