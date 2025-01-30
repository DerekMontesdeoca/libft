/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_classes.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontesd <dmontesd@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:36:33 by dmontesd          #+#    #+#             */
/*   Updated: 2025/01/30 00:32:11 by dmontesd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAR_CLASSES_H
# define CHAR_CLASSES_H

# define ALPHA 1
# define DIGIT 2 
# define ALNUM 4
# define ASCII 8
# define PRINT 16

int	test_char_class(int _class, int _char);

#endif
