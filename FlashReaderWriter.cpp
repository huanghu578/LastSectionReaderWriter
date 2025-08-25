#include "FlashReaderWriter.hpp"
#include "TgMbedHelper.h"

FlashReaderWriter::FlashReaderWriter(){
    if(this->init()!=0){        
        TG_ERR_FILE_FUN_LINE;
    }    
    _flashStartAddress = this->get_flash_start();    
    _flashSize         = this->get_flash_size();    
    _pageSize          = this->get_page_size();    
    _lastSectorSize    = this->get_sector_size(this->_flashStartAddress + this->_flashSize - 1);    
    _flashEndAddress   = this->_flashStartAddress + this->_flashSize;    
    _sectorSize        = this->get_sector_size(this->_flashEndAddress-1);
    lastSectorStart    = this->_flashEndAddress-this->_lastSectorSize;
    TG_DEBUG_FILE_FUN_LINE();    
}

CharsToReadWrite* FlashReaderWriter::readCharsFromLastSector(uint32_t lenToRead){
    auto dataHolder=(CharsToReadWrite*) malloc(sizeof(CharsToReadWrite));
    memset((void*)dataHolder, 0, sizeof(CharsToReadWrite));    
    if(this->read(dataHolder->bytes, lastSectorStart, lenToRead) != 0){
        TG_ERR_FILE_FUN_LINE;
    }
    dataHolder->len = lenToRead;    
    TG_DEBUG_FILE_FUN_LINE(__LINE__); 
    return dataHolder;
}

bool FlashReaderWriter::writeCharsToLastSector(CharsToReadWrite data){    
    if( _eraseOneSector(lastSectorStart) == true){
        if(this->program(data.bytes, lastSectorStart, data.len)!=0){
            TG_ERR_FILE_FUN_LINE;
        }
    }    
    TG_DEBUG_FILE_FUN_LINE(__LINE__); 
    return true;    
}

U8ToReadWrite *FlashReaderWriter::readU8sFromLastSector(uint32_t lenToRead){
    auto dataHolder=(U8ToReadWrite*) malloc(sizeof(U8ToReadWrite));
    memset((void*)dataHolder, 0, sizeof(U8ToReadWrite));
    if(this->read(dataHolder->bytes, lastSectorStart, lenToRead) != 0){
        TG_ERR_FILE_FUN_LINE;
    }
    dataHolder->len = lenToRead;
    TG_DEBUG_FILE_FUN_LINE(__LINE__); 
    return dataHolder;
}

bool FlashReaderWriter::writeU8sToLastSector(U8ToReadWrite data){    
    if( _eraseOneSector(lastSectorStart) == true){
        if(this->program(data.bytes, lastSectorStart, data.len)!=0){
            TG_ERR_FILE_FUN_LINE;
        }        
    }
    TG_DEBUG_FILE_FUN_LINE(__LINE__); 
    return true;    
}

bool FlashReaderWriter::_eraseOneSector(uint32_t startAddressOfSector){    
    if( this->erase(startAddressOfSector,this->_sectorSize) != 0){
        TG_ERR_FILE_FUN_LINE;
    }
    return true;
}