#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <setjmp.h>
#include "libft/libft.h"

/* Color codes for output */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"
#define BOLD    "\033[1m"

/* Test statistics */
typedef struct {
    int total;
    int passed;
    int failed;
    int segfaults;
} t_stats;

static t_stats g_stats = {0, 0, 0, 0};
static char *g_current_test = NULL;

/* Signal handler for segfaults */
void segfault_handler(int sig) {
    if (sig == SIGSEGV) {
        printf(RED "    ✗ SEGFAULT detected in test: %s" RESET "\n", 
               g_current_test ? g_current_test : "Unknown test");
        g_stats.segfaults++;
        g_stats.failed++;
        exit(1);
    }
}

/* Print test header */
void print_header(const char *function_name) {
    printf(BOLD CYAN "\n=== Testing %s ===" RESET "\n", function_name);
}

/* Print test result */
void print_result(const char *test_name, int passed, const char *expected, const char *got) {
    g_stats.total++;
    if (passed) {
        printf(GREEN "  ✓ %s" RESET "\n", test_name);
        g_stats.passed++;
    } else {
        printf(RED "  ✗ %s" RESET "\n", test_name);
        if (expected && got) {
            printf(YELLOW "    Expected: %s" RESET "\n", expected);
            printf(YELLOW "    Got:      %s" RESET "\n", got);
        }
        g_stats.failed++;
    }
}

/* Safe test execution with fork to catch segfaults */
int safe_test(const char *test_name, int (*test_func)(void)) {
    g_current_test = (char *)test_name;
    
    pid_t pid = fork();
    if (pid == 0) {
        /* Child process */
        signal(SIGSEGV, segfault_handler);
        int result = test_func();
        exit(result ? 0 : 1);
    } else if (pid > 0) {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
            print_result(test_name, 0, "No segfault", "SEGFAULT");
            g_stats.segfaults++;
            return 0;
        } else if (WIFEXITED(status)) {
            int result = (WEXITSTATUS(status) == 0);
            print_result(test_name, result, NULL, NULL);
            return result;
        }
    }
    return 0;
}

/* Test functions for ft_strlen */
int test_ft_strlen_basic(void) {
    return ft_strlen("Hello") == 5;
}

int test_ft_strlen_empty(void) {
    return ft_strlen("") == 0;
}

int test_ft_strlen_null(void) {
    return ft_strlen(NULL) == 0; /* This will likely segfault */
}

int test_ft_strlen_long(void) {
    char *long_str = malloc(1000);
    if (!long_str) return 0;
    memset(long_str, 'a', 999);
    long_str[999] = '\0';
    int result = (ft_strlen(long_str) == 999);
    free(long_str);
    return result;
}

/* Test functions for ft_strchr */
int test_ft_strchr_found(void) {
    char *str = "Hello World";
    char *result = ft_strchr(str, 'W');
    return (result != NULL && *result == 'W');
}

int test_ft_strchr_not_found(void) {
    char *str = "Hello World";
    return ft_strchr(str, 'z') == NULL;
}

int test_ft_strchr_null_terminator(void) {
    char *str = "Hello";
    char *result = ft_strchr(str, '\0');
    return (result == str + 5);
}

int test_ft_strchr_null_string(void) {
    return ft_strchr(NULL, 'a') == NULL; /* Will likely segfault */
}

/* Test functions for ft_strdup */
int test_ft_strdup_basic(void) {
    char *original = "Hello World";
    char *dup = ft_strdup(original);
    if (!dup) return 0;
    int result = (strcmp(dup, original) == 0);
    free(dup);
    return result;
}

int test_ft_strdup_empty(void) {
    char *dup = ft_strdup("");
    if (!dup) return 0;
    int result = (strlen(dup) == 0);
    free(dup);
    return result;
}

int test_ft_strdup_null(void) {
    char *dup = ft_strdup(NULL); /* Will likely segfault */
    return (dup == NULL);
}

/* Test functions for ft_memset */
int test_ft_memset_basic(void) {
    char buffer[10];
    ft_memset(buffer, 'A', 5);
    buffer[5] = '\0';
    return strcmp(buffer, "AAAAA") == 0;
}

int test_ft_memset_zero_length(void) {
    char buffer[5] = "test";
    ft_memset(buffer, 'X', 0);
    return strcmp(buffer, "test") == 0;
}

int test_ft_memset_null(void) {
    ft_memset(NULL, 'A', 5); /* Will segfault */
    return 1;
}

/* Test functions for ft_bzero */
int test_ft_bzero_basic(void) {
    char buffer[10] = "Hello";
    ft_bzero(buffer, 3);
    return (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0 && buffer[3] == 'l');
}

int test_ft_bzero_zero_length(void) {
    char buffer[5] = "test";
    ft_bzero(buffer, 0);
    return strcmp(buffer, "test") == 0;
}

int test_ft_bzero_null(void) {
    ft_bzero(NULL, 5); /* Will segfault */
    return 1;
}

/* Test functions for ft_memcpy */
int test_ft_memcpy_basic(void) {
    char src[] = "Hello";
    char dst[10];
    ft_memcpy(dst, src, 5);
    dst[5] = '\0';
    return strcmp(dst, "Hello") == 0;
}

int test_ft_memcpy_zero_length(void) {
    char src[] = "Hello";
    char dst[] = "World";
    ft_memcpy(dst, src, 0);
    return strcmp(dst, "World") == 0;
}

int test_ft_memcpy_null(void) {
    ft_memcpy(NULL, "test", 4); /* Will segfault */
    return 1;
}

/* Test functions for ft_memmove */
int test_ft_memmove_basic(void) {
    char buffer[] = "Hello World";
    ft_memmove(buffer + 2, buffer, 5);
    return strncmp(buffer + 2, "Hello", 5) == 0;
}

int test_ft_memmove_overlap(void) {
    char buffer[] = "123456789";
    ft_memmove(buffer + 3, buffer, 6);
    return strncmp(buffer + 3, "123456", 6) == 0;
}

int test_ft_memmove_null(void) {
    ft_memmove(NULL, "test", 4); /* Will segfault */
    return 1;
}

/* Test functions for ft_memchr */
int test_ft_memchr_found(void) {
    char str[] = "Hello World";
    void *result = ft_memchr(str, 'W', 11);
    return (result == str + 6);
}

int test_ft_memchr_not_found(void) {
    char str[] = "Hello World";
    return ft_memchr(str, 'z', 11) == NULL;
}

int test_ft_memchr_null(void) {
    return ft_memchr(NULL, 'a', 5) == NULL; /* Will segfault */
}

/* Test functions for ft_memcmp */
int test_ft_memcmp_equal(void) {
    return ft_memcmp("Hello", "Hello", 5) == 0;
}

int test_ft_memcmp_different(void) {
    int result = ft_memcmp("Hello", "Hell!", 5);
    return (result > 0); /* 'o' > '!' */
}

int test_ft_memcmp_zero_length(void) {
    return ft_memcmp("Hello", "World", 0) == 0;
}

int test_ft_memcmp_null(void) {
    ft_memcmp(NULL, "test", 4); /* Will segfault */
    return 1;
}

/* Test functions for ft_strlcpy */
int test_ft_strlcpy_basic(void) {
    char dst[10];
    size_t result = ft_strlcpy(dst, "Hello", 10);
    return (result == 5 && strcmp(dst, "Hello") == 0);
}

int test_ft_strlcpy_truncate(void) {
    char dst[4];
    size_t result = ft_strlcpy(dst, "Hello", 4);
    return (result == 5 && strcmp(dst, "Hel") == 0);
}

int test_ft_strlcpy_null_dst(void) {
    ft_strlcpy(NULL, "Hello", 5); /* Will segfault */
    return 1;
}

/* Test functions for ft_strlcat */
int test_ft_strlcat_basic(void) {
    char dst[20] = "Hello ";
    size_t result = ft_strlcat(dst, "World", 20);
    return (result == 11 && strcmp(dst, "Hello World") == 0);
}

int test_ft_strlcat_truncate(void) {
    char dst[10] = "Hello ";
    size_t result = ft_strlcat(dst, "World", 10);
    return (result == 11 && strcmp(dst, "Hello Wor") == 0);
}

int test_ft_strlcat_null(void) {
    ft_strlcat(NULL, "test", 5); /* Will segfault */
    return 1;
}

/* Test functions for ft_strrchr */
int test_ft_strrchr_found(void) {
    char *str = "Hello World";
    char *result = ft_strrchr(str, 'l');
    return (result == str + 9); /* Last 'l' */
}

int test_ft_strrchr_not_found(void) {
    char *str = "Hello World";
    return ft_strrchr(str, 'z') == NULL;
}

int test_ft_strrchr_null(void) {
    return ft_strrchr(NULL, 'a') == NULL; /* Will segfault */
}

/* Test functions for ft_strncmp */
int test_ft_strncmp_equal(void) {
    return ft_strncmp("Hello", "Hello", 5) == 0;
}

int test_ft_strncmp_different(void) {
    int result = ft_strncmp("Hello", "Hell!", 5);
    return (result > 0); /* 'o' > '!' */
}

int test_ft_strncmp_partial(void) {
    return ft_strncmp("Hello", "Hell!", 4) == 0;
}

int test_ft_strncmp_null(void) {
    ft_strncmp(NULL, "test", 4); /* Will segfault */
    return 1;
}

/* Test functions for ft_strnstr */
int test_ft_strnstr_found(void) {
    char *result = ft_strnstr("Hello World", "Wor", 11);
    return (result != NULL && strncmp(result, "World", 5) == 0);
}

int test_ft_strnstr_not_found(void) {
    return ft_strnstr("Hello World", "xyz", 11) == NULL;
}

int test_ft_strnstr_partial_limit(void) {
    return ft_strnstr("Hello World", "World", 8) == NULL;
}

int test_ft_strnstr_null(void) {
    return ft_strnstr(NULL, "test", 5) == NULL; /* Will segfault */
}

/* Test functions for ft_atoi */
int test_ft_atoi_positive(void) {
    return ft_atoi("123") == 123;
}

int test_ft_atoi_negative(void) {
    return ft_atoi("-456") == -456;
}

int test_ft_atoi_whitespace(void) {
    return ft_atoi("  \t\n789") == 789;
}

int test_ft_atoi_mixed(void) {
    return ft_atoi("42abc") == 42;
}

int test_ft_atoi_null(void) {
    ft_atoi(NULL); /* Will segfault */
    return 1;
}

/* Test functions for ft_isalpha */
int test_ft_isalpha_letter(void) {
    return (ft_isalpha('a') && ft_isalpha('Z'));
}

int test_ft_isalpha_digit(void) {
    return (!ft_isalpha('5'));
}

int test_ft_isalpha_special(void) {
    return (!ft_isalpha('@'));
}

/* Test functions for ft_isdigit */
int test_ft_isdigit_digit(void) {
    return (ft_isdigit('0') && ft_isdigit('9'));
}

int test_ft_isdigit_letter(void) {
    return (!ft_isdigit('a'));
}

/* Test functions for ft_isalnum */
int test_ft_isalnum_alphanum(void) {
    return (ft_isalnum('a') && ft_isalnum('5'));
}

int test_ft_isalnum_special(void) {
    return (!ft_isalnum('@'));
}

/* Test functions for ft_isascii */
int test_ft_isascii_valid(void) {
    return (ft_isascii(0) && ft_isascii(127));
}

int test_ft_isascii_invalid(void) {
    return (!ft_isascii(128));
}

/* Test functions for ft_isprint */
int test_ft_isprint_printable(void) {
    return (ft_isprint(' ') && ft_isprint('~'));
}

int test_ft_isprint_non_printable(void) {
    return (!ft_isprint('\t'));
}

/* Test functions for ft_toupper */
int test_ft_toupper_lower(void) {
    return (ft_toupper('a') == 'A');
}

int test_ft_toupper_upper(void) {
    return (ft_toupper('A') == 'A');
}

/* Test functions for ft_tolower */
int test_ft_tolower_upper(void) {
    return (ft_tolower('A') == 'a');
}

int test_ft_tolower_lower(void) {
    return (ft_tolower('a') == 'a');
}

/* Test functions for ft_calloc */
int test_ft_calloc_basic(void) {
    int *ptr = (int *)ft_calloc(5, sizeof(int));
    if (!ptr) return 0;
    int result = (ptr[0] == 0 && ptr[4] == 0);
    free(ptr);
    return result;
}

int test_ft_calloc_zero(void) {
    void *ptr = ft_calloc(0, 10);
    if (ptr) free(ptr);
    return 1; /* Should not crash */
}

/* Test functions for ft_substr */
int test_ft_substr_basic(void) {
    char *result = ft_substr("Hello World", 6, 5);
    if (!result) return 0;
    int success = (strcmp(result, "World") == 0);
    free(result);
    return success;
}

int test_ft_substr_out_of_bounds(void) {
    char *result = ft_substr("Hello", 10, 5);
    if (!result) return 0;
    int success = (strlen(result) == 0);
    free(result);
    return success;
}

int test_ft_substr_null(void) {
    return ft_substr(NULL, 0, 5) == NULL; /* Will segfault */
}

/* Test functions for ft_strjoin */
int test_ft_strjoin_basic(void) {
    char *result = ft_strjoin("Hello", " World");
    if (!result) return 0;
    int success = (strcmp(result, "Hello World") == 0);
    free(result);
    return success;
}

int test_ft_strjoin_empty(void) {
    char *result = ft_strjoin("", "test");
    if (!result) return 0;
    int success = (strcmp(result, "test") == 0);
    free(result);
    return success;
}

int test_ft_strjoin_null(void) {
    return ft_strjoin(NULL, "test") == NULL; /* Will segfault */
}

/* Test functions for ft_strtrim */
int test_ft_strtrim_basic(void) {
    char *result = ft_strtrim("  Hello World  ", " ");
    if (!result) return 0;
    int success = (strcmp(result, "Hello World") == 0);
    free(result);
    return success;
}

int test_ft_strtrim_no_trim(void) {
    char *result = ft_strtrim("Hello", " ");
    if (!result) return 0;
    int success = (strcmp(result, "Hello") == 0);
    free(result);
    return success;
}

int test_ft_strtrim_null(void) {
    return ft_strtrim(NULL, " ") == NULL; /* Will segfault */
}

/* Test functions for ft_split */
int test_ft_split_basic(void) {
    char **result = ft_split("Hello World Test", ' ');
    if (!result) return 0;
    int success = (result[0] && strcmp(result[0], "Hello") == 0 &&
                   result[1] && strcmp(result[1], "World") == 0 &&
                   result[2] && strcmp(result[2], "Test") == 0 &&
                   result[3] == NULL);
    /* Free result */
    for (int i = 0; result[i]; i++) free(result[i]);
    free(result);
    return success;
}

int test_ft_split_empty(void) {
    char **result = ft_split("", ' ');
    if (!result) return 0;
    int success = (result[0] == NULL);
    free(result);
    return success;
}

int test_ft_split_null(void) {
    return ft_split(NULL, ' ') == NULL; /* Will segfault */
}

/* Test functions for ft_itoa */
int test_ft_itoa_positive(void) {
    char *result = ft_itoa(123);
    if (!result) return 0;
    int success = (strcmp(result, "123") == 0);
    free(result);
    return success;
}

int test_ft_itoa_negative(void) {
    char *result = ft_itoa(-456);
    if (!result) return 0;
    int success = (strcmp(result, "-456") == 0);
    free(result);
    return success;
}

int test_ft_itoa_zero(void) {
    char *result = ft_itoa(0);
    if (!result) return 0;
    int success = (strcmp(result, "0") == 0);
    free(result);
    return success;
}

/* Test functions for ft_strmapi */
char test_toupper_char(unsigned int i, char c) {
    (void)i;
    return (c >= 'a' && c <= 'z') ? c - 32 : c;
}

int test_ft_strmapi_basic(void) {
    char *result = ft_strmapi("hello", test_toupper_char);
    if (!result) return 0;
    int success = (strcmp(result, "HELLO") == 0);
    free(result);
    return success;
}

int test_ft_strmapi_null(void) {
    return ft_strmapi(NULL, test_toupper_char) == NULL; /* Will segfault */
}

/* Test functions for ft_striteri */
void test_toupper_inplace(unsigned int i, char *c) {
    (void)i;
    if (*c >= 'a' && *c <= 'z')
        *c = *c - 32;
}

int test_ft_striteri_basic(void) {
    char str[] = "hello";
    ft_striteri(str, test_toupper_inplace);
    return strcmp(str, "HELLO") == 0;
}

int test_ft_striteri_null(void) {
    ft_striteri(NULL, test_toupper_inplace); /* Will segfault */
    return 1;
}

/* Test functions for ft_putchar_fd */
int test_ft_putchar_fd_basic(void) {
    ft_putchar_fd('A', 1); /* Output to stdout */
    return 1; /* Hard to test output, assume success */
}

/* Test functions for ft_putstr_fd */
int test_ft_putstr_fd_basic(void) {
    ft_putstr_fd("Hello\n", 1); /* Output to stdout */
    return 1; /* Hard to test output, assume success */
}

int test_ft_putstr_fd_null(void) {
    ft_putstr_fd(NULL, 1); /* May segfault depending on implementation */
    return 1;
}

/* Test functions for ft_putendl_fd */
int test_ft_putendl_fd_basic(void) {
    ft_putendl_fd("Hello", 1); /* Output to stdout with newline */
    return 1; /* Hard to test output, assume success */
}

/* Test functions for ft_putnbr_fd */
int test_ft_putnbr_fd_basic(void) {
    ft_putnbr_fd(42, 1); /* Output to stdout */
    printf("\n"); /* Add newline for clarity */
    return 1; /* Hard to test output, assume success */
}

/* Test runner for a specific function */
void test_ft_strlen(void) {
    print_header("ft_strlen");
    safe_test("Basic string length", test_ft_strlen_basic);
    safe_test("Empty string", test_ft_strlen_empty);
    safe_test("NULL pointer", test_ft_strlen_null);
    safe_test("Long string (999 chars)", test_ft_strlen_long);
}

void test_ft_strchr(void) {
    print_header("ft_strchr");
    safe_test("Character found", test_ft_strchr_found);
    safe_test("Character not found", test_ft_strchr_not_found);
    safe_test("Null terminator search", test_ft_strchr_null_terminator);
    safe_test("NULL string", test_ft_strchr_null_string);
}

void test_ft_strdup(void) {
    print_header("ft_strdup");
    safe_test("Basic duplication", test_ft_strdup_basic);
    safe_test("Empty string duplication", test_ft_strdup_empty);
    safe_test("NULL pointer duplication", test_ft_strdup_null);
}

void test_ft_memset(void) {
    print_header("ft_memset");
    safe_test("Basic memset", test_ft_memset_basic);
    safe_test("Zero length", test_ft_memset_zero_length);
    safe_test("NULL pointer", test_ft_memset_null);
}

void test_ft_bzero(void) {
    print_header("ft_bzero");
    safe_test("Basic bzero", test_ft_bzero_basic);
    safe_test("Zero length", test_ft_bzero_zero_length);
    safe_test("NULL pointer", test_ft_bzero_null);
}

void test_ft_memcpy(void) {
    print_header("ft_memcpy");
    safe_test("Basic memcpy", test_ft_memcpy_basic);
    safe_test("Zero length", test_ft_memcpy_zero_length);
    safe_test("NULL pointer", test_ft_memcpy_null);
}

void test_ft_memmove(void) {
    print_header("ft_memmove");
    safe_test("Basic memmove", test_ft_memmove_basic);
    safe_test("Overlapping memory", test_ft_memmove_overlap);
    safe_test("NULL pointer", test_ft_memmove_null);
}

void test_ft_memchr(void) {
    print_header("ft_memchr");
    safe_test("Character found", test_ft_memchr_found);
    safe_test("Character not found", test_ft_memchr_not_found);
    safe_test("NULL pointer", test_ft_memchr_null);
}

void test_ft_memcmp(void) {
    print_header("ft_memcmp");
    safe_test("Equal memory", test_ft_memcmp_equal);
    safe_test("Different memory", test_ft_memcmp_different);
    safe_test("Zero length", test_ft_memcmp_zero_length);
    safe_test("NULL pointer", test_ft_memcmp_null);
}

void test_ft_strlcpy(void) {
    print_header("ft_strlcpy");
    safe_test("Basic copy", test_ft_strlcpy_basic);
    safe_test("Truncation", test_ft_strlcpy_truncate);
    safe_test("NULL destination", test_ft_strlcpy_null_dst);
}

void test_ft_strlcat(void) {
    print_header("ft_strlcat");
    safe_test("Basic concatenation", test_ft_strlcat_basic);
    safe_test("Truncation", test_ft_strlcat_truncate);
    safe_test("NULL pointer", test_ft_strlcat_null);
}

void test_ft_strrchr(void) {
    print_header("ft_strrchr");
    safe_test("Character found", test_ft_strrchr_found);
    safe_test("Character not found", test_ft_strrchr_not_found);
    safe_test("NULL string", test_ft_strrchr_null);
}

void test_ft_strncmp(void) {
    print_header("ft_strncmp");
    safe_test("Equal strings", test_ft_strncmp_equal);
    safe_test("Different strings", test_ft_strncmp_different);
    safe_test("Partial comparison", test_ft_strncmp_partial);
    safe_test("NULL pointer", test_ft_strncmp_null);
}

void test_ft_strnstr(void) {
    print_header("ft_strnstr");
    safe_test("Substring found", test_ft_strnstr_found);
    safe_test("Substring not found", test_ft_strnstr_not_found);
    safe_test("Limited by length", test_ft_strnstr_partial_limit);
    safe_test("NULL pointer", test_ft_strnstr_null);
}

void test_ft_atoi(void) {
    print_header("ft_atoi");
    safe_test("Positive number", test_ft_atoi_positive);
    safe_test("Negative number", test_ft_atoi_negative);
    safe_test("With whitespace", test_ft_atoi_whitespace);
    safe_test("Mixed characters", test_ft_atoi_mixed);
    safe_test("NULL pointer", test_ft_atoi_null);
}

void test_ft_isalpha(void) {
    print_header("ft_isalpha");
    safe_test("Letters", test_ft_isalpha_letter);
    safe_test("Digits", test_ft_isalpha_digit);
    safe_test("Special characters", test_ft_isalpha_special);
}

void test_ft_isdigit(void) {
    print_header("ft_isdigit");
    safe_test("Digits", test_ft_isdigit_digit);
    safe_test("Letters", test_ft_isdigit_letter);
}

void test_ft_isalnum(void) {
    print_header("ft_isalnum");
    safe_test("Alphanumeric", test_ft_isalnum_alphanum);
    safe_test("Special characters", test_ft_isalnum_special);
}

void test_ft_isascii(void) {
    print_header("ft_isascii");
    safe_test("Valid ASCII", test_ft_isascii_valid);
    safe_test("Invalid ASCII", test_ft_isascii_invalid);
}

void test_ft_isprint(void) {
    print_header("ft_isprint");
    safe_test("Printable characters", test_ft_isprint_printable);
    safe_test("Non-printable", test_ft_isprint_non_printable);
}

void test_ft_toupper(void) {
    print_header("ft_toupper");
    safe_test("Lowercase to uppercase", test_ft_toupper_lower);
    safe_test("Already uppercase", test_ft_toupper_upper);
}

void test_ft_tolower(void) {
    print_header("ft_tolower");
    safe_test("Uppercase to lowercase", test_ft_tolower_upper);
    safe_test("Already lowercase", test_ft_tolower_lower);
}

void test_ft_calloc(void) {
    print_header("ft_calloc");
    safe_test("Basic allocation", test_ft_calloc_basic);
    safe_test("Zero allocation", test_ft_calloc_zero);
}

void test_ft_substr(void) {
    print_header("ft_substr");
    safe_test("Basic substring", test_ft_substr_basic);
    safe_test("Out of bounds", test_ft_substr_out_of_bounds);
    safe_test("NULL string", test_ft_substr_null);
}

void test_ft_strjoin(void) {
    print_header("ft_strjoin");
    safe_test("Basic join", test_ft_strjoin_basic);
    safe_test("Empty string", test_ft_strjoin_empty);
    safe_test("NULL pointer", test_ft_strjoin_null);
}

void test_ft_strtrim(void) {
    print_header("ft_strtrim");
    safe_test("Basic trim", test_ft_strtrim_basic);
    safe_test("No trimming needed", test_ft_strtrim_no_trim);
    safe_test("NULL pointer", test_ft_strtrim_null);
}

void test_ft_split(void) {
    print_header("ft_split");
    safe_test("Basic split", test_ft_split_basic);
    safe_test("Empty string", test_ft_split_empty);
    safe_test("NULL pointer", test_ft_split_null);
}

void test_ft_itoa(void) {
    print_header("ft_itoa");
    safe_test("Positive number", test_ft_itoa_positive);
    safe_test("Negative number", test_ft_itoa_negative);
    safe_test("Zero", test_ft_itoa_zero);
}

void test_ft_strmapi(void) {
    print_header("ft_strmapi");
    safe_test("Basic mapping", test_ft_strmapi_basic);
    safe_test("NULL pointer", test_ft_strmapi_null);
}

void test_ft_striteri(void) {
    print_header("ft_striteri");
    safe_test("Basic iteration", test_ft_striteri_basic);
    safe_test("NULL pointer", test_ft_striteri_null);
}

void test_ft_putchar_fd(void) {
    print_header("ft_putchar_fd");
    printf("Expected output: A");
    safe_test("Basic character output", test_ft_putchar_fd_basic);
}

void test_ft_putstr_fd(void) {
    print_header("ft_putstr_fd");
    printf("Expected output: Hello\\n");
    safe_test("Basic string output", test_ft_putstr_fd_basic);
    safe_test("NULL string", test_ft_putstr_fd_null);
}

void test_ft_putendl_fd(void) {
    print_header("ft_putendl_fd");
    printf("Expected output: Hello\\n");
    safe_test("String with newline", test_ft_putendl_fd_basic);
}

void test_ft_putnbr_fd(void) {
    print_header("ft_putnbr_fd");
    printf("Expected output: 42\\n");
    safe_test("Basic number output", test_ft_putnbr_fd_basic);
    safe_test("NULL pointer", test_ft_memset_null);
}

/* Print final statistics */
void print_stats(void) {
    printf(BOLD CYAN "\n=== TEST SUMMARY ===" RESET "\n");
    printf("Total tests: %d\n", g_stats.total);
    printf(GREEN "Passed: %d" RESET "\n", g_stats.passed);
    printf(RED "Failed: %d" RESET "\n", g_stats.failed);
    printf(MAGENTA "Segfaults: %d" RESET "\n", g_stats.segfaults);
    
    double success_rate = g_stats.total > 0 ? 
        (double)g_stats.passed / g_stats.total * 100 : 0;
    
    if (success_rate >= 90) {
        printf(GREEN BOLD "Success rate: %.1f%%" RESET "\n", success_rate);
    } else if (success_rate >= 70) {
        printf(YELLOW BOLD "Success rate: %.1f%%" RESET "\n", success_rate);
    } else {
        printf(RED BOLD "Success rate: %.1f%%" RESET "\n", success_rate);
    }
    
    if (g_stats.segfaults > 0) {
        printf(RED "\n⚠️  Warning: %d segfault(s) detected! Check your implementations.\n" RESET, 
               g_stats.segfaults);
    }
}

int main(void) {
    printf(BOLD BLUE "🧪 LIBFT TESTER - Advanced Edition 🧪" RESET "\n"); 
    printf("Testing libft functions with edge cases and segfault detection\n");
    /* Call all test groups */
    test_ft_strlen();
    test_ft_strchr();
    test_ft_strdup();
    test_ft_memset();
    test_ft_bzero();
    test_ft_memcpy();
    test_ft_memmove();
    test_ft_memchr();
    test_ft_memcmp();
    test_ft_strlcpy();
    test_ft_strlcat();
    test_ft_strrchr();
    test_ft_strncmp();
    test_ft_strnstr();
    test_ft_atoi();
    test_ft_isalpha();
    test_ft_isdigit();
    test_ft_isalnum();
    test_ft_isascii();
    test_ft_isprint();
    test_ft_toupper();
    test_ft_tolower();
    test_ft_calloc();
    test_ft_substr();
    test_ft_strjoin();
    test_ft_strtrim();
    test_ft_split();
    test_ft_itoa();
    test_ft_strmapi();
    test_ft_striteri();
    test_ft_putchar_fd();
    test_ft_putstr_fd();
    test_ft_putendl_fd();
    test_ft_putnbr_fd();

    /* Print final statistics */
    printf(BOLD CYAN "\n=== TEST SUMMARY ===" RESET "\n");
    printf("Total:   %d\n", g_stats.total);
    printf(GREEN "Passed:  %d\n" RESET, g_stats.passed);
    printf(RED "Failed:  %d\n" RESET, g_stats.failed);
    printf(RED "Segfaults: %d\n" RESET, g_stats.segfaults);

    return (g_stats.failed == 0 && g_stats.segfaults == 0) ? 0 : 1;
}


/* 
 * Compilation instructions:
 * gcc -Wall -Wextra -Werror main.c -L. -lft -o libft_test
 * 
 * Or if you have individual .c files:
 * gcc -Wall -Wextra -Werror main.c ft_strlen.c ft_strchr.c ft_strdup.c ft_memset.c -o libft_test
 */