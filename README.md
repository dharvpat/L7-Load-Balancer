# Layer 7 Load Balancer Dashboard

A comprehensive Layer 7 (L7) Load Balancer Dashboard featuring a high-performance C backend implementing the Weighted Least Connections algorithm and a user-friendly Flask frontend. This project allows you to manage load balancing rules, backend servers, and monitor real-time metrics through an intuitive web interface.

## Table of Contents

- [Features](#features)
- [Architecture Overview](#architecture-overview)
- [Directory Structure](#directory-structure)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
  - [Running the Backend](#running-the-backend)
  - [Running the Frontend](#running-the-frontend)
- [API Documentation](#api-documentation)
- [Configuration](#configuration)
- [Testing](#testing)

---

## Features

- **High-Performance C Backend**: Implements the Weighted Least Connections algorithm for efficient traffic distribution.
- **Flask Frontend**: Provides an intuitive web interface for managing load balancing rules and backend servers.
- **RESTful APIs**: Facilitates communication between the frontend and backend for seamless operations.
- **Real-Time Metrics Monitoring**: Offers insights into load balancer performance and backend server statuses.
- **Configuration Management**: Easily adjust settings and configurations via JSON files and the web interface.
- **Comprehensive Logging**: Detailed logs for monitoring and debugging purposes.
- **Unit Testing**: Ensures reliability and robustness of both backend and frontend components.

---

## Architecture Overview

The project consists of two main components:

1. **C Backend** (`backend_c/`): Handles the core load balancing logic using the Weighted Least Connections algorithm. It exposes RESTful APIs for managing load balancing rules, backend servers, and retrieving metrics.

2. **Flask Frontend** (`frontend_flask/`): Provides a user-friendly web interface for configuring the load balancer, managing backend servers, and viewing real-time metrics. It communicates with the C backend via the exposed APIs.

---

## Directory Structure

l7-load-balancer-dashboard/
├── backend_c/
│   ├── src/
│   ├── config/
│   ├── build/
│   ├── tests/
│   ├── Makefile
│   └── README.md
├── frontend_flask/
│   ├── app/
│   ├── config/
│   ├── tests/
│   ├── requirements.txt
│   ├── run.py
│   └── README.md
├── docs/
├── .gitignore
└── README.md

## Prerequisites

- Operating System: Linux (Ubuntu/Debian recommended)
- C Compiler: GCC
- Python: Version 3.7 or higher
- libmicrohttpd: For the C backend HTTP server
- json-c: For JSON parsing in C
- Python Packages: Listed in frontend_flask/requirements.txt

## Installation
1. Clone the Repository
```bash
git clone https://github.com/yourusername/l7-load-balancer-dashboard.git
cd l7-load-balancer-dashboard
```
2. Install Dependencies
Backend:
```bash
sudo apt-get update
sudo apt-get install libmicrohttpd-dev libjson-c-dev gcc make
```
Frontend:
```bash
cd frontend_flask
pip install -r requirements.txt
cd ..
```

## Usage
1. Running the Backend:
   - Compile:
   ```bash
   cd backend_c
   make
   ```
This will generate the executable at `backend_c/build/load_balancer`.
  - Run the backend:
  ```bash
  ./build/load_balancer config/load_balancer_config.json
  ```
The backend will start an HTTP server listening on port `8888`.
2. Running the Frontend
  - Ensure the Backend is Running
  - Make sure the backend is running on http://localhost:8888/api. If hosted elsewhere, update the BACKEND_URL in frontend_flask/app/routes.py and frontend_flask/app/utils.py.
  - Start the Frontend:
  ```bash
  cd frontend_flask
  python run.py
  ```
  The frontend will start on http://127.0.0.1:5000/.
3. Access it using a web browser and navigate to http://127.0.0.1:5000/

## API Documentation

### Backend API Endpoints

1. Retrieve All Load Balancing Rules
   - Endpoint: `GET /api/rules`
   - Description: Fetches all existing load balancing rules.
   - Response: 200 OK: Returns a JSON object containing an array of rules.
2. Add a New Load Balancing Rule
   - Endpoint: `POST /api/rules`
   - Description: Adds a new load balancing rule.
   - Request Body:
       - Content-Type: `application/json`
       - Example:
         ```json
         {
          "path": "/static/*",
          "algorithm": "weighted_least_connections",
          "servers": [
            {"id": "server3", "ip": "127.0.0.1", "port": 8083, "weight": 3},
            {"id": "server4", "ip": "127.0.0.1", "port": 8084, "weight": 1}
          ]
         }
         ```
  - Response: 201 Created: Rule added successfully.
3. Delete a Load Balancing Rule
  - Endpoint: `DELETE /api/rules/{path}`
  - Description: Deletes a load balancing rule based on the specified path.
  - Response:
    - 200 OK: Rule deleted successfully.
    - 404 Not Found: Rule not found.

## Configuration

### Backend Configuration (`backend_c/config/load_balancer_config.json`)
 
Defines the initial configuration of the load balancer, including rules and servers.
```json
{
  "rules": [
    {
      "path": "/api/*",
      "algorithm": "weighted_least_connections",
      "servers": [
        {"id": "server1", "ip": "127.0.0.1", "port": 8081, "weight": 2},
        {"id": "server2", "ip": "127.0.0.1", "port": 8082, "weight": 1}
      ]
    }
  ],
  "logging": {
    "level": "INFO",
    "file": "logs/load_balancer.log"
  }
}
```
Modify this file to adjust the load balancing rules and backend servers as needed.

### Frontend Configuration (`frontend_flask/app/config/config.py`)

Contains configuration settings for the Flask application.
```
import os

class Config:
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'your_secret_key'
    # Add other configuration variables as needed
```

## Testing

1. Run Backend Tests:
   ```bash
   cd backend_c
    make test
   ```
2. Run Frontend Tests:
   ```bash
   cd frontend_flask
   python -m unittest discover -s app/tests
   ```
## Acknowledgments

- libmicrohttpd: For simplifying the HTTP server implementation in C.
- json-c: For efficient JSON parsing and generation in C.
- Flask: For providing a lightweight and flexible web framework for the frontend.
