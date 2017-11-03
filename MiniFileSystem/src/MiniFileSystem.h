
/*
   minimal fs library for arduino EEPROM with a maximum of 255 files
 */

/*
   MiniFileSystem.h

   Contributors:
     Created by alejandro usuga <413j0@413j0.com>
 */
 
#ifndef _Mini_Filesystem_
#define _Mini_Filesystem_

#include "Arduino.h"
// maximum file length (don't use more than 255)
#ifndef _MINI_FILESYSTEM_MAX_FILE_LENGTH
#define _MINI_FILESYSTEM_MAX_FILE_LENGTH 255
#endif
// maximum name length (don't use more than 255)
#ifndef _MINI_FILESYSTEM_MAX_NAME_LENGTH
#define _MINI_FILESYSTEM_MAX_NAME_LENGTH 255
#endif
// base filesystem format (include first file)
#ifndef _MINI_FILESYSTEM_BASE_FORMAT
#define _MINI_FILESYSTEM_BASE_FORMAT {1,4,9,'b','a','s','e','b','a','s','e',' ','f','i','l','e'}
#endif
// base filesystem format format length
#ifndef _MINI_FILESYSTEM_BASE_FORMAT_LENGTH
#define _MINI_FILESYSTEM_BASE_FORMAT_LENGTH 16
#endif

class Filesystem
{
  public:
	/**
     * stores the currently selected file
     */
    int currentFile;
	
	/**
     * stores the currently free space
     */
    int freeSpace;
	
	/**
     * stores the current file lenght in bytes
     */
    int currentFileLength;
	
	/**
     * stores the current name length in bytes
     */
    int currentNameLength;
	
	/**
     * stores the the ammount of files currently stored
     */
    int fileCount;
	
	/**
     * stores the current filename followed by 0s
     */
    char* currentFileName = (char*)malloc(_MINI_FILESYSTEM_MAX_NAME_LENGTH);
	
	/**
     * stores the current file followed by 0s
     */
    byte* currentFileContents = (byte*)malloc(_MINI_FILESYSTEM_MAX_FILE_LENGTH);

	
	/**
     * Construct a new filesystem instance
     *
     */
	Filesystem();
	
	/**
     * Call after a manual edit of memory or public variable edit on the filesystem
	 * to update the rest of the viaribles
	 *
	 * ONLY CALL WHEN THE FILESYSTEM IS IN A CORRECT STATE
     */
	void Update();
	
	/**
     * Go to the next file
     */
	void NextFile();
	
	/**
     * Go to the previous file
     */
	void PrevFile();
	
	/**
     * Goto an especific file
	 * returns false if the file doent exist, otherwise true
     */
	void GoToFile(int file);
	
	/**
     * Edits the currently selected file
     *
     * @param new file name
     * @param the lenght of the  new file name (non null terminated)
	 * @param new file
     * @param the lenght of the  new file 
     * 
     * returns false on not enough free space
     */
	bool EditFile(char *name, int nameLength, byte *file, int fileLength);
	
	/**
     * Adds a new file to the filesystem
     *
     * @param new file name
     * @param the lenght of the  new file name (non null terminated)
	 * @param new file
     * @param the lenght of the  new file 
     * 
     * returns false on not enough free space
     */
	bool AddFile(char *name, int nameLength, byte *file, int fileLength);
	
	/**
     * Deletes the currently selected file
     * 
     * returns false if it is the only file on storage
     */
	bool DeleteFile();
	
	/**
     * Format the filesystem
     */
	void Format();
	
	/**
     * Format the filesystem and make all empty space 255
     */
	void DeepFormat();
	
	
  private: 
    int _currentFileStart;
    int _currentNameStart;
	
	int _GetNameStart(int file);
	int _GetFileStart(int file);
	void _MoveBlock(unsigned int blockStart, unsigned int blockEnd, int distance);
};


#endif




