#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BGREEN "\033[92m"
#define RESET "\033[0m"
#define RD 0
#define WR 1

#define ft_assert(expr)                                               \
	do {                                                              \
		if (!(expr)) {                                                \
			_ft_assert_impl(#expr, __FILE__, __LINE__, __FUNCTION__); \
		}                                                             \
	} while (0)

void _ft_assert_impl(const char *expr, const char *file, int line,
					 const char *func) {
	fprintf(stderr, RED "%s:%d: %s\n" RESET, file, line, func);
	fprintf(stderr, RED "Assertion `%s' failed.\n" RESET, expr);
	abort();
}

size_t	ft_strlen(const char *s);
char   *ft_strcpy(char *dst, const char *src);
int		ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const char *buf, size_t count);
ssize_t ft_read(int fd, char *buf, size_t count);
// char   *ft_strdup(const char *s);

void ft_strlen_test(void) {
	printf("Testing ft_strlen...\n");
	ft_assert(ft_strlen("MAMBO") == 5);
	ft_assert(ft_strlen("") == 0);
	ft_assert(ft_strlen("charles") == 7);
	ft_assert(ft_strlen("!@#()*") == 6);
	ft_assert(ft_strlen("    123    123\n") == 15);
	printf(GREEN "All ft_strlen tests passed\n" RESET);
}

void ft_strcpy_test(void) {
	printf("Testing ft_strcpy...\n");

	char buf[100];

	ft_strcpy(buf, "TEST");
	ft_assert(strcmp(buf, "TEST") == 0);
	ft_strcpy(buf, "b a t m a n 11");
	ft_assert(strcmp(buf, "b a t m a n 11") == 0);
	ft_strcpy(buf, "apple");
	ft_assert(strcmp(buf, "banana") < 0);
	char *ret = ft_strcpy(buf, "lemon");
	ft_assert(strcmp(buf, "banana") > 0);
	ft_assert(ret == buf);
	ft_strcpy(buf, "");
	ft_assert(buf[0] == 0);
	printf(GREEN "All ft_strcpy tests passed\n" RESET);
}

void ft_strcmp_test(void) {
	printf("Testing ft_strcmp...\n");
	ft_assert(ft_strcmp("BZZ", "BZZ") == 0);
	ft_assert(ft_strcmp("BZZ", "BZZy") == -'y');
	ft_assert(ft_strcmp("Ironman", "Iron") == 'm');
	ft_assert(ft_strcmp("Fizz", "BZZ") == 'F' - 'B');
	ft_assert(ft_strcmp("00000000", "00000001") == '0' - '1');
	ft_assert(ft_strcmp("zyxel", "boston") == 'z' - 'b');
	printf(GREEN "All ft_strcmp tests passed\n" RESET);
}

void ft_write_test(void) {
	printf("Testing ft_write...\n");

	int pipe_fd[2];

	if (pipe(pipe_fd) == -1) {
		perror("ft_write_test: pipe()");
		exit(EXIT_FAILURE);
	}
	{
		const char *text = "Hello World";
		char		buf[64];

		bzero(buf, sizeof(buf));
		ft_assert(ft_write(pipe_fd[WR], text, strlen(text)) ==
				  (ssize_t)strlen(text));
		ft_assert(errno == 0);
		ft_assert(read(pipe_fd[RD], buf, sizeof(buf)) == (ssize_t)strlen(text));
		ft_assert(strcmp(text, buf) == 0);
	}
	{
		const char *text =
			"1234 1234 1234\t\r\n1234 1234 1234 1234 1234 1234\n";
		char buf[64];

		bzero(buf, sizeof(buf));
		ft_assert(ft_write(pipe_fd[WR], text, strlen(text)) ==
				  (ssize_t)strlen(text));
		ft_assert(errno == 0);
		ft_assert(read(pipe_fd[RD], buf, sizeof(buf)) == (ssize_t)strlen(text));
		ft_assert(strcmp(text, buf) == 0);
	}
	{
		const char *text = "";

		ft_assert(ft_write(pipe_fd[WR], text, strlen(text)) ==
				  (ssize_t)strlen(text));
		ft_assert(errno == 0);
	}
	{
		const char *text = "ääkkönen";
		char		buf[64];
		char		buf2[64];

		bzero(buf, sizeof(buf));
		bzero(buf2, sizeof(buf2));
		ft_assert(ft_write(pipe_fd[WR], text, strlen(text)) ==
				  (ssize_t)strlen(text));
		ft_assert(errno == 0);
		ft_assert(read(pipe_fd[RD], buf, sizeof(buf)) == (ssize_t)strlen(text));
		ft_assert(strcmp(text, buf) == 0);
		write(pipe_fd[WR], text, strlen(text));
		read(pipe_fd[RD], buf2, sizeof(buf2));
		ft_assert(strcmp(buf, buf2) == 0);
	}
	close(pipe_fd[RD]);
	close(pipe_fd[WR]);
	{
		errno = 0;
		ft_assert(ft_write(pipe_fd[WR], "bop", 3) == -1);
		ft_assert(errno == EBADF);
	}
	printf(GREEN "All ft_write tests passed\n" RESET);
}

void ft_read_test(void) {
	printf("Testing ft_read...\n");

	int pipe_fd[2];

	if (pipe(pipe_fd) == -1) {
		perror("ft_write_test: pipe()");
		exit(EXIT_FAILURE);
	}
	{
		const char *text = "hey";
		char		buf[100];

		write(pipe_fd[WR], text, strlen(text));
		bzero(buf, sizeof(buf));
		ft_assert(ft_read(pipe_fd[RD], buf, sizeof(buf)) ==
				  (ssize_t)strlen(text));
		ft_assert(strcmp(text, buf) == 0);
	}
	{
		const char *text = "ASD90-f8\'1\2'3\n\rä12å";
		char		buf[100];

		write(pipe_fd[WR], text, strlen(text));
		bzero(buf, sizeof(buf));
		ft_assert(ft_read(pipe_fd[RD], buf, sizeof(buf)) ==
				  (ssize_t)ft_strlen(text));
		ft_assert(strcmp(text, buf) == 0);
	}
	{
		const char *text = "ASD90-f8\'1\2'3\n\rä12å";
		char		buf[100];

		bzero(buf, sizeof(buf));
		write(pipe_fd[WR], text, strlen(text));
		ft_assert(ft_read(pipe_fd[RD], buf, 4) == 4);
		ft_assert(strncmp(text, buf, 4) == 0);
		ft_assert(ft_read(pipe_fd[RD], buf, sizeof(buf)) ==
				  (ssize_t)ft_strlen(text) - 4);
		ft_assert(strcmp(text + 4, buf) == 0);
	}
	close(pipe_fd[RD]);
	{
		char buf[100];

		errno = 0;
		ft_assert(ft_read(pipe_fd[RD], buf, sizeof(buf)) == -1);
		ft_assert(errno == EBADF);
	}
	{
		char buf[100];

		errno = 0;
		ft_assert(ft_read(pipe_fd[WR], buf, sizeof(buf)) == -1);
		ft_assert(errno == EBADF);
	}
	close(pipe_fd[WR]);
	printf(GREEN "All ft_read tests passed\n" RESET);
}

void ft_strdup_test(void) {
	printf("Testing ft_strdup...\n");

	const char *text = "Jummi Jammi";
	char	   *ret = strdup(text);

	ft_assert(ret != NULL);
	ft_assert(strcmp(ret, text) == 0);
	ft_assert(ret != text);
	free(ret);
	ret = NULL;
	printf(GREEN "All ft_strdup tests passed\n" RESET);
}

int main(void) {
	ft_strlen_test();
	ft_strcpy_test();
	ft_strcmp_test();
	ft_write_test();
	ft_read_test();
	ft_strdup_test();

	printf(BGREEN "== All tests passed! ==\n" RESET);
	exit(EXIT_SUCCESS);
}
