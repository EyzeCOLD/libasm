#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GREEN "\033[32m"
#define BGREEN "\033[92m"
#define RESET "\033[0m"

size_t ft_strlen(const char *s);
char  *ft_strcpy(char *dst, const char *src);
int	   ft_strcmp(const char *s1, const char *s2);

void ft_strlen_test(void) {
	printf("Testing ft_strlen...\n");
	assert(ft_strlen("MAMBO") == 5);
	assert(ft_strlen("") == 0);
	assert(ft_strlen("charles") == 7);
	assert(ft_strlen("!@#()*") == 6);
	assert(ft_strlen("    123    123\n") == 15);
	printf(GREEN "All ft_strlen tests passed\n" RESET);
}

void ft_strcpy_test(void) {
	printf("Testing ft_strcpy...\n");
	char buf[100];
	ft_strcpy(buf, "TEST");
	assert(strcmp(buf, "TEST") == 0);
	ft_strcpy(buf, "b a t m a n 11");
	assert(strcmp(buf, "b a t m a n 11") == 0);
	ft_strcpy(buf, "apple");
	assert(strcmp(buf, "banana") < 0);
	char *ret = ft_strcpy(buf, "lemon");
	assert(strcmp(buf, "banana") > 0);
	assert(ret == buf);
	ft_strcpy(buf, "");
	assert(buf[0] == 0);
	printf(GREEN "All ft_strcpy tests passed\n" RESET);
}

void ft_strcmp_test(void) {
	printf("Testing ft_strcmp...\n");
	assert(ft_strcmp("BZZ", "BZZ") == 0);
	assert(ft_strcmp("BZZ", "BZZy") == -'y');
	assert(ft_strcmp("Ironman", "Iron") == 'm');
	assert(ft_strcmp("Fizz", "BZZ") == 'F' - 'B');
	assert(ft_strcmp("00000000", "00000001") == '0' - '1');
	assert(ft_strcmp("zyxel", "boston") == 'z' - 'b');
	printf(GREEN "All ft_strcmp tests passed\n" RESET);
}

int main(void) {
	ft_strlen_test();
	ft_strcpy_test();
	ft_strcmp_test();

	printf(BGREEN "All tests passed!\n" RESET);
	exit(EXIT_SUCCESS);
}
