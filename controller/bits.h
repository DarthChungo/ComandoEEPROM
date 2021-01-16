#ifndef _BITS_H_
#define _BITS_H_

#define BYTE unsigned char
#define WORD unsigned short

#define BIN0(val) (((val) & 0x01ll) ? 1 : 0)
#define BIN1(val) (((val) & 0x02ll) ? 1 : 0)
#define BIN2(val) (((val) & 0x04ll) ? 1 : 0)
#define BIN3(val) (((val) & 0x08ll) ? 1 : 0)
#define BIN4(val) (((val) & 0x10ll) ? 1 : 0)
#define BIN5(val) (((val) & 0x20ll) ? 1 : 0)
#define BIN6(val) (((val) & 0x40ll) ? 1 : 0)
#define BIN7(val) (((val) & 0x80ll) ? 1 : 0)

#endif
