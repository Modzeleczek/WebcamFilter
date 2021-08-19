#ifndef FileIO_HPP
#define FileIO_HPP

typedef unsigned char uint8_t;
int saveBinary(const char *fileName, const uint8_t *const buffer, const long length);
int loadBinary(const char *fileName, uint8_t **buffer, long *length);
int loadText(const char *fileName, char **buffer);

#endif // FileIO_HPP
