#include "HelperFunctions.h"
#include "MyClass.h"

void setup() {
  Serial.begin( 9600 );
  delay(1000);
   
  MyClass obj;
   
  obj.begin();
  helper_function();
}
 
void loop() {
  //Empty
}

