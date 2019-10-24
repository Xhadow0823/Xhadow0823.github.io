/*  TETRIS ver1.0  */
#include<iostream>
#include<conio.h>
#include<cstdlib>
#include"utilwin32.h"
#include<cstdlib>
#include<time.h>

//field's attributes
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIELD_ORI_X 13
#define FIELD_ORI_Y 2

//cell-state
#define BOUNDARY -2
#define FULL 1
#define EMPTY 0

enum _BLOCK{
    //BLOCK_TYPES
    BLOCK_TYPE_1,  //0
    BLOCK_TYPE_2,
    BLOCK_TYPE_3,
    BLOCK_TYPE_4,
    BLOCK_TYPE_5,
    BLOCK_TYPE_6,
    BLOCK_TYPE_7
};
const int BLOCK[7][4][4] = {{{1, 1, 0}, //BLOCK_TYPE_1
                             {1, 1, 0},
                             {0, 0, 0},
                             {0, 0, 0}},

                            {{1, 0, 0}, //BLOCK_TYPE_2
                             {1, 0, 0},
                             {1, 0, 0},
                             {1, 0, 0}},

                            {{1, 1, 1}, //BLOCK_TYPE_3
                             {0, 1, 0},
                             {0, 0, 0},
                             {0, 0, 0}},

                            {{0, 1, 0}, //BLOCK_TYPE_4
                             {0, 1, 0},
                             {1, 1, 0},
                             {0, 0, 0}},

                            {{1, 1, 0}, //BLOCK_TYPE_5
                             {0, 1, 1},
                             {0, 0, 0},
                             {0, 0, 0}},

                            {{0, 1, 1}, //BLOCK_TYPE_6
                             {1, 1, 0},
                             {0, 0, 0},
                             {0, 0, 0}},

                            {{1, 0, 0}, //BLOCK_TYPE_7
                             {1, 0, 0},
                             {1, 1, 0},
                             {0, 0, 0}}};

class _field{
  private:
    int data[FIELD_HEIGHT + 2][FIELD_WIDTH + 2];
    int old_x, old_y, old_type, old_direction;
    int score, level, lines;
    bool fail;

    void eliminatenshift(int y){  //PRIVATE
        gotofieldxy(1, y);
        for (int x = 1; x < FIELD_WIDTH; x++){
            std::cout << "¡¼";
            data[y][x] = EMPTY;
            delay(10);
        }
        //SHIFT
        for (int j = y; j > 1; j--){
            for (int i = 1; i <= FIELD_WIDTH; i++){
                data[j][i] = data[j - 1][i];
            }
        }
        refresh();
    }
    void printnextbox(void){  //PRIVATE
        //ùÝ ùß ùã ùå ùà ùâ ùÞ ùä ùá ¢¤ ùø
        for (int i = 0; i < 4; i++){
            gotoxy(1, 2 + i);
            std::cout << "ùø";
            gotoxy(9, 2 + i);
            std::cout << "ùø";
        }
        gotoxy(1, 1);
        std::cout << "ùÝ¢¤ NEXT¢¤ùß";
        gotoxy(1, 6);
        std::cout << "ùã¢¤¢¤¢¤¢¤¢¤¢¤¢¤ùå";
    }
    void printstatebox(void){  //PRIVATE
        gotoxy(1, 7);
        std::cout << "ùÝ¢¤¢¤¢¤¢¤¢¤¢¤¢¤ùß";
        gotoxy(1, FIELD_ORI_Y + FIELD_HEIGHT);
        std::cout << "ùã¢¤¢¤¢¤¢¤¢¤¢¤¢¤ùå";

        for (int i = 0; i < FIELD_ORI_Y + FIELD_HEIGHT - 8; i++){
            gotoxy(1, 8 + i);
            std::cout << "ùø";
            gotoxy(9, 8 + i);
            std::cout << "ùø";
        }

        gotoxy(3,  8);  std::cout << "SCORE";
        gotoxy(3, 11);  std::cout << "LEVEL";
        gotoxy(3, 14);  std::cout << "LINES";
    }
    void gotofieldxy(int x,int y){  //PRIVATE
        if(x>FIELD_WIDTH || y>FIELD_HEIGHT || x<1 || y<1)
            return;
        gotoxy(FIELD_ORI_X + (x - 1) * 2, FIELD_ORI_Y + (y - 1));
    }
    void removeoldblock(void){  //PRIVATE
        int rotatedblock[4][4] = {{0}};
        if(old_x == 0 || old_y == 0)  //first time
            return;
        if(old_direction == 0){
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    gotofieldxy(old_x + i, old_y + j);
                    if(BLOCK[old_type][j][i])
                        std::cout << "¡¼";
                }
            }
        }
        else{
            changedirect(old_type, rotatedblock, old_direction);
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    gotofieldxy(old_x + i, old_y + j);
                    if(rotatedblock[j][i])
                        std::cout << "¡¼";
                }
            }
        }
    }
    void changedirect(int type,int changed[4][4],int direction){  //PRIVATE
        int tmp[4][4] = {{0}};
        int shift_left=0, shift_up=0;
        if(direction == 0)
            return ;

        switch(type){
            case 0:
                for (int j = 0; j < 4; j++){
                    for (int i = 0; i < 4; i++){
                        changed[j][i] = BLOCK[0][j][i];
                    }
                }
                return;
                break;
            case 1:
                if(direction==2){
                    for (int j = 0; j < 4; j++){
                        for (int i = 0; i < 4; i++){
                            changed[j][i] = BLOCK[1][j][i];
                        }
                    }
                    return;
                }
                if(direction==3){
                    direction = 1;
                }
                break;
            case 2:
            case 4:
            case 5:
                switch (direction) {
                    case 1:
                        shift_left = 2;
                        break;
                    case 2:
                        shift_left = 1;
                        shift_up = 2;
                        break;
                    case 3:
                        shift_up = 1;
                        break;
                }
                break;
            case 3:
            case 6:
                switch(direction){
                    case 1:
                        shift_left = 1;
                        break;
                    case 2:
                        shift_left = 2;
                        shift_up = 1;
                        break;
                    case 3:
                        shift_up = 2;
                        break;
                }
                break;
            default:
                return;
        }
        //ROTATE
        for (int j = 0; j < 4; j++){
            for (int i = 0; i < 4; i++){
                tmp[j][i] = BLOCK[type][j][i];
            }
        }
        for (int i = 0; i < direction; i++){
            for (int y = 0; y < 4; y++){
                for (int x = 0; x < 4; x++){
                    changed[y][x] = tmp[3 - x][y];
                }
            }
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    tmp[j][i] = changed[j][i];
                }
            }
        }
        //SHIFT
        for (int t = 0; t < shift_left; t++){
            for (int i = 0; i < 3; i++){
                for (int j = 0; j < 4; j++){
                    changed[j][i] = changed[j][i + 1];
                }
            }
            for (int i = 0; i < 4; i++){
                changed[i][3] = 0;
            }
        }
        for (int t = 0; t < shift_up; t++){
            for (int j = 0; j < 3; j++){
                for (int i = 0; i < 4; i++){
                    changed[j][i] = changed[j+1][i];
                }
            }
            for (int i = 0; i < 4; i++){
                changed[3][i] = 0;
            }
        }
    }

  public:  //=============================================================
    _field(){  //CONSTRUCTOR
        //-----initialize field data-----
        for (int i = 0; i < FIELD_WIDTH+2 ; i++){
            data[0][i] = BOUNDARY;
            data[FIELD_HEIGHT+1][i] = BOUNDARY;
        }
        for (int i = 1; i < FIELD_HEIGHT+1 ; i++){
            data[i][0] = BOUNDARY;
            data[i][FIELD_WIDTH+1] = BOUNDARY;
        }
        for (int i = 1; i<=FIELD_WIDTH ; i++){
            for (int j = 1; j <= FIELD_HEIGHT; j++){
                data[j][i] = EMPTY;
            }
        }
        old_x = old_y = old_type = old_direction = 0;
        score = lines = 0;  level = 1;
        //-----FLAG-----
        fail = false;
    }
    void initialize(void){  //PRIVATE
        for (int i = 0; i < FIELD_WIDTH+2 ; i++){
            data[0][i] = BOUNDARY;
            data[FIELD_HEIGHT+1][i] = BOUNDARY;
        }
        for (int i = 1; i < FIELD_HEIGHT+1 ; i++){
            data[i][0] = BOUNDARY;
            data[i][FIELD_WIDTH+1] = BOUNDARY;
        }
        for (int i = 1; i<=FIELD_WIDTH ; i++){
            for (int j = 1; j <= FIELD_HEIGHT; j++){
                data[j][i] = EMPTY;
            }
        }
        old_x = old_y = old_type = old_direction = 0;
        score = lines = 0;  level = 1;
    }
    void printfield(void){
        //ùÝ ùß ùã ùå ùà ùâ ùÞ ùä ùá ¢¤ ùø
        gotoxy(FIELD_ORI_X , FIELD_ORI_Y - 1);
        for (int i = 0; i < FIELD_WIDTH ;i++){
            std::cout << "¢¤¢¤";
        }
        gotoxy(FIELD_ORI_X , FIELD_ORI_Y + FIELD_HEIGHT );
        for (int i = 0; i < FIELD_WIDTH ;i++){
            std::cout << "¢¤¢¤";
        }
        
        for (int i = 0; i < FIELD_HEIGHT ;i++){
            gotoxy(FIELD_ORI_X - 2, FIELD_ORI_Y + i );
            std::cout << "ùø";
            gotoxy(FIELD_ORI_X + FIELD_WIDTH*2, FIELD_ORI_Y + i );
            std::cout << "ùø";
        }

        gotoxy(FIELD_ORI_X - 2, FIELD_ORI_Y - 1);
        std::cout << "ùÝ¢¤";
        gotoxy(FIELD_ORI_X + FIELD_WIDTH*2 , FIELD_ORI_Y - 1);
        std::cout << "ùß";
        gotoxy(FIELD_ORI_X - 2, FIELD_ORI_Y + FIELD_HEIGHT);
        std::cout << "ùã¢¤";
        gotoxy(FIELD_ORI_X + FIELD_WIDTH*2 , FIELD_ORI_Y + FIELD_HEIGHT);
        std::cout << "ùå";

        for (int j = 0; j < FIELD_HEIGHT;j++){
            gotofieldxy(1, 1+j);
            for (int i = 0; i < FIELD_WIDTH; i++){
                std::cout << "¡¼";
            }
        }
        printnextbox();
        printstatebox();
    }
    void refresh(void){
        for (int j = 1; j <= FIELD_HEIGHT; j++){
            gotofieldxy(1, j);
            for (int i = 1; i <= FIELD_WIDTH; i++){
                if(data[j][i]==FULL){
                    std::cout << "¡½";
                }
                else{
                    std::cout << "¡¼";
                }
            }
        }
        showstate();
    }
    void checkneliminate(void){
        for (int j = FIELD_HEIGHT; j >= 1; j--){
            for (int i = 1; i <= FIELD_WIDTH; i++){
                if(data[j][i]==EMPTY){
                    break;
                }
                if(i==FIELD_WIDTH){  //eliminate
                    eliminatenshift(j);
                    addlines();
                    j++;
                }
            }
        }
        refresh();
    }
    void newblock(int type){  //¡¼UNCHECK-------------------------------
        putblock(FIELD_WIDTH / 2 - 1, 1, type, 0);
    }
    void shownextblock(const int type){
        //¡¼¡@¡½
        for (int j = 0; j < 4; j++){
            gotoxy(3, 2 + j);
            for (int i = 0; i < 3; i++){
                if(BLOCK[type][j][i])
                    std::cout << "¡½";
                else
                    std::cout << "¡@";
            }
        }
    }
    void addscore(int num){
        if(score == 999999)
            return;
        score += num;
    }
    void addlines(void){
        if(lines == 999999)
            return;
        lines++;
        level = lines / 10 + 1;
    }
    void showstate(void){
        gotoxy(3,  9);  std::cout << score;
        gotoxy(3, 12);  std::cout << level;
        gotoxy(3, 15);  std::cout << lines;
    }
    int getperiod(void){
        if(level>=23)
            return 10;
        else
            return 120 - (level - 1) * 5;
        /*  L1  : 120
            L2  : 115
            L3  : 110
            ...
            L23 :  10  */
    }
    void putblock(int x,int y,int type,int direction = 0){
        removeoldblock();
        old_x = x;  old_y = y;  old_type = type;  old_direction = direction;
        int rotatedblock[4][4] = {{0}};
        if(direction == 0){
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    gotofieldxy(x + i, y + j);
                    if(BLOCK[type][j][i])
                        std::cout << "¡½";
                }
            }
        }
        else{
            changedirect(type, rotatedblock, direction);
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    gotofieldxy(x + i, y + j);
                    if(rotatedblock[j][i])
                        std::cout << "¡½";
                }
            }
        }
    }
    bool thisplaceisok(int x,int y,int type,int direction){
        int rotatedblock[4][4] = {{0}};
        if(direction == 0){
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    if(BLOCK[type][j][i]){
                        if(data[y+j][x+i]!=EMPTY){
                            return false;
                        }
                    }
                }
            }
        }
        else{
            changedirect(type, rotatedblock, direction);
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    if(rotatedblock[j][i]){
                        if(data[y+j][x+i]!=EMPTY){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    void deposit(void){
        int rotatedblock[4][4] = {{0}};
        if(old_x == 0 || old_y == 0)  //first time
            return;

        if(old_direction == 0){
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    if(BLOCK[old_type][j][i]){
                        data[old_y+j][old_x+i] = FULL;
                    }
                }
            }
        }
        else{
            changedirect(old_type, rotatedblock, old_direction);
            for (int j = 0; j < 4; j++){
                for (int i = 0; i < 4; i++){
                    if(rotatedblock[j][i]){
                        data[old_y+j][old_x+i] = FULL;
                    }
                }
            }
        }
        old_x = old_y = old_type = old_direction = 0;
    }
}field;



int main (void){
    int next_type, cur_x, cur_y, cur_type, cur_dir;
    int speed = 120, time_counter, period;
    int f_key=-1, d_key=-1;
    int scoretoadd=0;

newgame:
    //srand(time(NULL));
    time_counter = 0;  scoretoadd = 0;
    field.initialize();
    field.printfield();
    next_type = rand() % 7;
    field.shownextblock(next_type);
    delay(1000);
newblock:
    time_counter = 0;  period = field.getperiod();
    cur_type = next_type;
    field.shownextblock(next_type = rand() % 7);
    field.refresh();  //refresh
    if(!field.thisplaceisok(FIELD_WIDTH / 2 - 1, 1, cur_type, 0)){
        std::cout << "¢Õ¢Ï¢Û¢Ó ¢Ý¢ä¢Ó¢à";
    }
    else{
        field.putblock(cur_x = FIELD_WIDTH / 2 - 1, cur_y = 1, cur_type, cur_dir = 0);
        delay(50);
    }
while (1){
    if (kbhit()){
        switch(f_key = getch()){
            case 27:  //ESC
                return 0;
                break;
            case 'r':
                goto newgame;
                break;
            case 's':
                goto newblock;
                break;
            case ' ':
                //TO THE BOTTOM
                scoretoadd = cur_y;
                while(field.thisplaceisok(cur_x, cur_y+1, cur_type, cur_dir)){
                    field.putblock(cur_x, ++cur_y, cur_type, cur_dir);
                    delay(15);
                }
                field.addscore(cur_y - scoretoadd);
                field.deposit();
                field.checkneliminate();
                goto newblock;
                break;
            case 0xe0:
            case 0x00:
                switch(d_key = getch()){
                    case 72:  //UP
                        if (field.thisplaceisok(cur_x, cur_y, cur_type, (cur_dir+1)%4)){
                            field.putblock(cur_x, ++cur_y, cur_type, ++cur_dir%=4);
                        }
                        break;
                    case 80:  //DOWN
                        if (field.thisplaceisok(cur_x, cur_y+1, cur_type, cur_dir)){
                            field.putblock(cur_x, ++cur_y, cur_type, cur_dir);
                            field.addscore(1);
                            field.showstate();
                        }
                        break;
                    case 75:  //LEFT
                        if (field.thisplaceisok(cur_x-1, cur_y, cur_type, cur_dir)){
                            field.putblock(--cur_x, cur_y, cur_type, cur_dir);
                        }
                        break;
                    case 77:  //RIGHT
                        if (field.thisplaceisok(cur_x+1, cur_y, cur_type, cur_dir)){
                            field.putblock(++cur_x, cur_y, cur_type, cur_dir);
                        }
                        break;
                }
                break;
        }
    }
    delay(period);  time_counter += 1;
    if(time_counter == 10){
        time_counter = 0;
        period = field.getperiod();
        if (field.thisplaceisok(cur_x, cur_y + 1, cur_type, cur_dir)){
            field.putblock(cur_x, ++cur_y, cur_type, cur_dir);
        }
        else{
            field.deposit();
            field.checkneliminate();
            goto newblock;
        }
    }
}

    return 0;
}
