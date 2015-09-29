//
// test program
//

//
// blink the built-in LED
// output something to serial
//

int led = D7;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(millis());

  digitalWrite(led, HIGH);
  delay(1000);

  digitalWrite(led, LOW);
  delay(500);

}
