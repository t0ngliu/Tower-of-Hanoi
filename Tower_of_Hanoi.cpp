#include <allegro.h>
#include "Stack.hpp"

using namespace std;

/*---------------------------- INITIAL GLOBAL DECLARATIONS ----------------------*/
Stack tower[3];
int block;       //number of rings on the pillars
int pause;          //milliseconds to pause between moves, so it's easier for the user to follow

//harder to reset if not global variable
int moves;          //numbers of moves
int recursiveCalls;          //number of recursive recursive calls

BITMAP *buffer;

/*----------------------------------GRAPHICS RELATED----------------------------*/
//prints the rings
void printRings(int x, int y, int w, int colour) {
    static BITMAP *yellow = load_bitmap("img_tower/Ring1.bmp", NULL);
    static BITMAP *red = load_bitmap("img_tower/Ring2.bmp", NULL);
    if (colour) {
        stretch_sprite(buffer, yellow, x, y, w, 30);
    }
    else {
        stretch_sprite(buffer, red, x, y, w, 30);
    }
}
END_OF_FUNCTION(printRings)

//prints the base & pillars    
void drawPillars() {
    static BITMAP *base = load_bitmap("img_tower/Base.bmp", NULL);  
    static BITMAP *pillar = load_bitmap("img_towerPillar.bmp", NULL);

    draw_sprite(buffer, pillar, 240, 175); 
    draw_sprite(buffer, pillar, 490, 175);
    draw_sprite(buffer, pillar, 740, 175);
    draw_sprite(buffer, base, 100, 550);
}
END_OF_FUNCTION(drawPillars)

//redraws the screen
void redrawScreen() {
    static BITMAP *background = load_bitmap("img_menu/Background.bmp", NULL);
    blit(background, buffer, 0, 0, 0, 0, 1000, 650);
    drawPillars();
    //for each pillar
    for (int i = 0; i < 3; i++) {
        //for each ring
        for (int j = 0; j < tower[i].getSize(); j++) {
            //changes the ring sizes accordingly 
            int ringLength = 200 - 100 / block * tower[i].get(j);
            //prints the ring
            printRings(250 + 250 * i - ringLength / 2, 
                       550 - 30 * (tower[i].getSize() - j), 
                       ringLength, tower[i].get(j) % 2);
        }
    }
    
    moves++;  //increments the number of moves counter
    
    textprintf_ex(buffer, font, 470, 595, makecol(255, 255, 0), -1, "Moves: %d", moves); 
    textprintf_ex(buffer, font, 440, 605, makecol(255, 255, 0), -1, "Recursive Calls: %d", recursiveCalls); 
    blit(buffer, screen, 0, 0, 0, 0, 1000, 650);
}
END_OF_FUNCTION(redrawScreen)

//fills the pillars with rings
void initrings(int rings) {
    for (int i = 0; i < rings; i++) {
        tower[0].push(i);
    }
}
END_OF_FUNCTION(initrings)

/*----------------------------------SOLVING CODE----------------------------*/
//moves the rings to solve the puzzle
void moveRings(int num, int from, int to) {
    recursiveCalls++;
    if (num == 1) {
        tower[to].push(tower[from].pop());
        redrawScreen();
        rest(pause);
    }
    else {
        moveRings(num - 1, from, 3 - to - from);
        tower[to].push(tower[from].pop());
        redrawScreen();
        rest(pause);
        moveRings(num - 1, 3 - to - from, to);
    }
}
END_OF_FUNCTION(moveRings)

/*----------------------------------MOUSE INPUT----------------------------*/
//checks if the mouse is in a box defined with diagonals at (x1, y1) and (x2, y2)
bool mouse_in(int x1, int y1, int x2, int y2) {
    return (mouse_x > x1 && mouse_x < x2 && mouse_y > y1 && mouse_y < y2) ? true : false;
}
END_OF_FUNCTION(mouse_in)

/*----------------------------------MAIN-----------------------------------*/
int main(int argc, char *argv[]) {  
    /*-------------------- ALLEGRO RELATED INITIALIZATION --------------------*/
    allegro_init();
    install_keyboard();
    install_mouse();
    enable_hardware_cursor();
    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1000, 650, 0, 0);
    show_mouse(screen);
    
    /*---------------------------- BITMAP LOADING ----------------------------*/
    buffer = create_bitmap(1000, 650);
    
    BITMAP *mainMenu = load_bitmap("img_menu/MainMenu.bmp", NULL);
    
    BITMAP *speed[3];
    speed[0] = load_bitmap("img_menu/Slow.bmp", NULL);
    speed[1] = load_bitmap("img_menu/Medium.bmp", NULL);
    speed[2] = load_bitmap("img_menu/Fast.bmp", NULL);
    
    BITMAP *ring[8];
    ring[0] = load_bitmap("img_menu/1.bmp", NULL);
    ring[1] = load_bitmap("img_menu/2.bmp", NULL);
    ring[2] = load_bitmap("img_menu/3.bmp", NULL);
    ring[3] = load_bitmap("img_menu/4.bmp", NULL);
    ring[4] = load_bitmap("img_menu/5.bmp", NULL);
    ring[5] = load_bitmap("img_menu/6.bmp", NULL);
    ring[6] = load_bitmap("img_menu/7.bmp", NULL);
    ring[7] = load_bitmap("img_menu/8.bmp", NULL);
    /*-----------------------------------------------------------------------*/
    
    char again;
    
    do {
        //initial settings
        blit(mainMenu, screen, 0, 0, 0, 0, 1000, 650);
        pause = 700;
        draw_sprite(screen, speed[0], 397, 437);
        block = 1;
        draw_sprite(screen, ring[0], 55, 295);
        
        recursiveCalls = 0;
        moves = -1; //the redrawScreen function is called once before the first moveRings
        
        //until the user pushes go button in the main menu
        while (!(mouse_b&1 && mouse_in(389, 537, 654, 626))) {
            //if mouse clicked
            if (mouse_b&1) {
                //checks each of the speed buttons
                for (int i = 0; i < 3; i++) {
                    //int x1, int y1, int x2, int y2
                    if (mouse_in(397 + 193 * i, 437, 566 + 193 * i, 497)) {
                        pause = (2 - i) * 300 + 100;
                        //redraws the screen
                        blit(mainMenu, buffer, 0, 0, 0, 0, 1000, 650);
                        draw_sprite(buffer, speed[i], 397 + 193 * i, 437);
                        draw_sprite(buffer, ring[block - 1], 55 + 114 * (block - 1), 295);
                        blit(buffer, screen, 0, 0, 0, 0, 1000, 650);
                    }
                } 
                //checks each of the rings buttons
                for (int i = 0; i < 8; i++) {
                    //int x1, int y1, int x2, int y2
                    if (mouse_in(55 + 115 * i, 301, 126 + 115 * i, 373)) {
                        block = i + 1;
                        //redraws the screen
                        blit(mainMenu, buffer, 0, 0, 0, 0, 1000, 650);
                        draw_sprite(buffer, ring[i], 55 + 114 * i, 295);
                        draw_sprite(buffer, speed[2 - (pause - 100)/ 300], 397 + 193 * (2 - (pause - 100) / 300), 437);
                        blit(buffer, screen, 0, 0, 0, 0, 1000, 650);
                    }
                }
                //checks if the user clicks the the quit button
                if (mouse_in(695, 539, 961, 625)) return 0;
            }
        }
        
        initrings(block);     //pushes the rings into the stack
        redrawScreen();              //prints the rings
        rest(pause);
        moveRings(block, 0, 2);     //solves the puzzle
    
        textout_ex(screen, font, "Push esc key to exit, or any other key to return to main menu", 250, 625, makecol(255, 255, 0), -1);
        again = readkey();       // waits for user to press key after it solves puzzle
        tower[2].emptyList();
    } while (again != 27);       //while escape key is not pushed
    
    destroy_bitmap(speed[0]);
    destroy_bitmap(speed[1]);
    destroy_bitmap(speed[2]);
    destroy_bitmap(ring[0]);
    destroy_bitmap(ring[1]);
    destroy_bitmap(ring[2]);
    destroy_bitmap(ring[3]);
    destroy_bitmap(ring[4]);
    destroy_bitmap(ring[5]);
    destroy_bitmap(ring[6]);
    destroy_bitmap(ring[7]);
    
    return 0;
}
END_OF_MAIN()
