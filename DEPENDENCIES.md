# Project Dependencies
---
## Libraries
|   Library   |    Version    |     Purpose                  |
|-------------|---------------|------------------------------|
| `libpqxx`   | 7.10.0        | PostgreSQL C++ Connector     |
| `libpq`     | System        | PostgreSQK C Library         |
| `postgresql`| System        | CLI Client for DB interaction|
---


## Installing libpqxx
- libpqxx 7.10.0 can be found via github:

```bash
git clone --branch 7.10.0 https://github.com/jtv/libpqxx.git
cd libpqxx

# Configure
cmake -S . -B build
#Compile
cmake --build build
#Install headers and lib
sudo cmake --install build
```

