#include "Platform.hpp"
#include "Base64.hpp"
namespace Base64 {
using namespace Sirikata;
static uint32 conservativeBase64Size(size_t x) {
    return (x+2)*4/3+6+x/64+(x%64?1:0);
}
static const uint8 _URL_SAFE_ALPHABET []= {
        (uint8)'A', (uint8)'B', (uint8)'C', (uint8)'D', (uint8)'E', (uint8)'F', (uint8)'G',
        (uint8)'H', (uint8)'I', (uint8)'J', (uint8)'K', (uint8)'L', (uint8)'M', (uint8)'N',
        (uint8)'O', (uint8)'P', (uint8)'Q', (uint8)'R', (uint8)'S', (uint8)'T', (uint8)'U', 
        (uint8)'V', (uint8)'W', (uint8)'X', (uint8)'Y', (uint8)'Z',
        (uint8)'a', (uint8)'b', (uint8)'c', (uint8)'d', (uint8)'e', (uint8)'f', (uint8)'g',
        (uint8)'h', (uint8)'i', (uint8)'j', (uint8)'k', (uint8)'l', (uint8)'m', (uint8)'n',
        (uint8)'o', (uint8)'p', (uint8)'q', (uint8)'r', (uint8)'s', (uint8)'t', (uint8)'u', 
        (uint8)'v', (uint8)'w', (uint8)'x', (uint8)'y', (uint8)'z',
        (uint8)'0', (uint8)'1', (uint8)'2', (uint8)'3', (uint8)'4', (uint8)'5', 
        (uint8)'6', (uint8)'7', (uint8)'8', (uint8)'9', (uint8)'-', (uint8)'_'
    };
	
int translateBase64(uint8*destination, const uint8* source, int numSigBytes) {
    uint8 temp[4];
    uint32 source0=source[0];
    uint32 source1=source[1];
    uint32 source2=source[2];
    uint32 inBuff =   ( numSigBytes > 0 ? ((source0 << 24) / 256) : 0 )
                     | ( numSigBytes > 1 ? ((source1 << 24) / 65536) : 0 )
                     | ( numSigBytes > 2 ? ((source2 << 24) / 65536/ 256) : 0 );

    destination[ 0 ] = _URL_SAFE_ALPHABET[ (inBuff >> 18)        ];
    destination[ 1 ] = _URL_SAFE_ALPHABET[ (inBuff >> 12) & 0x3f ];

    switch( numSigBytes )
    {
      case 3:
        destination[ 2 ] = _URL_SAFE_ALPHABET[ (inBuff >>  6) & 0x3f ];
        destination[ 3 ] = _URL_SAFE_ALPHABET[ (inBuff      ) & 0x3f ];
        return 4;
      case 2:
        destination[ 2 ] = _URL_SAFE_ALPHABET[ (inBuff >>  6) & 0x3f ];
        destination[ 3 ] = '=';
        return 4;
        
      case 1:
        destination[ 2 ] = '=';
        destination[ 3 ] = '=';
        return 4;

      default:
        return 0;
    }   // end switch
    
}

void toBase64(std::vector<unsigned char>&retval,
              const MemoryReference&firstInput, 
              const MemoryReference&secondInput){
    const MemoryReference*refs[2];
    refs[0]=&firstInput;
    refs[1]=&secondInput;
    unsigned int datalen=0;
    uint8 data[3];
    size_t curPlace=retval.size();
    retval.resize(curPlace+conservativeBase64Size(refs[1]->size()+refs[2]->size()));
    size_t retvalSize=retval.size();
    for (int i=0;i<3;++i) {
        const uint8*dat=(const uint8*)refs[i]->data();
        uint32 size=refs[i]->size();
        for (uint32 j=0;j<size;++j) {
            data[datalen++]=dat[j];
            if (datalen==3) {
                if (retvalSize<=curPlace+5) {
                    retval.resize(curPlace+5);
                }
                curPlace+=translateBase64(&*(retval.begin()+curPlace),data,datalen);
                datalen=0;
            }
        }
    }
    if (datalen) {
        if (retvalSize<=curPlace+5) {
            retval.resize(curPlace+5);
            fprintf(stderr,"conservative size estimate incorrect\n");
        }        
        curPlace+=translateBase64(&*(retval.begin()+curPlace),data,datalen);
    }
    retval.resize(curPlace);
}
static signed char WHITE_SPACE_ENC = -5; // Indicates white space in encoding
static signed char EQUALS_SIGN_ENC = -1; // Indicates equals sign in encoding
static int decode4to3(const signed char source[4],std::vector<unsigned char>& destination, int destOffset) {
    uint32 outBuf=((source[0]<<18)|(source[1]<<12));
    destination[destOffset]=(uint8)(outBuf/65536);
    if (source[2]==EQUALS_SIGN_ENC) {
        return 1;
    }
    outBuf|=(source[2]<<6);
    destination[destOffset+1]=(uint8)((outBuf/256)&255);                
    if (source[3]==EQUALS_SIGN_ENC) {
            return 2;
    }
    outBuf|=source[3];
    destination[destOffset+2]=(uint8)(outBuf&255);                        
    return 3;
}

static signed char DECODABET [] = {
        -9,-9,-9,-9,-9,-9,-9,-9,-9,                 // Decimal  0 -  8
        -5,-5,                                      // Whitespace: Tab and Linefeed
        -9,-9,                                      // Decimal 11 - 12
        -5,                                         // Whitespace: Carriage Return
        -9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,     // Decimal 14 - 26
        -9,-9,-9,-9,-9,                             // Decimal 27 - 31
        -5,                                         // Whitespace: Space
        -9,-9,-9,-9,-9,-9,-9,-9,-9,-9,              // Decimal 33 - 42
        62,                                         // Plus sign at decimal 43
        -9,-9,-9,                                   // Decimal 44 - 46
        63,                                         // Slash at decimal 47
        52,53,54,55,56,57,58,59,60,61,              // Numbers zero through nine
        -9,-9,-9,                                   // Decimal 58 - 60
        -1,                                         // Equals sign at decimal 61
        -9,-9,-9,                                      // Decimal 62 - 64
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,            // Letters 'A' through 'N'
        14,15,16,17,18,19,20,21,22,23,24,25,        // Letters 'O' through 'Z'
        -9,-9,-9,-9,-9,-9,                          // Decimal 91 - 96
        26,27,28,29,30,31,32,33,34,35,36,37,38,     // Letters 'a' through 'm'
        39,40,41,42,43,44,45,46,47,48,49,50,51,     // Letters 'n' through 'z'
        -9,-9,-9,-9,-9                                 // Decimal 123 - 126
};
static signed char URLSAFEDECODABET [] = {
        -9,-9,-9,-9,-9,-9,-9,-9,-9,                 // Decimal  0 -  8
        -5,-5,                                      // Whitespace: Tab and Linefeed
        -9,-9,                                      // Decimal 11 - 12
        -5,                                         // Whitespace: Carriage Return
        -9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,     // Decimal 14 - 26
        -9,-9,-9,-9,-9,                             // Decimal 27 - 31
        -5,                                         // Whitespace: Space
        -9,-9,-9,-9,-9,-9,-9,-9,-9,-9,              // Decimal 33 - 42
        62,                                         // Plus sign at decimal 43
        -9,62,-9,                                   // Decimal 44 - 46
        63,                                         // Slash at decimal 47
        52,53,54,55,56,57,58,59,60,61,              // Numbers zero through nine
        -9,-9,-9,                                   // Decimal 58 - 60
        -1,                                         // Equals sign at decimal 61
        -9,-9,-9,                                      // Decimal 62 - 64
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,            // Letters 'A' through 'N'
        14,15,16,17,18,19,20,21,22,23,24,25,        // Letters 'O' through 'Z'
        -9,-9,-9,-9,63,-9,                          // Decimal 91 - 94 underscore 95
        26,27,28,29,30,31,32,33,34,35,36,37,38,     // Letters 'a' through 'm'
        39,40,41,42,43,44,45,46,47,48,49,50,51,     // Letters 'n' through 'z'
        -9,-9,-9,-9,-9                                 // Decimal 123 - 127
};


void fromBase64(std::vector<unsigned char>&retval,
                const MemoryReference&a) {
    const uint8*begin=(const uint8*)a.data();
    const uint8*end=(const uint8*)begin+a.size();
    int outBuffPosn=retval.size();
  
    retval.resize(retval.size()+(((end-begin)*3)/4+3));//maximum of the size;
    int remainderShift=0;
    
    signed char b4[4];
    uint8 b4Posn=0;
    for (;begin!=end;++begin) {
        uint8 cur=(*begin);
        uint8 sbiCrop = (uint8)(cur & 0x7f); // Only the low seven bits
        signed char sbiDecode = URLSAFEDECODABET[ sbiCrop ];   // Special value
            
        // White space, Equals sign, or legit Base64 character
        // Note the values such as -5 and -9 in the
        // DECODABETs at the top of the file.
        if( sbiDecode >= WHITE_SPACE_ENC && cur==sbiCrop )  {
            if( sbiDecode >= EQUALS_SIGN_ENC ) {
                b4[ b4Posn++ ] = sbiDecode;           // Save non-whitespace
                if( b4Posn > 3 ) {                  // Time to decode?
                    outBuffPosn += decode4to3( b4, retval, outBuffPosn );
                    b4Posn = 0;
                    // If that was the equals sign, break out of 'for' loop
                    if( sbiDecode == EQUALS_SIGN_ENC ) {
                        break;
                    }   // end if: equals sign
                }   // end if: quartet built
            }   // end if: equals sign or better
        }   // end if: white space, equals sign or better
    }
    assert(outBuffPosn<=(int)retval.size());
    retval.resize(outBuffPosn);
}
}
