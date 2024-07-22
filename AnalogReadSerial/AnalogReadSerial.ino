#include <IRremote.h>

const int irReceiverPin = 3; // IR 수신기가 연결된 핀 (D3)
const int irLedPin = 4;      // IR LED가 연결된 핀 (D4)
IRrecv irrecv(irReceiverPin);
IRsend irsend(irLedPin);     // IR 송신기 설정
decode_results results;

// 신호 저장 배열
unsigned int capturedSignal[200];
int signalLength = 0;

void setup() {
  Serial.begin(115200);        // 시리얼 통신 시작
  irrecv.enableIRIn();         // IR 수신기 시작
  Serial.println("IR Receiver Initialized"); // 초기 메시지 출력
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println("Received IR signal");

    // 수신된 RAW 데이터를 저장하고 출력합니다.
    signalLength = results.rawlen;
    Serial.print("rawData[");
    Serial.print(signalLength);
    Serial.print("]: ");
    for (int i = 0; i < signalLength; i++) {
      capturedSignal[i] = results.rawbuf[i];
      Serial.print(capturedSignal[i], DEC);
      if (i < signalLength - 1) {
        Serial.print(", ");
      }
    }
    Serial.println();

    // 신호의 합을 출력합니다 (선택 사항)
    unsigned long sum = 0;
    for (int i = 1; i < signalLength; i++) {
      sum += abs(capturedSignal[i]);
    }
    Serial.print("Sum: ");
    Serial.println(sum);

    irrecv.resume(); // 다음 신호를 위해 수신기 재설정
  }

  // 버튼을 눌러 캡처된 신호를 다시 송신
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 's') { // 's' 키를 눌러 신호 송신
      Serial.println("Sending captured signal...");
      irsend.sendRaw(capturedSignal, signalLength, 38); // 38kHz 주파수로 송신
      Serial.println("Signal sent");
    }
  }
}
