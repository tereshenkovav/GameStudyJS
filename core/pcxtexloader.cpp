#include "pcxtexloader.h"
#include <QFile>
#include <QDebug>

void PcxTexLoader::copyPaletteToData(int * poz, const Color4 &pal)
{
    data[*poz]=pal.R ;
    data[*poz+1]=pal.G ;
    data[*poz+2]=pal.B ;
    data[*poz+3]=pal.A ;
    (*poz)+=4 ;
}

PcxTexLoader::PcxTexLoader()
{

}

PcxTexLoader::~PcxTexLoader()
{    
    delete[] data ;
}

bool PcxTexLoader::loadFile(const QString &filename, bool usetransp)
{    
    QFile file(filename);
    if (!file.exists()) return false ;
    if (!file.open(QIODevice::ReadOnly)) return false ;

    QByteArray res = file.readAll() ;
    file.close();

    width = 256*(unsigned char)res[9]+(unsigned char)res[8]+1 ;
    height = 256*(unsigned char)res[11]+(unsigned char)res[10]+1 ;

    int datalen = width*height*4 ;
    data = new uint8_t[datalen] ;

    QList<Color4> palette ;
    int poz = res.length()-256*3 ;
    Color4 c4 ;
    for (int c=0; c<256; c++) {
        c4.A = 255 ;
        c4.R = (unsigned char)res[poz++] ;
        c4.G = (unsigned char)res[poz++] ;
        c4.B = (unsigned char)res[poz++] ;
        palette.append(c4) ;
    }

    poz = 128 ;
    int datapoz = 0 ;
    while (datapoz<datalen) {
        int c = (unsigned char)res[poz++] ;
        int cnt = 1 ;
        if (c>=192) {
            cnt = c-192 ;
            c = (unsigned char)res[poz++] ;
        }
        if (usetransp)
            if (datapoz==0) palette[c].A=0 ;
        for (int i=0; i<cnt; i++)
            copyPaletteToData(&datapoz,palette[c]) ;
    }

    return true ;
}

int PcxTexLoader::getWidth() const
{
    return width ;
}

int PcxTexLoader::getHeight() const
{
    return height ;
}

uint8_t *PcxTexLoader::getData() const
{
    return data ;
}

