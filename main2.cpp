#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#define MAX 440
#define strmax 51
#include <algorithm>

using namespace std;

struct Img{
    int l;
    int c;
    int mat[MAX][MAX][3];
    };


void abre_imagem(char nome[], Img& imagem_in);
void salva_img(char nome[], Img imagem_out);
void converte_para_cinza(Img imagem_in, Img& imagem_out);
void corta_rgb(Img imagem_in, Img& imagem_out, int li, int ci, int lf, int cf);
void gira_90(Img imagem_in,Img& imagem_out);
void binariza(Img imagem_in, Img& imagem_out, int limiar);
void diminui_tamanho(Img imagem_in, Img& imagem_out);
void borra_imagem(Img imagem_in, Img& imagem_out);
void equaliza_histograma(Img imagem_in, Img& imagem_out);
void computa_derivada_horizontal(Img imagem_in, Img& imagem_out);
void remove_ruido_mediana(Img imagem_in, Img& imagem_out);
void aumenta_tamanho(Img imagem_in, Img& imagem_out);



int main(){
    char nome[strmax],novo_nome[strmax];
    int li=0,ci=0,lf=0,cf=0,n=0,limiar;
    Img imagem_in,imagem_out;
    cout<<"Insira o nome da imagem \n";
    cin>>nome;
    abre_imagem(nome,imagem_in); // ABRE IMAGEM
    cout<<"                MENU DE ESCOLHAS              \n\n"; //MENU
    cout<<"1 PARA CORTAR A IMAGEM\n";
    cout<<"2 PARA CONVERTER A IMAGEM EM CINZA\n";
    cout<<"3 PARA GIRAR A IMAGEM EM 90°\n";
    cout<<"4 PARA BINARIZAR A IMAGEM\n";
    cout<<"5 PARA DIMINUIR O TAMANHO DA IMAGEM\n";
    cout<<"6 PARA AUMENTAR A IMAGEM\n";
    cout<<"7 PARA BORRAR A IMAGEM\n";
    cout<<"8 PARA DERIVAR A IMAGEM\n";
    cout<<"9 PARA FILTRO(HISTOGRAMA)\n";
    cout<<"10 PARA REMOVER RUIDO\n";

    cin>>n; //ESCOLHAS DO MENU
    if(n==1){
    cout<<"Insira a linha inicial coluna inicial, e linha final coluna final \n";
    cin>>li>>ci>>lf>>cf;
    corta_rgb(imagem_in,imagem_out,li,ci,lf,cf);
    }
    if(n==2){
    converte_para_cinza(imagem_in,imagem_out);
    }
    if(n==3){
    gira_90(imagem_in,imagem_out);
    }
    if(n==4){
    converte_para_cinza(imagem_in,imagem_in);
    cout<<"Insira o limiar\n";
    cin>>limiar;
    binariza(imagem_in,imagem_out,limiar);
    }
    if(n==5){
    diminui_tamanho(imagem_in,imagem_out);
    }
    if(n==6){
    aumenta_tamanho(imagem_in,imagem_out);
    }
    if(n==7){
    borra_imagem(imagem_in,imagem_out);
    }
    if(n==8){
    cout<<"1 DERIVADA HORIZONTAL\n";
    cout<<"2 DERIVADA VERTICAL\n";
    computa_derivada_horizontal(imagem_in,imagem_out);
    }
    if(n==9){
    equaliza_histograma(imagem_in,imagem_out);
    }
    if(n==10){
    remove_ruido_mediana(imagem_in,imagem_out);
    }

    cout<<"Insira o documento de saida \n"; // SALVA IMAGEM
    cin>>novo_nome;
    salva_img(novo_nome,imagem_out);

    return 0;

}
void abre_imagem(char nome[],Img& imagem_in){
    fstream arq;
    arq.open(nome);
    if(!arq.is_open()){
        cout<<"O arquivo nao foi aberto \n";
        exit(1);
    }
    char P[3];
    int maximo;
    arq>>P;
    if(P[0]!='P'&& P[1]!='3'){
        cout<<"Arquivo aberto nao e de formato P3\n";
        exit(1);
    }
    arq>>imagem_in.c;
    arq>>imagem_in.l;
    arq>>maximo;
    for(int i=0; i<imagem_in.l; i++){
        for(int j=0; j<imagem_in.c;j ++){
            for(int f=0; f<3; f++){
                arq>>imagem_in.mat[i][j][f];
            }
        }
    }
}
void salva_img(char nome[], Img imagem_out){
    ofstream arq;
    arq.open(nome);
    arq<<"P3\n";
    arq<<imagem_out.c<<" "<<imagem_out.l<<endl<<"255";
    for(int i=0; i<imagem_out.l; i++){
        arq<<endl;
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                arq<<imagem_out.mat[i][j][f]<<" ";

            }
        }
    }
}
void converte_para_cinza(Img imagem_in, Img& imagem_out){
    imagem_out.c=imagem_in.c;
    imagem_out.l=imagem_in.l;
    int k=0;
    for(int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                imagem_out.mat[i][j][f]=(imagem_in.mat[i][j][0]+imagem_in.mat[i][j][1]+imagem_in.mat[i][j][2])/3;
            }
        }
    }
}
void corta_rgb(Img imagem_in, Img& imagem_out, int li, int ci, int lf, int cf){
    imagem_out.l=lf-li+1;
    imagem_out.c=cf-ci+1;
    int x=-1,y=-1,z=-1;
    for(int i=li; i<=lf; i++){
        x++;
        if(x==imagem_out.l)
            x=0;
        for(int j=ci; j<=cf; j++){
            y++;
            if(y==imagem_out.c)
                y=0;
            for(int f=0; f<3; f++){
                imagem_out.mat[x][y][f]=imagem_in.mat[i][j][f];
            }
        }
    }
}
void gira_90(Img imagem_in,Img& imagem_out){
    int k;
    cout<<"1 PARA GIRAR A IMAGEM PARA A DIREITA\n";
    cout<<"2 PARA GIRAR A IMAGEM PARA A ESQUERDA\n";
    cin>>k;
    imagem_out.c=imagem_in.l;
    imagem_out.l=imagem_in.c;
    for(int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                if(k==1)
                    imagem_out.mat[i][j][f]=imagem_in.mat[imagem_in.l-j][i][f];
                if(k==2)
                    imagem_out.mat[i][j][f]=imagem_in.mat[j][imagem_in.c-i][f];
            }
        }
    }
}
void binariza(Img imagem_in, Img& imagem_out, int limiar){
    imagem_out.l=imagem_in.l;
    imagem_out.c=imagem_in.c;
    for(int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                if(imagem_in.mat[i][j][f]<limiar){
                    imagem_out.mat[i][j][f]=0;
                    }
                else
                    imagem_out.mat[i][j][f]=255;
            }
        }
    }
}
void diminui_tamanho(Img imagem_in, Img& imagem_out){
    imagem_out.l=imagem_in.l/2;
    imagem_out.c=imagem_in.c/2;
    for(int i=0; i<imagem_in.l; i++){
        if(i%2!=0)
            continue;
        for(int j=0; j<imagem_in.c; j++){
            if(j%2!=0)
                continue;
            for(int f=0; f<3; f++){
                imagem_out.mat[i/2][j/2][f]=imagem_in.mat[i][j][f];
            }
        }
    }
}
void borra_imagem(Img imagem_in, Img& imagem_out){
    imagem_out.l=imagem_in.l;
    imagem_out.c=imagem_in.c;
    for (int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0;f<3;f++){
                if(i==0 && j==0){
                    imagem_out.mat[i][j][f]=((imagem_in.mat[i][j][f])+(imagem_in.mat[i][j+1][f])+(imagem_in.mat[i+1][j][f])+(imagem_in.mat[i+1][j+1][f]))/9;
                    continue;
                }
                if(i==0 && j==imagem_out.c-1){
                    imagem_out.mat[i][j][f]=((imagem_in.mat[i][j][f])+(imagem_in.mat[i][j-1][f])+(imagem_in.mat[i+1][j-1][f])+imagem_in.mat[i+1][j][f])/9;
                    continue;
                }
                if(i==imagem_out.l-1 && j==0){
                    imagem_out.mat[i][j][f]=((imagem_in.mat[i][j][f])+(imagem_in.mat[i][j+1][f])+(imagem_in.mat[i-1][j][f])+(imagem_in.mat[i-1][j+1][f]))/9;
                    continue;
                }
                if(i==imagem_out.l-1 && j==imagem_out.c-1){
                    imagem_out.mat[i][j][f]=((imagem_in.mat[i][j][f])+(imagem_in.mat[i][j-1][f])+(imagem_in.mat[i-1][j-1][f])+(imagem_in.mat[i-1][j][f]))/9;
                    continue;
                }
                if(j==imagem_out.c-1){
                    imagem_out.mat[i][j][f]=((imagem_in.mat[i][j][f])+(imagem_in.mat[i][j-1][f])+(imagem_in.mat[i-1][j-1][f])+(imagem_in.mat[i-1][j][f])+
                    (imagem_in.mat[i+1][j-1][f])+(imagem_in.mat[i+1][j+1][f]))/9;
                    continue;
                }
                if(i==imagem_out.l-1 ){
                    imagem_out.mat[i][j][f]=((imagem_in.mat[i][j-1][f])+(imagem_in.mat[i][j][f])+(imagem_in.mat[i][j+1][f])+(imagem_in.mat[i-1][j-1][f])+
                    (imagem_in.mat[i-1][j][f])+(imagem_in.mat[i-1][j+1][f]))/9;
                    continue;
                }
                if(i==0){
                imagem_out.mat[i][j][f]=((imagem_in.mat[i][j-1][f])+(imagem_in.mat[i][j][f])+(imagem_in.mat[i][j+1][f])+
                (imagem_in.mat[i+1][j-1][f])+(imagem_in.mat[i+1][j][f])+(imagem_in.mat[i+1][j+1][f]))/9;
                    continue;
                }
                if(j==0){
                imagem_out.mat[i][j][f]=((imagem_in.mat[i-1][j][f])+(imagem_in.mat[i-1][j+1][f])+
                (imagem_in.mat[i][j][f])+(imagem_in.mat[i][j+1][f])+
                (imagem_in.mat[i+1][j][f])+(imagem_in.mat[i+1][j+1][f]))/9;
                    continue;
                }
                if(i!=0 && j!=0){
                imagem_out.mat[i][j][f]=((imagem_in.mat[i-1][j-1][f])+(imagem_in.mat[i-1][j][f])+(imagem_in.mat[i-1][j+1][f])+
                (imagem_in.mat[i][j-1][f])+(imagem_in.mat[i][j][f])+(imagem_in.mat[i][j+1][f])+
                (imagem_in.mat[i+1][j-1][f])+(imagem_in.mat[i+1][j][f])+(imagem_in.mat[i+1][j+1][f]))/9;
                }

            }
        }
    }
}
void equaliza_histograma(Img imagem_in, Img& imagem_out){
    imagem_out.l=imagem_in.l;
    imagem_out.c=imagem_in.c;
    int vr[256]={0},vg[256]={0},vb[256]={0},somar=0,somag=0,somab=0;
    for(int i=0; i<imagem_in.l; i++){
        for(int j=0; j<imagem_in.c; j++){
            for(int f=0; f<3; f++){
                if(f==0)
                vr[imagem_in.mat[i][j][f]]++;
                if(f==1)
                vg[imagem_in.mat[i][j][f]]++;
                if(f==2)
                vb[imagem_in.mat[i][j][f]]++;
            }
        }
    }
    for(int i=0; i<imagem_in.l; i++){
        for(int j=0; j<imagem_in.c; j++){
            for(int f=0; f<3; f++){
                for(int k=0; k<=imagem_in.mat[i][j][f]; k++){
                    if(f==0)
                    somar=somar+vr[k];
                    if(f==1)
                    somag=somag+vg[k];
                    if(f==2)
                    somab=somab+vb[k];
                    if(k==imagem_in.mat[i][j][f]){
                        if(f==0){
                        imagem_out.mat[i][j][f]=(somar*255)/(imagem_out.l*imagem_out.c);
                        somar=0;
                        }
                        if(f==1){
                        imagem_out.mat[i][j][f]=(somag*255)/(imagem_out.l*imagem_out.c);
                        somag=0;
                        }
                        if(f==2){
                        imagem_out.mat[i][j][f]=(somab*255)/(imagem_out.l*imagem_out.c);
                        somab=0;
                        }
                    }
                }
            }
        }
    }
}
void computa_derivada_horizontal(Img imagem_in, Img& imagem_out){
    imagem_out.l=imagem_in.l;
    imagem_out.c=imagem_in.c;
    int n;
    cin>>n;
    float mat[MAX][MAX][3],mat2[MAX][MAX][3];
    for(int i=0; i<imagem_in.l; i++){
        for(int j=0; j<imagem_in.c; j++){
            for(int f=0; f<3; f++){
                mat[i][j][f]=imagem_in.mat[i][j][f];
            }
        }
    }
    if(n==1){
        for(int i=0; i<imagem_in.l; i++){
            for(int j=0; j<imagem_in.c; j++){
                for(int f=0; f<3; f++){
                    if(i==0 && j==0){
                        mat2[i][j][f]=((mat[i][j+1][f]*2)+(mat[i+1][j+1][f]))/4;
                        continue;
                    }
                    if(i==0 && j==imagem_out.c-1){
                        mat2[i][j][f]=((mat[i][j-1][f]*-2)+(mat[i+1][j-1][f]*-1))/4;
                        continue;
                    }
                    if(i==imagem_out.l-1 && j==0){
                        mat2[i][j][f]=((mat[i-1][j+1][f])+(mat[i][j+1][f]*2)/4);
                        continue;
                    }
                    if(i==imagem_out.l-1 && j==imagem_out.c-1){
                        mat2[i][j][f]=((mat[i][j-1][f]*-2)+(mat[i-1][j-1][f]*-1))/4;
                        continue;
                    }
                    if(i==0){
                        mat2[i][j][f]=((mat[i][j-1][f]*-2)+(mat[i][j+1][f]*2)+
                        (mat[i+1][j-1][f]*-1)+(mat[i+1][j+1][f]))/4;
                        continue;
                    }
                    if(i==imagem_out.l-1 ){
                        mat2[i][j][f]=((mat[i][j-1][f]*-2)+(mat[i][j+1][f]*2)+(mat[i-1][j-1][f]*-1)+(mat[i-1][j+1][f]*1))/4;
                        continue;
                    }
                    if(j==0 ){
                        mat2[i][j][f]=((mat[i-1][j+1][f])+(mat[i][j+1][f]*2)+(mat[i+1][j+1][f]))/4;
                        continue;
                    }
                    if(j==imagem_out.l-1){
                        mat2[i][j][f]=((mat[i-1][j-1][f]*-1)+(mat[i][j-1][f]*-2)+(mat[i+1][j-1][f])*-1)/4;
                        continue;
                    }
                    if(i!=0 && j!=0){
                    mat2[i][j][f]=((mat[i-1][j+1][f])+(mat[i][j+1][f]*2)+(mat[i+1][j+1][f])+(mat[i-1][j-1][f]*-1)+(mat[i][j-1][f]*-2)+(mat[i][j][f]*-1))/4;
                    }
                }
            }
        }
    }
    if(n==2){
        for(int i=0; i<imagem_in.l; i++){
            for(int j=0; j<imagem_in.c; j++){
                for(int f=0; f<3; f++){
                    if(i==0 && j==0){
                        mat2[i][j][f]=((mat[i+1][j][f]*2)+(mat[i+1][j+1][f]))/4;
                        continue;
                    }
                    if(i==0 && j==imagem_out.c-1){
                        mat2[i][j][f]=((mat[i+1][j-1][f]*1)+(mat[i+1][j][f]*2))/4;
                        continue;
                    }
                    if(i==imagem_out.l-1 && j==0){
                        mat2[i][j][f]=((mat[i-1][j][f]*-2)+(mat[i-1][j+1][f]*-1))/4;
                        continue;
                    }
                    if(i==imagem_out.l-1 && j==imagem_out.c-1){
                        mat2[i][j][f]=((mat[i-1][j-1][f]*-1)+(mat[i-1][j][f]*-2))/4;
                        continue;
                    }
                    if(i==0){
                        mat2[i][j][f]=((mat[i+1][j-1][f]*1)+(mat[i+1][j][f]*2)+(mat[i+1][j+1][f]*1))/4;
                        continue;
                    }
                    if(i==imagem_out.l-1){
                        mat2[i][j][f]=((mat[i-1][j-1][f]*-1)+(mat[i-1][j][f]*-2)+(mat[i-1][j+1][f]*-1))/4;
                        continue;
                    }
                    if(j==0){
                        mat2[i][j][f]=((mat[i-1][j][f]*-2)+(mat[i-1][j+1][f]*-1)+(mat[i+1][j][f]*2)+(mat[i+1][j+1][f]*1))/4;
                        continue;
                    }
                    if(j==imagem_out.c-1){
                        mat2[i][j][f]=((mat[i-1][j-1][f]*-1)+(mat[i-1][j][f]*-2)+(mat[i+1][j-1][f]*1)+(mat[i+1][j][f]*2))/4;
                        continue;
                    }
                    if(i!=0 && j!=0){
                    mat2[i][j][f]=((mat[i-1][j-1][f]*-1)+(mat[i-1][j][f]*-2.0)+(mat[i-1][j+1][f]*-1)
                    +(mat[i+1][j-1][f]*1)+(mat[i+1][j][f]*2)+(mat[i+1][j+1][f]*1))/4;
                    }
                }
            }
        }
    }
        for(int i=0; i<imagem_in.l; i++){
        for(int j=0; j<imagem_in.c; j++){
            for(int f=0; f<3; f++){
                imagem_out.mat[i][j][f]=abs(mat2[i][j][f]);
            }
        }
    }
}


void remove_ruido_mediana(Img imagem_in, Img& imagem_out){
    imagem_out.l=imagem_in.l;
    imagem_out.c=imagem_in.c;
    int n=9,aux=0;
    int v[MAX];
    for(int i=0; i<10; i++){
        v[i]=0;
    }
    for(int i=0; i<imagem_in.l; i++){
        for(int j=0; j<imagem_in.c; j++){
            for(int f=0; f<3; f++){
                if(i==0 && j==0){
                v[0]=imagem_in.mat[i][j][f];v[1]=imagem_in.mat[i][j+1][f];v[2]=imagem_in.mat[i+1][j][f];v[3]=imagem_in.mat[i+1][j+1][f];
                }
                if(i==0 && j!=0){
                v[0]=imagem_in.mat[i][j-1][f];v[1]=imagem_in.mat[i][j][f];v[2]=imagem_in.mat[i][j+1][f];v[3]=imagem_in.mat[i+1][j-1][f];
                v[4]=imagem_in.mat[i+1][j][f];v[5]=imagem_in.mat[i+1][j+1][f];
                }
                if(j==0 && i!=0){
                v[0]=imagem_in.mat[i-1][j][f];v[1]=imagem_in.mat[i-1][j+1][f];v[2]=imagem_in.mat[i][j][f];v[3]=imagem_in.mat[i][j+1][f];
                v[4]=imagem_in.mat[i+1][j][f];v[5]=imagem_in.mat[i+1][j+1][f];
                }
                if(i!=0 && j!=0){
                v[0]=imagem_in.mat[i-1][j-1][f];v[1]=imagem_in.mat[i-1][j][f];v[2]=imagem_in.mat[i-1][j+1][f];v[3]=imagem_in.mat[i][j-1][f];
                v[4]=imagem_in.mat[i][j][f];v[5]=imagem_in.mat[i][j+1][f];v[6]=imagem_in.mat[i+1][j-1][f];v[7]=imagem_in.mat[i+1][j][f];
                v[8]=imagem_in.mat[i+1][j+1][f];
                }
                for (int x=0; x<n; x++){
                    for (int y=0; y<n; y++){
                        if (v[x] < v[y]){
                            aux = v[x];
                            v[x] = v[y];
                            v[y] = aux;
                        }
                    }
                }
            imagem_out.mat[i][j][f]=v[4];
            aux=0;
            }
        }
    }
}
void aumenta_tamanho(Img imagem_in, Img& imagem_out){
    imagem_out.l=(imagem_in.l*2)-1;
    imagem_out.c=(imagem_in.c*2)-1;
    // zera matriz
    for(int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                imagem_out.mat[i][j][f]=0;
            }
        }
    }
    // primeira etapa
     for(int i=0,x=0; i<imagem_out.l; i+=2,x++){
        for(int j=0,y=0; j<imagem_out.c; j+=2,y++){
            for(int f=0; f<3; f++){
                    imagem_out.mat[i][j][f]=imagem_in.mat[x][y][f];

            }
        }
    }


   for(int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                if(i==0 || i==imagem_out.l-1){
                    if(i%2!=0){
                        imagem_out.mat[i][j][f]=(imagem_in.mat[i][j-1][f]+imagem_in.mat[i][j+1][f])/2;
                    }
                }
                 if(j==0 || j==imagem_out.c-1){
                    if(j%2!=0){
                        imagem_out.mat[i][j][f]=(imagem_in.mat[i-1][j][f]+imagem_in.mat[i+1][j][f])/2;
                    }
                }
            }
        }
    }



     for(int i=0; i<imagem_out.l; i++){
        for(int j=0; j<imagem_out.c; j++){
            for(int f=0; f<3; f++){
                if(i%2!=0 && j%2!=0){
                    imagem_out.mat[i][j][f]=(imagem_out.mat[i-1][j-1][f]+imagem_out.mat[i-1][j+1][f]+imagem_out.mat[i+1][j-1][f]+
                    imagem_out.mat[i+1][j+1][f])/4;
                }
            }
        }
    }


    for(int i=1; i<imagem_out.l-1; i++){
        for(int j=1; j<imagem_out.c-1; j++){
            for(int f=0; f<3; f++){
                if(i%2==0 && j%2!=0 ){
                    imagem_out.mat[i][j][f]=(imagem_out.mat[i-1][j][f]+imagem_out.mat[i+1][j][f]+imagem_out.mat[i][j-1][f]+
                    imagem_out.mat[i][j+1][f])/4;
                }
                if(j%2==0 && i%2!=0 ){
                    imagem_out.mat[i][j][f]=(imagem_out.mat[i-1][j][f]+imagem_out.mat[i+1][j][f]+imagem_out.mat[i][j-1][f]+
                    imagem_out.mat[i][j+1][f])/4;
                }
            }
        }
    }
}

