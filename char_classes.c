/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_classes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontesd <dmontesd@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:11:14 by dmontesd          #+#    #+#             */
/*   Updated: 2025/01/30 00:32:49 by dmontesd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "char_classes.h"

static int	*initialize_char_classes_arr(void)
{
	static int	char_classes[128];
	int			i;

	i = 0;
	while (i < 128)
	{
		char_classes[i] = 0;
		if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z'))
			char_classes[i] |= ALPHA | ALNUM;
		if (i >= 0 && i <= 127)
			char_classes[i] |= ASCII;
		if (i >= '0' && i <= '9')
			char_classes[i] |= DIGIT | ALNUM;
		if (i >= 32 && i <= 126)
			char_classes[i] |= PRINT;
		++i;
	}
	return (char_classes);
}

int	test_char_class(int _class, int _char)
{
	static bool		is_initialized = false;
	static int		*char_classes;
	unsigned char	uc;

	if (!is_initialized)
	{
		char_classes = initialize_char_classes_arr();
		is_initialized = true;
	}
	uc = _char;
	if (uc < 128)
		return (char_classes[uc] & _class);
	return (0);
}
