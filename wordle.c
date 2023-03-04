// Nita Eduard-Eugen, 315CC - Problema 3, Wordle
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// functia principala de rulare a jocului
int rulare(int joc) {
    while (joc == 1) {
        // initiere ecran
        clear();
        initscr();

        // initiere utilizare culori
        start_color();

        // definire perechi de culori
        init_pair(1, COLOR_BLACK, COLOR_GREEN);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);

        // determinare dimensiuni ecran
        int x, y;
        getmaxyx(stdscr, x, y);
        move(0, y / 2);
        printw("Wordle\n");
        refresh();

        int h = 5, w = 10, sty = 5, stx = x / 2, i, j;
        int pozx[6][6], pozy[6][6];

        // creare tabal de joc
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 5; j++) {
                WINDOW* win = newwin(h, w, sty, stx);
                refresh();
                box(win, 0, 0);
                wrefresh(win);
                // memorare pozitii
                pozx[i][j] = sty + h / 2;
                pozy[i][j] = stx + w / 2;
                stx = stx + 10;
            }
            stx = x / 2;
            sty = sty + 5;
        }
        // mutare cursor
        move(pozx[0][0], pozy[0][0]);
        i = 0;
        j = 0;
        int c = 0, event = 0, guess_nr, k, l, ap, final = 0;
        char temp;
        char cuvant[5];
        curs_set(0);
        noecho();
        // definire dictionar
        char dict[29][6] = {
            "arici", "atent", "baiat", "ceata", "debut", "peste", "fixat",
            "hamac", "harta", "jalon", "jucam", "lacat", "magie", "nufar",
            "oaste", "perus", "rigle", "roman", "sanie", "scris", "sonda",
            "texte", "tipar", "titan", "zebra", "vapor", "vatra"};
        int ver[29][6] = {0};
        char guess[5];

        // generare cuvant aleator
        srand(time(NULL));
        guess_nr = rand() % (27 + 1 - 0);
        move(pozx[0][0], pozy[0][0]);

        // ascultare input
        while ((i != 6) && (final != 1)) {
            int menu = 0;
            if ((j >= 0) && (j < 5)) {
                temp = getch();
                // stergere cuvant
                if (temp == 127) {
                    cuvant[j] == '\0';
                    if (j > 0) {
                        j--;
                    }
                    move(pozx[i][j], pozy[i][j]);
                    printw(" ");
                    move(pozx[i][j], pozy[i][j]);

                } else {
                    // restrictii pentru a se introduce doar litere
                    if ((temp >= 65) && (temp <= 90)) {
                        temp = temp + 32;
                    }
                    // conversie in caz de litere mari
                    if ((temp >= 97) && (temp <= 122)) {
                        cuvant[j] = temp;
                        j++;
                        printw("%c", temp);
                        move(pozx[i][j], pozy[i][j]);
                    }
                    // afisare mesaj de avertizare
                    if (event == 1) {
                        move(1, 1);
                        printw("                                 ");
                        move(pozx[i][j], pozy[i][j]);
                        event = 0;
                    }
                    if (temp == '\n') {
                        WINDOW* win = newwin(10, 10, 0, 0);
                        box(win, 0, 0);
                        move(1, 1);
                        printw("Introduceti un cuvant de 5 litere");
                        event = 1;
                        move(pozx[i][j], pozy[i][j]);
                    }
                    // afisare meniu
                    if (temp == ':') {
                        menu = 1;
                        move(40, 0);
                        printw(
                            "Acesta este meniul de joc al aplicatiei\nPentru "
                            "un joc nou, apasati n\nPentru iesirea din joc, "
                            "apasati x\nPentru a va reintoarce la joc, apasati "
                            ":");
                    }
                    while (menu == 1) {
                        char verf;
                        verf = getch();
                        if (verf == ':') {
                            menu = 0;
                            move(40, 0);
                            printw(
                                "                                           \n "
                                "                            \n                "
                                "                      \n                      "
                                "                            ");
                            move(pozx[i][j], pozy[i][j]);
                        }
                        // inchidere joc
                        if (verf == 'x') {
                            joc = 0;
                            clear();
                            endwin();
                            return 1;
                        }
                        // resetare joc
                        if (verf == 'n') {
                            menu = 0;
                            move(40, 0);
                            printw(
                                "                                           \n "
                                "                            \n                "
                                "                      \n                      "
                                "                            ");
                            move(pozx[i][j], pozy[i][j]);
                            joc = 1;
                            return 0;
                        }
                    }
                }
                // efectuare colorari
            } else if (j == 5) {
                move(pozx[i][j], pozy[i][j]);
                temp = getch();
                int ver[29][6] = {0};
                if (temp == '\n') {
                    j = 0;
                    for (k = 0; k < 5; k++) {
                        // gasire litera pe pozitia respectiva
                        if (cuvant[k] == dict[guess_nr][k]) {
                            // afisare casuta verde
                            WINDOW* win = newwin(h, w, pozx[i][k] - h / 2,
                                                 pozy[i][k] - w / 2);
                            refresh();
                            wattron(win, COLOR_PAIR(1));
                            box(win, 0, 0);
                            redrawwin(win);
                            ver[guess_nr][k] = 1;
                            move(pozx[i][k], pozy[i][k]);
                            printw("%c", cuvant[k]);
                            wrefresh(win);
                        } else {
                            int ok = 1, ap = 0;
                            for (l = 0; l < 5; l++) {
                                // cautare litera in cuvant
                                if ((cuvant[k] == dict[guess_nr][l]) &&
                                    (ok == 1) && (ver[guess_nr][l] == 0)) {
                                    ok == 0;
                                    ver[guess_nr][l] = 1;
                                    int m = 0;
                                    // verificare pentru aparitii duble
                                    for (m = 0; m < 5; m++) {
                                        if (cuvant[k] == dict[guess_nr][m]) {
                                            ap++;
                                        }
                                        if ((cuvant[m] == dict[guess_nr][m]) &&
                                            (cuvant[k] == cuvant[m])) {
                                            ap--;
                                        }
                                    }
                                    // afisare casuta galbena
                                    if (ap != 0) {
                                        WINDOW* win =
                                            newwin(h, w, pozx[i][k] - h / 2,
                                                   pozy[i][k] - w / 2);
                                        refresh();
                                        wattron(win, COLOR_PAIR(2));
                                        box(win, 0, 0);
                                        redrawwin(win);
                                        move(pozx[i][k], pozy[i][k]);
                                        printw("%c", cuvant[k]);
                                        wrefresh(win);
                                    }
                                }
                            }
                        }
                    }
                    i++;
                    move(pozx[i][j], pozy[i][j]);
                } else if (temp == 127) {
                    cuvant[j] == '\0';
                    if (j > 0) {
                        j--;
                    }
                    move(pozx[i][j], pozy[i][j]);
                    printw(" ");
                    move(pozx[i][j], pozy[i][j]);
                }
                // verificare gasire cuvant final
                if (strcmp(cuvant, dict[guess_nr]) == 0) {
                    final = 1;
                }
            }
        }
        move(y / 2.5, x / 1.2);
        if (final == 1) {
            printw(
                "AI CASTIGAT!\nApasa Y pentru a reincepe jocul sau orice "
                "altceva pentru a iesi");
        } else {
            printw(
                "\nAI PIERDUT! Cuvantul era %s\nApasa Y pentru a reincepe "
                "jocul sau orice altceva pentru a iesi",
                dict[guess_nr]);
        }
        // incepere joc nou
        if (getch() == 'y') {
            joc = 1;
        } else {
            joc = 0;
        }
    }
}

int main() {
    int joc = 1, n;
    // initializare
    clear();
    n = rulare(joc);
    // verificare in caz de rejucare
    if (n == 0) {
        joc = 1;
        rulare(joc);
    }
    // inchidere ecran
    endwin();
    return 0;
}
