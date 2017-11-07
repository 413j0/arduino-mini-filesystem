#define _MINI_FILESYSTEM_MAX_FILE_LENGTH 63

#define _MINI_FILESYSTEM_MAX_NAME_LENGTH 20
#include "MiniFileSystem.h"
#include "EEPROM.h"
Filesystem eeprom;
void setup() {

  while (!Serial) {
    ;
  }
  delay(8000);
  eeprom.DeepFormat();//to prevent wearing out the EEPROM yo can use Format(); instead, DeepFormat(); was used so the dump would be clearer
}

void loop() {
  fileInfo();
  eeprom.AddFile("testfile1", 9, "testing first file", 18);
  fileInfo();
  eeprom.AddFile("testing file2", 13, "testing file write", 18);
  fileInfo();
  eeprom.AddFile("3", 1, "3", 1);
  fileInfo();
  eeprom.AddFile("very large file and name4", 25, "1234567890qwertyuiopasdfghjklñzxcvbnm,.- large file", 52);//52 because ñ is 2 chars
  fileInfo();
  eeprom.AddFile("file5", 5, "another file", 12);
  fileInfo();
  int counter = 54;
  while (true) {//fill memory with files
    char newname[10];
    for (int i = 0; i < 10; i++) {
      newname[i] = counter;
    }
    char newfile[200];
    for (int i = 0; i < 200; i++) {
      newfile[i] = counter;
    }
    counter ++;
    if (!eeprom.AddFile(newname, 10, newfile, 200)) {//AddFile() returns false on not enough space, so we can use it to check for full memory
      break;
    }
  }
  fileInfo();
  eeprom.GoToFile(1);
  eeprom.EditFile("1", 1, "1", 1);
  fileInfo();
  eeprom.GoToFile(2);
  eeprom.EditFile("2", 1, "2", 1);
  fileInfo();
  eeprom.GoToFile(1);
  eeprom.DeleteFile();
  fileInfo();
  eeprom.GoToFile(4);
  eeprom.DeleteFile();
  fileInfo();
  while (true) {
    ;
  }

}
void fileInfo() {
  eeprom.GoToFile(0);
  for (int i = 0; i < eeprom.fileCount; i++) {
    Serial.print("current File:           ");
    Serial.println(eeprom.currentFile);

    Serial.print("free Space:             ");
    Serial.println(eeprom.freeSpace);

    Serial.print("current FileLength:     ");
    Serial.println(eeprom.currentFileLength);

    Serial.print("current Name Length:    ");
    Serial.println(eeprom.currentNameLength);

    Serial.print("file Count:             ");
    Serial.println(eeprom.fileCount);

    Serial.print("filename:               ");
    Serial.println(eeprom.currentFileName);

    Serial.print("file(ascii):            ");
    Serial.println((char*)eeprom.currentFileContents);

    Serial.print("file(int bytes):        ");
    Serial.print('{');
    for (int i = 0; i < eeprom.currentFileLength; i++) {
      Serial.print((int)eeprom.currentFileContents[i]);
      Serial.print(',');
    }
    Serial.print('}');
    Serial.println();

    Serial.println("dump:");
    for (int i = 0; i < EEPROM.length(); i++) {
      Serial.print((int)EEPROM.read(i));
      Serial.print(',');
    }
    Serial.println();
    Serial.println();

    eeprom.NextFile();
  }
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
}


