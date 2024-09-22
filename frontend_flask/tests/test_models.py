# frontend_flask/app/tests/test_models.py

import unittest
from app.models import LoadBalancingRule, BackendServer

class TestModels(unittest.TestCase):
    def test_load_balancing_rule(self):
        servers = [
            BackendServer(id="server1", ip="127.0.0.1", port=8081, weight=2),
            BackendServer(id="server2", ip="127.0.0.1", port=8082, weight=1)
        ]
        rule = LoadBalancingRule(path="/api/*", algorithm="weighted_least_connections", servers=servers)
        self.assertEqual(rule.path, "/api/*")
        self.assertEqual(rule.algorithm, "weighted_least_connections")
        self.assertEqual(len(rule.servers), 2)
        self.assertEqual(rule.servers[0].id, "server1")
        self.assertEqual(rule.servers[1].weight, 1)

if __name__ == '__main__':
    unittest.main()