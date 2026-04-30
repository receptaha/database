#ifndef PAGER_H
#define PAGER_H
#include <fstream>
using namespace std;

const uint32_t PAGE_SIZE = 4096;
const uint32_t MAX_PAGES = 100;

class Pager {
private:
    fstream& fileStream;
    uint32_t fileByte;
    void* pages[MAX_PAGES];

public:
    Pager(fstream& connection);
    uint32_t getFileByte() const;
    uint32_t getPageCount() const;

    void *getPage(uint32_t pageNumber);

    void flush(uint32_t pageNumber) const;
};




#endif
