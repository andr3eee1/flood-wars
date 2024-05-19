/*
   Nume program   : alpha-q.c
   Nume concurent : Pana Andrei, Paunescu Vlad
   E-mail         : panaandreimihai@gmail.com, svsvpaunescu@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 50
#define MAXM 50
#define INFINIT 2147483647

char mat[MAXN+2][MAXM+2];
int dist[MAXN+2][MAXN+2];
int coada2[MAXN*2+2];
char juc;

char mut[]={'@','#','+','.','*'};

char frecvfill[127];

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

int isGameOver(char table[MAXN+2][MAXM+2],int n,int m){
  char ctable[MAXN+2][MAXM+2];
  int l,c,i;

  //copiez tabla in ctable
  for(l=0;l<=n+1;l++){
    for(c=0;c<=m+1;c++){
      ctable[l][c]=table[l][c];
    }
  }

  fill(ctable,n,1,'X',ctable[n][1]);
  fill(ctable,1,m,'Y',ctable[1][m]);
  for(i=0;i<5;i++){
    frecvfill[(int)mut[i]]=0;
  }
  frecvfill['X']=frecvfill['Y']=frecvfill['Z']=0;

  c=m+1;
  l=1;
  while(l<=n&&c==m+1){
    c=1;
    do{
      if(ctable[l][c]!='X'&&ctable[l][c]!='Y'&&ctable[l][c]!='Z'){
        for(i=0;i<5;i++){
          frecvfill[(int)mut[i]]=0;
        }
        frecvfill['X']=frecvfill['Y']=frecvfill['Z']=0;
        fill(ctable,l,c,'Z',-1);
      }
      c++;
    }while(c<=m&&(frecvfill['X']+1)*(frecvfill['Y']+1)==2);
    l++;
  }

  return (l==n+1);
}
int distelem(int ls,int cs,int n,int m, int d){
  int i,l,c,elem,e2;
  int coada[2*MAXN+2][2];
  int coada2[2*MAXN+2][2];
  elem=1;
  coada[1][0]=ls;
  coada[1][1]=cs;
  for(l=1;l<=n;l++){
    for(c=1;c<=m;c++){
      dist[l][c]=-1;
    }
  }
  dist[ls][cs]=d;
  d++;
  while(elem>0){
    e2=0;
    for(i=1;i<=elem;i++){
      l=coada[i][0];
      c=coada[i][1];
      if(dist[l+1][c]==-1){
        e2++;
        coada2[e2][0]=l+1;
        coada2[e2][1]=c;
        dist[l+1][c]=d;
      }
      if(dist[l-1][c]==-1){
        e2++;
        coada2[e2][0]=l-1;
        coada2[e2][1]=c;
        dist[l-1][c]=d;
      }
      if(dist[l][c-1]==-1){
        e2++;
        coada2[e2][0]=l;
        coada2[e2][1]=c-1;
        dist[l][c-1]=d;
      }
      if(dist[l][c+1]==-1){
        e2++;
        coada2[e2][0]=l;
        coada2[e2][1]=c+1;
        dist[l][c+1]=d;
      }
    }
    for(i=1;i<=e2;i++){
      coada[i][0]=coada2[i][0];
      coada[i][1]=coada2[i][1];
    }
    elem=e2;
    d++;
  }
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

        if(frecvfill['X']==1&&frecvfill['Y']==0){
          pctj+=pct;
        }else if(frecvfill['X']==0&&frecvfill['Y']==1){
          pcts+=pct;
        }
      }
    }
  }

  pct=pctj-pcts;
  if(juc=='S'){
    pct=pcts-pctj;
  }

  return pct;
}

static inline int max(int a, int b){
  return a>b?a:b;
}

static inline int min(int a, int b){
  return a<b?a:b;
}

int evalStatic(char table[MAXN+2][MAXM+2],int n,int m){
  int score=0;

  score+=getScore(table,n,m);

  return score;
}

int negamax(char table[MAXN+2][MAXM+2],int n,int m,int depth,int alpha,int beta,int player,int jucl,int jucc){
  int i,val,l,c;
  char ctable[MAXN+2][MAXM+2];
  if(depth==0||isGameOver(table,n,m)){
    return player*evalStatic(table,n,m);
  }

  val=-INFINIT;
  i=0;
  while(i<5&&alpha<beta){
    if(mut[i]!=table[n][1]&&mut[i]!=table[1][m]){
      for(l=0;l<=n+1;l++){
        for(c=0;c<=m+1;c++){
          ctable[l][c]=table[l][c];
        }
      }
      fill(ctable,jucl,jucc,mut[i],ctable[jucl][jucc]);

      val=max(val,-negamax(ctable,n,m,depth-1,-beta,-alpha,-player,(jucl==1?n:1),(jucc==1?m:1)));
      alpha=max(alpha,val);
    }
    i++;
  }

  return val;
}

int main(){
  int n,m,l,c,ljuc,cjuc,p,mutch,i,val;
  char ch,chjuc;
  char ctable[MAXN+2][MAXM+2];

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
  distelem(4,2,n,m,1);
  // printf("%d %d\n",n,m);
  chjuc=mat[n][1];
  ljuc=n;
  cjuc=1;
  if(juc=='S'){
    chjuc=mat[1][m];
    ljuc=1;
    cjuc=m;
  }

  mutch='D';
  val=-INFINIT;
  for(i=0;i<5;i++){
    if(mut[i]!=mat[n][1]&&mut[i]!=mat[1][m]){
      for(l=0;l<=n+1;l++){
        for(c=0;c<=m+1;c++){
          ctable[l][c]=mat[l][c];
        }
      }
      fill(ctable,ljuc,cjuc,mut[i],ctable[ljuc][cjuc]);

      p=negamax(ctable,n,m,6,-INFINIT,INFINIT,1,ljuc,cjuc);
      if(p>val){
        val=p;
        mutch=mut[i];
      }
    }
  }
  fill(mat,ljuc,cjuc,mutch,chjuc);

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
