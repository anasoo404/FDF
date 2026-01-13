# MinilibX - Documentation Technique Complète

## 1. ARCHITECTURE BAS NIVEAU

### Le Système X11
```
Application (FDF)
    ↓ mlx_* functions
MinilibX (libmlx.a)
    ↓ XOpenDisplay, XCreateWindow, XPutImage...
Xlib (libX11.so)
    ↓ X Protocol
X Server (process séparé)
    ↓ drawing commands
GPU Driver
    ↓
Écran physique
```

**X11 = Client-Server Architecture:**
- Ton programme = client X11
- X Server = process qui gère l'écran/input
- Communication via sockets (local ou réseau)
- Display = connexion au serveur X

### Ce que MinilibX encapsule vraiment

**Xlib functions utilisées en interne:**
```c
XOpenDisplay()      → mlx_init()
XCreateWindow()     → mlx_new_window()
XCreateImage()      → mlx_new_image()
XPutImage()         → mlx_put_image_to_window()
XNextEvent()        → mlx_loop()
XDrawPoint()        → mlx_pixel_put()
```

---

## 2. STRUCTURES DE DONNÉES

### Pointeurs Opaques
MinilibX te donne des `void*` mais en réalité:

```c
// mlx_init() retourne:
typedef struct s_xvar {
    Display     *display;      // Connexion X11
    Window      root;          // Fenêtre root
    int         screen;        // Numéro d'écran
    int         depth;         // Profondeur couleur
    Visual      *visual;       // Info visuelle
    Colormap    cmap;          // Map de couleurs
    // ... autres champs internes
} t_xvar;

// mlx_new_window() retourne:
typedef struct s_win_list {
    Window      window;        // ID fenêtre X11
    GC          gc;            // Graphics Context
    int         width;
    int         height;
    // ... événements, hooks
} t_win_list;

// mlx_new_image() retourne:
typedef struct s_img {
    XImage      *image;        // Structure XImage
    char        *data;         // Buffer pixel (VRAI BUFFER!)
    int         size_line;     // Octets par ligne
    int         bpp;           // Bits per pixel
    int         endian;        // Byte order
    int         width;
    int         height;
} t_img;
```

---

## 3. LE BUFFER D'IMAGE (CRUCIAL)

### mlx_new_image() - Allocation mémoire

**Ce qui se passe:**
1. Alloue un buffer en RAM (malloc)
2. Crée une XImage qui pointe vers ce buffer
3. Le buffer = tableau de pixels en mémoire

```c
void *img = mlx_new_image(mlx, width, height);
char *addr = mlx_get_data_addr(img, &bpp, &size_line, &endian);
```

**addr = pointeur vers le buffer raw**

### Format du Buffer

**Bits per pixel (bpp):**
- Généralement 32 bits (4 octets par pixel)
- Format: `0xTTRRGGBB` (ARGB ou BGRA selon endian)

**size_line (line_length):**
- Octets par ligne d'image
- ≠ width * 4 (padding possible)
- Toujours utiliser size_line pour calculs

**Endianness:**
- Little endian: BB GG RR TT
- Big endian: TT RR GG BB
- Linux x86: généralement little endian

### Écrire un Pixel (MÉTHODE RAPIDE)

```c
void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char    *dst;

    // Calcul de l'offset dans le buffer
    dst = img->addr + (y * img->line_length + x * (img->bpp / 8));

    // Écriture directe en mémoire
    *(unsigned int*)dst = color;
}
```

**Pourquoi c'est rapide:**
- Pas d'appel système X11
- Écriture directe RAM
- mlx_put_image_to_window() copie tout d'un coup

**mlx_pixel_put() vs buffer:**
- `mlx_pixel_put()`: appel X11 immédiat (LENT)
- Buffer: écriture RAM puis flush (RAPIDE x1000)

---

## 4. FONCTIONS MINILIBX DÉTAILLÉES

### mlx_init()
```c
void *mlx_init(void);
```
**Fait:**
- `XOpenDisplay(NULL)` - Connexion au serveur X
- Initialise les structures internes
- Récupère infos écran/visuel

**Retourne:** pointeur vers structure connexion (void*)
**Échec:** NULL si pas de serveur X

### mlx_new_window()
```c
void *mlx_new_window(void *mlx, int w, int h, char *title);
```
**Fait:**
- `XCreateWindow()` - Crée fenêtre X11
- Configure attributs (taille, titre, événements)
- Map la fenêtre (rend visible)

**Retourne:** pointeur vers structure fenêtre
**Note:** Fenêtre pas encore affichée tant que pas dans loop

### mlx_new_image()
```c
void *mlx_new_image(void *mlx, int w, int h);
```
**Fait:**
- Alloue buffer mémoire (w × h × 4 octets généralement)
- Crée XImage pointant vers buffer
- Initialise métadonnées

**Retourne:** pointeur vers structure image
**Important:** Buffer en RAM, pas sur GPU encore

### mlx_get_data_addr()
```c
char *mlx_get_data_addr(void *img, int *bpp, int *size_line, int *endian);
```
**Paramètres:**
- `bpp`: OUT - bits per pixel (généralement 32)
- `size_line`: OUT - octets par ligne (avec padding)
- `endian`: OUT - byte order (0=big, 1=little)

**Retourne:** pointeur direct vers buffer pixel
**Usage:** Manipulation rapide des pixels

### mlx_put_image_to_window()
```c
int mlx_put_image_to_window(void *mlx, void *win, void *img, int x, int y);
```
**Fait:**
- `XPutImage()` - Copie buffer vers fenêtre X11
- Transfert RAM → VRAM
- Affichage à l'écran

**Position:** (x, y) = coin haut-gauche de l'image dans fenêtre
**Performance:** Appel une fois par frame, pas par pixel

### mlx_pixel_put()
```c
int mlx_pixel_put(void *mlx, void *win, int x, int y, int color);
```
**Fait:**
- `XDrawPoint()` immédiat
- Envoie commande au serveur X par socket

**Couleur:** 0xTTRRGGBB
**Éviter:** ULTRA LENT (1 appel système par pixel)
**Usage:** Debug uniquement

### mlx_loop()
```c
int mlx_loop(void *mlx);
```
**Fait:**
- Boucle infinie
- `XNextEvent()` - Attend événements X11
- Dispatch vers hooks enregistrés

**Bloquant:** Ne retourne jamais (sauf mlx_loop_end)

### mlx_loop_hook()
```c
int mlx_loop_hook(void *mlx, int (*f)(void*), void *param);
```
**Enregistre fonction appelée à chaque itération de loop**
**Usage:** Rendering continu, animations
**Attention:** Peut spam CPU si pas de throttling

### mlx_hook()
```c
int mlx_hook(void *win, int event, int mask, int (*f)(), void *param);
```
**Events X11:**
```c
KeyPress        = 2
KeyRelease      = 3
ButtonPress     = 4    // Souris
ButtonRelease   = 5
MotionNotify    = 6    // Mouvement souris
DestroyNotify   = 17   // Fermeture fenêtre
```

**Mask:** généralement 0 (auto)
**Remplace:** mlx_key_hook (plus flexible)

### mlx_key_hook() [Legacy]
```c
int mlx_key_hook(void *win, int (*f)(int keycode, void*), void *param);
```
**Limité:** Seulement KeyPress
**Préférer:** mlx_hook() pour key press/release

### mlx_mouse_hook()
```c
int mlx_mouse_hook(void *win, int (*f)(int button, int x, int y, void*), void *param);
```
**Boutons:**
- 1 = Gauche
- 2 = Milieu
- 3 = Droite
- 4 = Scroll up
- 5 = Scroll down

---

## 5. GESTION MÉMOIRE

### Allocation Flow
```
mlx_init()
    └─> malloc structures internes

mlx_new_window()
    └─> XCreateWindow() + structures

mlx_new_image(w, h)
    └─> malloc(w * h * 4) buffer
```

### Libération (IMPORTANT)
```c
// Ordre inverse de création
mlx_destroy_image(mlx, img);        // free buffer
mlx_destroy_window(mlx, win);       // XDestroyWindow
mlx_destroy_display(mlx);           // XCloseDisplay
free(mlx);                          // Structure mlx
```

**Fuites courantes:**
- Oublier mlx_destroy_image
- Recréer images sans détruire
- Exit sans cleanup

### Valgrind et MinilibX
```bash
valgrind --leak-check=full --show-leak-kinds=all ./fdf map.fdf
```
**Fuites normales:**
- Xlib alloue des choses non libérées (ignorable)
- Fontconfig, xcb (dépendances X11)

**Tes fuites à toi:**
- "definitely lost" dans ton code
- Images pas destroyed
- Parsing map (split, etc.)

---

## 6. GESTION DES ÉVÉNEMENTS

### Event Loop Interne
```c
mlx_loop(mlx);

// Équivalent à:
while (1) {
    XNextEvent(display, &event);
    switch (event.type) {
        case KeyPress:
            call_key_hook(...);
            break;
        case DestroyNotify:
            exit_program();
            break;
        // ... autres events
    }
}
```

### KeyCodes Linux
```c
#define KEY_ESC     65307
#define KEY_W       119
#define KEY_A       97
#define KEY_S       115
#define KEY_D       100
#define KEY_UP      65362
#define KEY_DOWN    65364
#define KEY_LEFT    65361
#define KEY_RIGHT   65363
#define KEY_PLUS    61
#define KEY_MINUS   45
```

**Obtenir keycodes:**
```bash
xev  # Lance outil, appuie sur touches
```

### Hook Pattern Optimal
```c
typedef struct s_data {
    void    *mlx;
    void    *win;
    void    *img;
    // ... ton contexte
} t_data;

int key_press(int keycode, t_data *data)
{
    if (keycode == KEY_ESC)
        exit_clean(data);
    else if (keycode == KEY_W)
        data->offset_y -= 10;
    // ... actions
    render(data);  // Re-render
    return (0);
}

int close_window(t_data *data)
{
    exit_clean(data);
    return (0);
}

// Setup
mlx_hook(win, 2, 0, key_press, data);        // KeyPress
mlx_hook(win, 17, 0, close_window, data);    // Close (X)
```

---

## 7. OPTIMISATION & PERFORMANCE

### Rendering Pipeline Optimal
```c
1. Créer image une fois (init)
2. Chaque frame:
   a. Clear buffer (memset ou loop)
   b. Dessiner tous pixels dans buffer
   c. mlx_put_image_to_window() UNE FOIS
3. Destroy image (exit)
```

### Double Buffering (Anti-Flicker)
```c
typedef struct s_data {
    void    *img_front;  // Image affichée
    void    *img_back;   // Image en cours de dessin
    // ... swap après chaque frame
} t_data;

void render(t_data *d) {
    draw_to_buffer(d->img_back);  // Dessine
    mlx_put_image_to_window(d->mlx, d->win, d->img_back, 0, 0);
    swap_buffers(d);  // Optionnel selon besoins
}
```

### Éviter les Appels Lents
**LENT:**
- mlx_pixel_put() dans loop
- mlx_string_put() par frame (alloue/dessine à chaque fois)
- Créer/détruire images par frame

**RAPIDE:**
- Écriture directe buffer
- Une image, modifier buffer
- mlx_put_image_to_window() par frame

### FPS Control
```c
#include <sys/time.h>

long get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int render_loop(t_data *data) {
    static long last_frame = 0;
    long now = get_time_ms();

    if (now - last_frame < 16)  // ~60 FPS
        return (0);

    last_frame = now;
    render_frame(data);
    return (0);
}

mlx_loop_hook(mlx, render_loop, data);
```

---

## 8. COULEURS & FORMATS

### Format Couleur Standard
```c
int color = 0xTTRRGGBB;
// TT = Transparence (ignoré généralement)
// RR = Rouge (0x00 à 0xFF)
// GG = Vert
// BB = Bleu
```

### Créer Couleur RGB
```c
int create_trgb(int t, int r, int g, int b)
{
    return (t << 24 | r << 16 | g << 8 | b);
}
```

### Extraire Composantes
```c
int get_t(int trgb) { return ((trgb >> 24) & 0xFF); }
int get_r(int trgb) { return ((trgb >> 16) & 0xFF); }
int get_g(int trgb) { return ((trgb >> 8) & 0xFF); }
int get_b(int trgb) { return (trgb & 0xFF); }
```

### Gradient Linéaire
```c
int lerp_color(int c1, int c2, float t)
{
    int r = get_r(c1) * (1 - t) + get_r(c2) * t;
    int g = get_g(c1) * (1 - t) + get_g(c2) * t;
    int b = get_b(c1) * (1 - t) + get_b(c2) * t;
    return (create_trgb(0, r, g, b));
}
```

---

## 9. FDF SPÉCIFIQUE

### Projections Isométrique
```c
// Point 3D → 2D iso
void iso_project(t_point *p, t_data *d)
{
    int x = p->x;
    int y = p->y;
    int z = p->z;

    // Projection isométrique
    p->screen_x = (x - y) * cos(0.523599);  // 30° en radians
    p->screen_y = (x + y) * sin(0.523599) - z;

    // Centre dans fenêtre + zoom
    p->screen_x = p->screen_x * d->zoom + d->offset_x;
    p->screen_y = p->screen_y * d->zoom + d->offset_y;
}
```

### Bresenham (Ligne)
```c
void draw_line(t_data *d, t_point p1, t_point p2)
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = p1.x < p2.x ? 1 : -1;
    int sy = p1.y < p2.y ? 1 : -1;
    int err = dx - dy;

    while (1) {
        my_pixel_put(d->img, p1.x, p1.y, color);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; p1.x += sx; }
        if (e2 < dx) { err += dx; p1.y += sy; }
    }
}
```

### Structure FDF Typique
```c
typedef struct s_point {
    int     x, y, z;        // Coordonnées 3D
    int     screen_x;       // Position écran après projection
    int     screen_y;
    int     color;          // Couleur du point
} t_point;

typedef struct s_map {
    t_point **points;       // Grille 2D de points
    int     width;          // Nombre colonnes
    int     height;         // Nombre lignes
    int     z_min, z_max;   // Pour normalisation
} t_map;

typedef struct s_data {
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;

    t_map   *map;
    float   zoom;
    int     offset_x;
    int     offset_y;
    float   angle_x;        // Rotation
    float   angle_y;
    float   angle_z;
    int     projection;     // ISO, PARALLEL, etc.
} t_data;
```

---

## 10. DEBUG & TOOLS

### Compilation avec Debug Symbols
```makefile
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
```

### Valgrind
```bash
# Leaks
valgrind --leak-check=full --show-leak-kinds=all ./fdf map.fdf

# Track origins
valgrind --track-origins=yes ./fdf map.fdf
```

### LLDB (Debugger)
```bash
lldb ./fdf
(lldb) b main               # Breakpoint
(lldb) run map.fdf          # Lance
(lldb) n                    # Next line
(lldb) s                    # Step into
(lldb) p variable           # Print
(lldb) bt                   # Backtrace
(lldb) frame variable       # Variables locales
```

### GDB
```bash
gdb ./fdf
(gdb) break main
(gdb) run map.fdf
(gdb) next
(gdb) print variable
(gdb) backtrace
```

### AddressSanitizer
```bash
# Compile avec -fsanitize=address
./fdf map.fdf
# Détecte: buffer overflow, use-after-free, double-free
```

### Norminette & Leaks au 42
```bash
# Norm
norminette srcs/ includes/

# Leaks wrapper 42
leaks --atExit -- ./fdf map.fdf
```

---

## 11. ERREURS COURANTES

### Segfault Classiques
1. **Buffer overrun:**
   ```c
   // MAUVAIS: dépassement image
   my_pixel_put(img, x, y, color);  // x ou y hors limites
   ```
   **Fix:** Vérifier bounds avant écriture

2. **Use after free:**
   ```c
   mlx_destroy_image(mlx, img);
   my_pixel_put(img, x, y, color);  // BOOM
   ```

3. **NULL pointer:**
   ```c
   void *img = mlx_new_image(mlx, w, h);
   // Pas de check si img == NULL
   char *addr = mlx_get_data_addr(img, ...);  // Segfault si img NULL
   ```

### Memory Leaks
```c
// Créer image en loop sans destroy
while (rendering) {
    void *img = mlx_new_image(mlx, w, h);  // LEAK!
    // ...
}

// Fix: créer une fois, réutiliser
void *img = mlx_new_image(mlx, w, h);
while (rendering) {
    memset(addr, 0, h * line_len);  // Clear
    // draw...
}
```

### X11 Connection Lost
```
Error: Can't open display
```
**Causes:**
- Pas de serveur X (SSH sans X forwarding)
- DISPLAY variable pas set
- Pas de droits d'accès

**Fix:**
```bash
echo $DISPLAY  # Doit afficher :0 ou similaire
export DISPLAY=:0
```

---

## 12. WORKFLOW FDF COMPLET

### Étapes FDF
```
1. Parse map (.fdf)
   ├─ Allouer structure t_map
   ├─ Lire fichier ligne par ligne
   ├─ Split, atoi pour valeurs z
   └─ Stocker dans grille 2D

2. Init MLX
   ├─ mlx_init()
   ├─ mlx_new_window()
   └─ mlx_new_image() + get_data_addr()

3. Setup hooks
   ├─ mlx_hook(KeyPress)
   ├─ mlx_hook(DestroyNotify)
   └─ mlx_loop_hook() si animation

4. Render initial
   ├─ Projeter tous points 3D → 2D
   ├─ Dessiner lignes entre points (grille)
   └─ mlx_put_image_to_window()

5. Event loop
   └─ mlx_loop() [bloquant]

6. Cleanup (exit hook)
   ├─ mlx_destroy_image()
   ├─ mlx_destroy_window()
   ├─ Free map
   └─ mlx_destroy_display() + free(mlx)
```

### Makefile Type
```makefile
NAME = fdf
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MLXFLAGS = -lmlx -lXext -lX11 -lm

SRCS = main.c parse.c draw.c projection.c events.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(MLXFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
```

---

## 13. CHECKLIST FDF

- [ ] Parse .fdf sans leaks
- [ ] Créer grille de points 3D
- [ ] Projection isométrique
- [ ] Dessiner lignes (Bresenham)
- [ ] Gestion couleurs/gradient
- [ ] ESC pour exit propre
- [ ] Close window (X) fonctionne
- [ ] Pas de leaks (valgrind)
- [ ] Pas de segfault sur maps invalides
- [ ] Bonus: zoom, translation, rotation

---

## 14. PATH TO 125 - BONUS AVANCÉS

### Rotations 3D (MATRICES)

**Théorie:**
- Rotation = multiplication par matrice 3×3
- Ordre des rotations change le résultat (non-commutatif)
- Convention: X → Y → Z

**Rotation autour X (pitch):**
```c
void rotate_x(t_point *p, float angle)
{
    float y = p->y;
    float z = p->z;

    p->y = y * cos(angle) - z * sin(angle);
    p->z = y * sin(angle) + z * cos(angle);
}
```

**Rotation autour Y (yaw):**
```c
void rotate_y(t_point *p, float angle)
{
    float x = p->x;
    float z = p->z;

    p->x = x * cos(angle) + z * sin(angle);
    p->z = -x * sin(angle) + z * cos(angle);
}
```

**Rotation autour Z (roll):**
```c
void rotate_z(t_point *p, float angle)
{
    float x = p->x;
    float y = p->y;

    p->x = x * cos(angle) - y * sin(angle);
    p->y = x * sin(angle) + y * cos(angle);
}
```

**Pipeline complet:**
```c
void transform_point(t_point *p, t_data *d)
{
    // 1. Centrer sur origine
    p->x -= d->map->width / 2;
    p->y -= d->map->height / 2;

    // 2. Appliquer rotations
    rotate_x(p, d->angle_x);
    rotate_y(p, d->angle_y);
    rotate_z(p, d->angle_z);

    // 3. Projection (iso/parallèle/perspective)
    project(p, d->projection_type);

    // 4. Zoom + translation
    p->screen_x = p->x * d->zoom + d->offset_x;
    p->screen_y = p->y * d->zoom + d->offset_y;
}
```

**Controls rotation:**
```c
// Dans key_press:
if (keycode == KEY_Q) data->angle_x += 0.1;
if (keycode == KEY_E) data->angle_x -= 0.1;
if (keycode == KEY_R) data->angle_y += 0.1;
if (keycode == KEY_F) data->angle_y -= 0.1;
```

---

### Projections Multiples

**1. Isométrique (standard):**
```c
void project_iso(t_point *p)
{
    float x = p->x;
    float y = p->y;
    float z = p->z;

    p->x = (x - y) * cos(0.523599);  // 30°
    p->y = (x + y) * sin(0.523599) - z;
}
```

**2. Parallèle (cabinet):**
```c
void project_parallel(t_point *p)
{
    p->x = p->x - p->z * 0.5;  // Facteur oblique
    p->y = p->y - p->z * 0.5;
    // p->z non utilisé pour projection
}
```

**3. Perspective (vraie 3D):**
```c
void project_perspective(t_point *p, float fov, float distance)
{
    float factor = fov / (distance + p->z);

    p->x = p->x * factor;
    p->y = p->y * factor;
    // Plus z est grand, plus point est petit
}
```

**Switch entre projections:**
```c
// KEY_P = cycle projections
if (keycode == KEY_P)
{
    data->projection = (data->projection + 1) % 3;
    render(data);
}
```

---

### Parsing Couleur Avancé

**Format .fdf avec couleurs:**
```
0,0xFF0000 10,0x00FF00 5,0x0000FF
20 15,0xFFFFFF 8
```

**Parser ligne avec couleur:**
```c
int parse_point(char *str, int *z, int *color)
{
    char    **split;
    int     has_color;

    split = ft_split(str, ',');
    if (!split)
        return (0);

    *z = ft_atoi(split[0]);

    if (split[1])  // Couleur présente
    {
        *color = ft_atoi_base(split[1], 16);  // Hexa
        has_color = 1;
    }
    else
    {
        *color = 0xFFFFFF;  // Blanc par défaut
        has_color = 0;
    }

    free_split(split);
    return (1);
}
```

**ft_atoi_base pour hexa:**
```c
int ft_atoi_base(char *str, int base)
{
    int result = 0;
    int i = 0;

    // Skip "0x" ou "0X"
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        i = 2;

    while (str[i])
    {
        result *= base;
        if (str[i] >= '0' && str[i] <= '9')
            result += str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f')
            result += str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F')
            result += str[i] - 'A' + 10;
        i++;
    }
    return (result);
}
```

---

### Gradient d'Altitude

**Couleur selon hauteur Z:**
```c
int get_altitude_color(int z, int z_min, int z_max)
{
    float ratio;
    int   r, g, b;

    if (z_max == z_min)
        return (0xFFFFFF);

    ratio = (float)(z - z_min) / (z_max - z_min);

    // Gradient bleu → vert → rouge
    if (ratio < 0.5)
    {
        r = 0;
        g = (int)(ratio * 2 * 255);
        b = 255 - g;
    }
    else
    {
        r = (int)((ratio - 0.5) * 2 * 255);
        g = 255 - r;
        b = 0;
    }

    return (create_trgb(0, r, g, b));
}
```

**Application lors parsing:**
```c
// Dans parse_map, après lecture z:
if (!has_color)  // Pas de couleur dans .fdf
    point.color = get_altitude_color(z, map->z_min, map->z_max);
```

---

### Zoom Dynamique (Auto-fit)

**Calculer zoom optimal:**
```c
float calculate_zoom(t_map *map, int win_w, int win_h)
{
    float   zoom_x, zoom_y;
    float   map_width, map_height;

    // Taille projetée de la map
    map_width = map->width * 1.5;   // Approximation iso
    map_height = map->height * 1.5;

    // Facteur Z pour tenir compte de la hauteur
    map_height += (map->z_max - map->z_min);

    // Zoom pour tenir dans fenêtre (80% de marge)
    zoom_x = (win_w * 0.8) / map_width;
    zoom_y = (win_h * 0.8) / map_height;

    // Prendre le plus petit
    return (zoom_x < zoom_y ? zoom_x : zoom_y);
}
```

**Centrage auto:**
```c
void center_map(t_data *d)
{
    d->offset_x = d->win_width / 2;
    d->offset_y = d->win_height / 2;
}
```

---

### Z-Scaling (Amplifier Hauteur)

**Multiplier altitude:**
```c
typedef struct s_data {
    // ... autres champs
    float   z_scale;  // Facteur multiplicateur Z
} t_data;

void transform_point(t_point *p, t_data *d)
{
    p->x -= d->map->width / 2;
    p->y -= d->map->height / 2;
    p->z *= d->z_scale;  // AMPLIFICATION

    rotate_x(p, d->angle_x);
    // ... suite
}
```

**Controls:**
```c
// + / - pour ajuster relief
if (keycode == KEY_PLUS)
{
    data->z_scale += 0.1;
    render(data);
}
if (keycode == KEY_MINUS)
{
    data->z_scale -= 0.1;
    if (data->z_scale < 0.1)
        data->z_scale = 0.1;
    render(data);
}
```

---

### Clipping (Optimisation)

**Ne dessiner que ce qui est visible:**
```c
int is_visible(int x, int y, t_data *d)
{
    return (x >= 0 && x < d->win_width &&
            y >= 0 && y < d->win_height);
}

void draw_line_clipped(t_data *d, t_point p1, t_point p2)
{
    // Si les deux points hors écran, skip
    if (!is_visible(p1.screen_x, p1.screen_y, d) &&
        !is_visible(p2.screen_x, p2.screen_y, d))
        return;

    // Sinon dessiner (Bresenham gérera les bords)
    draw_line(d, p1, p2);
}
```

**Cohen-Sutherland (clipping avancé):**
```c
#define INSIDE  0  // 0000
#define LEFT    1  // 0001
#define RIGHT   2  // 0010
#define BOTTOM  4  // 0100
#define TOP     8  // 1000

int compute_outcode(int x, int y, t_data *d)
{
    int code = INSIDE;

    if (x < 0)              code |= LEFT;
    else if (x >= d->win_width)   code |= RIGHT;
    if (y < 0)              code |= TOP;
    else if (y >= d->win_height)  code |= BOTTOM;

    return (code);
}

// Clip line (algo complet = long, cherche Cohen-Sutherland)
```

---

### Gestion Erreurs Parsing

**Validation robuste:**
```c
int validate_map_line(char *line, int *width)
{
    char    **split;
    int     i;

    split = ft_split(line, ' ');
    if (!split)
        return (0);

    i = 0;
    while (split[i])
        i++;

    if (*width == 0)
        *width = i;
    else if (*width != i)  // Lignes pas même largeur
    {
        free_split(split);
        return (0);
    }

    free_split(split);
    return (1);
}

int is_valid_map(char *filename)
{
    int     fd;
    char    *line;
    int     width = 0;
    int     height = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (0);

    while ((line = get_next_line(fd)))
    {
        if (!validate_map_line(line, &width))
        {
            free(line);
            close(fd);
            return (0);
        }
        free(line);
        height++;
    }
    close(fd);

    return (height > 0 && width > 0);
}
```

**Error handling main:**
```c
int main(int ac, char **av)
{
    t_data  data;

    if (ac != 2)
    {
        ft_putstr_fd("Usage: ./fdf <map.fdf>\n", 2);
        return (1);
    }

    if (!is_valid_map(av[1]))
    {
        ft_putstr_fd("Error: Invalid map\n", 2);
        return (1);
    }

    // ... init, parse, render
}
```

---

### Menu/HUD (Bonus stylé)

**Afficher infos à l'écran:**
```c
void draw_hud(t_data *d)
{
    char    *str;

    mlx_string_put(d->mlx, d->win, 10, 20, 0xFFFFFF, "Controls:");
    mlx_string_put(d->mlx, d->win, 10, 40, 0xFFFFFF, "WASD: Move");
    mlx_string_put(d->mlx, d->win, 10, 60, 0xFFFFFF, "Q/E: Rotate X");
    mlx_string_put(d->mlx, d->win, 10, 80, 0xFFFFFF, "+/-: Z Scale");
    mlx_string_put(d->mlx, d->win, 10, 100, 0xFFFFFF, "P: Projection");

    // Infos dynamiques
    str = ft_itoa((int)d->zoom);
    mlx_string_put(d->mlx, d->win, 10, 140, 0x00FF00, "Zoom:");
    mlx_string_put(d->mlx, d->win, 70, 140, 0x00FF00, str);
    free(str);
}
```

**Note:** `mlx_string_put` est lent, appeler qu'en cas de changement.

---

### Animation Smooth

**Rotation automatique:**
```c
int animate(t_data *d)
{
    static int frame = 0;

    frame++;

    if (frame % 2 == 0)  // Throttle
    {
        d->angle_z += 0.01;  // Rotation lente
        render(d);
    }

    return (0);
}

// Dans main:
mlx_loop_hook(mlx, animate, &data);
```

---

### Architecture Clean

**Séparation fichiers (bonus implicite):**
```
srcs/
├── main.c              # Init, loop
├── parse.c             # Lecture .fdf
├── parse_utils.c       # Split, atoi_base, validation
├── draw.c              # Bresenham, render
├── projection.c        # Iso, parallèle, perspective
├── transform.c         # Rotations, translations
├── color.c             # Gradient, lerp
├── events.c            # Hooks clavier/souris
├── utils.c             # Free, error handling
└── init.c              # Setup MLX, structures
```

**Header organisé:**
```c
// fdf.h
#ifndef FDF_H
# define FDF_H

# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include <math.h>
# include <fcntl.h>

// Keycodes
# define KEY_ESC    65307
# define KEY_W      119
# define KEY_A      97
# define KEY_S      115
# define KEY_D      100
// ... tous les defines

// Enums
typedef enum e_projection
{
    ISOMETRIC,
    PARALLEL,
    PERSPECTIVE
}   t_projection;

// Structures
typedef struct s_point { /* ... */ } t_point;
typedef struct s_map { /* ... */ } t_map;
typedef struct s_img { /* ... */ } t_img;
typedef struct s_data { /* ... */ } t_data;

// Prototypes par catégorie
// Parsing
t_map   *parse_map(char *filename);
int     validate_map(char *filename);
void    free_map(t_map *map);

// Drawing
void    render(t_data *d);
void    draw_line(t_data *d, t_point p1, t_point p2);
void    my_pixel_put(t_img *img, int x, int y, int color);

// Transformations
void    transform_point(t_point *p, t_data *d);
void    rotate_x(t_point *p, float angle);
void    project_iso(t_point *p);

// Colors
int     get_altitude_color(int z, int z_min, int z_max);
int     lerp_color(int c1, int c2, float t);

// Events
int     key_press(int keycode, t_data *d);
int     close_window(t_data *d);

// Utils
void    exit_clean(t_data *d);
void    print_error(char *msg);

#endif
```

---

### Makefile Pro

```makefile
NAME        = fdf

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3
MLXFLAGS    = -lmlx -lXext -lX11 -lm

LIBFT_DIR   = lib/libft
LIBFT       = $(LIBFT_DIR)/libft.a

INCLUDES    = -I includes -I $(LIBFT_DIR)/includes

SRCS_DIR    = srcs
SRCS        = main.c parse.c parse_utils.c draw.c projection.c \
              transform.c color.c events.c utils.c init.c

OBJS_DIR    = objs
OBJS        = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

# Colors
GREEN       = \033[0;32m
RED         = \033[0;31m
RESET       = \033[0m

all: $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)Compiled: $<$(RESET)"

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) $(MLXFLAGS) -o $(NAME)
	@echo "$(GREEN)FDF compiled successfully!$(RESET)"

clean:
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(RED)Objects cleaned$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(RED)Executable removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re
```

---

### Testing Complet

**Script de test:**
```bash
#!/bin/bash

MAPS=(test_maps/*.fdf)

echo "Testing FDF..."

for map in "${MAPS[@]}"
do
    echo -n "Testing $map... "
    timeout 2 ./fdf "$map" > /dev/null 2>&1
    if [ $? -eq 124 ]; then
        echo "OK (window opened)"
    else
        echo "FAIL"
    fi
done

echo ""
echo "Memory leaks check:"
valgrind --leak-check=full --error-exitcode=1 ./fdf test_maps/42.fdf 2>&1 | grep "definitely lost"
```

---

## 15. CHECKLIST COMPLÈTE 125/100

### Mandatory (100pts)
- [ ] Parse .fdf correctement
- [ ] Affichage wireframe 3D
- [ ] Projection isométrique
- [ ] Fenêtre + gestion MLX
- [ ] ESC ferme proprement
- [ ] Croix rouge (X) ferme proprement
- [ ] Pas de leaks
- [ ] Pas de segfault
- [ ] Norminette 100%
- [ ] Makefile (all, clean, fclean, re)

### Bonus (25pts max)
- [ ] Zoom (scroll ou +/-)
- [ ] Translation (WASD ou flèches)
- [ ] Rotation (3 axes)
- [ ] Projection alternative (parallèle/perspective)
- [ ] Couleurs altitude/gradient
- [ ] Bonus perso (menu, animation, etc.)

### Qualité Code (impression éval)
- [ ] Architecture propre (fichiers séparés)
- [ ] Pas de leaks même en bonus
- [ ] Gestion erreurs complète
- [ ] Code lisible/commenté
- [ ] Démo impressionnante

---

## RESSOURCES

**Man pages X11:**
```bash
man XOpenDisplay
man XCreateWindow
man XPutImage
man XNextEvent
```

**Tester events:**
```bash
xev  # Voir keycodes et events X11
```

**MinilibX source (si dispo):**
```bash
# À 42, check:
/usr/local/lib/libmlx.a
# Ou compile depuis source
```

**Algo Bresenham:**
- Wikipedia: Bresenham's line algorithm
- Géométrie discrète

**Projections 3D:**
- Isométrique: angles 30° (standard)
- Perspective: division par z
- Parallèle: pas de déformation

**Matrices de rotation:**
- Wikipedia: Rotation matrix
- 3Blue1Brown (YouTube): Linear transformations

---

## TL;DR - ROADMAP 125/100

**Semaine 1: Mandatory**
1. Parse .fdf → grille 3D
2. Init MLX + fenêtre
3. Bresenham + buffer
4. Projection iso basique
5. Hooks ESC + close

**Semaine 2: Polish + Bonus**
1. Zoom + translation
2. Rotations 3D (matrices)
3. Couleurs gradient
4. Gestion erreurs
5. Code clean + norm

**Jour éval:**
- Démo sur maps stylées (julia, mars)
- Montre rotations smooth
- Explique matrices/projections
- Zero leaks, zero segfault

**GG, t'as ton 125.**
