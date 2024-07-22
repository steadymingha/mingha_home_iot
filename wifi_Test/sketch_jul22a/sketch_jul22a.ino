#include <IRremote.h>

// 핀 정의
const int recvPin = 2;   // IR 수신기 핀
const int sendPin = 3;   // IR 송신기 핀

IRrecv irrecv(recvPin);
IRsend irsend;

decode_results results;
signed int rawData[700];

void setup() {
  // 시리얼 통신 시작
  Serial.begin(115200);

  // IR 수신기 시작
  irrecv.enableIRIn();
}

void loop() {


  // // 1초마다 RAW 데이터 송신
  // irsend.sendRaw(rawData, 67, 38); // 38kHz로 송신
  // delay(1000);  // 1초 대기

  // 리모컨 신호 수신
  if (irrecv.decode(&results)) {
    // 수신한 RAW 데이터 출력
    Serial.println("Received RAW data:");
    for (int i = 1; i < results.rawlen; i++) {
      Serial.print(results.rawbuf[i] * USECPERTICK);
      Serial.print(" ");
    }
    Serial.println();
    
    // 수신 완료 표시
    irrecv.resume();
  }
}