# frontend_flask/app/config/config.py

import os

class Config:
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'your_secret_key'
    # Add other configuration variables as needed