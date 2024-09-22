# frontend_flask/app/utils.py

import requests
import json
from flask import current_app, flash

def get_all_rules():
    try:
        response = requests.get("http://localhost:8888/api/rules")
        response.raise_for_status()
        return response.json().get('rules', [])
    except requests.RequestException as e:
        current_app.logger.error(f"Error fetching rules: {e}")
        flash('Error fetching rules from backend.', 'danger')
        return []

def add_rule(rule_data):
    try:
        headers = {'Content-Type': 'application/json'}
        response = requests.post("http://localhost:8888/api/rules", json=rule_data, headers=headers)
        response.raise_for_status()
        return response.json()
    except requests.RequestException as e:
        current_app.logger.error(f"Error adding rule: {e}")
        flash('Error adding rule to backend.', 'danger')
        return None

def delete_rule(rule_path):
    try:
        response = requests.delete(f"http://localhost:8888/api/rules/{rule_path}")
        response.raise_for_status()
        return response.json()
    except requests.RequestException as e:
        current_app.logger.error(f"Error deleting rule: {e}")
        flash('Error deleting rule from backend.', 'danger')
        return None

def get_all_servers():
    try:
        response = requests.get("http://localhost:8888/api/servers")
        response.raise_for_status()
        return response.json().get('servers', [])
    except requests.RequestException as e:
        current_app.logger.error(f"Error fetching servers: {e}")
        flash('Error fetching servers from backend.', 'danger')
        return []

def add_server(server_data):
    try:
        headers = {'Content-Type': 'application/json'}
        response = requests.post("http://localhost:8888/api/servers", json=server_data, headers=headers)
        response.raise_for_status()
        return response.json()
    except requests.RequestException as e:
        current_app.logger.error(f"Error adding server: {e}")
        flash('Error adding server to backend.', 'danger')
        return None

def delete_server(server_id):
    try:
        response = requests.delete(f"http://localhost:8888/api/servers/{server_id}")
        response.raise_for_status()
        return response.json()
    except requests.RequestException as e:
        current_app.logger.error(f"Error deleting server: {e}")
        flash('Error deleting server from backend.', 'danger')
        return None

def get_metrics():
    try:
        response = requests.get("http://localhost:8888/api/metrics/realtime")
        response.raise_for_status()
        return response.json()
    except requests.RequestException as e:
        current_app.logger.error(f"Error fetching metrics: {e}")
        flash('Error fetching metrics from backend.', 'danger')
        return {}