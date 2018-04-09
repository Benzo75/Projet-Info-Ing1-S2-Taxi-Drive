#include <allegro.h>
#include <math.h>
#include <stdio.h>


#define NIMAGE_OBJET 16
#define SIZE_X 10
#define SIZE_Y 10
#define SIZE_Z 3
#define ECHELLE 16

BITMAP *tabImageObjet[NIMAGE_OBJET];
BITMAP *page;
void lancerToutAllegro(int largeur, int hauteur);
BITMAP *chargerImage(char *nomFichierImage);
void projection(int *xe, int *ye, int x, int y, int z);
void chargerScene3D(unsigned char mat[SIZE_Z][SIZE_Y][SIZE_X], char *nomFichier);
void afficherScene3D(unsigned char mat[SIZE_Z][SIZE_Y][SIZE_X]);


int main()
{
    int fin=0;
    BITMAP *image;
    BITMAP *image2;
    BITMAP *image3;
    lancerToutAllegro(800,600);

    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    image=chargerImage("menu.bmp");
    image2=chargerImage("menu_2.bmp");
    image3=chargerImage("menu_3.bmp");

    while (!fin)
    {
        clear_bitmap(page);
        /// de (350,300) a (400,300) : jouer
        /// de (350,340) a (400,340) : quitter
        masked_blit(image,screen,0,0,150,100,SCREEN_W,SCREEN_H);
        if ((mouse_x>=350)&&(mouse_x<=400)&&((mouse_y>=295)&&(mouse_y<=310)))
        {
            blit(image2,screen,0,0,150,100,SCREEN_W,SCREEN_H);
            rest(200);
        }
        if (((mouse_x>=350)&&(mouse_x<=405))&&((mouse_y>=330)&&(mouse_y<=345)))
        {
            blit(image3,screen,0,0,150,100,SCREEN_W,SCREEN_H);
            rest(200);
        }
        if ((mouse_b&1)&&(mouse_x>=350)&&(mouse_x<=400)&&((mouse_y>=295)&&(mouse_y<=310)))
        {

            clear_bitmap(screen);
            unsigned char mat[SIZE_Z][SIZE_Y][SIZE_X];
            int xroi=9,yroi=0,zroi=2;
            tabImageObjet[0]=chargerImage("images/iso_cube1.bmp");
            tabImageObjet[1]=chargerImage("images/iso_cube2.bmp");
            tabImageObjet[2]=chargerImage("images/iso_cube3.bmp");
            tabImageObjet[3]=chargerImage("images/iso_cube4.bmp");
            tabImageObjet[4]=chargerImage("images/iso_cube5.bmp");
            tabImageObjet[5]=chargerImage("images/iso_cube6.bmp");
            tabImageObjet[6]=chargerImage("images/iso_cube7.bmp");
            tabImageObjet[7]=chargerImage("images/iso_cube8.bmp");
            tabImageObjet[8]=chargerImage("images/iso_cube9.bmp");
            tabImageObjet[9]=chargerImage("images/iso_cube10.bmp");
            tabImageObjet[10]=chargerImage("images/iso_cube20.bmp");
            tabImageObjet[11]=chargerImage("images/car_s.bmp");
            tabImageObjet[12]=chargerImage("images/car_d.bmp");
            tabImageObjet[13]=chargerImage("images/car_n.bmp");
            tabImageObjet[14]=chargerImage("images/car_g.bmp");
            tabImageObjet[15]=tabImageObjet[11];
            chargerScene3D(mat, "donnees/scene3D_1.txt");
            while (!key[KEY_ESC])
            {
                mat[zroi][yroi][xroi]=0;
                if (key[KEY_LEFT] && xroi>0 && mat[zroi][yroi][xroi-1]==0)
                {
                    tabImageObjet[15]=tabImageObjet[14];

                    xroi--;
                }
                if (key[KEY_RIGHT] && xroi<SIZE_X-1 && mat[zroi][yroi][xroi+1]==0)
                {
                    tabImageObjet[15]=tabImageObjet[12];

                    xroi++;
                }
                if (key[KEY_UP] && yroi>0 && mat[zroi][yroi-1][xroi]==0)
                {
                    tabImageObjet[15]=tabImageObjet[13];

                    yroi--;
                }
                if (key[KEY_DOWN] && yroi<SIZE_Y-1 && mat[zroi][yroi+1][xroi]==0)
                {
                    tabImageObjet[15]=tabImageObjet[11];
                    yroi++;
                }
                if (key[KEY_SPACE] && (mat[zroi][yroi+1][xroi]!=0)&&(mat[zroi+1][yroi+1][xroi]==0))
                {
                    zroi++;
                    yroi++;
                }
                if (key[KEY_ENTER] && (mat[zroi-1][yroi][xroi]==9))
                    mat[zroi-1][yroi][xroi]=10;
                // Le roi tombe ?
                while (zroi>0 && mat[zroi-1][yroi][xroi]==0) zroi--;
                // Placement du roi dans la matrice à sa nouvelle position
                // (si il n'y a pas eu déplacement il réapparait au même endroit)
                mat[zroi][yroi][xroi]=16;

                clear_bitmap(page);
                afficherScene3D(mat);
                textprintf_ex(page,font,378,30,makecol(150,200,240),makecol(0,0,0),"NIVEAU 1");
                blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);

                /// une pause pour temporiser l'enchaînement des images
                /// comme les déplacements se font case par case il faut ralentir
                /// mais ce n'est PAS une bonne solution de ralentir à ce niveau
                /// il faut réfléchir à des déplacements plus progressifs avec une pause globale plus courte
                rest(200);        // ( 200 ms -> à peu près 5 images/s )
            }
            fin=1;

        }
        if ((mouse_b&1)&&((mouse_x>=350)&&(mouse_x<=405))&&((mouse_y>=330)&&(mouse_y<=345)))
        {
            fin=1;
        }
    }
    return 0;
}
END_OF_MAIN()

void projection(int *pxe, int *pye, int x, int y, int z)
{
    *pxe = 2*x - 2*y + SCREEN_W/2;
    *pye = x + y - 2*z + SCREEN_H/3;
}
void afficherScene3D(unsigned char mat[SIZE_Z][SIZE_Y][SIZE_X])
{
    int x, y, z;
    int xe, ye;
    BITMAP *img;
    for (y=0; y<SIZE_Y; y++)
        for (x=0; x<SIZE_X; x++)
            for (z=0; z<SIZE_Z; z++)
                if ( mat[z][y][x]!=0 )
                {
                    projection(&xe, &ye, ECHELLE*x, ECHELLE*y, ECHELLE*z);
                    img= tabImageObjet[ mat[z][y][x]-1 ];
                    draw_sprite(page, img, xe-img->w/2, ye-img->h);
                }
}

void chargerScene3D(unsigned char mat[SIZE_Z][SIZE_Y][SIZE_X], char *nomFichier)
{
    FILE *fp;
    int x, y, z, val;
    fp=fopen(nomFichier,"r");
    if (fp==NULL)
    {
        allegro_message("Catastrophe, j'ai un besoin vital du fichier %s\nmais je n'arrive pas a le trouver ou a l'ouvrir", nomFichier);
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    for (z=0; z<SIZE_Z; z++)
        for (y=0; y<SIZE_Y; y++)
            for (x=0; x<SIZE_X; x++)
            {
                fscanf(fp,"%d",&val);
                mat[z][y][x]=val;
            }

    fclose(fp);
}



///  ------------  AUXILIAIRES ------------------

// Regroupe les initialisations d'allegro, mode graphique, clavier, souris
void lancerToutAllegro(int largeur, int hauteur)
{
    allegro_init();
    install_keyboard();
    install_mouse();

    /// OUVERTURE MODE GRAPHIQUE (ouverture fenêtre allegro)
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,largeur,hauteur,0,0)!=0)
    {
        allegro_message("probleme mode graphique : %s", allegro_error);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen); // Affiche pointeur de souris en mode allegro
}

// Fonctionne comme load_bitmap sauf qu'il ne prend pas NULL en 2ème paramètre
// et qu'il termine le programme (avec message) en cas d'échec
//    Exemple de fonction wrapper (emballage d'une fct par une autre fct)
BITMAP *chargerImage(char *nomFichierImage)
{
    BITMAP *bmp;
    bmp=load_bitmap(nomFichierImage,NULL);
    if (bmp==NULL)
    {
        allegro_message("pas pu trouver/charger %s",nomFichierImage);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    printf("Image charg\202e : %s\n", nomFichierImage);
    return bmp;
}
