/*
PROMPT POUR FUTURE CONVERSATION:

"Je travaille sur FDF (projet 42). J'ai besoin d'un assistant qui:
- Répond de manière TRÈS CONCISE (1-3 lignes sauf si complexe)
- Pas de pincettes, vérités brutes quand je demande
- Focus ACTION: 'code maintenant' plutôt que sur-expliquer
- M'explique les outils de debug au fur et à mesure (valgrind, lldb, etc.)
- Assume que je comprends les bases mais galère sur l'exécution
- Ne me donne pas tout le code, juste les indices/directions
- Me pousse à coder au lieu de procrastiner
- Ton direct, pas condescendant, comme un pote qui code

Style: Pragmatique, efficace, pas de blabla inutile."
*/

// ============================================================
0 0 0 0 0 1 2 3 4 5 6 7 7 7 7 7 6 5 4 3 2 1 0 0 0 0 0
0 0 0 0 0 1 2 3 4 5 6 7 8,0xFFFFFF 8,0xFFFFFF 8,0xFFFFFF 7 6 5 4 3 2 1 0 0 0 0 0
0 0 0 0 0 1 2 3 4 5 6 7 8,0xFFFFFF 8,0xFFFFFF 8,0xFFFFFF 7 6 5 4 3 2 1 0 0 0 0 0
0 0 0 0 0 1 2 3 4 5 6 7 8,0xFFFFFF 8,0xFFFFFF 8,0xFFFFFF 7 6 5 4 3 2 1 0 0 0 0 0
0 0 0 0 0 1 2 3 4 5 6 7 8,0xFFFFFF 8,0xFFFFFF 8,0xFFFFFF 7 6 5 4 3 2 1 0 0 0 0 0
0 0 0 0 0 1 2 3 4 5 6 7 8,0xFFFFFF 9,0xFFFFFF 8,0xFFFFFF 7 6 5 4 3 2 1 0 0 0 0 0
📖 COURS COMPLET - Les 3 Piliers de FDF
1️⃣ LA MINILIBX - Ta bibliothèque graphique
C est quoi ?
MinilibX = une bibliothèque simple pour afficher des pixels, créer des fenêtres, gérer la souris/clavier.

Les fonctions essentielles:

// 1. INITIALISATION
void *mlx = mlx_init();                          // Démarre la connexion graphique
void *win = mlx_new_window(mlx, 800, 600, "FDF"); // Crée fenêtre 800x600

// 2. AFFICHER UN PIXEL
mlx_pixel_put(mlx, win, x, y, color);            // Dessine 1 pixel à (x,y)
// color = 0xRRGGBB (hex) ex: 0xFF0000 = rouge

// 3. GÉRER LES ÉVÉNEMENTS
int key_hook(int keycode, void *param) {
    if (keycode == 65307) // ESC
        exit(0);
    return (0);
}
mlx_key_hook(win, key_hook, NULL);               // Détecte les touches

// 4. BOUCLE INFINIE (garde la fenêtre ouverte)
mlx_loop(mlx);                                   // Attend les événements

Exemple minimal qui fonctionne:
#include "minilibx-linux/mlx.h"
#include <stdlib.h>

int main(void)
{
    void *mlx = mlx_init();
    void *win = mlx_new_window(mlx, 800, 600, "Ma fenêtre");

    // Dessine une ligne rouge horizontale
    for (int i = 100; i < 700; i++)
        mlx_pixel_put(mlx, win, i, 300, 0xFF0000);

    mlx_loop(mlx); // Garde la fenêtre ouverte
    return (0);
}

2️⃣ LE FORMAT .FDF - Comprendre les nombres
Qu est-ce que représentent ces nombres ?
0  0  0  0  0
0 10 10 10  0
0 10 20 10  0
0 10 10 10  0
0  0  0  0  0

Chaque nombre = une HAUTEUR (altitude Z)

Visualise ça comme une carte topographique vue du dessus:
       x →
    0  1  2  3  4
y 0 [0  0  0  0  0]  ← ligne 0 (au sol, altitude 0)
↓ 1 [0 10 10 10  0]  ← ligne 1 (murs qui montent à 10)
  2 [0 10 20 10  0]  ← ligne 2 (sommet à 20 au centre!)
  3 [0 10 10 10  0]  ← ligne 3 (murs qui montent à 10)
  4 [0  0  0  0  0]  ← ligne 4 (au sol)

  En 3D, ça donne une pyramide:

Les bords (0) sont au sol
Les murs (10) montent
Le sommet (20) est le plus haut

Autre exemple:
0 0 0
0 5 0
0 0 0

3️⃣ LE PARSING - Transformer le fichier en structure
Étapes du parsing:

A) Lire le fichier ligne par ligne
int fd = open("test_maps/42.fdf", O_RDONLY);
char *line;
while ((line = get_next_line(fd)) != NULL)
{
    // Traiter chaque ligne
}
close(fd);

B) Découper chaque ligne en nombres
// Ligne: "0 10 20 10 0"
char **split = ft_split(line, ' '); // ["0", "10", "20", "10", "0"]

C) Convertir en entiers et stocker
typedef struct s_point {
    int x;      // Position x dans la grille
    int y;      // Position y dans la grille
    int z;      // HAUTEUR (le nombre du fichier)
    int color;
} t_point;

// Exemple pour le point central de ta pyramide:
t_point point;
point.x = 2;      // Colonne 2
point.y = 2;      // Ligne 2
point.z = 20;     // Hauteur 20 (lu dans le fichier)
point.color = 0xFFFFFF;

D) Structure complète
typedef struct s_fdf {
    t_point **map;  // Tableau 2D de points
    int width;      // 5 (nb colonnes)
    int height;     // 5 (nb lignes)
    void *mlx;
    void *win;
} t_fdf;

// map[0][0] = point (x=0, y=0, z=0)
// map[2][2] = point (x=2, y=2, z=20) <- sommet pyramide!
// map[4][4] = point (x=4, y=4, z=0)

4️⃣ LA PROJECTION ISOMÉTRIQUE - Passer de 3D à 2D
Le problème:
Tu as des points 3D (x, y, z) mais l écran est 2D !

La solution: projection isométrique
C est une formule mathématique qui "aplatit" la 3D en 2D tout en gardant
l impression de profondeur.

Visualisation:
Vue du DESSUS (2D):          Vue ISOMÉTRIQUE (2D mais avec illusion 3D):

  * - * - *                         *
  |   |   |                        / \
  * - * - *            →          *   *
  |   |   |                      / \ / \
  * - * - *                     *   *   *
                               / \ / \ / \
                              *   *   *   *
La formule magique:
// Pour transformer un point 3D (x, y, z) en 2D (x_iso, y_iso):

// Étape 1: Appliquer la rotation isométrique
float x_iso = (x - y) * cos(0.523599);  // 0.523599 radians = 30°
float y_iso = (x + y) * sin(0.523599) - z;

// Étape 2: Zoomer (scale) et centrer
x_iso = x_iso * zoom + offset_x;
y_iso = y_iso * zoom + offset_y;

Pourquoi cette formule ?
cos(30°) ≈ 0.866 et sin(30°) = 0.5

(x - y) : crée langle entre x et y
* cos(30°) : incline vers la droite
(x + y) * sin(30°) : donne la profondeur
- z : la hauteur "soulève" le point vers le haut

Exemple concret:
// Ton point au centre de la pyramide:
t_point p = {2, 2, 20};  // x=2, y=2, z=20

// Projection iso:
float x_iso = (2 - 2) * 0.866 = 0
float y_iso = (2 + 2) * 0.5 - 20 = 2 - 20 = -18

// Avec zoom = 20 et centre = (400, 300):
int screen_x = 0 * 20 + 400 = 400
int screen_y = -18 * 20 + 300 = 300 - 360 = -60  (au-dessus du centre!)

Le point à z=20 apparaît en haut de l écran ! ⬆️


🎯 RÉSUMÉ - Comment tout connecter
1. PARSER:
   fichier "0 10 20" → map[y][x].z = 20

2. POUR CHAQUE POINT:
   (x, y, z) → projection_iso() → (screen_x, screen_y)

3. DESSINER:
   Relier chaque point à ses voisins avec draw_line()
   - Point(x, y) connecté à Point(x+1, y)  [droite]
   - Point(x, y) connecté à Point(x, y+1)  [bas]

// gcc srcs/*.c -Lminilibx-linux -lmlx -lXext -lX11 -lm -Iminilibx-linux -o fdf
