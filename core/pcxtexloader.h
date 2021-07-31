#ifndef PCXTEXLOADER_H
#define PCXTEXLOADER_H

#include <QString>

struct Color4 {
    uint8_t R ;
    uint8_t G ;
    uint8_t B ;
    uint8_t A ;
};

class PcxTexLoader
{
private:
    uint8_t * data ;
    int width ;
    int height ;
    void copyPaletteToData(int * poz, const Color4 & pal) ;
public:
    PcxTexLoader();
    ~PcxTexLoader() ;
    bool loadFile(const QString & filename, bool usetransp) ;
    int getWidth() const ;
    int getHeight() const ;    
    uint8_t * getData() const ;    
};

#endif // PCXTEXLOADER_H
