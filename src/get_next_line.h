/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontesd <dmontesd@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:02:58 by dmontesd          #+#    #+#             */
/*   Updated: 2025/04/22 05:33:47 by dmontesd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

typedef struct s_buf
{
	int		fd;
	char	*buf;
	size_t	index;
	ssize_t	bytes_read;
	bool	nl_found;
}	t_buf;

typedef struct s_dynstr
{
	char	*data;
	size_t	size;
	size_t	len;
}	t_dynstr;

typedef enum e_buf_read_result
{
	BUF_READ_RESULT_OK,
	BUF_READ_RESULT_DONE,
	BUF_READ_RESULT_ERROR,
}	t_buf_read_result;

char	*get_next_line(int fd);

void	reset_buf(void);

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

void	dynstr_init(t_dynstr *str);

bool	dynstr_ensure_size(t_dynstr *str, size_t ensured);

t_buf	*get_buf(void);

#endif
