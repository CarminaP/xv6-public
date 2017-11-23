#include "types.h"
#include "fcntl.h"
#include "stat.h"
#include "user.h"
#define passwdFile "/passwd"
#define maxLength 100

int
get_id(char *current)
{
    int i=0, j=0, id=0;
    char id_str[2];
    while (current[i] != ':'){
        i++;
    }
    i++;
    while (current[i] != ':'){
        id_str[j] = current[i];
        j++;
        i++;
    }
    id = atoi(id_str);
    return id;
}

int
username_exists(char *username, char *candidate){
    int i = 0;
    while (username[i] != ':'){
        if (username[i] != candidate[i]) return 0;
        i++;
    }
    return 1;
}

char *
itoa (int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

int
create_user(char *username, int id, char *passwd)
{
    int file, w;
    char newUser[maxLength];
    char readBuff[1024];
    char idBuff[maxLength];
    itoa(id, idBuff,10);

    strcpy( newUser, username);
    strcpy( newUser + strlen(newUser), ":");
    strcpy( newUser + strlen(newUser), idBuff);
    strcpy( newUser + strlen(newUser), ":");
    strcpy( newUser + strlen(newUser), passwd);
    strcpy( newUser + strlen(newUser), ":");
    strcpy( newUser + strlen(newUser), "\n");

    if((file = open(passwdFile, O_RDWR)) < 0){
		printf(1, "Open File Error: No se pudo crear el usuario\n");
		return 0;
    } else {
		read(file, readBuff, sizeof(readBuff));
		if((w = write(file, newUser, strlen(newUser))) > 0){
			printf(1,"Nuevo usuario creado exitosamente\n");
		}
		close(file);
		return 1;
    }
}

int
readLine(int fd, char *buf, int size)
{
  int i=0,n=0;
  char c;
  while(read(fd,&c,1) > 0){
    n++;
    if(c != '\n'){
      buf[i] = c;
      i++;
      if(i == size - 1){
        break;
      }
    } else {
      break;
    }
  }
  buf[i] = '\0';
  return n;
}

int
main( int argc, char *argv[] ){
    if(argc != 3){
        printf(2, "Syntax error: useradd <<username>> <<password>>\n");
        exit();
    }

    int r_file, l=0, current_id=1;
    char record[maxLength];

    r_file = open(passwdFile, O_RDONLY);

    while ((l = readLine(r_file, record, sizeof(record))) > 0){
        if (username_exists(record, argv[1])){
            printf(2, "Este nombre de usuario ya esta en uso, vuelva a intentar\n");
            exit();
        }
        current_id = get_id(record);
    }
    close(r_file);
    create_user(argv[1], current_id + 1, argv[2]);
    exit();
}
