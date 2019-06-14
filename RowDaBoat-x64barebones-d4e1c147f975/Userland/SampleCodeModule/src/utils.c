#include <utils.h>

/*STANDARD C LIBRARY FUNCTIONS*/

char getChar(){
	char * buff;
	sys_read(0, buff, 1);
	return buff[0];
}

void putChar(char c){
	sys_write(0, &c, 1);
}

int scanf(const char * fmt, ...){
    va_list list;
    va_start(list, fmt);

		char str[MAX_BUFFER] = {0};
		int len = 0;
		char key;

		print("ANTES WHILE");
		//Le habilita al usuario a escribir hasta el enter
		while((key = getKey()) != '\n'){
			if (key == '\b'){
				str[len - 1] = 0;
				len--;
			} else {
				if (len < MAX_BUFFER){
					str[len++] = key;
				}
			}
			putChar(key);
		}

		int i = 0;
		int pos = 0;
		int matches = 0;
		int j = 0;

		int aux;
print("DESPUES WHILE");
		//Lee el formato
    while(fmt[i] != 0){
    	if(fmt[i] == '%'){
        switch (str[i+1]) {
          case 'd':
						matches++;
						int * ptrD = va_arg(list, int*);
						char num[15] = {0};
						j = 0;
						//Itera el string que fue input
						while(str[pos] != ' ' || str[pos] != '\n' || str[pos] != 0 || str[pos] != '\t'){
							if (!isNumeric(str[pos])){
								num[j++] = str[pos++];
							} else {
								return -1;
							}
						}
						aux = atoi(num, j);
						ptrD = &aux;
            i += 2;
            break;

          case 's':
					matches++;
						char * ptrS = va_arg(list, char*);
						j = 0;
						char buffer[MAX_BUFFER] = {0};

						while(str[pos] != ' ' || str[pos] != '\n' || str[pos] != 0 || str[pos] != '\t'){
							buffer[j++] = str[pos++];
						}

						ptrS = buffer;
            i += 2;
            break;

					case 'c':
					matches++;
					char * ptrC = va_arg(list, char*);
					j = 0;

					if(str[pos] != ' ' || str[pos] != '\n' || str[pos] != 0 || str[pos] != '\t'){
						ptrC = (str + pos);
						pos++;
					}

					i += 2;
					break;
          }
        }
    }
print("DESPUES DESPUES WHILE");
		// newStr[len] = 0;
		// len++;
		// sys_write(0, newStr, len);
		return matches;
}

void printf(char * str, ...){
    va_list list;
    va_start(list, str);
    int i = 0;
		//ncPrint(str);

		char newStr[MAX_BUFFER] = {0};
		int len = 0;

    while(str[i] != 0){
    	if(str[i] == '%' && (i == 0 || str[i-1] != '\\')){
				//ncPrintDec(i);
				//ncPrint("-");
            char buffer[MAX_BUFFER] = {0};
            switch (str[i+1]) {
                case 'd':
										//ncPrintDec(va_arg(list,int));
                    itoa((int) va_arg(list,int), buffer, 10);
                    len += concat((newStr + len), buffer);
                    i += 2;
                    break;
                case 's':
										len += concat((newStr + len), va_arg(list,char*));
                    i += 2;
                    break;
										default:
										i += 2;
										break;
            }
        }
        else {
            newStr[len] = str[i];
						len++;
						i++;
        }
    }

		newStr[len] = 0;
		len++;
		sys_write(0, newStr, len);
}

/* ------------------------------- */
/* HELPER FUNCTIONS */

int concat(char * to, const char * from){
	int i = 0;
	while(from[i] != 0){
		to[i] = from[i];
		i++;
	}
	return i;
}

// inline function to swap two numbers
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

int atoi(const char* buffer, int len){
	int i = 0;
	int result = 0;
	while(buffer[i] != 0){
		result += (pow(10, len) * (buffer[i] - 48));
		i++;
		len--;
	}
	return result;
}

int pow(int base, int exponent){
	int result = 1;
	for (int i = 0; i < exponent; i++){
		result = result * base;
	}
	return result;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = value;

	int i = 0;
	while (n)
	{
		int r = n % base;

		//if (r >= 10)
		//	buffer[i++] = 65 + (r - 10);
		//else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

/* ------------------------------- */

int isNumeric(char c){
	return '0' <= c || c <= '9';
}

char getKey(void){
	char buff;
	sys_get_key(0, &buff);
	return buff;
}

// char * getTime(void){
// 	char * buff;
// 	sys_time(buff);
// 	return buff;
// }

char * getTime(void){
	int seconds = sys_time(SECONDS);
	int minutes = sys_time(MINUTES);
	int hours = sys_time(HOURS);

	char * buff;

	if (hours < 10){
		itoa(0, buff, 10);
		itoa(hours, buff + 1, 10);
	} else{
		itoa(hours, buff, 10);
	}

	*(buff + 2) = ':';

	if (minutes < 10){
		itoa(0, buff + 3, 10);
		itoa(minutes, buff + 4, 10);
	} else{
		itoa(minutes, buff + 3, 10);
	}

	*(buff + 5) = ':';

	if (seconds < 10){
		itoa(0, buff + 6, 10);
		itoa(minutes, buff + 7, 10);
	} else{
		itoa(minutes, buff + 6, 10);
	}
	*(buff + 8) = 0;

	return buff;
}

char * getDate(void){
	int day = sys_time(DAY_OF_MONTH);
	int month = sys_time(MONTH);
	int year = sys_time(YEAR);
	char * buff;

	if (day < 10){
		itoa(0, buff, 10);
		itoa(day, buff + 1, 10);
	} else{
		itoa(day, buff, 10);
	}

	*(buff + 2) = '/';

	if (month < 10){
		itoa(0, buff + 3, 10);
		itoa(month, buff + 4, 10);
	} else{
		itoa(month, buff + 3, 10);
	}

	*(buff + 5) = '/';
	//int century = get_time(CENTURY);

	year = 2000 + year;
	itoa(year, buff + 6, 10);
	
	*(buff + 10) = 0;

	return buff;
}

void makeSound(void){
	sys_beep();
}

void goToSleep(int ticks){
	sys_sleep(ticks);
}

void writeKey(char * key){
	sys_write_key(0, key);
}

int strcmp(const char * stra, const char * strb){
	int result = 0;
	int i;
	for (i = 0; *(stra + i) != 0 && *(strb + i) != 0 && !result; i++){
		if (*(stra + i) < *(strb + i)){
			return -1;
		} else if (*(stra + i) > *(strb + i)){
			return 1;
		}
	}
	if (*(stra + i) == 0 && *(strb + i) == 0 && result != 0){
		return 0;
	} else if (*(stra + i) != 0 && *(strb + i) == 0){
		return 1;
	} else if (*(stra + i) == 0 && *(strb + i) != 0){
		return -1;
	}
	//ncPrintDec(result);
	return result;
}

int strlen(const char * str){
	int i = 0;
	for(;str[i] != 0; i++);
	return i;
}

void print(const char * str){
  sys_write(1, str, strlen(str));
}

void printN(const char * str, int length){
  sys_write(1,str,length);
}
