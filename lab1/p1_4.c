#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5

typedef enum { false, true } bool;
typedef int(*Operator)(int, int, bool);

inline static int addition(int first, int second, bool state) {
	if (!state)
		return first + second;
	printf(__FUNCTION__);
	return 0;
}
inline static int subtraction(int first, int second, bool state) {
	if (!state)
		return first - second;
	printf(__FUNCTION__);
	return 0;
}
inline static int multiple(int first, int second, bool state) {
	if (!state)
		return first * second;
	printf(__FUNCTION__);
	return 0;
}
inline static int division(int first, int second, bool state) {
	if (!state)
	{
		if(!second)
			return 0;
		return first / second;
	}
	printf(__FUNCTION__);
	return 0;
}
inline static int modulus(int first, int second, bool state) {
	if (!state)
		return first % second;
	printf(__FUNCTION__);
	return 0;
}

size_t set_function(Operator *, Operator);
bool calculator(Operator *, int, int, int);
void show_menu(Operator *);
void get_command(int *);
void get_operand(int *, int *);

int main(int argc, char * argv[])
{
	int command = 0, first, second;
	Operator * opers = (Operator*)malloc(sizeof(Operator) * SIZE);

	set_function(opers, addition);
	set_function(opers, subtraction);
	set_function(opers, multiple);
	set_function(opers, division);
	set_function(opers, modulus);

	do {
		show_menu(opers);
		get_command(&command);
		if (command == SIZE)
			break;
		get_operand(&first, &second);
	} while (command != SIZE && calculator(opers, command, first, second));

	return 0;
}

/*set_function (*/
size_t set_function(Operator * functions, Operator function)
{
	static size_t count = 0;
	functions[count] = function;
	return count++;
}
bool calculator(Operator * oper, int command, int first, int second)
{
	printf("Result = %d\n", oper[command](first, second, false));
	return true;
}
void show_menu(Operator * functions)
{
	printf("============================\n");
	for (int i = 0; i < SIZE; i++)
	{
		printf("%d. ", i);
		functions[i](0, 0, true);
		printf("\n");
	}
	printf("%d. exit\n", SIZE);
	printf("============================\n");
}
void get_command(int * command)
{
	printf("Select Operation: ");
	scanf("%d", command);
}
void get_operand(int * first, int * second)
{
	printf("Input 2 operand: ");
	scanf("%d %d", first, second);
}