/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:09:32 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/14 01:01:42 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# define BASE_10 "0123456789"
# define BASE_16L "0123456789abcdef"
# define BASE_16U "0123456789ABCDEF"
# define NOT_FREE 0
# define FREE 1

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *format, ...);

/*utils*/
char	*ft_str_range(char *stock, int start, int end);
int		my_atoi(char *nbr);

/*print*/
int		print_addr(void *ptr);
int		print_char(int c);
int		print_unsigned_nbr_base(unsigned long nb, char *base);
int		print_signed_nbr_base(long nb, char *base);
int		print_str(char *str, int to_free);

#endif
