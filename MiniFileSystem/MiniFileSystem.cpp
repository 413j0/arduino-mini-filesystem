/*
  MiniFileSystem.cpp - Library for handling
  a mini filesystem on EEPROM by 413j0
*/

#include "Arduino.h"
#include "EEPROM.h"
#include "MiniFileSystem.h"


  //public:
  
	Filesystem::Filesystem()
    {
      currentFile = 0;
      Update();
    }
	
	Filesystem::Filesystem(int freeBytes)
	{
      ignoredSpace = freeBytes;
	  currentFile = 0;
      Update();
	}
    void Filesystem::Update()
    {
      fileCount = EEPROM.read(0);
      if (fileCount >= 1) {
        if (currentFile > fileCount) {
          currentFile = 0;
        }
        currentNameLength = EEPROM.read(currentFile + 1);
        currentFileLength = EEPROM.read(fileCount + currentFile + 1);
        freeSpace = (EEPROM.length() - ignoredSpace);
        freeSpace = freeSpace - 1;											//minus the file number byte
        for (int i = 0; i < fileCount; i++) {								//iterate trough file count
          freeSpace = freeSpace - EEPROM.read(1 + i);						//minus the file name lenght
          freeSpace = freeSpace - EEPROM.read((fileCount + 1) + i);			//minus the file lenght
		  freeSpace -= 2;													//minus 2 bytes for each lenght byte
        }
		
        _currentNameStart = _GetNameStart(currentFile);                          
        _currentFileStart = _GetFileStart(currentFile); 


        for (int i = 0; i < currentNameLength; i++) {//set current name
          currentFileName[i] = EEPROM.read(_currentNameStart + i);
        }
        for (int i = currentNameLength; i < _MINI_FILESYSTEM_MAX_NAME_LENGTH; i++) {       //null the rest of the vector
          currentFileName[i] = '\0';
        }
        for (int i = 0; i < currentFileLength; i++) {//set current file
          currentFileContents[i] = EEPROM.read(_currentFileStart + i);
        }
        for (int i = currentFileLength; i < _MINI_FILESYSTEM_MAX_FILE_LENGTH; i++) {       //null the rest of the vector
          currentFileContents[i] = 0;
        }
		
		
      } else {
        Format();
        Update();
      }
    }
    void Filesystem::NextFile()
    {
      if (currentFile < (fileCount - 1) ) {
        currentFile += 1;
      } else {
        currentFile = 0;
      }
      Update();
    }
    void Filesystem::PrevFile()
    {
      if (currentFile > 0) {
        currentFile -= 1;
      } else {
        currentFile = (fileCount - 1);
      }
      Update();
    }
	void Filesystem::GoToFile(int file){
		currentFile = file;
		Update();
	}
    bool Filesystem::EditFile(char *name, int nameLength, byte *file, int fileLength)
    {
		Update();
		int difference =(nameLength + fileLength) -(currentFileLength + currentNameLength);
		if (difference <= freeSpace) {
			_MoveBlock(_currentFileStart + currentFileLength , ((EEPROM.length() - ignoredSpace) - (freeSpace +1)), difference);
				
			EEPROM.update((currentFile + 1), nameLength);
			EEPROM.update((fileCount + currentFile + 1), fileLength);//must update before calling _GetFileStart()
			
			for (int i = 0; i < nameLength; i++) { //add name
				EEPROM.update(_currentNameStart +i, name[i]);
			}
			for (int i = 0; i < fileLength; i++) { //add file
				EEPROM.update(_GetFileStart(currentFile) +i, file[i]);
			}
			Update();
		return true;
		} else {
			return false;
		}
    }

    bool Filesystem::AddFile(char *name, int nameLength, byte *file, int fileLength)
    {
	  Update();
      if ((nameLength + fileLength + 2) <= freeSpace) {//return false if the file doesn't fit
        _MoveBlock((fileCount * 2) + 1, ((EEPROM.length() - ignoredSpace) - (freeSpace +1)), 2); //make space for new size definitions
        _MoveBlock(fileCount + 1, (fileCount * 2), 1);

        EEPROM.update(((fileCount * 2) + 2), fileLength);//set the file size

        EEPROM.update((fileCount + 1), nameLength);//set the name size

		fileCount += 1;//use before calling _GetNameStart or _GetFileStart
		EEPROM.update(0, fileCount);//set the new file count
		
		int newNameStart = _GetNameStart(fileCount -1);
        int newFileStart = _GetFileStart(fileCount -1);

        for (int i = 0; i < nameLength; i++) { //add name
          EEPROM.update(newNameStart +i, name[i]);
        }
        for (int i = 0; i < fileLength; i++) { //add file
          EEPROM.update(newFileStart +i, file[i]);
        }
        Update();
        return true;
      } else {
        return false;
      }
    }
    bool Filesystem::DeleteFile()
    {
	  Update();
      if (fileCount > 1) {
		_MoveBlock(currentFile + 2, (fileCount + currentFile ), -1);
		_MoveBlock((fileCount + currentFile + 2), _currentNameStart -1, -2);
		//_MoveBlock(_currentNameStart + currentNameLength + 1, _currentFileStart -1, -2 -currentNameLength);
		_MoveBlock(_currentFileStart + currentFileLength , ((EEPROM.length() - ignoredSpace) - (freeSpace +1)), -2 -currentNameLength -currentFileLength);
		fileCount -= 1;
        EEPROM.update(0, fileCount);
        if (currentFile > 0) {
          currentFile -= 1;
        }
        Update();
        return true;
      } else {
        return false;
      }
    }
    void Filesystem::Format()
    {
      byte baseFormat[] = _MINI_FILESYSTEM_BASE_FORMAT;
      for (int i = 0; i < _MINI_FILESYSTEM_BASE_FORMAT_LENGTH; i++) {
        EEPROM.update(i, baseFormat[i]);
      }
      Update();
    }
    void Filesystem::DeepFormat()
    {
      Format();
      for (int i = _MINI_FILESYSTEM_BASE_FORMAT_LENGTH; i < (EEPROM.length() - ignoredSpace); i++) {
        EEPROM.update(i, 255);
      }
    }
	

  //private:

    void Filesystem::_MoveBlock(unsigned int blockStart, unsigned int blockEnd, int distance)//use negative numbers to move before
    {
		
	  if (distance > 0){
		  for(int i = blockEnd; i>= blockStart;i--){
			  EEPROM.update(i + distance, EEPROM.read(i));
		  }
	  }else{
		  for(int i = blockStart; i<= blockEnd;i++){
			  EEPROM.update(i + distance, EEPROM.read(i));
		  }
	  }
    }
	int Filesystem::_GetNameStart(int file)
	{
		int NameStart = (fileCount * 2) + 1;                          //set the start to as if file 0
		for (int i = 0; i < file; i++){                            // iterate trough lengths adding the previous names
		  NameStart += (EEPROM.read(i +1));
		}
		for (int i = 0; i < file; i++){                            // iterate trough lengths adding the previous files
		  NameStart += (EEPROM.read(fileCount + i +1));
		}
		return NameStart;
	}
	int Filesystem::_GetFileStart(int file)
	{
		int FileStart = (fileCount * 2) + 1;         //set the start to as if file 0
		for (int i = 0; i <= file; i++){                            // iterate trough lengths adding the previous names including his own 
		  FileStart += (EEPROM.read(i +1));
		  //Serial.print("adding ");Serial.print((int)EEPROM.read(i +1));Serial.print(" as the description of file ");Serial.print(i);
		}
		for (int i = 0; i < file; i++){  					 // iterate trough lengths adding the previous files
		  FileStart += (EEPROM.read(fileCount + i +1));
		}
		return FileStart;
	}







