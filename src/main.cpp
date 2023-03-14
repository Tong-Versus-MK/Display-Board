#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Bounce2.h>

#define BoardSize 8
int green = 25;
int red = 33;
int yellow = 32;
int turn = 0;
int mode = 0;
int x = 0;
int y = 0;
int board[8];
int recv_first_item;
int MAZE[BoardSize][BoardSize] = {
  {0,1,2,0,0,0,0,2},
  {0,1,1,1,0,0,1,0},
  {0,0,0,1,0,0,1,1},
  {1,1,0,3,2,0,0,0},
  {0,0,0,2,3,0,0,0},
  {0,0,1,0,0,1,0,0},
  {0,1,1,1,0,1,1,0},
  {2,0,0,2,0,0,1,0}
};

int twocolour_0_2_Yellow = 23;
int twocolour_0_2_Red = 22;
int twocolour_0_7_Yellow = 21;
int twocolour_0_7_Red = 19;
int twocolour_3_3_Yellow = 18;
int twocolour_3_3_Red = 5;
int twocolour_3_4_Yellow = 17;
int twocolour_3_4_Red = 16;
int twocolour_4_3_Yellow = 4;
int twocolour_4_3_Red = 0;
int twocolour_4_4_Yellow = 2;
int twocolour_4_4_Red = 15;
int twocolour_7_0_Yellow = 26;
int twocolour_7_0_Red = 27;
int twocolour_7_3_Yellow = 14;
int twocolour_7_3_Red = 12;

typedef struct recv_message {
    int mode;
    int turn;
    int x;
    int y;
    int board[8];
} recv_message;

recv_message recv_mess;

String success;
esp_now_peer_info_t peerInfo;

uint8_t broadcastAddress[] = { 0x24, 0x0A, 0xC4, 0x9B, 0x8F, 0xEC };


void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
    memcpy(&recv_mess, incomingData, sizeof(recv_mess));
    // Serial.print("recv turn : ");
    // Serial.println(recv_mess.turn);
    turn = recv_mess.turn;
    mode = recv_mess.mode;
    x = recv_mess.y;
    y = recv_mess.x;

    if(turn){
        digitalWrite(green,0);          //yellow = turn MK
        digitalWrite(yellow,1);
    }
    else{
        digitalWrite(green,1);          //green = turn Tong
        digitalWrite(yellow,0);
    }

    if(mode == 2){
        recv_first_item = 0;
    }
    if(recv_first_item==0){
        for(int i=0;i<BoardSize;i++){
            board[i] = recv_mess.board[i];
        }
        for(int i=0;i<BoardSize;i++){
            Serial.print(board[i]);
            Serial.print(" ");
        }

        if(board[0] == 2){
            digitalWrite(twocolour_0_2_Red,1);
            digitalWrite(twocolour_0_2_Yellow,0);
            MAZE[0][2] = 2;
        }else if(board[0] == 3){
            MAZE[0][2] = 3;
            digitalWrite(twocolour_0_2_Yellow,1);
            digitalWrite(twocolour_0_2_Red,0);
        }

        if(board[1] == 2){
            MAZE[0][7] = 2;
            digitalWrite(twocolour_0_7_Red,1);
            digitalWrite(twocolour_0_7_Yellow,0);
        }else if(board[1] == 3){
            MAZE[0][7] = 3;
            digitalWrite(twocolour_0_7_Yellow,1);
            digitalWrite(twocolour_0_7_Red,0);
        }

        if(board[2] == 2){
            MAZE[3][3] = 2;
            digitalWrite(twocolour_3_3_Red,1);
            digitalWrite(twocolour_3_3_Yellow,0);
        }else if(board[2] == 3){
            MAZE[3][3] = 3;
            digitalWrite(twocolour_3_3_Yellow,1);
            digitalWrite(twocolour_3_3_Red,0);
        }

        if(board[3] == 2){
            MAZE[3][4] = 2;
            digitalWrite(twocolour_3_4_Red,1);
            digitalWrite(twocolour_3_4_Yellow,0);
        }else if(board[3] == 3){
            MAZE[3][4] = 3;
            digitalWrite(twocolour_3_4_Yellow,1);
            digitalWrite(twocolour_3_4_Red,0);
        }

        if(board[4] == 2){
            MAZE[4][3] = 2;
            digitalWrite(twocolour_4_3_Red,1);
            digitalWrite(twocolour_4_3_Yellow,0);
        }else if(board[4] == 3){
            MAZE[4][3] = 3;
            digitalWrite(twocolour_4_3_Yellow,1);
            digitalWrite(twocolour_4_3_Red,0);
        }

        if(board[5] == 2){
            MAZE[4][4] = 2;
            digitalWrite(twocolour_4_4_Red,1);
            digitalWrite(twocolour_4_4_Yellow,0);
        }else if(board[5] == 3){
            MAZE[4][4] = 3;
            digitalWrite(twocolour_4_4_Yellow,1);
            digitalWrite(twocolour_4_4_Red,0);
        }

        if(board[6] == 2){
            MAZE[7][0] = 2;
            digitalWrite(twocolour_7_0_Red,1);
            digitalWrite(twocolour_7_0_Yellow,0);
        }else if(board[6] == 3){
            MAZE[7][0] = 3;
            digitalWrite(twocolour_7_0_Yellow,1);
            digitalWrite(twocolour_7_0_Red,0);
        }

        if(board[7] == 2){
            MAZE[7][3] = 2;
            digitalWrite(twocolour_7_3_Red,1);
            digitalWrite(twocolour_7_3_Yellow,0);
        }else if(board[7] == 3){
            MAZE[7][3] = 3;
            digitalWrite(twocolour_7_3_Yellow,1);
            digitalWrite(twocolour_7_3_Red,0);
        }
        recv_first_item = 1;
    }

    /* Received Position from gameEngine */
    MAZE[x][y] = 0;
    
    // Serial.println();
    // Serial.print("turn : ");
    // Serial.println(turn);
    // Serial.print("mode : ");
    // Serial.println(mode);
    // Serial.print("x : ");
    // Serial.println(x);
    // Serial.print("y : ");
    // Serial.println(y);
    // Serial.println("====================");
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(yellow,OUTPUT);

  pinMode(twocolour_0_2_Yellow,OUTPUT);
  pinMode(twocolour_0_2_Red,OUTPUT);
  pinMode(twocolour_0_7_Yellow,OUTPUT);
  pinMode(twocolour_0_7_Red,OUTPUT);
  pinMode(twocolour_3_3_Yellow,OUTPUT);
  pinMode(twocolour_3_3_Red,OUTPUT);
  pinMode(twocolour_3_4_Yellow,OUTPUT);
  pinMode(twocolour_3_4_Red,OUTPUT);
  pinMode(twocolour_4_3_Yellow,OUTPUT);
  pinMode(twocolour_4_3_Red,OUTPUT);
  pinMode(twocolour_4_4_Yellow,OUTPUT);
  pinMode(twocolour_4_4_Red,OUTPUT);
  pinMode(twocolour_7_0_Yellow,OUTPUT);
  pinMode(twocolour_7_0_Red,OUTPUT);
  pinMode(twocolour_7_3_Yellow,OUTPUT);
  pinMode(twocolour_7_3_Red,OUTPUT);
}

void loop() {
    if(mode == 1){
        digitalWrite(red,1);            //red = Duel mode
    }
    else{
        digitalWrite(red,0);
    }

    //  Pos (2,0) 
    if(MAZE[0][2] == 0){
        digitalWrite(twocolour_0_2_Red,0);
        digitalWrite(twocolour_0_2_Yellow,0);
    }
    //  Pos (7,0) 
    if(MAZE[0][7] == 0){
        digitalWrite(twocolour_0_7_Red,0);
        digitalWrite(twocolour_0_7_Yellow,0);
    }
    //  Pos (3,3) 
    if(MAZE[3][3] == 0){
        digitalWrite(twocolour_3_3_Red,0);
        digitalWrite(twocolour_3_3_Yellow,0);
    }
    //  Pos (4,3) 
    if(MAZE[3][4] == 0){
        digitalWrite(twocolour_3_4_Red,0);
        digitalWrite(twocolour_3_4_Yellow,0);
    }
    //  Pos (3,4) 
    if(MAZE[4][3] == 0){
        digitalWrite(twocolour_4_3_Red,0);
        digitalWrite(twocolour_4_3_Yellow,0);
    }
    //  Pos (4,4) 
    if(MAZE[4][4] == 0){
        digitalWrite(twocolour_4_4_Red,0);
        digitalWrite(twocolour_4_4_Yellow,0);
    }
    //  Pos (0,7) 
    if(MAZE[7][0] == 0){
        digitalWrite(twocolour_7_0_Red,0);
        digitalWrite(twocolour_7_0_Yellow,0);
    }
    //  Pos (3,7) 
    if(MAZE[7][3] == 0){
        digitalWrite(twocolour_7_3_Red,0);
        digitalWrite(twocolour_7_3_Yellow,0);
    }
}