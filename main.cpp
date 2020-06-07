#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <stdexcept>
//#include <shlobj.h>
//#include <objbase.h>
#include <thread>
using namespace std;

void fim()
{
    string a;
    cin.ignore(5,'\n');
    return;
}
string Browse()
{
    char filename[ MAX_PATH ];

    OPENFILENAME ofn;
    ZeroMemory( &filename, sizeof( filename ) );
    ZeroMemory( &ofn,      sizeof( ofn ) );
    ofn.lStructSize  = sizeof( ofn );
    ofn.hwndOwner    = NULL;
    ofn.lpstrFilter  = "Arquivos de Legenda\0*.srt\0Any File\0*.*\0";
    ofn.lpstrFile    = filename;
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrTitle   = "Selecione o arquivo de legenda";
    ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA( &ofn ))
    {
        string retorno=filename;
        return retorno;
    }
    else
        throw runtime_error("Arquivo nao selecionado.");
}
string meio;
bool adianta (string linha,int delay,fstream &sai)
{
    int h,m,s,ms;
    int hr,mr,sr,msr;
    int total=0;
    string ret;
    stringstream aux;
    aux<<linha;
    getline(aux,ret,':');
    h=stoi(ret);
    getline(aux,ret,':');
    m=stoi(ret);
    getline(aux,ret,',');
    s=stoi(ret);
    getline(aux,ret,' ');
    ms=stoi(ret);
    ret.clear();
    total=ms+1000*s+60*1000*m+60*60*1000*h-delay;
    hr=total/(1000*60*60);
    total=total-hr*1000*60*60;
    mr=total/(1000*60);
    total=total-mr*1000*60;
    sr=total/1000;
    msr=total-1000*sr;
    if(msr<0||sr<0||mr<0||h<0)
        throw invalid_argument(string("Adiantar essa quantidade estoura o tempo ")+string("minimo.\n"));
    sai<<setfill('0')<<setw(2)<<hr<<":"<<setw(2)<<mr<<":"<<setw(2)<<sr<<":"<<setw(3)<<msr<<meio;
    getline(aux,ret,':');
    h=(int)ret[ret.size()-1]-48+10*(ret[ret.size()-2]-48);
    getline(aux,ret,':');
    m=stoi(ret);
    getline(aux,ret,',');
    s=stoi(ret);
    getline(aux,ret);
    ms=stoi(ret);
    total=ms+1000*s+60*1000*m+60*60*1000*h-delay;
    hr=total/(1000*60*60);
    total=total-hr*1000*60*60;
    mr=total/(1000*60);
    total=total-mr*1000*60;
    sr=total/1000;
    msr=total-1000*sr;
    sai<<setfill('0')<<setw(2)<<hr<<":"<<setw(2)<<mr<<":"<<setw(2)<<sr<<":"<<setw(3)<<msr<<endl;
    return 1;
}

void atrasa (string linha,int delay,fstream &sai)
{
    int h,m,s,ms;
    int hr,mr,sr,msr;
    int carry=0;
    string ret;
    stringstream aux;
    aux<<linha;
    getline(aux,ret,':');
    h=stoi(ret);
    getline(aux,ret,':');
    m=stoi(ret);
    getline(aux,ret,',');
    s=stoi(ret);
    getline(aux,ret,' ');
    ms=stoi(ret);
    ret.clear();
    if(ms+delay>999)
        carry=(ms+delay)/1000;
    msr=(ms+delay)%1000;
    sr=(s+carry)%60;
    if(s+carry>59)
    {
        carry=(s+carry)/60;
    }
    else
        carry=0;
    mr=(m+carry)%60;
    if(m+carry>59)
    {
        carry=(m+carry)/60;
    }
    else
        carry=0;
    hr=h+carry;
    sai<<setfill('0')<<setw(2)<<hr<<":"<<setw(2)<<mr<<":"<<setw(2)<<sr<<":"<<setw(3)<<msr<<meio;
    getline(aux,ret,':');
    h=(int)ret[ret.size()-1]-48+10*(ret[ret.size()-2]-48);
    getline(aux,ret,':');
    m=stoi(ret);
    getline(aux,ret,',');
    s=stoi(ret);
    getline(aux,ret);
    ms=stoi(ret);
    if(ms+delay>999)
        carry=(ms+delay)/1000;
    msr=(ms+delay)%1000;
    sr=(s+carry)%60;
    if(s+carry>59)
    {
        carry=(s+carry)/60;
    }
    else
        carry=0;
    mr=(m+carry)%60;
    if(m+carry>59)
    {
        carry=(m+carry)/60;
    }
    else
        carry=0;
    hr=h+carry;
    sai<<setfill('0')<<setw(2)<<hr<<":"<<setw(2)<<mr<<":"<<setw(2)<<sr<<":"<<setw(3)<<msr<<endl;
}

int main()
{

    fstream ent,sai;
    string linha,nomeent,nomesai;
    int delay;
    int aux=0;
    bool estado;
    try
    {
        nomeent=Browse();
    }
    catch (exception &e)
    {
        cout<<e.what();
        return 0;
    }
    nomesai=nomeent;
    nomesai.pop_back();
    nomesai.pop_back();
    nomesai.pop_back();
    nomesai.pop_back();
    nomesai+=to_string(2)+".srt";
    ent.open(nomeent,fstream::in);
    sai.open(nomesai,fstream::out);
    cout<<"Digite 0 se quiser adiantar ou 1 se quiser atrasar\n";
    cin>>estado;
    cout<<"Insira o tempo desejado de delay em ms (coloque um menos antes se quiser ser em segundos:\n";
    cin>>delay;
    try
    {
        if(delay<0)
            delay=delay*(-1)*1000;
        if(estado)
            while(getline(ent,linha))
            {
                if(linha.find("-->")!=string::npos)
                {
                    if(aux==0)
                    {
                        while(linha[aux]!=' ')
                            aux++;
                        while(linha[aux]<47||linha[aux]>58)
                            meio+=linha[aux++];
                    }
                    atrasa(linha,delay,sai);
                }
                else
                    sai<<linha<<endl;
            }
        else
            while(getline(ent,linha))
            {
                if(linha.find("-->")!=string::npos)
                {
                    if(aux==0)
                    {
                        while(linha[aux]!=' ')
                            aux++;
                        while(linha[aux]<47||linha[aux]>58)
                            meio+=linha[aux++];
                    }
                    if(adianta(linha,delay,sai)==0)
                    {
                        cout<<"Esse adiantamento estoura o tempo 0:00, nao foi possivel realizar a operacao\n";
                        ent.close();
                        sai.close();
                        return 1;
                    }
                }
                else
                    sai<<linha<<endl;
            }
        ent.close();
        sai.close();
    }
    catch (exception &e)
    {
        cout<<e.what();
        cout<<"Pressione enter para sair\n";
        cin.sync();
        cin.ignore();
        ent.close();
        sai.close();
        return 0;
    }
    cout<<"Operacao realizada com sucesso\n";
    cout<<"Pressione enter para sair\n";
    cin.sync();
    cin.ignore();
    return 0;
}
