#include <stdbool.h>
#define _GNU_SOURCE

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <bsd/string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../libft.h"

void	*__real_malloc(size_t size);

void	*__wrap_malloc(size_t size)
{
	check_expected(size);
	unsigned char	*memory_block = test_malloc(size);
	return (memset(memory_block, '\xff', size));
}


void __real_free(void *ptr);

void __wrap_free(void *ptr)
{
	test_free(ptr);
}

void    test_isalpha(void **state)
{
	int	res1;
	int	res2;

	(void) state;
	for (int i = -1; i < 256; ++i)
	{
		res1 = isalpha(i);
		res2 = ft_isalpha(i);
		assert_true((res1 == 0 && res2 == 0) || (res1 != 0 && res2 != 0));
	}
}

void    test_isdigit(void **state)
{
	int	res1;
	int	res2;

	(void) state;
	for (int i = -1; i < 256; ++i)
	{
		res1 = isdigit(i);
		res2 = ft_isdigit(i);
		assert_true((res1 == 0 && res2 == 0) || (res1 != 0 && res2 != 0));
	}
}

void    test_isalnum(void **state)
{
	int	res1;
	int	res2;

	(void) state;
	for (int i = -1; i < 256; ++i)
	{
		res1 = isalnum(i);
		res2 = ft_isalnum(i);
		assert_true((res1 == 0 && res2 == 0) || (res1 != 0 && res2 != 0));
	}
}

void    test_isascii(void **state)
{
	int	res1;
	int	res2;

	(void) state;
	for (int i = -1; i < 256; ++i)
	{
		res1 = isascii(i);
		res2 = ft_isascii(i);
		assert_true((res1 == 0 && res2 == 0) || (res1 != 0 && res2 != 0));
	}
}

void    test_memset_basic(void **state)
{
	char	buffer_a[10] = {0};
	char	buffer_b[10] = "aaaaaaaaa";

	(void) state;
	ft_memset(buffer_a, 'a', 9);
	assert_string_equal(buffer_a, buffer_b);
}

void	test_memset_zero(void **state)
{
	char	buffer_a[5] = {0};
	char	buffer_b[5] = {0};

	(void) state;
	ft_memset(buffer_a, 'z', 0);
	assert_memory_equal(buffer_a, buffer_b, 5);
}

void	test_memset_unsigned(void **state)
{
	char	buffer_a[2];
	char	buffer_b[3] = "\xff\xff";

	(void) state;
	ft_memset(buffer_a, (unsigned char)'\xff', 2);
	assert_memory_equal(buffer_a, buffer_b, 2);
}

void	test_memcpy_unsigned_char(void **state)
{
	char	*src = "\xff\x12\200";
	char	dst[3];
	
	(void) state;
	ft_memcpy(dst, src, 3);
	assert_memory_equal(src, dst, 3);
}

// Lesson learned: clever way to avoid temp array.
void	test_memmove_nop(void **state)
{
	char *expected = "~\x12\200\0x\n";
	char actual[100] =  "~\x12\200\0x\n";

	(void) state;
	ft_memmove(actual+1, actual+1, 4);
	assert_memory_equal(expected, actual, 6);
}

void	test_memmove_nop_2(void **state)
{
	char *expected = "~\x12\200\0x\n";
	char actual[100] =  "~\x12\200\0x\n";

	(void) state;
	ft_memmove(actual, actual+3, 0);
	assert_memory_equal(expected, actual, 6);
}

void	test_memmove_overlap_right(void **state)
{
	char	*expected = "YYou are a sad, strange, little man, and you have my "
		"pity.";
	char	actual[100] = "You are a sad, strange, little man, and you have "
		"my pity.";

	(void) state;
	ft_memmove(actual + 1, actual, 58);
	assert_string_equal(actual, expected);
}

void	test_memmove_overlap_left(void **state)
{
	char	*expected = "he dude abides..";
	char	actual[] = "The dude abides.";

	(void) state;
	ft_memmove(actual, actual + 1, 15);
	assert_string_equal(actual, expected);
}

void	test_strlcpy_basic(void **state)
{
	char	*expected = "..........";
	char	actual[11];
    size_t	res1 = ft_strlcpy(actual, expected, sizeof(actual));

	(void) state;
	assert_string_equal(actual, expected);
	assert_int_equal(res1, 10);
}

void	test_strlcpy_puts_null_if_trunc(void **state)
{
	char	expected[] = "Uncle Charlie, wann";
	char	src[] = "Uncle Charlie, wanna see a movie?";
	char	dst[20];

	(void) state;
	size_t	len = ft_strlcpy(dst, src, sizeof(dst));
	assert_string_equal(dst, expected);
	assert_int_equal(len, 33);
}

void	test_strlcpy_puts_no_null_if_no_space(void **state)
{
	char	expected[] = "You're making me really nervous!";
	char	src[] = "Oh man!";
	char	dst[] = "You're making me really nervous!";

	(void) state;
	size_t	len = ft_strlcpy(dst, src, 0);
	assert_string_equal(dst, expected);
	assert_int_equal(len, 7);
}

void	test_strlcpy_copies_full_string_if_len_smaller(void **state)
{
	char	expected[] = " Number 1\0a loose floor\xff board. ";
	char	src[] = "Number 1";
	char	dst[] = " That was a loose floor\xff board. ";

	(void) state;
	size_t	len = ft_strlcpy(dst + 1, src, 9);
	assert_memory_equal(dst, expected, 33);
	assert_int_equal(len, 8);
}

// Lesson learned:
// If dest is not nul-terminated, it will not run off the string.
// If src is not nul-terminated, it will run off the string reading garbage 
// values. Null must be checked dynamically unlike strlcpy.
void	test_strlcat_basic(void **state)
{
	char	dst[100] = "All work no play";
	char	src[] = " makes Charlie a dull boy.";
	char	*expected = "All work no play makes Charlie a dull boy.";

	(void) state;
	size_t	len = ft_strlcat(dst, src, 43);
	assert_string_equal(dst, expected);
	assert_int_equal(len, 42);
}

void	test_strlcat_nop(void **state)
{
	char	dst[100] = "Things we have in common?";
	char	src[] = " Same last name.";
	char	*expected = "Things we have in common?";

	(void) state;
	size_t	len = ft_strlcat(dst, src, 0);
	assert_string_equal(dst, expected);
	assert_int_equal(len, 16);
}

void	test_strlcat_trunc_exact_dest(void **state)
{
	char	dst[100] = "Charlie's marriage";
	char	src[] = " is just as likely to fail as mine.";
	char	*expected = "Charlie's marriage";

	(void) state;
	size_t	len = ft_strlcat(dst, src, 18);
	assert_string_equal(dst, expected);
	assert_int_equal(len, 35+18);
}

void	test_strlcat_trunc_src(void **state)
{
	char	dst[100] = "Carbunkles are technically";
	char	src[] = " in the boil family.";
	char	*expected = "Carbunkles are technically in the boil";

	(void) state;
	size_t	len = ft_strlcat(dst, src, 39);
	assert_string_equal(dst, expected);
	assert_int_equal(len, 46);
}

void	test_strlcat_empty(void **state)
{
	char	dst[100] = "";
	char	src[] = "";
	char	*expected = "";

	(void) state;
	size_t	len = ft_strlcat(dst, src, 10);
	assert_string_equal(dst, expected);
	assert_int_equal(len, 0);
}

// Lesson learned:
// Func signature is interesting. Receives a const * but return a *. This 
// is for const correctness and mainly to document that a function doesn't
// modify the parameter. Yet, once you return it, being able to modify it 
// is more flexible, in case you passed a non const * initially.
void	test_strchr_basic(void **state)
{
	char	*search = "I'll remember. I'll act surprised";
	char	*expected = search + 15;
	char	*found;

	(void) state;
	found = ft_strchr(search + 1, 'I');
	assert_ptr_equal(found, expected);
}

void	test_strchr_unsigned(void **state)
{
	char	*search = "\200\xffrn\n";
	char	*expected = search + 1;
	char	*found;

	(void) state;
	found = ft_strchr(search, (unsigned char)'\xff');
	assert_ptr_equal(found, expected);
}

void	test_strchr_unsigned_2(void **state)
{
	char	*search = "\200\xffrn\n";
	char	*expected = search + 5;
	char	*found;

	(void) state;
	found = ft_strchr(search, 256);
	assert_ptr_equal(found, expected);
}

// Lesson learned:
// Integer promotion and casting to unsigned.
void	test_strncmp_basic(void **state)
{
	char	*s1 = "You're weating pijama bottoms, right?";
	char	*s2 = "You're wearing pijama bottoms, right?";

	(void) state;
	int result = ft_strncmp(s1, s2, 38);
	assert_int_equal(result, 2);
}

void	test_strncmp_n_limit(void **state)
{
	char	*s1 = "";
	char	*s2 = "question";

	(void) state;
	int result = ft_strncmp(s1, s2, 2);
	assert_int_equal(result, 'q' * -1);
}

void	test_strncmp_unsigned(void **state)
{
	char	*s1 = "!\200\xfa";
	char	*s2 = "!\200";

	(void) state;
	int result = ft_strncmp(s1, s2, 3);
	assert_int_equal(result, 0xfa);
}

void	test_strncmp_smaller_size(void **state)
{
	char	*s1 = "I like a little buzz before I pray";
	char	*s2 = "I like a little buzz before I pray";

	(void) state;
	int result = ft_strncmp(s1, s2, 100);
	assert_int_equal(result, 0);
}

void	test_memchr_basic(void **state)
{
	char	*search = "No, she's a masseuseuse and a stripper.";
	char	*expected = search + 12;

	(void) state;
	char	*result = ft_memchr(search, 'm', 39);
	assert_ptr_equal(result, expected);
}

void	test_memcmp_basic(void **state)
{
	char	*s1 = "\xff\xff\xffh";
 	char	*s2 = "\xff\xff\xffhello";

	(void) state;
	int	result = ft_memcmp(s1, s2, 5);
	assert_int_equal(result, 'e' * -1);
}

void	test_strnstr(
		void **state, char *haystack, char *needle,
		size_t len, int offset
) {
	char	*result = ft_strnstr(haystack, needle, len);
	char	*expected = offset >= 0 ? haystack + offset : NULL;

	(void) state;
	assert_ptr_equal(result, expected);
}

void	test_strnstr_basic(void **state)
{
	test_strnstr(
		state,
		"Isn't that Chelsea's friend Gale?",
		"Chelsea",
		34,
		11
	);
}

void	test_strnstr_trunc(void **state)
{
	test_strnstr(
		state,
		"We should probably keep it between us.",
		"keep",
		10,
		-1
	);
}

void	test_strnstr_trunc_mid(void **state)
{
	test_strnstr(
			state,
			"Tough night bonkie?",
			"night",
			9,
			-1
			);
}

void	test_strnstr_empty_haystack(void **state)
{
	test_strnstr(
			state,
			"",
			"Nouup, that's what happened, end of story.",
			10,
			-1
			);
}

void	test_strnstr_empty_needle(void **state)
{
	test_strnstr(
			state,
			"Thank you, I love you so much",
			"",
			100,
			0
			);
}

void	test_strnstr_big_size(void **state)
{
	test_strnstr(
			state,
			"I'm never going to drink again",
			"attaboy",
			2398479283,
			-1
			);
}

// Lesson Learned:
// Atoi just calls strtol and doesn't check for any errors whatsover. That 
// means that it will only set errno when long int overflows or underflows. It 
// also means that the return value is just whatever strtol returns casted to
// int. In conclusion, to emulate the behavior of atoi correctly, one needs to 
// implement strtol and cast the value to int...
void	test_atoi(void **state, char *n_str, int expected)
{
	(void) state;
	int	result = result = ft_atoi(n_str);
	assert_int_equal(result, expected);
}

void	test_atoi_zero(void **state)
{
	test_atoi(state, "0", 0);
}

void	test_atoi_negative_zero(void **state)
{
	test_atoi(state, "-0", 0);
}

void	test_atoi_int_max(void **state)
{
	test_atoi(state, "2147483647", INT_MAX);
}

void	test_atoi_int_max_minus_one(void **state)
{
	test_atoi(state, "2147483646", 2147483646);
}

void	test_atoi_int_min(void **state)
{
	test_atoi(state, "-2147483648", INT_MIN);
}

void	test_atoi_int_round(void **state)
{
	test_atoi(state, "2000000000", 2000000000);
}

void	test_atoi_int_round_minus_one(void **state)
{
	test_atoi(state, "1999999999", 1999999999);
}

void	test_atoi_negative(void **state)
{
	test_atoi(state, "-239847", -239847);
}

void	test_atoi_positive(void **state)
{
	test_atoi(state, "9834798", 9834798);
}

void	test_atoi_space_prefix_non_digit_suffix(void **state)
{
	test_atoi(state, "     -329487dh", -329487);
}

void	test_atoi_space_prefix_non_digit_suffix_2(void **state)
{
	test_atoi(state, "     -329487.23", -329487);
}

void	test_atoi_spaces(void **state)
{
	test_atoi(state, "\v\f\r\t\n 983 4", 983);
}

void	test_atoi_0_and_spaces(void **state)
{
	test_atoi(state, "\f0\t\n00034", 0);
}

void	test_ft_calloc(void **state, size_t size, size_t nmemb, bool is_null)
{
	(void) state;
	expect_value(__wrap_malloc, size, nmemb*size);
	char	*ptr = ft_calloc(nmemb, size);
	if (is_null)
		assert_null(ptr);
	else 
	{
		for (size_t i = 0; i < nmemb*size; ++i)
			assert_int_equal(ptr[i], 0);
		free(ptr);
	}
}

void	test_ft_calloc_basic(void **state)
{
	test_ft_calloc(state, 23, sizeof(int), false);
}

void	test_ft_calloc_empty(void **state)
{
	test_ft_calloc(state, 0, 1, false);
}

void	test_ft_calloc_empty_2(void **state)
{
	test_ft_calloc(state, 1, 0, false);
}

void	test_ft_calloc_empty_3(void **state)
{
	test_ft_calloc(state, 0, 0, false);
}

void	test_strdup(void **state, char *str)
{
	(void) state;
	size_t sz = strlen(str) + 1;
	expect_value(__wrap_malloc, size, sz);
	char	*result = ft_strdup(str);
	assert_string_equal(result, str);
	free(result);
}

void	test_strdup_basic(void **state)
{
	test_strdup(state, "You are walking the wrong way alan");
}

void	test_strdup_empty(void **state)
{
	test_strdup(state, "");
}

void	test_substr(
		void **state, char *str, size_t start, size_t len, char* expected
) {
	(void) state;
	expect_value(__wrap_malloc, size, strlen(expected) + 1);
	char	*result = ft_substr(str, start, len);
	assert_non_null(result);
	assert_string_equal(result, expected);
	free(result);
}

void	test_substr_basic(void **state)
{
	test_substr(
			state, 
			"When I come closer you pull away.",
			5,
			13,
			"I come closer"
			);
}

void	test_substr_all_string(void **state)
{
	test_substr(
			state,
			"With the hope I don't make a mistake again.",
			0,
			43,
			"With the hope I don't make a mistake again."
			);
}

void	test_substr_zero_len(void **state)
{
	test_substr(
			state,
			"The moon is my witness-",
			5,
			0,
			""
			);
}

void	test_substr_giant_len(void **state)
{
	test_substr(
			state,
			"When I think about you",
			0,
			1378498734,
			"When I think about you"
			);
}

void	test_substr_out_of_bounds_start(void **state)
{
	test_substr(
			state, 
			"How the turn tables!", 
			98347, 
			1, 
			""
			);
}

void	test_substr_out_of_bounds_start_zero_len(void **state)
{
	test_substr(
			state, 
			"How the turn tables!", 
			98347, 
			0, 
			""
			);
}

void	test_strjoin(
	void **state, const char *str1, const char *str2, const char *expected
) {
	(void) state;
	expect_value(__wrap_malloc, size, strlen(str1) + strlen(str2) + 1);
	char	*result = ft_strjoin(str1, str2);
	assert_non_null(result);
	assert_string_equal(result, expected);
	free(result);
}

void	test_strjoin_basic(void **state)
{
	test_strjoin(
			state, 
			"Senor", 
			" advertencia", 
			"Senor advertencia"
			);
}

void	test_strjoin_empty(void **state)
{
	test_strjoin( state, "", "", "");
}

void	test_strjoin_empty_1(void **state)
{
	test_strjoin(
			state, "", "Maybe that was not your intention", 
			 "Maybe that was not your intention"
			);
}

void	test_strjoin_empty_2(void **state)
{
	test_strjoin(
			state, "A dose of intuition", "",
			"A dose of intuition"
			);
}

void	test_strjoin_single_space_str2(void **state)
{
	test_strjoin( state, "what", " ", "what ");
}

void	test_strtrim(void **state, char *str, char *set, char *expected)
{
	(void) state;
	expect_value(__wrap_malloc, size, strlen(expected) + 1);
	char	*result = ft_strtrim(str, set);
	assert_non_null(result);
	assert_string_equal(result, expected);
	free(result);
}

void	test_strtrim_basic(void **state)
{
	test_strtrim(
			state,
			"### We have all gathered here ###",
			"# ",
			"We have all gathered here"
			);
}

void	test_strtrim_all_in_set(void **state)
{
	test_strtrim( state, "### !#!     #!", "# !", "");
}

void	test_strtrim_all_but_last_in_set(void **state)
{
	test_strtrim( state, "### !#!     #!a", "# !", "a");
}

void	test_strtrim_all_but_first_in_set(void **state)
{
	test_strtrim( state, "a### !#!     #!", "# !", "a");
}

void	test_strtrim_all_in_set_in_middle(void **state)
{
	test_strtrim( state, "a### !#!     #!a", "# !", "a### !#!     #!a");
}

void	test_strtrim_empty_src(void **state)
{
	test_strtrim( state, "", "helo", "");
}

void	test_strtrim_empty_set(void **state)
{
	test_strtrim(
			state,
			"There are friends to make songs about.",
			"",
			"There are friends to make songs about."
			);
}

void	test_strtrim_repeated_characters_on_set(void **state)
{
	test_strtrim(
			state,
			"There are friends to make songs about",
			"ttttttttTTTTTTTTTT",
			"here are friends to make songs abou"
			);
}

void	test_split(void **state, char *s, char c, char **expected)
{
	(void) state;
	int	i = 0;
	for (; expected[i] != NULL; ++i);
	expect_value(__wrap_malloc, size, sizeof(char*) * (i + 1));
	i = 0;
	for (; expected[i] != NULL; ++i)
		expect_value(__wrap_malloc, size, strlen(expected[i])+ 1);
	char	**result = ft_split(s, c);
	i = 0;
	for (; expected[i] != NULL && result[i] != NULL ; ++i)
	{
		assert_string_equal(result[i], expected[i]);
		free(result[i]);
	}
	assert_null(expected[i]);
	assert_null(result[i]);
	free(result);
}

void	test_split_basic(void **state)
{
	test_split(
		state,
		"On the floor unstoppable looking like ----",
		'o',
		(char *[]){
			"On the fl", "r unst", "ppable l", "king like ----", NULL
		}
	);
}

void	test_split_only_delim(void **state)
{
	test_split( state, "eeeeeee", 'e', (char *[]){NULL});
}

void	test_split_no_delim(void **state)
{
	test_split( state, "Are you free?", '~', (char*[]){"Are you free?", NULL});
}

void	test_split_empty_str(void **state)
{
	test_split( state, "", 'a', (char*[]){NULL});
}

void	test_split_searching_null_byte(void **state)
{
	test_split(
			state,
			"Dance to the rhythm.",
			'\0',
			(char*[]){"Dance to the rhythm.", NULL}
			);
}

void	test_itoa(void **state, int n, char *expected)
{
	(void) state;
	expect_value(__wrap_malloc, size, strlen(expected) + 1);
	char	*result = ft_itoa(n);
	assert_non_null(result);
	assert_string_equal(result, expected);
	free(result);
}

void	test_itoa_basic(void **state)
{
	test_itoa(state, 9834, "9834");
}

void	test_itoa_negative(void **state)
{
	test_itoa(state, -2347890, "-2347890");
}

void	test_itoa_zero(void **state)
{
	test_itoa(state, 0, "0");
}

void	test_itoa_int_max(void **state)
{
	test_itoa(state, INT_MAX, "2147483647");
}

void	test_itoa_int_min(void **state)
{
	test_itoa(state, INT_MIN, "-2147483648");
}

void	test_itoa_round_number(void **state)
{
	test_itoa(state, 2000000000, "2000000000");
}

void	test_itoa_round_number_minus_one(void **state)
{
	test_itoa(state, 1999999999, "1999999999");
}

void	test_itoa_neg_round_number(void **state)
{
	test_itoa(state, -2000000000, "-2000000000");
}

void	test_itoa_neg_round_number_minus_one(void **state)
{
	test_itoa(state, -1999999999, "-1999999999");
}

void	test_strmapi(
	void **state, char *s, char (*f)(unsigned int, char), char *expected
) {
	(void) state;
	int size = strlen(expected) + 1;
	expect_value(__wrap_malloc, size, size);
	char *	result = ft_strmapi(s, f);
	assert_non_null(result);
	assert_string_equal(result, expected);
	free(result);
}

char	ft_toupper_lambda(unsigned int i, char c)
{
	(void) i;
	return (toupper(c));
}

char	ft_id(unsigned int i, char c)
{
	(void) i;
	return (c);
}

void	test_strmapi_basic(void **state)
{
	test_strmapi(
		state,
		"Listen up pal: I'm dating your ex-wife and there's nothing you can do"
		" about it.",
		ft_toupper_lambda,
		"LISTEN UP PAL: I'M DATING YOUR EX-WIFE AND THERE'S NOTHING YOU CAN DO"
		" ABOUT IT."
	);
}

void	test_strmapi_empty(void **state)
{
	test_strmapi(state, "", ft_toupper_lambda, "");
}

void	test_striteri(
	void **state, const char *s, void (*f)(unsigned int, char *), char *expected
) {
	(void) state;
	char *s2 = strdup(s);
	ft_striteri(s2, f);
	assert_string_equal(s2, expected);
}

void	id_mut(unsigned int i, char *c)
{
	*c += i;
}

void	test_striteri_basic(void **state)
{
	test_striteri(
		state,
		"aaaaaaaaaaaaaaaaaaaaaaaa",
		id_mut,
		"abcdefghijklmnopqrstuvwx"
	);
}

void	test_striteri_empty(void **state)
{
	test_striteri( state, "", id_mut, "");
}

ssize_t __real_write(int fd, const void *buf, size_t count);
ssize_t __wrap_write(int fd, const void *buf, size_t count)
{
	(void) fd;
	check_expected(buf);
	check_expected(count);
	return (count);
}

void	test_putendl_fd_basic(void **state)
{
	const char	*original = "The tall blonde chick in the red track suit " 
		"is starting to freak me out ";
	char *string = strdup(original);
	(void) state;
	expect_memory(
			__wrap_write,
			buf,
			"The tall blonde chick in the red track suit "
			"is starting to freak me out ",
			44+28
			);
	expect_value(__wrap_write, count, 44+28);
	expect_memory( __wrap_write, buf, "\n", 1);
	expect_value(__wrap_write, count, 1);
	ft_putendl_fd(string, 0);
	assert_string_equal(original, string);
}

void	test_putnbr_fd_basic(void **state)
{
	(void) state;
	expect_memory(__wrap_write, buf, "-982734", 7);
	expect_value(__wrap_write,  count, 7);
	ft_putnbr_fd(-982734, 0);
}	

void	test_putnbr_fd_zero(void **state)
{
	(void) state;
	expect_memory(__wrap_write, buf, "0", 1);
	expect_value(__wrap_write,  count, 1);
	ft_putnbr_fd(0, 0);
}	

void	test_list_create(void **state)
{
	(void) state;
	int	content = 9;
	expect_value(__wrap_malloc, size, sizeof(t_list));
	t_list *lst = ft_lstnew(&content);
	t_list expected = {.content = &content, .next = NULL};
	assert_memory_equal(lst, &expected, sizeof(t_list));
	free(lst);
}

void	test_list_push_front(void **state)
{
	(void) state;
	int	content[] = {1, 2, 3, 4, 5, 6};
	expect_any(__wrap_malloc, size);
	t_list *lst = ft_lstnew(content);
	assert_non_null(lst);
	t_list *new;
	for (int i = 1; i < 6; ++i)
	{
		expect_any(__wrap_malloc, size);
		new = ft_lstnew(content + i);
		assert_non_null(new);
		ft_lstadd_front(&lst, new);
	}
	int i = 6;
	for (t_list *cur = lst; cur != NULL; cur = cur->next, --i)
		assert_int_equal(*(int *)(cur->content), i);
	ft_lstclear(&lst, NULL);
}

void	test_list_push(void **state)
{
	(void) state;
	int content[] = {1,2,3,4,5,6};
	expect_any(__wrap_malloc, size);
	t_list *lst = ft_lstnew(content);
	assert_non_null(lst);
	t_list *new;
	for (int i = 1; i < 6; ++i)
	{
		expect_any(__wrap_malloc, size);
		new = ft_lstnew(content + i);
		assert_non_null(new);
		ft_lstadd_back(&lst, new);
	}
	int i = 1;
	for (t_list *cur = lst; cur != NULL; cur = cur->next, ++i)
		assert_int_equal(*(int *)(cur->content), i);
	ft_lstclear(&lst, NULL);
}

void	test_list_delone(void **state)
{
	(void) state;
	t_list *lst = test_malloc(sizeof(t_list));
	lst->content = test_calloc(10, 1);
	ft_lstdelone(lst, free);
}

void incr(void *p)
{
	*(int *)p += 1;
}

void	test_lstiter(void **state)
{
	(void) state;
	int content[] = {1,2,3,4,5,6};
	expect_any(__wrap_malloc, size);
	t_list *lst = ft_lstnew(content);
	assert_non_null(lst);
	t_list *new;
	for (int i = 1; i < 6; ++i)
	{
		expect_any(__wrap_malloc, size);
		new = ft_lstnew(content + i);
		assert_non_null(new);
		ft_lstadd_back(&lst, new);
	}
	ft_lstiter(lst, incr);
	int i = 1;
	for (t_list *cur = lst; cur != NULL; cur = cur->next, ++i)
		assert_int_equal(*(int *)(cur->content), i + 1);
	ft_lstclear(&lst, NULL);
}

void *incrmap(void *p)
{
	int *ret = __real_malloc(sizeof(int));
	*ret = *(int *)p + 1;
	return (ret);
}

void	test_lstmap(void **state)
{
	(void) state;
	int content[] = {1,2,3,4,5,6};
	expect_any(__wrap_malloc, size);
	t_list *lst = ft_lstnew(content);
	assert_non_null(lst);
	t_list *new;
	for (int i = 1; i < 6; ++i)
	{
		expect_any(__wrap_malloc, size);
		new = ft_lstnew(content + i);
		assert_non_null(new);
		ft_lstadd_back(&lst, new);
	}
	int i = 1;
	for (t_list *cur = lst; cur != NULL; cur = cur->next, ++i)
		assert_int_equal(*(int *)(cur->content), i);

	for (int i = 0; i < 6; ++i)
		expect_value(__wrap_malloc, size, sizeof(t_list));

	t_list *lst2 = ft_lstmap(lst, incrmap, free);
	i = 1;
	for (t_list *cur = lst2; cur != NULL; cur = cur->next, ++i)
		assert_int_equal(*(int *)(cur->content), i + 1);

	ft_lstclear(&lst, NULL);
	ft_lstclear(&lst2, NULL);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_isalpha),
		cmocka_unit_test(test_isdigit),
		cmocka_unit_test(test_isalnum),
		cmocka_unit_test(test_isascii),
		cmocka_unit_test(test_memset_basic),
		cmocka_unit_test(test_memset_zero),
		cmocka_unit_test(test_memset_unsigned),
		cmocka_unit_test(test_memcpy_unsigned_char),
		cmocka_unit_test(test_memmove_nop),
		cmocka_unit_test(test_memmove_nop_2),
		cmocka_unit_test(test_memmove_overlap_left),
		cmocka_unit_test(test_memmove_overlap_right),
		cmocka_unit_test(test_strlcpy_basic),
		cmocka_unit_test(test_strlcpy_puts_null_if_trunc),
		cmocka_unit_test(test_strlcpy_puts_no_null_if_no_space),
		cmocka_unit_test(test_strlcpy_copies_full_string_if_len_smaller),
		cmocka_unit_test(test_strlcat_basic),
		cmocka_unit_test(test_strlcat_nop),
		cmocka_unit_test(test_strlcat_trunc_exact_dest),
		cmocka_unit_test(test_strlcat_trunc_src),
		cmocka_unit_test(test_strlcat_empty),
		cmocka_unit_test(test_strchr_basic),
		cmocka_unit_test(test_strchr_unsigned),
		cmocka_unit_test(test_strchr_unsigned_2),
		cmocka_unit_test(test_strncmp_basic),
		cmocka_unit_test(test_strncmp_n_limit),
		cmocka_unit_test(test_strncmp_unsigned),
		cmocka_unit_test(test_strncmp_smaller_size),
		cmocka_unit_test(test_memchr_basic),
		cmocka_unit_test(test_memcmp_basic),
		cmocka_unit_test(test_strnstr_basic),
		cmocka_unit_test(test_strnstr_trunc),
		cmocka_unit_test(test_strnstr_trunc_mid),
		cmocka_unit_test(test_strnstr_empty_haystack),
		cmocka_unit_test(test_strnstr_empty_needle),
		cmocka_unit_test(test_strnstr_big_size),
		cmocka_unit_test(test_atoi_zero),
		cmocka_unit_test(test_atoi_negative_zero),
		cmocka_unit_test(test_atoi_int_max),
		cmocka_unit_test(test_atoi_int_max_minus_one),
		cmocka_unit_test(test_atoi_int_min),
		cmocka_unit_test(test_atoi_int_round),
		cmocka_unit_test(test_atoi_int_round_minus_one),
		cmocka_unit_test(test_atoi_negative),
		cmocka_unit_test(test_atoi_positive),
		cmocka_unit_test(test_atoi_space_prefix_non_digit_suffix),
		cmocka_unit_test(test_atoi_space_prefix_non_digit_suffix_2),
		cmocka_unit_test(test_atoi_spaces),
		cmocka_unit_test(test_atoi_0_and_spaces),
		cmocka_unit_test(test_ft_calloc_basic),
		cmocka_unit_test(test_ft_calloc_empty),
		cmocka_unit_test(test_ft_calloc_empty_2),
		cmocka_unit_test(test_ft_calloc_empty_3),
		cmocka_unit_test(test_strdup_basic),
		cmocka_unit_test(test_strdup_empty),
		cmocka_unit_test(test_substr_basic),
		cmocka_unit_test(test_substr_all_string),
		cmocka_unit_test(test_substr_zero_len),
		cmocka_unit_test(test_substr_giant_len),
		cmocka_unit_test(test_substr_out_of_bounds_start_zero_len),
		cmocka_unit_test(test_strjoin_basic),
		cmocka_unit_test(test_strjoin_empty),
		cmocka_unit_test(test_strjoin_empty_1),
		cmocka_unit_test(test_strjoin_empty_2),
		cmocka_unit_test(test_strjoin_empty_2),
		cmocka_unit_test(test_strjoin_single_space_str2),
		cmocka_unit_test(test_strtrim_basic),
		cmocka_unit_test(test_strtrim_all_in_set),
		cmocka_unit_test(test_strtrim_all_but_last_in_set),
		cmocka_unit_test(test_strtrim_all_but_first_in_set),
		cmocka_unit_test(test_strtrim_all_in_set_in_middle),
		cmocka_unit_test(test_strtrim_empty_src),
		cmocka_unit_test(test_strtrim_empty_set),
		cmocka_unit_test(test_strtrim_repeated_characters_on_set),
		cmocka_unit_test(test_split_basic),
		cmocka_unit_test(test_split_only_delim),
		cmocka_unit_test(test_split_no_delim),
		cmocka_unit_test(test_split_empty_str),
		cmocka_unit_test(test_split_searching_null_byte),
		cmocka_unit_test(test_itoa_basic),
		cmocka_unit_test(test_itoa_negative),
		cmocka_unit_test(test_itoa_zero),
		cmocka_unit_test(test_itoa_int_max),
		cmocka_unit_test(test_itoa_int_min),
		cmocka_unit_test(test_itoa_round_number),
		cmocka_unit_test(test_itoa_round_number_minus_one),
		cmocka_unit_test(test_itoa_neg_round_number),
		cmocka_unit_test(test_itoa_neg_round_number_minus_one),
		cmocka_unit_test(test_strmapi_basic),
		cmocka_unit_test(test_striteri_basic),
		cmocka_unit_test(test_striteri_empty),
		cmocka_unit_test(test_putendl_fd_basic),
		cmocka_unit_test(test_putnbr_fd_basic),
		cmocka_unit_test(test_putnbr_fd_zero),
		cmocka_unit_test(test_list_create),
		cmocka_unit_test(test_list_push_front),
		cmocka_unit_test(test_list_push),
		cmocka_unit_test(test_list_delone),
		cmocka_unit_test(test_lstiter),
		cmocka_unit_test(test_lstmap),
	};
	return (cmocka_run_group_tests(tests, NULL, NULL));
}
