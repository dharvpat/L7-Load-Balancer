# Layer 7 Load Balancer Dashboard - Flask Frontend

## Overview

This Flask frontend provides a web-based user interface for configuring and monitoring the **Layer 7 (L7) Load Balancer** implemented in C. It communicates with the backend via RESTful APIs to manage load balancing rules, backend servers, and display real-time metrics.

## Features

- **Dashboard:** Displays real-time metrics and backend server statuses.
- **Load Balancing Rules Management:** Create, view, and delete load balancing rules.
- **Backend Servers Management:** Add, view, and delete backend servers.
- **Global Settings:** Adjust settings such as logging levels.
- **User Feedback:** Provides success and error messages based on backend interactions.
- **Responsive Design:** Ensures the UI is accessible across various devices.

## Setup Instructions

### Prerequisites

- **Python 3.7+**
- **pip**

### Installing Dependencies

Navigate to the `frontend_flask` directory and install the required packages:

```bash
cd frontend_flask
pip install -r requirements.txt
```
### Running Frontend
To run the frontend GUI, first make sure the backend is running on a local host usually `http://localhost:8888/api` This address needs to be inputted to BACKEND_URL in app/routes.py and app/utils.py

then start the flask app:
`python run.py`