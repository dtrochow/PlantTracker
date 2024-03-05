# PlantTracker Development Plan

## M0

[Vlad]
- Backend written in Rust
  - Web Server on PC
    - Rust actix as framework (https://actix.rs/)
    - Should be written in OS agnostic way
- Frontend written in TypeScript - React library
    - Basic view:
      - Mocked temperature value (e.g., 400 F, random or counter)
      - Button to update the temperature
      - [optional] Button for converting the temperature unit
        - All temperature assumed to be Celsius
        - Conversion on frontend/client side

[Dominik]
- Node devices written in Rust
  - First device on Raspberry Pi Pico WiFi
  - RPi Pico as WiFi hotspot
  - Pico can establish connection with backend - ZeroMQ library
  - Communication over TCP
  - [?] Rust prost library
  - Receiving message from backend - update request
  - Sending mocked temperature value (in Celsius)
