void readCardUID() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

//  Serial.println();
//  Serial.print(" UID tag :");
  content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  sendDataToSever(content);
  
}

void sendDataToSever(String card_uid) {
  if (WiFi.status() == WL_CONNECTED) {

    http.begin(client, serverAddress);
    http.addHeader("Content-Type", "application/json");
    res_message =  "{\"card_id\":\"";
    res_message += card_uid;
    res_message +=  "\",\"card_reader\":\"";
    res_message += cardReaderUID;
    res_message += "\"}";
    Serial.println(res_message);
    httpResponseCode = http.POST(res_message);
    httpResponsePayload = http.getString();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("payload: ");
    Serial.println(httpResponsePayload);
    http.end();
    place_action = true;
  }
}
