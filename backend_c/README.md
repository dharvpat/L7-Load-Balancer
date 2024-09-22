# Layer 7 Load Balancer Dashboard - C Backend

## Overview

This C backend implements a **Layer 7 (L7) Load Balancer** using the **Weighted Least Connections** algorithm. It exposes RESTful APIs to manage load balancing rules and backend servers, facilitating communication with a Flask-based frontend.

## Features

- **Weighted Least Connections Algorithm:** Distributes incoming traffic based on server weights and current active connections.
- **RESTful APIs:** Allows dynamic management of load balancing rules and backend servers.
- **Logging:** Comprehensive logging for monitoring and debugging.
- **Configuration Management:** Easily configurable via JSON files.
- **Unit Testing:** Ensures reliability of load balancing logic and utility functions.


## Setup Instructions
run `cd backed_c` followed by make

finally run the load balancer:

./build/load_balancer_config/load_balancer_config.json


## Running Tests:

run `make test`

### Prerequisites

- **GCC Compiler**
- **libmicrohttpd Development Libraries**
- **json-c Development Libraries**
- **Make**

#### **Installing Dependencies on Ubuntu/Debian:**

```bash
sudo apt-get update
sudo apt-get install libmicrohttpd-dev libjson-c-dev gcc make