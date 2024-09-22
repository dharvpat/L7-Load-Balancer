# frontend_flask/app/tests/test_utils.py

import unittest
from unittest.mock import patch
from app.utils import get_all_rules

class TestUtils(unittest.TestCase):
    @patch('app.utils.requests.get')
    def test_get_all_rules_success(self, mock_get):
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
        rules = get_all_rules()
        self.assertEqual(len(rules), 1)
        self.assertEqual(rules[0]['path'], "/api/*")
    
    @patch('app.utils.requests.get')
    def test_get_all_rules_failure(self, mock_get):
        mock_get.side_effect = requests.RequestException("Failed to connect")
        rules = get_all_rules()
        self.assertEqual(rules, [])

if __name__ == '__main__':
    unittest.main()