const int numPins = 12; // Number of LED pins
const int ledPins[numPins] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // Pins connected to LEDs

// Braille patterns for digits 0 to 9
const int braillePatterns[10][6] = {
  {0, 0, 1, 0, 1, 1}, // 0
  {0, 1, 0, 0, 0, 0}, // 1
  {0, 1, 1, 0, 0, 0}, // 2
  {0, 1, 0, 0, 1, 0}, // 3
  {0, 1, 0, 0, 1, 1}, // 4
  {0, 1, 0, 0, 0, 1}, // 5
  {0, 1, 1, 0, 1, 0}, // 6
  {0, 1, 1, 0, 1, 1}, // 7
  {0, 1, 1, 0, 0, 1}, // 8
  {0, 0, 1, 0, 1, 0}  // 9
};

// Braille pattern for number indicator, plus sign, and equal sign
const int specialSymbols[2][6] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1}, // Number indicator
};

const int equalsign[2][6] = {
  {0, 0, 0, 1, 0, 1},
  {1, 0, 1, 0, 0, 0}  // Equal sign
};

const int operationsymbols[4][6] = {
  {0, 0, 0, 0, 0, 0}, //0 for pairing of the
  {0, 0, 1, 1, 0, 1}, //+ in Braille
  {0, 0, 1, 0, 0, 1}, //- in Braille
  {0, 0, 0, 1, 0, 0}, // x in Braille
};

void setup() {
  for (int i = 0; i < numPins; i++) {
    pinMode(ledPins[i], OUTPUT); // Set LED pins as output
  }
  Serial.begin(9600); // Initialize serial communication
}

void resetLEDs() {
  for (int i = 0; i < numPins; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void displayEquation(String equationString) {
  int equationLength = equationString.length();
  if (equationLength % 2 != 0 || equationLength < 6 || equationLength > 10) {
    // Invalid equation format
    return;
  }

  for (int i = 0; i < equationLength; i += 2) {
    int a = equationString.substring(i, i + 2).toInt();
    int tens = a / 10;
    int ones = a % 10;

    if (i == 0) {
      displaySpecialSymbol(1); // Display number indicator
      delay(3000);
    } else if (i == equationLength - 2) {
      displayequal(0); // Display equal sign
      delay(3000);
    } else if (i == 2) {
      displayDigits(tens, ones); // Display first number
      delay(3000);
    } else if (i == 4) {
      displayoperations(tens); // Display operation
      delay(3000);
    } else {
      displayDigits(tens, ones); // Display second number
      delay(3000);
    }

    resetLEDs(); // Reset LEDs
  }
  delay(5000); // Wait for 5 seconds after displaying each equation
}

void printEquation(String equationString) {
  int equationLength = equationString.length();
  if (equationLength % 2 != 0 || equationLength < 6 || equationLength > 10) {
    // Invalid equation format
    return;
  }

  Serial.print("Equation ");
  Serial.print(equationString);
  Serial.print(" is (ni) ");

  int firstNumber = equationString.substring(2, 4).toInt();
  Serial.print(firstNumber);

  int operation = equationString.substring(4, 6).toInt() / 10;
  switch (operation) {
    case 1:
      Serial.print(" + ");
      break;
    case 2:
      Serial.print(" - ");
      break;
    case 3:
      Serial.print(" * ");
      break;
    default:
      break;
  }

  int secondNumber = equationString.substring(6, 8).toInt();
  Serial.print(secondNumber);
  Serial.println(" =");
}

void loop() {

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read input string until newline
    int numEquations = countSpaces(input) + 1; // Count the number of equations

    for (int i = 0; i < numEquations; i++) {
      int spaceIndex = input.indexOf(' '); // Find the index of the next space
      String equationString;

      if (spaceIndex == -1) {
        equationString = input; // If no space found, use the remaining string
        input = ""; // Clear the input string
      } else {
        equationString = input.substring(0, spaceIndex); // Extract the equation string
        input = input.substring(spaceIndex + 1); // Remove the extracted equation from the input
      }

      Serial.print( "Equation: \n");
      printEquation(equationString); // Print the equation
      Serial.print("Displaying LED patterns...\n");
      displayEquation(equationString); // Display the equation
      Serial.print("Done!\n\n");
     
    }
    Serial.print("Waiting for the next input...\n\n");
  }
  
}

int countSpaces(String str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == ' ') {
      count++;
    }
  }
  return count;
}

void displayDigits(int tens, int ones) {
  // Display Braille pattern for the tens digit (first 6 LEDs)
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], braillePatterns[tens][i]); // Output Braille pattern to LEDs
  }

  // Display Braille pattern for the ones digit (next 6 LEDs)
  for (int i = 6; i < 12; i++) {
    digitalWrite(ledPins[i], braillePatterns[ones][i - 6]); // Output Braille pattern to LEDs
  }
}

void displaySpecialSymbol(int symbolIndex) {
  // Display Braille pattern for the special symbol
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], specialSymbols[symbolIndex][i]); // Output Braille pattern to LEDs
  }
  for (int i = 6; i < 12; i++) {
    digitalWrite(ledPins[i], specialSymbols[0][i - 6]);
  }
}

void displayoperations(int tens) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], operationsymbols[tens][i]); // Output Braille pattern to LEDs
  }
  for (int i = 6; i < 12; i++) {
    digitalWrite(ledPins[i], operationsymbols[0][i - 6]);
  }
}

void displayequal(int equal) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], equalsign[equal][i]); // Output Braille pattern to LEDs
  }
  for (int i = 6; i < 12; i++) {
    digitalWrite(ledPins[i], equalsign[1][i - 6]);
  }
}