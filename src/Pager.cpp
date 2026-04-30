#include "../include/Pager.h"
#include <iostream>


Pager::Pager(const string& filePath) : filePath(filePath){
    if (fileStream.is_open()) {
        fileStream.close();
    }

    fileStream.open(filePath, ios::in | ios::out | ios::binary);
    if (!fileStream.is_open()) {
        fileStream.open(filePath, ios::out);
        fileStream.close();
        fileStream.open(filePath, ios::in | ios::out | ios::binary);
    }

    if (!fileStream.is_open()) {
        cerr << "Filestream error" << endl;
        return;
    }

    for (uint32_t i = 0; i < MAX_PAGES; i++) {
        pages[i] = nullptr;
    }

    fileStream.seekg(0, ios::end);
    this->fileByte = fileStream.tellg();
    fileStream.seekg(0, ios::beg);

    cout << "Pager has been started successfully. File size: " << this->fileByte << " byte." << endl;
}

uint32_t Pager::getFileByte() const {
    return this->fileByte;
}

uint32_t Pager::getPageCount() const {
    return (this->fileByte % PAGE_SIZE == 0) ? (this->fileByte / PAGE_SIZE) : ((this->fileByte / PAGE_SIZE) + 1);
}

void* Pager::getPage(uint32_t pageNumber) {
    if (pageNumber >= MAX_PAGES) {
        cerr << "GET PAGE ERROR: Requested page number is bigger than MAX_PAGE";
        exit(1);
    }

    if (pages[pageNumber] != nullptr) return pages[pageNumber]; // İstenilen sayfa pager'ın elindeyse direkt sayfayı verir.

    char* buffer = new char[PAGE_SIZE];
    uint32_t offset = pageNumber * PAGE_SIZE;


    if (offset < this->getFileByte()) {
        // İnsert işlemi yapılmıyorsa
        fileStream.seekg(offset, ios::beg);
        uint32_t bytesToRead = this->getFileByte() - offset < PAGE_SIZE ? this->getFileByte() - offset : PAGE_SIZE;
        fileStream.read(buffer, bytesToRead);
    }

    pages[pageNumber] = buffer;
    return buffer;
}

void Pager::flush(uint32_t pageNumber){
    if (pageNumber >= MAX_PAGES) {
        cerr << "FLUSH ERROR: Requested page number is bigger than MAX_PAGE";
        exit(1);
    }

    if (this->pages[pageNumber] == nullptr) return;

    uint32_t offset = pageNumber * PAGE_SIZE;
    fileStream.seekp(offset, ios::beg);

    fileStream.write((char*) this->pages[pageNumber] , PAGE_SIZE);
    fileStream.flush();
}
