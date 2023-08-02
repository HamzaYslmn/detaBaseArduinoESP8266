#include "detabaseEsp8266.h"

DetabaseEsp8266::DetabaseEsp8266(const char* apiKey, const char* projectId, const char* baseName, size_t capacity)
  : apiKey(apiKey), projectId(projectId), baseName(baseName), capacity(capacity), doc(capacity) {}

void DetabaseEsp8266::addData(const char* key, const String& value) {
  doc["items"][0][key] = value;
}

void DetabaseEsp8266::addData(const char* key, const char* value) {
  doc["items"][0][key] = value;
}

void DetabaseEsp8266::addData(const char* key, int value) {
  doc["items"][0][key] = value;
}

void DetabaseEsp8266::addData(const char* key, float value) {
  doc["items"][0][key] = value;
}

void DetabaseEsp8266::addData(const char* key, bool value) {
  doc["items"][0][key] = value;
}

bool DetabaseEsp8266::sendData() {
  // Build the URL for the base
  String url = "https://database.deta.sh/v1/" + String(projectId) + "/" + String(baseName) + "/items";

  // Serialize the JSON payload to a string
  String payload;
  serializeJson(doc, payload);

  // Initialize the HTTP client and set headers
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", String(apiKey));

  // Send the PUT request with the JSON payload
  int httpResponseCode = http.PUT(payload);

  // Store the response code and payload
  lastResponseCode = httpResponseCode;
  responsePayload = http.getString();

  // Check the response code
  return httpResponseCode >= 200 && httpResponseCode < 300;
}

void DetabaseEsp8266::getItem(const String& key) {
  // Build the URL for the specific item
  String url = "https://database.deta.sh/v1/" + String(projectId) + "/" + String(baseName) + "/items/" + key;

  // Initialize the HTTP client and set headers
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", String(apiKey));

  // Send the GET request
  int httpResponseCode = http.GET();

  // Store the response code and payload
  lastResponseCode = httpResponseCode;
  responsePayload = http.getString();
}

bool DetabaseEsp8266::deleteItem(const String& key) {
  // Build the URL for the specific item
  String url = "https://database.deta.sh/v1/" + String(projectId) + "/" + String(baseName) + "/items/" + key;

  // Initialize the HTTP client and set headers
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", String(apiKey));

  // Send the DELETE request
  int httpResponseCode = http.sendRequest("DELETE");

  // Store the response code and payload
  lastResponseCode = httpResponseCode;
  responsePayload = http.getString();

  // Check the response code
  return httpResponseCode >= 200 && httpResponseCode < 300;
}

String DetabaseEsp8266::getData(const char* key) {
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, responsePayload);
  return doc[key].as<String>();
}

int DetabaseEsp8266::getIntData(const char* key) {
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, responsePayload);
  return doc[key].as<int>();
}

float DetabaseEsp8266::getFloatData(const char* key) {
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, responsePayload);
  return doc[key].as<float>();
}

bool DetabaseEsp8266::getBoolData(const char* key) {
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, responsePayload);
  return doc[key].as<String>() == "true";
}
