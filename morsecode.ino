#include <LiquidCrystal.h> // inkluderar LiquidCrystal biblioteket 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Skapar en LC objekt. Parameters: (rs, enable, d4, d5, d6, d7)


int buzzer = 8 ;

int buttonState = 0;     // kanppens nuvarande värde
bool lastbuttonState = false; // knappens föregående värde
int startPressed = 0;    // när knappen hålls in (trycks ner) 
int endPressed = 0;      // när knappen släppts 
int holdTime = 0;        // hur länge knappen var tryckt
int idleTime = 0;        // hur länge kanppen var inaktiv
int lastMillis = 0;
String buff = "";


void setup ()
{
  lcd.begin(16, 2); // Initierar gränssnittet till LCD-skärmen och specificerar dimensionerna (bredd och höjd) på skärmen
  Serial.begin(9600);
  pinMode (buzzer, INPUT) ;
}
void loop() {
  buttonState = digitalRead(buzzer); // läser knappens input



  if (buttonState == LOW) {
    //if buffer full
    if ((millis() - endPressed) % 65536 > 500) { //en bokstav
      if (buff != "") {
        Serial.println(retBuf());
        clearBuf();
        lastbuttonState = true;
      }

    }
    if ((millis() - endPressed) % 65536 > 1500) { //ett ord
      if (lastbuttonState == true) {
        Serial.println("SPACE HERE");
        lastbuttonState = false;
      }

    }

  }

  else if (buttonState == HIGH ) { // kanppens tillstånd ändrats
    startPressed = millis();

    updateState();
  }



}



void updateState() {
  // kanppen har precis tryckts ned


  while (digitalRead(buzzer) == HIGH) {
    endPressed = millis();
  }


  // knappen har precis släppts

  holdTime = endPressed - startPressed;
  Serial.println(holdTime);

  if (holdTime < 500) {
    Serial.println("DIT");
    buff = buff + ".";
  }

  else if (holdTime >= 500) {
    Serial.println("DAH");
    buff = buff + "-";
  }
  Serial.println(buff);


}


String retBuf() {
  return morseToChar(buff);
}


String morseToChar(const String& morse) {
  static String letters[] = {".-", "-...", "-.-.", "-..", ".",
                             "..-.", "--.", "....", "..", ".---",
                             "-.-", ".-..", "--", "-.", "---",
                             ".--.", "--.-", ".-.", "...", "-",
                             "..-", "...-", ".--", "-..-",
                             "-.--", "--.."
                            };
// Morsekod för olika bokstäver


  for (int k = 0; k < sizeof(letters); k++) {

    if (letters[k] == morse) {
      lcd.print((char) (k + 65));
      return ((String) (k + 65));
    }
  }
  return "ERROR";

}

void clearBuf() {
  buff = "";
}

// clear:ar
