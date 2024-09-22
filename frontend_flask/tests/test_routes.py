# frontend_flask/app/tests/test_routes.py

import unittest
from app import create_app
from unittest.mock import patch

class TestRoutes(unittest.TestCase):
    def setUp(self):
        self.app = create_app()
        self.client = self.app.test_client()
    
    @patch('frontend_flask.app.routes.requests.get')
    def test_dashboard(self, mock_get):
        # Mock the backend response
        mock_get.return_value.status_code = 200
        mock_get.return_value.json.return_value = {
            "total_requests": 100,
            "active_connections": 5,
            "servers": [
                {"id": "server1", "ip": "127.0.0.1", "port": 8081, "active_connections": 2, "status": "up"},
                {"id": "server2", "ip": "127.0.0.1", "port": 8082, "active_connections": 3, "status": "up"}
            ]
        }
        response = self.client.get('/dashboard')
        self.assertEqual(response.status_code, 200)
        self.assertIn(b'Total Requests: 100', response.data)
        self.assertIn(b'Active Connections: 5', response.data)
    
    @patch('frontend_flask.app.routes.requests.get')
    def test_rules_page(self, mock_get):
        # Mock the backend response
        mock_get.return_value.status_code = 200
        mock_get.return_value.json.return_value = {
            "rules": [
                {
                    "path": "/api/*",
                    "algorithm": "weighted_least_connections",
                    "servers": [
                        {"id": "server1", "ip": "127.0.0.1", "port": 8081, "weight": 2},
                        {"id": "server2", "ip": "127.0.0.1", "port": 8082, "weight": 1}
                    ]
                }
            ]
        }
        response = self.client.get('/rules')
        self.assertEqual(response.status_code, 200)
        self.assertIn(b'/api/*', response.data)
        self.assertIn(b'server1', response.data)
        self.assertIn(b'server2', response.data)

if __name__ == '__main__':
    unittest.main()