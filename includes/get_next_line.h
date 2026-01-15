/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmaili <asmaili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:06:46 by asmaili           #+#    #+#             */
/*   Updated: 2026/01/15 16:30:45 by asmaili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define MIN_FDS 0
# define MAX_FDS 4096

# define CONTINUE 0
# define SUCCESS 1
# define ERROR 2

# include <stdlib.h>
# include <unistd.h>

size_t	ft_strlen(const char *str);
char	*ft_str_range_f(char *stock, int start, int end, int free_stock);
char	*ft_strjoin_bytes_read(char *stock, char *buf_read, size_t bytes_read);
char	*get_next_line(int fd);

#endif
