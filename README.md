*This project has been created as part of the 42 curriculum by asmaili*

# FDF - Wireframe Renderer

3D wireframe visualization of height maps using isometric projection.

## Features

- Parse `.fdf` files (grid of heights)
- Isometric projection
- Line drawing (Bresenham's algorithm)
- MinilibX graphics

## Compilation

```bash
make
```

## Usage

```bash
./fdf test_maps/pyramide.fdf
```

## Project Structure

```
FDF/
├── main.c              # Entry point
├── srcs/               # Source files
│   ├── parse.c         # File parsing
│   ├── draw.c          # Line drawing
│   ├── projection.c    # Isometric projection
│   ├── events.c        # Event handling
│   └── utils.c         # Utilities
├── includes/           # Headers
│   ├── fdf.h
|   ├── get_next_line.h
|   ├── ft_printf.h
│   ├── libft.h
│   └── mlx.h
├── lib/                # Libraries
│   ├── libft.a
│   └── libmlx.a
└── test_maps/          # Test files
```

## Ressources

42 Docs : https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html#introduction

MiniLibX : - man
		   - man3

IA : - handling project structure
	 - advices for the begining
