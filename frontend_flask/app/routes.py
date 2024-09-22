from flask import Blueprint, render_template, request, redirect, url_for, flash, jsonify
import requests
import json

main = Blueprint('main', __name__)

# Configuration
BACKEND_URL = "http://localhost:8888/api"  # Update if backend is hosted elsewhere

@main.route('/')
def index():
    return redirect(url_for('main.dashboard'))

@main.route('/dashboard')
def dashboard():
    # Fetch metrics from backend (assuming an endpoint exists)
    metrics_response = requests.get(f"{BACKEND_URL}/metrics/realtime")
    if metrics_response.status_code == 200:
        metrics = metrics_response.json()
    else:
        metrics = {}
        flash('Failed to retrieve metrics from backend.', 'danger')
    return render_template('dashboard.html', metrics=metrics)

@main.route('/rules', methods=['GET', 'POST'])
def rules():
    if request.method == 'POST':
        # Add a new load balancing rule
        data = request.form.to_dict()
        # Construct the JSON payload
        rule = {
            "path": data.get('path'),
            "algorithm": "weighted_least_connections",
            "servers": []
        }
        # Assume servers are submitted as lists
        server_ids = request.form.getlist('server_id')
        server_ips = request.form.getlist('server_ip')
        server_ports = request.form.getlist('server_port')
        server_weights = request.form.getlist('server_weight')
        
        for sid, sip, sport, sweight in zip(server_ids, server_ips, server_ports, server_weights):
            server = {
                "id": sid,
                "ip": sip,
                "port": int(sport),
                "weight": int(sweight)
            }
            rule["servers"].append(server)
        
        # Send POST request to backend
        headers = {'Content-Type': 'application/json'}
        response = requests.post(f"{BACKEND_URL}/rules", data=json.dumps(rule), headers=headers)
        
        if response.status_code == 201:
            flash('Load balancing rule added successfully.', 'success')
        else:
            flash(f'Failed to add rule: {response.json().get("error", "Unknown error")}', 'danger')
        
        return redirect(url_for('main.rules'))
    
    # GET request - retrieve all rules
    response = requests.get(f"{BACKEND_URL}/rules")
    if response.status_code == 200:
        rules = response.json().get('rules', [])
    else:
        rules = []
        flash('Failed to retrieve rules from backend.', 'danger')
    return render_template('rules.html', rules=rules)

@main.route('/rules/delete/<path:rule_path>', methods=['POST'])
def delete_rule(rule_path):
    # Send DELETE request to backend
    response = requests.delete(f"{BACKEND_URL}/rules/{rule_path}")
    if response.status_code == 200:
        flash('Load balancing rule deleted successfully.', 'success')
    else:
        flash(f'Failed to delete rule: {response.json().get("error", "Unknown error")}', 'danger')
    return redirect(url_for('main.rules'))

@main.route('/settings', methods=['GET', 'POST'])
def settings():
    if request.method == 'POST':
        # Update global settings
        # For simplicity, assume settings include logging level
        logging_level = request.form.get('logging_level')
        settings = {
            "logging": {
                "level": logging_level
            }
        }
        headers = {'Content-Type': 'application/json'}
        response = requests.put(f"{BACKEND_URL}/config", data=json.dumps(settings), headers=headers)
        if response.status_code == 200:
            flash('Settings updated successfully.', 'success')
        else:
            flash(f'Failed to update settings: {response.json().get("error", "Unknown error")}', 'danger')
        return redirect(url_for('main.settings'))
    
    # GET request - retrieve current settings
    response = requests.get(f"{BACKEND_URL}/config")
    if response.status_code == 200:
        config = response.json()
    else:
        config = {}
        flash('Failed to retrieve settings from backend.', 'danger')
    return render_template('settings.html', config=config)

@main.route('/servers', methods=['GET', 'POST'])
def servers():
    if request.method == 'POST':
        # Add a new backend server
        data = request.form.to_dict()
        server = {
            "id": data.get('server_id'),
            "ip": data.get('server_ip'),
            "port": int(data.get('server_port')),
            "weight": int(data.get('server_weight'))
        }
        headers = {'Content-Type': 'application/json'}
        response = requests.post(f"{BACKEND_URL}/servers", data=json.dumps(server), headers=headers)
        if response.status_code == 201:
            flash('Backend server added successfully.', 'success')
        else:
            flash(f'Failed to add server: {response.json().get("error", "Unknown error")}', 'danger')
        return redirect(url_for('main.servers'))
    
    # GET request - retrieve all backend servers
    response = requests.get(f"{BACKEND_URL}/servers")
    if response.status_code == 200:
        servers = response.json().get('servers', [])
    else:
        servers = []
        flash('Failed to retrieve backend servers from backend.', 'danger')
    return render_template('servers.html', servers=servers)

@main.route('/servers/delete/<server_id>', methods=['POST'])
def delete_server(server_id):
    # Send DELETE request to backend
    response = requests.delete(f"{BACKEND_URL}/servers/{server_id}")
    if response.status_code == 200:
        flash('Backend server deleted successfully.', 'success')
    else:
        flash(f'Failed to delete server: {response.json().get("error", "Unknown error")}', 'danger')
    return redirect(url_for('main.servers'))
