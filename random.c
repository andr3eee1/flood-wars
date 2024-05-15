//TODO: add the program header

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 50
#define MAXM 50

int mat[MAXN+2][MAXM+2];

char mut[]={'@','#','+','.','*'};

void fill(int l,int c,char f,char tf){
  mat[l][c]=f;
  if(mat[l+1][c]==tf){
    fill(l+1,c,f,tf);
  }
  if(mat[l-1][c]==tf){
    fill(l-1,c,f,tf);
  }
  if(mat[l][c-1]==tf){
    fill(l,c-1,f,tf);
  }
  if(mat[l][c+1]==tf){
    fill(l,c+1,f,tf);
  }
}

int main(){
  int n,m,l,c,r,ljuc,cjuc;
  char ch,juc,chjuc;

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
  // printf("%d %d\n",n,m);

  chjuc=mat[n][1];
  ljuc=n;
  cjuc=1;
  if(juc=='S'){
    chjuc=mat[1][m];
    ljuc=1;
    cjuc=m;
  }

  srand(time(NULL));
  r=rand()%3+1;

  l=0;
  while(l<5&&r>0){
    if(mut[l]!=mat[n][1]&&mut[l]!=mat[1][m]){
      r--;
    }
    l++;
  }

  // printf(":%c;%c:\n",mut[l-1],chjuc);

  fill(ljuc,cjuc,mut[l-1],chjuc);

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