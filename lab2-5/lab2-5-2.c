#include <wiringPi.h>
#include <softPwm.h>

// Set LED pin
#define LED_PIN 17
const LedRed[8] = {4, 17, 18, 27, 22, 23, 24, 25};

// LED intensity control function
void LedControl(int intensity){
	softPwmWrite(LED_PIN, intensity);
}

int main(void){
    int i;  
	if (wiringPiSetupGpio() == -1)
  		return 1;
    // LED 핀 출력으로 설정
    for(i=0; i<8; i++){
    	pinMode(LedRed[i], OUTPUT);
        digitalWrite(LedRed[i], 0);
	}
    // LED 핀 PWM 제어 핀으로 설정, 주기: 128ms
    softPwmCreate(LED_PIN, 0, 128);
    while(1){
    	int duty_cycle, intensity;
        // Duty cycle 0부터 100까지 증가시키면서 밝아진다
        for(duty_cycle=0; duty_cycle<=100; duty_cycle++){
        	intensity = (int)(128 * duty_cycle / 100);
            LedControl(intensity);
            delay(10); // 0.01초씩 밝아짐
        }
        
        // Duty cycle 100부터 0까지 감소하면서 어두워진다
        for(duty_cycle=100; duty_cycle>=0; duty_cycle--){
        	intensity = (int)(128 * duty_cycle / 100);
            LedControl(intensity);
            delay(10); // 0.01초씩 밝아짐
        }
    }
 	return 0;
 }