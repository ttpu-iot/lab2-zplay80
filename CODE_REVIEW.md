# Code Review: lab2_ex1.ino

## Overview
This document provides a comprehensive review of the solution code in `src/lab2_ex1.ino` against the requirements specified in `README.md`.

## Summary
✅ **Overall Assessment:** The solution is mostly correct and functional with a few minor issues to address.

---

## Detailed Review

### ✅ **Correct Implementation**

#### 1. Pin Configuration (Requirement: setup() step 1)
- ✅ LED pins correctly defined: Red=26, Green=27, Blue=14, Yellow=12
- ✅ All pins configured as OUTPUT
- ✅ All LEDs initialized to LOW state

#### 2. Serial Communication (Requirement: setup() step 2)
- ✅ Serial initialized at 115200 baud
- ✅ Name and lab printed: "Ziyo, Lab 2"

#### 3. WiFi Connection (Requirement: setup() steps 3-4)
- ✅ WiFi set to station mode (implicitly done by WiFi.begin())
- ✅ Correct SSID: "Wokwi-GUEST"
- ✅ Correct password: "" (empty)
- ✅ Waits for connection with visual feedback (dots)
- ✅ Prints IP address after connection

#### 4. Loop - WiFi Reconnection (Requirement: loop() step 1)
- ✅ Checks WiFi status
- ✅ Reconnects if disconnected
- ✅ Prints status messages

#### 5. HTTP Request (Requirement: loop() step 2)
- ✅ HTTPClient created correctly
- ✅ Correct API URL used
- ✅ GET request performed
- ✅ HTTP response code checked
- ✅ Non-200 codes handled with error message
- ✅ Raw JSON printed to Serial
- ✅ HTTP connection properly closed with http.end()

#### 6. JSON Parsing (Requirement: loop() step 2)
- ✅ ArduinoJson v7 used (StaticJsonDocument)
- ✅ Correct field access: doc["feeds"][0]["fieldN"]
- ✅ LED state correctly set based on "HIGH" or "LOW" values
- ✅ Parse errors handled with error message

#### 7. Timing (Requirement: loop() step 3)
- ✅ 5000ms delay implemented

---

## ⚠️ **Issues Identified**

### Issue #1: WiFi Mode Not Explicitly Set
**Severity:** Minor  
**Location:** Line 27  
**Description:** The requirement states "Set Wi-Fi to **station mode** (`WIFI_STA`)". While `WiFi.begin()` implicitly sets station mode, it's better to be explicit.

**Current Code:**
```cpp
WiFi.begin(ssid, password);
```

**Recommended Fix:**
```cpp
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
```

---

### Issue #2: Deprecated JSON Library API
**Severity:** Minor  
**Location:** Line 52  
**Description:** ArduinoJson v7 has deprecated `StaticJsonDocument`. The recommended approach is to use `JsonDocument` instead.

**Current Code:**
```cpp
StaticJsonDocument<1024> doc;
```

**Recommended Fix:**
```cpp
JsonDocument doc;
```

**Reference:** ArduinoJson v7 migration guide suggests using `JsonDocument` for automatic memory management.

---

### Issue #3: JSON Parse Error Check Logic
**Severity:** Minor  
**Location:** Line 53  
**Description:** The condition checks if deserialization equals `Ok`, but this is not the standard way to check for errors in ArduinoJson v7.

**Current Code:**
```cpp
if (deserializeJson(doc, json) == DeserializationError::Ok) {
```

**Recommended Fix:**
```cpp
DeserializationError error = deserializeJson(doc, json);
if (!error) {
```

Or alternatively:
```cpp
if (deserializeJson(doc, json) == DeserializationError::Ok) {
```
(Current code actually works, but the idiomatic way is to check `!error`)

---

### Issue #4: Edge Case - Empty Feeds Array
**Severity:** Low  
**Location:** Line 54  
**Description:** The code assumes `feeds` array always has at least one element. If the array is empty, accessing `doc["feeds"][0]` could cause undefined behavior.

**Current Code:**
```cpp
JsonObject f = doc["feeds"][0];
```

**Recommended Fix:**
```cpp
JsonArray feeds = doc["feeds"];
if (feeds.size() > 0) {
  JsonObject f = feeds[0];
  digitalWrite(redLED,    f["field1"] == "HIGH" ? HIGH : LOW);
  digitalWrite(greenLED,  f["field2"] == "HIGH" ? HIGH : LOW);
  digitalWrite(blueLED,   f["field3"] == "HIGH" ? HIGH : LOW);
  digitalWrite(yellowLED, f["field4"] == "HIGH" ? HIGH : LOW);
} else {
  Serial.println("No feeds data available");
}
```

---

## 📋 **Checklist Verification**

Based on README.md requirements:

- [x] ✅ On startup, Serial prints name and lab-2  
  - Prints "Ziyo, Lab 2"

- [x] ✅ ESP32 connects to `Wokwi-GUEST` and prints its IP  
  - Connects successfully and prints IP address

- [x] ✅ Every ~5 sec: perform HTTP GET request to Read API  
  - Uses correct API endpoint
  - 5 second delay implemented

- [x] ✅ Gets response as raw JSON string and prints to Serial  
  - Raw JSON printed with `Serial.println(json)`

- [x] ✅ Parses JSON using ArduinoJson library  
  - Uses ArduinoJson v7 with StaticJsonDocument

- [x] ✅ Updates all 4 LEDs accordingly  
  - All 4 LEDs updated based on field1-field4 values

- [x] ✅ Response code other than 200 printed as error, program keeps running  
  - Checks `if (code == 200)` and prints error otherwise
  - Continues loop after error

---

## 🎯 **Recommendations**

### Priority Fixes:
1. **Add explicit WiFi.mode(WIFI_STA)** - For clarity and best practices
2. **Update to JsonDocument** - To follow ArduinoJson v7 recommendations
3. **Add empty feeds array check** - To prevent potential crashes

### Optional Improvements:
1. **Add timeout for WiFi connection** - Prevent infinite waiting if connection fails
2. **Add HTTP timeout** - Prevent hanging if server doesn't respond
3. **Add more detailed error logging** - Include deserialization error messages

---

## ✅ **Conclusion**

The solution successfully implements all the required functionality and would work correctly in the Wokwi simulation environment. The code is well-structured, readable, and follows the exercise requirements closely.

**Grade Recommendation:** A- (90-95%)  
**Functional:** ✅ Yes  
**Meets Requirements:** ✅ Yes  
**Code Quality:** ✅ Good (with minor improvements possible)

The identified issues are minor and mostly related to best practices rather than functionality. The code would execute successfully and control the LEDs as expected based on ThingSpeak data.

---

## 📝 **Student Details**
- **Name:** Ziyo
- **Assignment:** Lab 2 - ESP32 + HTTP (ThingSpeak) + ArduinoJson
- **Repository:** ttpu-iot/lab2-zplay80
