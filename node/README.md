## Building

For Raspberry Pico, thumbv6m-none-eabi toolchain is needed, which can be added using `rustup`

```bash
rustup target add thumbv6m-none-eabi
```

Create .env file in current directory with following envirnoment variables:

```bash
WIFI_SSID=<ssid_of_new_access_point>
WIFI_PASSWORD=<password_of_new_access_point>
```

Use `cargo build` to build firmware. Use `cargo run` to flash firmware onto chip using `elf2uf2-rs` library.

To use debuggers/flash firmware via probe hardware, `probe-rs` is recommended, which can be installed using following command

`cargo install probe-rs --features cli`

Then in `.cargo/config.toml` switch runners by commenting out current one and uncommenting appropriate one.