void changeGateState() {
  if (place_action == true) {
    if (httpResponseCode == 202){
      Serial.println("process");
      digitalWrite(D1,LOW);
      delay(2000);
      digitalWrite(D1,HIGH);
    }
    else{
      digitalWrite(BUZZER_PIN, HIGH);   // turn buzzer ON
      delay(1000);   
      digitalWrite(BUZZER_PIN, LOW);  
    }
    place_action  =  false;
    delay(3000);
  }
}
