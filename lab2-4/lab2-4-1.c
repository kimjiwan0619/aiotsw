// GPIO Acccess Liabrary
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// 각 FND와 연결된 라즈베리파이 핀(S0, S1, …, S) => FND_SEL port
const int FndSelectPin[6] = { 4, 17, 18, 27, 22, 23 };
// FND의 LED와 연결된 라즈베리파이 핀(A, B, …, H) => FND_DB
const int FndPin[8] = { 6, 12, 13, 16, 19, 20, 26, 21 };
// FND에 출력되는 0~9 까지의 비트값 배열
const int FndFont[5] = { 0x76, 0x79, 0x38, 0x38, 0x3F}; // H, E, L, L, O

// 초기화 함수, WiringPi 라이브러리 초기화, Select 핀 및 LED 핀 초기화를 담당)
void Init(){
	int i;
    
    if(wiringPiSetupGpio()==-1){
    	printf("Wiringpi Error!");
        exit(-1);
    }
    
    for(i=0; i<6; i++){
    	pinMode(FndSelectPin[i], OUTPUT);
        digitalWrite(FndSelectPin[i], HIGH); // FND_SEL은 LOW가 Select하는 것임을 주의!
    }
	for( i = 0; i < 8; i++ ) {
      pinMode( FndPin[i], OUTPUT ); // LED 핀을 출력으로 설정
      digitalWrite( FndPin[i], LOW ); // LED 핀을 OFF
    }
}

// FND를 선택하는 함수, S0 ~ S4 중 파라미터(position)에 해당하는 FND 선택
void FndSelect (int position) {
	int i;
	for( i = 0; i < 5; i++ ) {
		if( i == position ) {
			digitalWrite( FndSelectPin[ i ], LOW ); // 선택된 FND의 Select 핀 ON
		}
		else {
			digitalWrite( FndSelectPin[ i ], HIGH ); // 선택되지 않은 FND의 Select 핀 OFF
		}
	}
}

// FND 출력함수 => 해당하는 position에 숫자 출력
void FndDisplay(int position, int num){
	int i, j;
    int flag = 0; // FNDpin[i]을 ON/OFF하기 위한 flag
    int shift = 0x01; // FndFont와 And연산하여 출력할 LED 상태 결정
    
    for(i=0; i<8; i++){
    	flag = (FndFont[num] & shift); // 만약 i=0, FndFont[0]=0x3F이라 하면, (0b00111111 & 0b00000001)이 된다.
        
        digitalWrite(FndPin[i], flag); // FndPin[i]를 flag(0또는1)로 ON.OFF
        
        shift << =1; // 왼쪽으로 1비트 shift한다. 0b00000001 -> 0b00000010
    }
    FndSelect(position); // position으로 받은 FND를 선택
}

int main() {
  
	int pos;
    int i = 0;
    int data[5] = {0, 1, 2, 3, 4}; // HELLO
    
    Init();
    
    unsigned int time = millis(); // 출력을 계속 유지시키기 위함
    while(1){
    	for(pos=0; pos<5; pos++){
        	FndDisplay((pos+i)%5, data[pos]); // circular 하기 위함
            delay(1);
        }
        
        if(millis()-time > 500){ // 500ms 지나야 i가 바뀌면서 Pos에 i가 더해짐.
        	i = (i+1) % 5;
            time = millis();
        }
        return 0;
    }
}